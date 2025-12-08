#include "helpers.hpp"

#include <fstream>
#include <ios>
#include <limits>
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
  // file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  // file.ignore(std::numeric_limits<std::streamsize>::max(), '\r');
  while (std::getline(file, curr_line)) {
    if (curr_line.back() == '\r') {
      curr_line.pop_back();
    }
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
  // std::cout << "LogEntry::toStr() called with cats : " <<
  // vectorToString(_cats)
  //           << std::endl; // [debugging]
  for (const auto &cat : _cats) {
    std::string log_cat{cat};
    auto it = _cats_map.find(cat);
    if (it != _cats_map.end()) {
      log_cat = it->second.second;
    }
    res += "[" + log_cat + "]";
  }
  res += " " + _msg;
  return res;
}

std::ostream &operator<<(std::ostream &os, const LogEntry &log_entry) {
  os << (log_entry.toStr());
  return os;
}

} // namespace helper
