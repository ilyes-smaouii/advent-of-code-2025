#include "helpers.hpp"

#include <cstdarg>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace helper {

std::string getFileContentRaw(const std::string &filename) {
  auto file = std::ifstream(filename, std::ios_base::in);
  std::stringstream file_stream{};
  file_stream << file.rdbuf();
  return file_stream.str();
}

std::vector<std::string> getFileContentAslines(const std::string &filename) {
  auto file = std::ifstream(filename, std::ios_base::in);
  if (!file.is_open()) {
    throw std::runtime_error(
        "getFileContentAslines() error : couldn't open file "
        "!\n(filename : " +
        filename + ")");
  }
  std::string curr_line{};
  std::vector<std::string> lines{};
  while (std::getline(file, curr_line)) {
    lines.push_back(curr_line);
  }
  return lines;
}

std::vector<std::string> rawToLines(const std::string &raw) {
  std::vector<std::string> lines{{}};
  std::stringstream raw_strm{raw};
  while (std::getline(raw_strm, *(lines.end() - 1))) {
    lines.emplace_back();
  }
  lines.pop_back();
  return lines;
}

std::string LogEntry::toStr() const {
  std::string res;
  for (const auto &cat : _cats) {
    std::string msg{cat};
    auto it = _cats_map.find(cat);
    if (it != _cats_map.end()) {
      msg = it->second.second;
    }
    res += "[" + msg + "]";
  }
  res += " " + _msg;
  return res;
}

} // namespace helper
