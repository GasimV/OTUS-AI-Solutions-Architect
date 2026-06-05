from __future__ import annotations

import hashlib
import re
import uuid
from functools import lru_cache
from typing import Any

from .schemas import Chunk, DocumentInput


ENTITY_PATTERN = re.compile(
    r"\b(?:"
    r"(?:\u0413\u041e\u0421\u0422\s?\d+(?:[.\d-]*\d)?)|"
    r"(?:ISO\s?\d+(?:[.\d-]*\d)?)|"
    r"(?:[A-Z\u0410-\u042f\u0401]{2,}-\d{2,})|"
    r"(?:\d{2,4}-[A-Z\u0410-\u042f\u0401]{2,8})|"
    r"(?:[A-Z][A-Za-z0-9&.-]*(?:\s+[A-Z][A-Za-z0-9&.-]*){1,2})|"
    r"(?:[\u0410-\u042f\u0401][\u0430-\u044f\u0451]{2,}(?:\s+[\u0410-\u042f\u0401][\u0430-\u044f\u0451]{2,}){1,2})"
    r")\b",
    re.UNICODE,
)

CYRILLIC_TOKEN_PATTERN = re.compile(r"[\u0430-\u044f\u0451]+", re.I)
SPACE_PATTERN = re.compile(r"\s+")


def stable_id(*parts: str) -> str:
    digest = hashlib.sha256("::".join(parts).encode("utf-8")).hexdigest()
    return str(uuid.uuid5(uuid.NAMESPACE_URL, digest))


def extract_entities(text: str, limit: int = 12) -> tuple[str, ...]:
    entities: list[str] = []
    seen: set[str] = set()
    for match in ENTITY_PATTERN.finditer(text):
        entity = normalize_entity(match.group(0))
        if len(entity) < 3 or entity in seen:
            continue
        seen.add(entity)
        entities.append(entity)
        if len(entities) >= limit:
            break
    return tuple(entities)


def normalize_entity(value: str) -> str:
    cleaned = SPACE_PATTERN.sub(" ", value.strip()).replace("\u0451", "\u0435").replace("\u0401", "\u0415")
    if not cleaned:
        return ""
    if _looks_like_code(cleaned):
        return cleaned.upper().replace(" ", "")
    analyzer = _morph_analyzer()
    normalized: list[str] = []
    for token in cleaned.lower().split():
        if analyzer is not None and CYRILLIC_TOKEN_PATTERN.fullmatch(token):
            parsed = analyzer.parse(token)
            normalized.append(parsed[0].normal_form if parsed else token)
        else:
            normalized.append(token)
    return " ".join(normalized)


def chunk_document(document: DocumentInput, max_chars: int = 900, overlap: int = 120) -> list[Chunk]:
    paragraphs = [p.strip() for p in re.split(r"\n\s*\n", document.text) if p.strip()]
    chunks: list[str] = []
    current = ""

    for paragraph in paragraphs or [document.text.strip()]:
        if len(current) + len(paragraph) + 2 <= max_chars:
            current = f"{current}\n\n{paragraph}".strip()
            continue
        if current:
            chunks.append(current)
        if len(paragraph) <= max_chars:
            current = paragraph
            continue
        start = 0
        while start < len(paragraph):
            chunks.append(paragraph[start : start + max_chars])
            start += max_chars - overlap
        current = ""

    if current:
        chunks.append(current)

    result: list[Chunk] = []
    for ordinal, text in enumerate(chunks):
        chunk_id = stable_id(document.tenant_id, document.document_id, str(ordinal), text[:80])
        result.append(
            Chunk(
                id=chunk_id,
                document_id=document.document_id,
                tenant_id=document.tenant_id,
                text=text,
                acl_groups=document.acl_groups,
                ordinal=ordinal,
                metadata={"title": document.title, **document.metadata},
                entities=extract_entities(text),
            )
        )
    return result


def _looks_like_code(value: str) -> bool:
    return bool(re.search(r"\d", value) and re.search(r"[A-Z\u0410-\u042f\u0401]{2,}", value, re.I))


@lru_cache(maxsize=1)
def _morph_analyzer() -> Any | None:
    try:
        import pymorphy3
    except Exception:
        return None
    return pymorphy3.MorphAnalyzer()
