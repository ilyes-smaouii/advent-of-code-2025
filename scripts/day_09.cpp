#include "helpers.hpp"

#include <cstdint>
#include <cstdlib>
#include <format>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace hlp = helper;

// PART 1

using coord_num_t = std::uint32_t;
using coord_t = std::pair<coord_num_t, coord_num_t>;
using area_t = std::uint64_t;

coord_t parseLine(const std::string &line) {
  coord_t res;
  auto &[row, col] = res;
  std::stringstream string_strm{line};
  string_strm >> row;
  string_strm.ignore(1);
  string_strm >> col;
  return res;
}

area_t computeArea(const coord_t &coord_a, const coord_t &coord_b) {
  const auto diff_row = 1 + std::abs(static_cast<int64_t>(coord_a.first) -
                                     static_cast<int64_t>(coord_b.first));
  const auto diff_col = 1 + std::abs(static_cast<int64_t>(coord_a.second) -
                                     static_cast<int64_t>(coord_b.second));
  return diff_row * diff_col;
}
namespace std {
template <>
struct less<coord_t> {
  bool operator()(const coord_t &coord_a, const coord_t &coord_b) const {
    return coord_a.first < coord_b.first ||
           (coord_a.first == coord_b.first && coord_a.second < coord_b.second);
  }
};
template <>
struct less<std::pair<coord_t, coord_t>> {
  bool operator()(const std::pair<coord_t, coord_t> &corners_a,
                  const std::pair<coord_t, coord_t> &corners_b) const {
    const auto area_a = computeArea(corners_a.first, corners_a.second);
    const auto area_b = computeArea(corners_b.first, corners_b.second);
    return std::less()(area_a, area_b);
  }
};
} // namespace std

std::vector<coord_t> parseLines(const std::vector<std::string> &lines) {
  std::vector<coord_t> coordinates{};
  coordinates.reserve(lines.size());
  for (const auto &line : lines) {
    coordinates.push_back(parseLine(line));
  }
  return coordinates;
}

int main(int argc, char *argv[]) {
  const std::string le_filename{"../inputs/day_09_input.txt"};
  // auto le_content = hlp::getFileContentRaw(le_filename);
  auto le_lines = hlp::getFileContentAslines(le_filename);
  // auto le_table = hlp::getFileContentAsTable(le_filename);
  auto le_test_lines = hlp::rawToLines(R"(7,1
11,1
11,7
9,7
9,5
2,5
2,3
7,3)");

  // le_lines = le_test_lines;

  // PART 1

  auto le_coords = parseLines(le_lines);

  std::set<std::pair<coord_t, coord_t>> rectangles_set{};

  for (const auto &coord_a : le_coords) {
    for (const auto &coord_b : le_coords) {
      if (coord_a == coord_b) {
        continue;
      }
      rectangles_set.insert(std::make_pair(coord_a, coord_b));
    }
  }

  auto largest_rectangle = *(--rectangles_set.end());
  auto &[pt_a, pt_b] = largest_rectangle;
  auto res_area =
      computeArea(std::make_pair(0, 0), std::make_pair(2000000, 20000000));
  res_area = computeArea(pt_a, pt_b);

  hlp::printLogEntries({"R"}, "Largest rectangle :",
                       std::format("{}, {}", pt_a.first, pt_a.second),
                       std::format("{}, {}", pt_b.first, pt_b.second));
  hlp::printLogEntries({"R"}, "Its area :", res_area);

  std::cout << "res_areau : " << res_area << std::endl;
}