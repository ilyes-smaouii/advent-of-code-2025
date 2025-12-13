#include "helpers.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>
#include <iterator>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

namespace hlp = helper;

// PART 1

using coord_num_t = std::int64_t;
using area_t = std::uint64_t;

struct Coordinates /* : std::pair<coord_num_t, coord_num_t> */ {
public:
  // using pt = std::pair<coord_num_t, coord_num_t>;
  // Coordinates() = default;
  // Coordinates(const pt &pair) : pt(pair) {}
  // Coordinates(pt &&pair) : pt(pair) {}
  // Coordinates(coord_num_t row, coord_num_t col) : pt(row, col) {}
  // using pt::operator=;
  // Coordinates &operator=(const Coordinates &other) {
  //   row = other.row;
  //   col = other.col;
  //   return *this;
  // }

  // pt &asPair() { return static_cast<pt &>(*this); };
  // const pt &asPair() const { return asPair(); };
  // template <std::size_t I>
  // std::enable_if_t<(I < 2), coord_num_t&> get() const;
  // template <>
  // coord_num_t &get<0>() const {
  //   return row;
  // }
  // template <>
  // coord_num_t &get<1>() const {
  //   return col;
  // }
  auto operator<=>(const Coordinates &other) const = default;

  coord_num_t row;
  coord_num_t col;

protected:
};

std::ostream &operator<<(std::ostream &os, const Coordinates &coord) {
  os << "(" << coord.row << "," << coord.col << ")";
  return os;
}

namespace std {
// template <>
// struct tuple_size<Coordinates> {
//   static constexpr std::size_t value{2};
// };

// template <std::size_t I>
// struct tuple_element<I, Coordinates> {
//   using type = std::enable_if_t<(I < 3), coord_num_t>;
// };

// template <std::size_t I>
// std::enable_if_t<(I < 2), coord_num_t &> get(Coordinates &coord);
// template <>
// coord_num_t &get<0>(Coordinates &coord) {
//   return coord.row;
// }
// template <>
// coord_num_t &get<1>(Coordinates &coord) {
//   return coord.col;
// }

} // namespace std

using cell_t = Coordinates;

struct Rectangle {
  cell_t corner_a;
  cell_t corner_b;
};

// namespace std {
// template <std::size_t I>
// cell_t get(const Rectangle &rect);
// template <>
// cell_t get<0>(const Rectangle &rect) {
//   return rect.a;
// }
// template <>
// cell_t get<1>(const Rectangle &rect) {
//   return rect.b;
// }
// } // namespace std

cell_t parseLine(const std::string &line) {
  hlp::printLogEntries({"D"}, "Parsing line :", line);
  cell_t res;
  auto &[row, col] = res;
  std::stringstream string_strm{line};
  string_strm >> row;
  string_strm.ignore(1);
  string_strm >> col;
  hlp::printLogEntries({"D"},
                       std::format("Got res : ({},{})", res.row, res.col));
  return res;
}

area_t computeArea(const cell_t coord_a, const cell_t coord_b) {
  const auto diff_row = 1 + std::abs(static_cast<int64_t>(coord_a.row) -
                                     static_cast<int64_t>(coord_b.col));
  const auto diff_col = 1 + std::abs(static_cast<int64_t>(coord_a.row) -
                                     static_cast<int64_t>(coord_b.col));
  return diff_row * diff_col;
}
namespace std {
template <>
struct hash<cell_t> {
  std::size_t operator()(const cell_t &cell) const {
    auto hasher = hash<coord_num_t>();
    auto res = hasher(cell.row);
    res ^= hasher(cell.col) + 0x9e3779b9 + (res << 6) + (res >> 2);
    return res;
  }
};

template <>
struct less<cell_t> {
  bool operator()(const cell_t &coord_a, const cell_t &coord_b) const {
    return coord_a.row < coord_b.row ||
           (coord_a.row == coord_b.row && coord_a.col < coord_b.col);
  }
};
// template <>
// struct less<std::pair<cell_t, cell_t>> {
//   bool operator()(const std::pair<cell_t, cell_t> &corners_a,
//                   const std::pair<cell_t, cell_t> &corners_b) const {
//     const auto area_a = computeArea(corners_a.first, corners_a.second);
//     const auto area_b = computeArea(corners_b.first, corners_b.second);
//     return std::less()(area_a, area_b);
//   }
// };
} // namespace std

struct MyRectangleLess {
  bool operator()(const Rectangle &rectangle_a,
                  const Rectangle &rectangle_b) const {
    const auto area_a = computeArea(rectangle_a.corner_a, rectangle_a.corner_b);
    const auto area_b = computeArea(rectangle_b.corner_a, rectangle_b.corner_b);
    return std::less()(area_a, area_b) ||
           (area_a == area_b &&
            std::less()(
                std::make_pair(rectangle_a.corner_a, rectangle_a.corner_b),
                std::make_pair(rectangle_b.corner_a, rectangle_b.corner_b)));
  }
};

std::vector<cell_t> parseLines(const std::vector<std::string> &lines) {
  std::vector<cell_t> coordinates{};
  coordinates.reserve(lines.size());
  for (const auto &line : lines) {
    coordinates.push_back(parseLine(line));
    hlp::printLogEntries({"D"},
                         "parseLines() - Just added :", coordinates.back());
  }
  return coordinates;
}

// PART 2

template <typename CELL_T>
class LineView {
public:
  LineView(std::size_t cell_count) : _cell_count(cell_count) {
    _data = std::make_shared<CELL_T[]>(_cell_count);
  }

  LineView(std::shared_ptr<CELL_T[]> data, std::size_t cell_count)
      : _data(data), _cell_count(cell_count) {}

  CELL_T &cellAt(std::size_t cell_idx);
  const CELL_T &cellAt(std::size_t cell_idx) const;

protected:
  std::shared_ptr<CELL_T[]> _data;
  std::size_t _cell_count{};
};

template <typename CELL_T>
class TableView {
public:
  static constexpr std::size_t MAX_CELL_STR_SIZE{1}; // arbitrary, but whatever

  TableView(coord_num_t row_count, coord_num_t col_count)
      : _row_count(row_count), _col_count(col_count) {
    _data = std::make_shared<CELL_T[]>(row_count * col_count);
  }

  TableView(std::shared_ptr<CELL_T[]> data, coord_num_t row_count,
            coord_num_t col_count)
      : _data(data), _row_count(row_count), _col_count(col_count) {}

  coord_num_t rowCount() const { return _row_count; }
  coord_num_t colCount() const { return _col_count; }

  void fillWith(const CELL_T &content) {
    for (coord_num_t i{0}; i < _row_count * _col_count; i++) {
      _data[i] = content;
    }
  }

  CELL_T &cellAt(coord_num_t row_idx, coord_num_t col_idx,
                 bool safety_checks = true) {
    hlp::printLogEntries({"D"}, std::format("cellAt() called with args {}, {}",
                                            row_idx, col_idx));
    if (safety_checks && (row_idx >= _row_count || col_idx >= _col_count)) {
      throw std::runtime_error(
          std::format("TableView<>::cellAt() error : row_idx ({}) >= "
                      "_row_count ({}) || col_idx ({}) >= _col_count ({}) !",
                      row_idx, _row_count, col_idx, _col_count));
    }
    return _data[row_idx * _col_count + col_idx];
  }

  const CELL_T &cellAt(coord_num_t row_idx, coord_num_t col_idx,
                       bool safety_checks = true) const {
    return cellAt(row_idx, col_idx, safety_checks);
  }

  LineView<CELL_T> operator[](coord_num_t row_idx) {
    if (row_idx >= _row_count) {
      throw std::runtime_error(std::format(
          "TableView<>::operator[] error : row_idx ({}) >= _row_count ({}) !",
          row_idx, _row_count));
    }
    return {&(_data[row_idx * _col_count]), _col_count};
  }

  TableView<CELL_T> copy() const {
    auto res = TableView<CELL_T>{_row_count, _col_count};
    std::memcpy(res._data.get(), _data.get(),
                sizeof(CELL_T) * _row_count * _col_count);
    return res;
  }

  std::string str() const {
    // return {};
    std::string res{};
    res.reserve(MAX_CELL_STR_SIZE * _row_count * _col_count);
    // std::stringstream res;
    coord_num_t offset{0};
    for (coord_num_t row_idx{0}; row_idx < _row_count; row_idx++) {
      for (coord_num_t col_idx{0}; col_idx < _col_count; col_idx++) {
        res += _data[offset++];
      }
      res += '\n';
    }
    return res;
  }

protected:
  std::shared_ptr<CELL_T[]> _data;
  coord_num_t _row_count{}, _col_count{};
};

constexpr char EMPTY_CELL{'.'}, RED_CELL{'#'}, GREEN_CELL{'X'};

TableView<char> getBaseTable(const std::vector<cell_t> &coords) {
  coord_num_t max_row{0}, max_col{0};
  for (const auto &[col, row] : coords) {
    max_row = std::max(max_row, row);
    max_col = std::max(max_col, col);
  }
  auto res = TableView<char>{max_row + 2, max_col + 2};
  res.fillWith(EMPTY_CELL);
  for (const auto &[col, row] : coords) {
    res.cellAt(row, col) = RED_CELL;
  }
  return res;
}

std::int8_t getDir(std::uint64_t a, std::uint64_t b) {
  return a < b ? 1 : a == b ? 0 : -1;
}

TableView<char> getBordersTable(const std::vector<cell_t> &coords) {
  coord_num_t max_row{0}, max_col{0};
  for (const auto &[col, row] : coords) {
    max_row = std::max(max_row, row);
    max_col = std::max(max_col, col);
  }
  auto res = TableView<char>{max_row + 2, max_col + 2};
  res.fillWith(EMPTY_CELL);
  // auto iter = coords.begin();
  for (std::size_t coord_idx{0}; coord_idx < coords.size(); coord_idx++) {
    auto &[curr_col, curr_row]{coords[coord_idx]};
    auto &[next_col, next_row]{coords[(coord_idx + 1) % coords.size()]};
    hlp::printLogEntries(
        {"BORD"},
        std::format(
            "getBordersTable() currently at border from ({},{}) to ({},{})",
            curr_row, curr_col, next_row, next_col));
    std::int8_t row_incr = getDir(curr_row, next_row);
    std::int8_t col_incr = getDir(curr_col, next_col);
    if (row_incr * col_incr != 0) {
      throw std::runtime_error("getBordersTable() error : diagonal border !");
    }
    for (std::int64_t row_idx{static_cast<int64_t>(curr_row) + row_incr},
         col_idx{static_cast<int64_t>(curr_col) + col_incr};
         row_idx != next_row || col_idx != next_col;
         row_idx += row_incr, col_idx += col_incr) {
      res.cellAt(row_idx, col_idx) = GREEN_CELL;
    }
    res.cellAt(curr_row, curr_col) = RED_CELL;
    res.cellAt(next_row, next_col) = RED_CELL;
  }
  return res;
}

std::unordered_set<cell_t> getRectangleBorders(const cell_t &a,
                                               const cell_t &b) {
  std::unordered_set<cell_t> res{};
  const auto row_incr{getDir(a.row, b.row)}, col_incr{getDir(a.col, b.col)};
  for (coord_num_t row{a.row}; row != a.row + row_incr; row += row_incr) {
    res.insert({{row, a.col}, {row, b.col}});
  }
  for (coord_num_t col{a.col}; col != a.col + col_incr; col += col_incr) {
    res.insert({{a.row, col}, {b.row, col}});
  }
  return res;
}

std::unordered_set<cell_t> getRectangleBorders(const Rectangle &rect) {
  return getRectangleBorders(rect.corner_a, rect.corner_b);
}

area_t computeArea(const Rectangle &rect) {
  return computeArea(rect.corner_a, rect.corner_b);
}

using cell_set_t = std::unordered_set<cell_t>;

std::pair<coord_num_t, coord_num_t>
getTableSize(const std::vector<cell_t> &coords) {
  coord_num_t max_row{0}, max_col{0};
  for (const auto &[col, row] : coords) {
    max_row = std::max(max_row, row);
    max_col = std::max(max_col, col);
  }
  return {max_row + 2, max_col + 2};
}

cell_set_t getBordersCells(const std::vector<cell_t> &coords) {
  auto [max_row, max_col] = getTableSize(coords);
  cell_set_t res{};
  // auto iter = coords.begin();
  for (std::size_t coord_idx{0}; coord_idx < coords.size(); coord_idx++) {
    auto &[curr_col, curr_row]{coords[coord_idx]};
    auto &[next_col, next_row]{coords[(coord_idx + 1) % coords.size()]};
    // hlp::printLogEntries(
    //     {"BORD"},
    //     std::format(
    //         "getBordersTable() currently at border from ({},{}) to ({},{})",
    //         curr_row, curr_col, next_row, next_col));
    std::int8_t row_incr = getDir(curr_row, next_row);
    std::int8_t col_incr = getDir(curr_col, next_col);
    if (row_incr * col_incr != 0) {
      throw std::runtime_error("getBordersTable() error : diagonal border !");
    }
    for (std::int64_t row_idx{static_cast<int64_t>(curr_row) + row_incr},
         col_idx{static_cast<int64_t>(curr_col) + col_incr};
         row_idx != next_row || col_idx != next_col;
         row_idx += row_incr, col_idx += col_incr) {
      res.insert({row_idx, col_idx});
    }
    res.insert({next_row, next_col});
  }
  return res;
}

inline cell_set_t getNeighbors(const cell_t &cell) {
  auto &[row, col] = cell;
  return {{row - 1, col - 1}, {row - 1, col},    {row - 1, col + 1},
          {row, col - 1},     {row, col + 1},    {row + 1, col - 1},
          {row + 1, col},     {row + 1, col + 1}};
}

inline cell_set_t getNextLayer(const cell_set_t &exclude_set,
                               const cell_set_t &curr_layer) {
  cell_set_t next_layer{};
  for (const auto &cell : curr_layer) {
    auto neighbors = getNeighbors(cell);
    for (const auto &neighbor : neighbors) {
      if (!exclude_set.contains(neighbor)) {
        next_layer.insert(neighbor);
      }
    }
  }
  return next_layer;
}

cell_set_t getEdges(coord_num_t row_count, coord_num_t col_count) {
  cell_set_t edges{};
  for (coord_num_t row_idx{-1}; row_idx <= row_count; row_idx++) {
    edges.insert({{row_idx, -1}, {row_idx, col_count}});
  }
  for (coord_num_t col_idx{-1}; col_idx <= row_count; col_idx++) {
    edges.insert({{-1, col_idx}, {row_count, col_idx}});
  }
  return edges;
}

TableView<char> tableWithArea(const cell_set_t &area, coord_num_t row_count,
                              coord_num_t col_count) {
  TableView<char> final_table{row_count, col_count};
  final_table.fillWith(EMPTY_CELL);
  for (const auto &[row, col] : area) {
    final_table.cellAt(row, col) = GREEN_CELL;
  }
  return final_table;
}

void displayArea(const cell_set_t &area, coord_num_t row_count,
                 coord_num_t col_count) {
  for (coord_num_t row{0}; row < row_count; row++) {
    for (coord_num_t col{0}; col < row_count; col++) {
      if (area.contains({row, col})) {
        std::cout << RED_CELL;
      } else {
        std::cout << EMPTY_CELL;
      }
    }
    std::cout << '\n';
  }
  std::cout << std::flush;
}

cell_set_t
growAreaToMax(const cell_set_t &exclude_set, const cell_set_t &initial_area,
              const std::pair<coord_num_t, coord_num_t> &row_and_col_count) {
  cell_set_t exclude{exclude_set}, final_area{initial_area},
      curr_layer{initial_area};
  while (curr_layer.size() > 0) {
    exclude.insert(curr_layer.begin(), curr_layer.end());
    final_area.insert(curr_layer.begin(), curr_layer.end());
    hlp::printLogEntries(
        {"D", "GROW"},
        std::format(
            "growAreaToMax() - current area (grew by {}, currently at {}) :",
            curr_layer.size(), final_area.size()));
    // displayArea(final_area, row_and_col_count.first,
    // row_and_col_count.second);
    curr_layer = getNextLayer(exclude, curr_layer);
  }
  hlp::printLogEntries({"GROW"}, "Final area size :", final_area.size());
  return final_area;
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
  hlp::printLogEntries(
      {"R"}, std::format("Got coords ({} of them)", le_coords.size()));
  hlp::printLogEntries({"D"}, "Coords :", hlp::vectorToString(le_coords));

  std::set<Rectangle, MyRectangleLess> rectangles_set{};

  for (std::size_t i{0}; i < le_coords.size(); i++) {
    for (std::size_t j{i}; j < le_coords.size(); j++) {
      if (le_coords.at(i) == le_coords.at(j)) {
        hlp::printLogEntries(
            {"D"}, std::format("Ignoring this pair of coords : ({},{}) ({},{})",
                               le_coords.at(i).row, le_coords.at(i).col,
                               le_coords.at(j).row, le_coords.at(j).col));
      }
      rectangles_set.insert({le_coords.at(i), le_coords.at(j)});
    }
  }
  hlp::printLogEntries({"R"}, std::format("Got rectangles (got {} rectangles)",
                                          rectangles_set.size()));

  if (rectangles_set.size() == 0) {
    throw std::runtime_error("Error : no rectangles in set !");
  }

  auto largest_rectangle = *(rectangles_set.rbegin());
  // largest_rectangle = *(--rectangles_set.end());
  auto &[pt_a, pt_b] = largest_rectangle;
  hlp::printLogEntries({"R"}, "Got largest rectangle");
  auto res_area = computeArea({0, 0}, {20000000, 20000000});
  res_area = computeArea(pt_a, pt_b);

  hlp::printLogEntries(
      {"R"}, "Largest rectangle :", std::format("{}, {}", pt_a.row, pt_a.col),
      std::format("{}, {}", pt_b.row, pt_b.col));
  hlp::printLogEntries({"R"}, "Its area :", res_area);

  std::cout << "res_area (Part 1) : " << res_area << std::endl;

  // PART 2

  hlp::LOG_CATS_MAP["I"].first = true;
  hlp::LOG_CATS_MAP["D"].first = false;
  hlp::LOG_CATS_MAP["VIZ"] = std::make_pair(false, "VIZ");
  hlp::LOG_CATS_MAP["GROW"] = std::make_pair(true, "GROW");
  // hlp::LOG_CATS_MAP["BORD"] = std::make_pair(true, "BORDERS");

  // auto le_base_table = getBaseTable(le_coords);
  // auto le_borders_table = getBordersTable(le_coords);

  // hlp::printLogEntries({"I"}, "Base table :", '\n' + le_base_table.str());

  hlp::printLogEntries({"I", {"VIZ"}}, "Got (not) borders table");

  // hlp::printLogEntries({"I", "VIZ"},
  //                      "Table with borders :", '\n' +
  //                      le_borders_table.str());

  auto [le_rc, le_cc] = getTableSize(le_coords);
  hlp::printLogEntries(
      {"I"}, "Row count and col count :", std::format("{}, {}", le_rc, le_cc));
  auto le_borders = getBordersCells(le_coords);
  auto le_edges = getEdges(le_rc, le_cc);
  le_borders.insert(le_edges.begin(), le_edges.end());
  hlp::printLogEntries({"I"}, "About to call growAreaToMax()");
  auto le_test_area =
      growAreaToMax(le_borders, {{70000, 2000}}, {le_rc, le_cc});

  hlp::printLogEntries({"I"}, tableWithArea(le_test_area, le_rc, le_cc).str());

  std::cout << "res_area (Part 2) : " << res_area << std::endl;
}