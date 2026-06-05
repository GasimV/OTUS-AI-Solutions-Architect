from __future__ import annotations

import argparse
from pathlib import Path

from huggingface_hub import snapshot_download


def main() -> None:
    parser = argparse.ArgumentParser(description="Download a Hugging Face model snapshot for offline vLLM serving.")
    parser.add_argument("--repo-id", default="Qwen/Qwen3-8B-AWQ")
    parser.add_argument("--target", default="models/qwen3-awq")
    parser.add_argument("--revision", default=None)
    args = parser.parse_args()

    target = Path(args.target).resolve()
    target.mkdir(parents=True, exist_ok=True)

    snapshot_download(
        repo_id=args.repo_id,
        revision=args.revision,
        local_dir=str(target),
        local_dir_use_symlinks=False,
        resume_download=True,
    )

    print(f"Downloaded {args.repo_id} to {target}")


if __name__ == "__main__":
    main()
