#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace adra::util {

std::string generate_uuid();

std::string now_iso8601_utc();
std::string today_iso_date();
std::string slugify(const std::string& s);

bool ensure_directory(const std::string& path);
bool write_file(const std::string& path, const std::string& content);
std::optional<std::string> read_file(const std::string& path);
std::vector<std::string> list_files(const std::string& dir, const std::string& ext);
bool remove_file(const std::string& path);
bool path_exists(const std::string& path);

std::string to_lower(std::string s);
std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, char delim);
std::string join(const std::vector<std::string>& parts, const std::string& sep);
std::string replace_all(std::string s, const std::string& from, const std::string& to);

}  // namespace adra::util
