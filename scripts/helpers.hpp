#pragma once

#include <cstddef>
// #include <cstdint>
#include <iostream>
// #include <istream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace helper {

/*
  Simple vector wrapper
  To represent it as a table, with a number of rows and columns
 */
template <typename T>
class MyTable {
public:
  MyTable();

protected:
  std::size_t _row_count{}, _col_count{};
  std::vector<T> _vec{};
};

std::string getFileContentRaw(const std::string &filename);

std::vector<std::string> getFileContentAslines(const std::string &filename);

std::vector<std::string> rawToLines(const std::string &raw);

std::vector<std::vector<char>>
getFileContentAsTable(const std::string &filename);

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
  os << "vec(";
  for (const auto &e : vec) {
    os << e << ",";
  }
  os << ")";
  return os;
}

template <typename T>
std::string vectorToString(const std::vector<T> &vec) {
  std::stringstream string_str{};
  string_str << "vec(";
  for (const auto &e : vec) {
    string_str << e << ",";
  }
  string_str << ")";
  // auto str = string_str.str();
  // str.pop_back();
  return string_str.str();
}

// inline std::ostream &
// operator<<(std::ostream &os,
//            const std::vector<std::vector<std::uint8_t>> &buttons) {
//   for (const auto &button : buttons) {
//     os << '{';
//     for (const auto &light_idx : button) {
//       os << static_cast<int>(light_idx) << ", ";
//     }
//     os << "} ";
//   }
//   return os;
// }

using cat_t = std::string;
using msg_t = std::string;
using cat_map_t = std::unordered_map<cat_t, std::pair<bool, msg_t>>;

inline std::unordered_map<std::string, std::pair<bool, std::string>>
    LOG_CATS_MAP{
        {"T", {false, "TESTING"}},
        {"I", {false, "INFO"}},
        {"R", {true, "RESULTS"}},
        {"D", {false, "DEBUGGING"}},
    };

class LogEntry {
public:
  template <typename T>
  LogEntry(const T &msg, const std::vector<cat_t> &cats) : _cats(cats) {
    std::stringstream string_strm{};
    string_strm << msg;
    _msg = string_strm.str();
  }

  // template <>
  LogEntry(const std::string &msg, const std::vector<cat_t> &cats)
      : _msg(msg), _cats(cats) {}

  std::string toStr() const;

protected:
  msg_t _msg{};
  std::vector<cat_t> _cats;
  const cat_map_t &_cats_map{LOG_CATS_MAP};
};

std::ostream &operator<<(std::ostream &os, const LogEntry &log_entry);

template <typename T>
std::vector<LogEntry>
getLogEntriesAux(std::vector<cat_t> cats,
                 std::shared_ptr<std::vector<LogEntry>> log_entries_ptr,
                 T msg) {
  std::vector<LogEntry> &log_entries{*log_entries_ptr};
  bool should_log = false;
  for (const auto &cat : cats) {
    auto it = LOG_CATS_MAP.find(cat);
    if (it != LOG_CATS_MAP.end()) {
      if (it->second.first) {
        should_log = true;
        break;
      }
    }
  }
  if (should_log) {
    log_entries.emplace_back(std::forward<T>(msg), cats);
  }
  return log_entries;
}

template <typename T, typename... Args>
std::vector<LogEntry>
getLogEntriesAux(const std::vector<cat_t> &cats,
                 std::shared_ptr<std::vector<LogEntry>> log_entries_ptr,
                 T &&msg, Args... msgs) {
  std::vector<LogEntry> &log_entries{*log_entries_ptr};
  bool should_log = false;
  for (const auto &cat : cats) {
    auto it = LOG_CATS_MAP.find(cat);
    if (it != LOG_CATS_MAP.end()) {
      if (it->second.first) {
        should_log = true;
        break;
      }
    }
  }
  if (should_log) {
    log_entries.emplace_back(std::forward<T>(msg), cats);
  }
  return getLogEntriesAux(cats, log_entries_ptr, msgs...);
}

template <typename T, typename... Args>
std::vector<LogEntry> getLogEntries(const std::vector<cat_t> &cats, T &&msg,
                                    Args... msgs) {
  return getLogEntriesAux(cats, std::make_shared<std::vector<LogEntry>>(),
                          std::forward<T>(msg), msgs...);
}

template <typename... Args>
void printLogEntries(std::vector<cat_t> cats, Args... msgs) {
  auto log_entries = getLogEntries(cats, msgs...);
  // std::cout << "printLogEntries() - log_entries : "
  //           << vectorToString(log_entries) << std::endl; // [debugging]
  for (const auto &log_entry : log_entries) {
    std::cout << log_entry.toStr() << std::endl;
  }
}

// MyTable<std::string> getFileContentAsTable(const std::string &filename);

} // namespace helper