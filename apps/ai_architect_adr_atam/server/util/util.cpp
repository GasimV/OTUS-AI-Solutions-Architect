#include "util/util.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>

namespace fs = std::filesystem;

namespace adra::util {

std::string generate_uuid() {
    static thread_local std::mt19937_64 rng{std::random_device{}()};
    std::uniform_int_distribution<uint64_t> dist;
    uint64_t a = dist(rng);
    uint64_t b = dist(rng);
    // RFC4122 v4 framing
    a = (a & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
    b = (b & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;
    char buf[37];
    std::snprintf(buf, sizeof(buf),
                  "%08x-%04x-%04x-%04x-%012llx",
                  static_cast<unsigned>((a >> 32) & 0xFFFFFFFF),
                  static_cast<unsigned>((a >> 16) & 0xFFFF),
                  static_cast<unsigned>(a & 0xFFFF),
                  static_cast<unsigned>((b >> 48) & 0xFFFF),
                  static_cast<unsigned long long>(b & 0xFFFFFFFFFFFFULL));
    return std::string(buf);
}

std::string now_iso8601_utc() {
    const auto now = std::chrono::system_clock::now();
    const auto t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif
    std::ostringstream os;
    os << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return os.str();
}

std::string today_iso_date() {
    const auto now = std::chrono::system_clock::now();
    const auto t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream os;
    os << std::put_time(&tm, "%Y-%m-%d");
    return os.str();
}

std::string slugify(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    bool prev_dash = false;
    for (char c : s) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
            prev_dash = false;
        } else if (c == ' ' || c == '-' || c == '_' || c == '.') {
            if (!prev_dash && !out.empty()) {
                out.push_back('-');
                prev_dash = true;
            }
        }
    }
    while (!out.empty() && out.back() == '-') out.pop_back();
    if (out.empty()) out = "untitled";
    if (out.size() > 80) out.resize(80);
    return out;
}

bool ensure_directory(const std::string& path) {
    std::error_code ec;
    fs::create_directories(path, ec);
    return !ec;
}

bool write_file(const std::string& path, const std::string& content) {
    fs::path p(path);
    if (p.has_parent_path()) {
        std::error_code ec;
        fs::create_directories(p.parent_path(), ec);
    }
    std::ofstream ofs(path, std::ios::binary | std::ios::trunc);
    if (!ofs) return false;
    ofs.write(content.data(), static_cast<std::streamsize>(content.size()));
    return ofs.good();
}

std::optional<std::string> read_file(const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) return std::nullopt;
    std::ostringstream os;
    os << ifs.rdbuf();
    return os.str();
}

std::vector<std::string> list_files(const std::string& dir, const std::string& ext) {
    std::vector<std::string> out;
    std::error_code ec;
    if (!fs::exists(dir, ec)) return out;
    for (const auto& entry : fs::directory_iterator(dir, ec)) {
        if (ec) break;
        if (!entry.is_regular_file()) continue;
        const auto name = entry.path().filename().string();
        if (ext.empty() || entry.path().extension().string() == ext) {
            out.push_back(entry.path().string());
        }
    }
    std::sort(out.begin(), out.end());
    return out;
}

bool remove_file(const std::string& path) {
    std::error_code ec;
    return fs::remove(path, ec);
}

bool path_exists(const std::string& path) {
    std::error_code ec;
    return fs::exists(path, ec);
}

std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return {};
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == delim) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    out.push_back(cur);
    return out;
}

std::string join(const std::vector<std::string>& parts, const std::string& sep) {
    std::string out;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) out += sep;
        out += parts[i];
    }
    return out;
}

std::string replace_all(std::string s, const std::string& from, const std::string& to) {
    if (from.empty()) return s;
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.size(), to);
        pos += to.size();
    }
    return s;
}

}  // namespace adra::util
