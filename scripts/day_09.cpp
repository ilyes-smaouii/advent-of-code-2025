#include "helpers.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>
#include <ios>
#include <iterator>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
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
  using num_t = coord_num_t;
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

struct Range {
  coord_num_t min;
  coord_num_t max;
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
  const auto diff_row = std::abs(static_cast<int64_t>(coord_a.row) -
                                 static_cast<int64_t>(coord_b.row));
  const auto diff_col = std::abs(static_cast<int64_t>(coord_a.col) -
                                 static_cast<int64_t>(coord_b.col));
  return (1 + diff_row) * (1 + diff_col);
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

/*
  Custom Less operator() on Rectangles that should be independent on
  the order of the corners, and allow
  {a, b} to be considered equal to {b, a} for any a and b
 */
struct RectangleLessBase {

  bool operator()(const Rectangle &rectangle_a,
                  const Rectangle &rectangle_b) const {
    auto rect_a_corner_min =
        std::min(rectangle_a.corner_a, rectangle_a.corner_b);
    auto rect_a_corner_max =
        std::max(rectangle_a.corner_a, rectangle_a.corner_b);
    auto rect_b_corner_min =
        std::min(rectangle_b.corner_a, rectangle_b.corner_b);
    auto rect_b_corner_max =
        std::max(rectangle_b.corner_a, rectangle_b.corner_b);
    return std::less()(std::make_pair(rect_a_corner_min, rect_a_corner_max),
                       std::make_pair(rect_b_corner_min, rect_b_corner_max));
  }
};

struct RectangleLessOnArea {
  bool operator()(const Rectangle &rectangle_a,
                  const Rectangle &rectangle_b) const {
    const auto area_a = computeArea(rectangle_a.corner_a, rectangle_a.corner_b);
    const auto area_b = computeArea(rectangle_b.corner_a, rectangle_b.corner_b);
    return std::less()(area_a, area_b) ||
           (area_a == area_b && RectangleLessBase()(rectangle_a, rectangle_b));
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

// template <typename CELL_T>
// class LineView {
// public:
//   LineView(std::size_t cell_count) : _cell_count(cell_count) {
//     _data = std::make_shared<CELL_T[]>(_cell_count);
//   }

//   LineView(std::shared_ptr<CELL_T[]> data, std::size_t cell_count)
//       : _data(data), _cell_count(cell_count) {}

//   CELL_T &cellAt(std::size_t cell_idx);
//   const CELL_T &cellAt(std::size_t cell_idx) const;

// protected:
//   std::shared_ptr<CELL_T[]> _data;
//   std::size_t _cell_count{};
// };

// // template <typename CELL_T>
// // class TableView {
// // public:
// //   static constexpr std::size_t MAX_CELL_STR_SIZE{1}; // arbitrary, but
// //   whatever

// //   TableView(coord_num_t row_count, coord_num_t col_count)
// //       : _row_count(row_count), _col_count(col_count) {
// //     _data = std::make_shared<CELL_T[]>(row_count * col_count);
// //   }

// //   TableView(std::shared_ptr<CELL_T[]> data, coord_num_t row_count,
// //             coord_num_t col_count)
// //       : _data(data), _row_count(row_count), _col_count(col_count) {}

// //   coord_num_t rowCount() const { return _row_count; }
// //   coord_num_t colCount() const { return _col_count; }

// //   void fillWith(const CELL_T &content) {
// //     for (coord_num_t i{0}; i < _row_count * _col_count; i++) {
// //       _data[i] = content;
// //     }
// //   }

// //   CELL_T &cellAt(coord_num_t row_idx, coord_num_t col_idx,
// //                  bool safety_checks = true) {
// //     hlp::printLogEntries({"D"}, std::format("cellAt() called with args {},
// //     {}",
// //                                             row_idx, col_idx));
// //     if (safety_checks && (row_idx >= _row_count || col_idx >= _col_count))
// {
// //       throw std::runtime_error(
// //           std::format("TableView<>::cellAt() error : row_idx ({}) >= "
// //                       "_row_count ({}) || col_idx ({}) >= _col_count ({})
// !",
// //                       row_idx, _row_count, col_idx, _col_count));
// //     }
// //     return _data[row_idx * _col_count + col_idx];
// //   }

// //   const CELL_T &cellAt(coord_num_t row_idx, coord_num_t col_idx,
// //                        bool safety_checks = true) const {
// //     return cellAt(row_idx, col_idx, safety_checks);
// //   }

// //   LineView<CELL_T> operator[](coord_num_t row_idx) {
// //     if (row_idx >= _row_count) {
// //       throw std::runtime_error(std::format(
// //           "TableView<>::operator[] error : row_idx ({}) >= _row_count ({})
// //           !", row_idx, _row_count));
// //     }
// //     return {&(_data[row_idx * _col_count]), _col_count};
// //   }

// //   TableView<CELL_T> copy() const {
// //     auto res = TableView<CELL_T>{_row_count, _col_count};
// //     std::memcpy(res._data.get(), _data.get(),
// //                 sizeof(CELL_T) * _row_count * _col_count);
// //     return res;
// //   }

// //   std::string str() const {
// //     // return {};
// //     std::string res{};
// //     res.reserve(MAX_CELL_STR_SIZE * _row_count * _col_count);
// //     // std::stringstream res;
// //     coord_num_t offset{0};
// //     for (coord_num_t row_idx{0}; row_idx < _row_count; row_idx++) {
// //       for (coord_num_t col_idx{0}; col_idx < _col_count; col_idx++) {
// //         res += _data[offset++];
// //       }
// //       res += '\n';
// //     }
// //     return res;
// //   }

// // protected:
// //   std::shared_ptr<CELL_T[]> _data;
// //   coord_num_t _row_count{}, _col_count{};
// // };

// constexpr char EMPTY_CELL{'.'}, RED_CELL{'#'}, GREEN_CELL{'X'};

// // TableView<char> getBaseTable(const std::vector<cell_t> &coords) {
// //   coord_num_t max_row{0}, max_col{0};
// //   for (const auto &[col, row] : coords) {
// //     max_row = std::max(max_row, row);
// //     max_col = std::max(max_col, col);
// //   }
// //   auto res = TableView<char>{max_row + 2, max_col + 2};
// //   res.fillWith(EMPTY_CELL);
// //   for (const auto &[col, row] : coords) {
// //     res.cellAt(row, col) = RED_CELL;
// //   }
// //   return res;
// // }

// std::int8_t getDir(std::uint64_t a, std::uint64_t b) {
//   return a < b ? 1 : a == b ? 0 : -1;
// }

// // TableView<char> getBordersTable(const std::vector<cell_t> &coords) {
// //   coord_num_t max_row{0}, max_col{0};
// //   for (const auto &[col, row] : coords) {
// //     max_row = std::max(max_row, row);
// //     max_col = std::max(max_col, col);
// //   }
// //   auto res = TableView<char>{max_row + 2, max_col + 2};
// //   res.fillWith(EMPTY_CELL);
// //   // auto iter = coords.begin();
// //   for (std::size_t coord_idx{0}; coord_idx < coords.size(); coord_idx++) {
// //     auto &[curr_col, curr_row]{coords[coord_idx]};
// //     auto &[next_col, next_row]{coords[(coord_idx + 1) % coords.size()]};
// //     hlp::printLogEntries(
// //         {"BORD"},
// //         std::format(
// //             "getBordersTable() currently at border from ({},{}) to
// ({},{})",
// //             curr_row, curr_col, next_row, next_col));
// //     std::int8_t row_incr = getDir(curr_row, next_row);
// //     std::int8_t col_incr = getDir(curr_col, next_col);
// //     if (row_incr * col_incr != 0) {
// //       throw std::runtime_error("getBordersTable() error : diagonal border
// //       !");
// //     }
// //     for (std::int64_t row_idx{static_cast<int64_t>(curr_row) + row_incr},
// //          col_idx{static_cast<int64_t>(curr_col) + col_incr};
// //          row_idx != next_row || col_idx != next_col;
// //          row_idx += row_incr, col_idx += col_incr) {
// //       res.cellAt(row_idx, col_idx) = GREEN_CELL;
// //     }
// //     res.cellAt(curr_row, curr_col) = RED_CELL;
// //     res.cellAt(next_row, next_col) = RED_CELL;
// //   }
// //   return res;
// // }

// /*
//   Get set of border cells for rectangle with corners a and b
//  */
// std::unordered_set<cell_t> getRectangleBorders(const cell_t &a,
//                                                const cell_t &b) {
//   std::unordered_set<cell_t> res{};
//   const auto row_incr{getDir(a.row, b.row)}, col_incr{getDir(a.col, b.col)};
//   for (coord_num_t row{a.row}; row != a.row + row_incr; row += row_incr) {
//     res.insert({{row, a.col}, {row, b.col}});
//   }
//   for (coord_num_t col{a.col}; col != a.col + col_incr; col += col_incr) {
//     res.insert({{a.row, col}, {b.row, col}});
//   }
//   return res;
// }

// std::unordered_set<cell_t> getRectangleBorders(const Rectangle &rect) {
//   return getRectangleBorders(rect.corner_a, rect.corner_b);
// }

area_t computeArea(const Rectangle &rect) {
  return computeArea(rect.corner_a, rect.corner_b);
}

// using cell_set_t = std::unordered_set<cell_t>;

// /*
//   Find minimum table size needed to contain all coords (+ 1-cell margin
//   layer)
//  */
// std::pair<coord_num_t, coord_num_t>
// getTableSize(const std::vector<cell_t> &coords) {
//   coord_num_t max_row{0}, max_col{0};
//   for (const auto &[col, row] : coords) {
//     max_row = std::max(max_row, row);
//     max_col = std::max(max_col, col);
//   }
//   return {max_row + 2, max_col + 2};
// }

// /*
//   Iterate through coords and trace line between each pair of consecutive
//   cells
//  */
// cell_set_t getBordersCells(const std::vector<cell_t> &coords) {
//   auto [max_row, max_col] = getTableSize(coords);
//   cell_set_t res{};
//   for (std::size_t coord_idx{0}; coord_idx < coords.size(); coord_idx++) {
//     auto &[curr_col, curr_row]{coords[coord_idx]};
//     auto &[next_col, next_row]{coords[(coord_idx + 1) % coords.size()]};
//     // hlp::printLogEntries(
//     //     {"BORD"},
//     //     std::format(
//     //         "getBordersTable() currently at border from ({},{}) to
//     ({},{})",
//     //         curr_row, curr_col, next_row, next_col));
//     std::int8_t row_incr = getDir(curr_row, next_row);
//     std::int8_t col_incr = getDir(curr_col, next_col);
//     if (row_incr * col_incr != 0) {
//       throw std::runtime_error("getBordersTable() error : diagonal border
//       !");
//     }
//     for (std::int64_t row_idx{static_cast<int64_t>(curr_row) + row_incr},
//          col_idx{static_cast<int64_t>(curr_col) + col_incr};
//          row_idx != next_row || col_idx != next_col;
//          row_idx += row_incr, col_idx += col_incr) {
//       res.insert({row_idx, col_idx});
//     }
//     res.insert({next_row, next_col});
//   }
//   return res;
// }

// inline cell_set_t getNeighbors(const cell_t &cell) {
//   auto &[row, col] = cell;
//   return {{row - 1, col - 1}, {row - 1, col},    {row - 1, col + 1},
//           {row, col - 1},     {row, col + 1},    {row + 1, col - 1},
//           {row + 1, col},     {row + 1, col + 1}};
// }

// cell_set_t getNextLayer(const cell_set_t &exclude_set,
//                         const cell_set_t &curr_layer) {
//   cell_set_t next_layer{};
//   for (const auto &cell : curr_layer) {
//     auto neighbors = getNeighbors(cell);
//     for (const auto &neighbor : neighbors) {
//       if (!exclude_set.contains(neighbor)) {
//         next_layer.insert(neighbor);
//       }
//     }
//   }
//   return next_layer;
// }

// /*
//   Only one candidate (chosen depending on direction)
//  */
// template <typename CustomCellLess>
// cell_set_t getNextLayerDir(const cell_set_t &exclude_set,
//                            const cell_set_t &curr_layer) {
//   using my_set_t = std::set<cell_t, CustomCellLess>;
//   my_set_t next_layer{};
//   for (const auto &cell : curr_layer) {
//     auto neighbors = getNeighbors(cell);
//     for (const auto &neighbor : neighbors) {
//       if (!exclude_set.contains(neighbor)) {
//         next_layer.insert(neighbor);
//       }
//     }
//   }
//   if (next_layer.size() == 0) {
//     return {};
//   }
//   auto &res = *(next_layer.begin());
//   hlp::printLogEntries(
//       {"I"},
//       std::format("getNextLayerDir() - returning ({}, {})", res.row,
//       res.col));
//   return {res};
// }

// /*
//   Compute set of cells that constitute edges of general map
//  */
// cell_set_t getEdges(coord_num_t row_count, coord_num_t col_count) {
//   cell_set_t edges{};
//   for (coord_num_t row_idx{-1}; row_idx <= row_count; row_idx++) {
//     edges.insert({{row_idx, -1}, {row_idx, col_count}});
//   }
//   for (coord_num_t col_idx{-1}; col_idx <= col_count; col_idx++) {
//     edges.insert({{-1, col_idx}, {row_count, col_idx}});
//   }
//   return edges;
// }

// // TableView<char> tableWithArea(const cell_set_t &area, coord_num_t
// row_count,
// //                               coord_num_t col_count) {
// //   TableView<char> final_table{row_count, col_count};
// //   final_table.fillWith(EMPTY_CELL);
// //   for (const auto &[row, col] : area) {
// //     final_table.cellAt(row, col) = GREEN_CELL;
// //   }
// //   return final_table;
// // }

// void displayAreaEff(const cell_set_t &red_area, const cell_set_t &green_area,
//                     coord_num_t row_count, coord_num_t col_count) {
//   for (coord_num_t row{-1}; row <= row_count; row++) {
//     for (coord_num_t col{-1}; col <= col_count; col++) {
//       if (red_area.contains({row, col})) {
//         std::cout << RED_CELL;
//       } else if (green_area.contains({row, col})) {
//         std::cout << GREEN_CELL;
//       } else {
//         std::cout << EMPTY_CELL;
//       }
//     }
//     std::cout << '\n';
//   }
//   std::cout << std::flush;
// }

// // cell_set_t
// // growAreaToMax(const cell_set_t &exclude_set, const cell_set_t
// &initial_area,
// //               const std::pair<coord_num_t, coord_num_t>
// &row_and_col_count) {
// //   cell_set_t exclude{exclude_set}, final_area{initial_area},
// //       curr_layer{initial_area};
// //   while (curr_layer.size() > 0) {
// //     exclude.insert(curr_layer.begin(), curr_layer.end());
// //     final_area.insert(curr_layer.begin(), curr_layer.end());
// //     hlp::printLogEntries(
// //         {"D", "GROW"},
// //         std::format(
// //             "growAreaToMax() - current area (grew by {}, currently at {})
// :",
// //             curr_layer.size(), final_area.size()));
// //     // displayArea(final_area, row_and_col_count.first,
// //     // row_and_col_count.second);
// //     curr_layer = getNextLayer(exclude, curr_layer);
// //   }
// //   hlp::printLogEntries({"GROW"}, "Final area size :", final_area.size());
// //   return final_area;
// // }

// template <cell_t::num_t ROW_MUL, cell_t::num_t COL_MUL>
// struct CustomLess {
//   bool operator()(const cell_t &cell_a, const cell_t &cell_b) const {
//     // std::array<coord_num_t, 3> a_scores = {
//     //     ROW_MUL * cell_a.row + COL_MUL * cell_a.col, cell_a.row,
//     cell_a.col};
//     // std::array<coord_num_t, 3> b_scores = {
//     //     ROW_MUL * cell_b.row + COL_MUL * cell_b.col, cell_b.row,
//     cell_b.col};
//     //     return std::lexicographical_compare(a_scores.begin(),
//     a_scores.end(),
//     //                                         b_scores.begin(),
//     //                                         b_scores.end());
//     const coord_num_t a_score{ROW_MUL * cell_a.row + COL_MUL * cell_a.col},
//         b_score{ROW_MUL * cell_b.row + COL_MUL * cell_b.col};
//     return std::less()(a_score, b_score) ||
//            (a_score == b_score && std::less()(cell_a, cell_b));
//   }
// };

// struct DirectionalCellOrderings {
//   using TopLeft = CustomLess<1, 1>;
//   using Top = CustomLess<1, 0>;
//   using TopRight = CustomLess<1, -1>;
//   using Left = CustomLess<0, 1>;
//   using Right = CustomLess<0, -1>;
//   using BottomLeft = CustomLess<-1, 1>;
//   using Bottom = CustomLess<-1, 0>;
//   using BottomRight = CustomLess<-1, -1>;
// };

// template <typename CustomLess>
// struct CustomCellSetType {
//   using type = std::set<cell_t, CustomLess>;
// };

// template <typename CustomLess>
// using CustomCellSetType_t = CustomCellSetType<CustomLess>::type;

// struct CustomCellSetTypes {
//   using TopLeft = CustomCellSetType_t<DirectionalCellOrderings::TopLeft>;
//   using Top = CustomCellSetType_t<DirectionalCellOrderings::Top>;
//   using TopRight = CustomCellSetType_t<DirectionalCellOrderings::TopRight>;
//   using Left = CustomCellSetType_t<DirectionalCellOrderings::Left>;
//   using Right = CustomCellSetType_t<DirectionalCellOrderings::Right>;
//   using BottomLeft =
//   CustomCellSetType_t<DirectionalCellOrderings::BottomLeft>; using Bottom =
//   CustomCellSetType_t<DirectionalCellOrderings::Bottom>; using BottomRight =
//       CustomCellSetType_t<DirectionalCellOrderings::BottomRight>;
// };

// bool checkIfAreaSpilled(const cell_set_t &area, coord_num_t row_count,
//                         coord_num_t col_count) {
//   for (const auto &[row, col] : area) {
//     if (row * col == 0 || (1 + row - row_count) * (1 + col - col_count) == 0)
//     {
//       hlp::printLogEntries(
//           {"R"}, std::format("Found cell on edge : ({}, {})", row, col));
//       return true;
//     }
//   }
//   return false;
// }

// bool checkIfWillSpill(
//     const cell_set_t &exclude_set, const cell_set_t &initial_area,
//     const std::pair<coord_num_t, coord_num_t> &row_and_col_count) {
//   auto &[row_count, col_count] = row_and_col_count;
//   cell_set_t exclude{exclude_set}, final_area{initial_area},
//       curr_layer{initial_area};
//   std::uint64_t i{0};
//   const auto max_dim =
//       std::max(row_and_col_count.first, row_and_col_count.second);
//   while (curr_layer.size() > 0 && i < max_dim) {
//     if (i % 100 == 0) {
//       if (checkIfAreaSpilled(final_area, row_count, col_count)) {
//         return true;
//       }
//     }
//     exclude.insert(curr_layer.begin(), curr_layer.end());
//     final_area.insert(curr_layer.begin(), curr_layer.end());
//     hlp::printLogEntries({"D", "GROW"},
//                          std::format("growAreaToMax() - current area (grew by
//                          "
//                                      "{}, currently at {}, i at {}) :",
//                                      curr_layer.size(), final_area.size(),
//                                      i));
//     // displayArea(final_area, row_and_col_count.first,
//     // row_and_col_count.second);
//     curr_layer = getNextLayer(exclude, curr_layer);
//     i++;
//   }
//   hlp::printLogEntries({"GROW"}, "Final area size :", final_area.size());
//   return checkIfAreaSpilled(final_area, row_count, col_count);
// }

// template <typename CustomCellLess>
// bool checkIfWillSpillEff(
//     const cell_set_t &exclude_set, const cell_set_t &initial_area,
//     const std::pair<coord_num_t, coord_num_t> &row_and_col_count) {
//   auto &[row_count, col_count] = row_and_col_count;
//   cell_set_t exclude{exclude_set}, final_area{initial_area},
//       curr_layer{initial_area};
//   std::uint64_t i{0};
//   const auto max_dim =
//       std::max(row_and_col_count.first, row_and_col_count.second);
//   while (curr_layer.size() > 0 /* && i < max_dim */) {
//     if (i % 100 == 0) {
//       if (checkIfAreaSpilled(final_area, row_count, col_count)) {
//         return true;
//       }
//     }
//     exclude.insert(curr_layer.begin(), curr_layer.end());
//     final_area.insert(curr_layer.begin(), curr_layer.end());
//     hlp::printLogEntries({"D", "GROW"},
//                          std::format("growAreaToMax() - current area (grew by
//                          "
//                                      "{}, currently at {}, i at {}) :",
//                                      curr_layer.size(), final_area.size(),
//                                      i));
//     // displayArea(final_area, row_and_col_count.first,
//     // row_and_col_count.second);
//     curr_layer = getNextLayerDir<CustomCellLess>(exclude, curr_layer);
//     i++;
//   }
//   hlp::printLogEntries({"GROW"}, "Final area size :", final_area.size());
//   return checkIfAreaSpilled(final_area, row_count, col_count);
// }

// template <typename T>
// concept IsCellSetType =
//     std::same_as<T, std::set<cell_t, typename T::key_compare>> ||
//     std::same_as<T, std::unordered_set<cell_t, typename T::hasher>>;

// template <IsCellSetType CellSetType>
// bool exploreNextCandidate(cell_set_t &exclude_set, CellSetType &candidates,
//                           cell_set_t &final_area) {
//   bool res{false};
//   if (candidates.size() == 0) {
//     return res;
//   }
//   auto chosen_candidate_iter = candidates.begin();
//   auto neighbors = getNeighbors(*chosen_candidate_iter);
//   for (const auto &neighbor : neighbors) {
//     if (!exclude_set.contains(neighbor)) {
//       candidates.insert(neighbor);
//       final_area.insert(neighbor);
//       res = true;
//     }
//   }
//   exclude_set.insert(*chosen_candidate_iter);
//   candidates.erase(chosen_candidate_iter);
//   return res;
// }

// template <IsCellSetType CellSetType>
// bool checkIfAreaSpilled_v2(const CellSetType &area, coord_num_t row_count,
//                            coord_num_t col_count) {
//   for (const auto &[row, col] : area) {
//     if (row * col == 0 || (1 + row - row_count) * (1 + col - col_count) == 0)
//     {
//       hlp::printLogEntries(
//           {"R"}, std::format("Found cell on edge : ({}, {})", row, col));
//       return true;
//     }
//   }
//   return false;
// }

// template <IsCellSetType CellSetType>
// bool checkIfWillSpillEff_v2(
//     cell_set_t &exclude, CellSetType &candidates,
//     const std::pair<coord_num_t, coord_num_t> &row_and_col_count) {
//   auto &[row_count, col_count] = row_and_col_count;
//   cell_set_t final_area{candidates.begin(), candidates.end()};
//   std::uint64_t i{0};
//   const auto max_dim =
//       std::max(row_and_col_count.first, row_and_col_count.second);
//   while (candidates.size() > 0 /* && i < max_dim */) {
//     if (i % 500 == 0) {
//       if (checkIfAreaSpilled_v2<cell_set_t>(final_area, row_count,
//       col_count)) {
//         hlp::printLogEntries(
//             {"I"},
//             std::format(
//                 "checkIfWillSpillEff_v2() - Found spill at iteration n°{}",
//                 i));
//         return true;
//       }
//     }
//     // exclude.insert(candidates.begin(), candidates.end());
//     // final_area.insert(candidates.begin(), candidates.end());
//     hlp::printLogEntries(
//         {"D", "GROW"},
//         std::format("checkIfWillSpillEff_v2() - iteration n°{} - candidates "
//                     "count : {}, final area size : {}",
//                     i, candidates.size(), final_area.size()));
//     // displayArea(final_area, row_and_col_count.first,
//     // row_and_col_count.second);
//     bool found_new_candidates =
//         exploreNextCandidate<CellSetType>(exclude, candidates, final_area);
//     i++;
//   }
//   hlp::printLogEntries({"GROW"}, "Final area size :", final_area.size());
//   // displayAreaEff(final_area, exclude, row_count, col_count);
//   return checkIfAreaSpilled_v2<cell_set_t>(final_area, row_count, col_count);
// }

bool checkIfRangesIntersect(const Range &range_a, const Range &range_b) {
  auto &[min_a, max_a] = range_a;
  auto &[min_b, max_b] = range_b;
  return (min_a <= max_b && max_a >= min_b);
}

bool checkIfRectangleIsSplitAux(const Rectangle &rect, const cell_t &cell_a,
                                const cell_t &cell_b) {
  auto &[row_cell_a, col_cell_a] = cell_a;
  auto &[row_cell_b, col_cell_b] = cell_b;
  auto &[rect_a, rect_b] = rect;
  auto &[row_rect_a, col_rect_a] = rect_a;
  auto &[row_rect_b, col_rect_b] = rect_b;
  const coord_num_t max_line_row{std::max(row_cell_a, row_cell_b)},
      max_line_col{std::max(col_cell_a, col_cell_b)};
  const coord_num_t min_line_row{std::min(row_cell_a, row_cell_b)},
      min_line_col{std::min(col_cell_a, col_cell_b)};
  const coord_num_t max_rect_row{std::max(row_rect_a, row_rect_b) - 1},
      max_rect_col{std::max(col_rect_a, col_rect_b) - 1};
  const coord_num_t min_rect_row{std::min(row_rect_a, row_rect_b) + 1},
      min_rect_col{std::min(col_rect_a, col_rect_b) + 1};
  const Range range_line_row{min_line_row, max_line_row},
      range_line_col{min_line_col, max_line_col};
  const Range range_rect_row{min_rect_row, max_rect_row},
      range_rect_col{min_rect_col, max_rect_col};
  return checkIfRangesIntersect(range_line_row, range_rect_row) &&
         checkIfRangesIntersect(range_line_col, range_rect_col);
}

bool checkIfRectangleIsSplit(const Rectangle &rect,
                             const std::vector<cell_t> &coords) {
  for (std::size_t i{0}; i < coords.size(); i++) {
    auto &cell_a = coords.at(i);
    auto &cell_b = coords.at((i + 1) % coords.size());
    if (checkIfRectangleIsSplitAux(rect, cell_a, cell_b)) {
      return true;
    }
  }
  return false;
}

std::string rectangleToStr(const Rectangle &rect) {
  return std::format("({}, {}), ({}, {})", rect.corner_a.row, rect.corner_a.col,
                     rect.corner_b.row, rect.corner_b.col);
}

Rectangle findLargestUnsplitRectangle(
    const std::set<Rectangle, RectangleLessOnArea> &rectangles_ordered,
    const std::vector<cell_t> &coords) {
  if (hlp::LOG_CATS_MAP["D"].first) {
    for (auto &rect : rectangles_ordered) {
      hlp::printLogEntries(
          {"D"}, "findLargestUnsplitRectangle :", rectangleToStr(rect),
          std::format("(area : {})", computeArea(rect)));
    }
  }
  for (auto rect_iter = rectangles_ordered.rbegin();
       rect_iter != rectangles_ordered.rend(); rect_iter++) {
    if (!checkIfRectangleIsSplit(*rect_iter, coords)) {
      return *rect_iter;
    }
  }
  throw std::runtime_error("findLargestUnsplitRectangle() error : couldn't "
                           "find unsplit rectangle !");
}

std::set<Rectangle, RectangleLessOnArea>
getRectanglesOrdered(const std::vector<cell_t> &coords) {
  std::set<Rectangle, RectangleLessOnArea> final_set{};
  for (std::size_t i{0}; i < coords.size(); i++) {
    for (std::size_t j{i + 1}; j < coords.size(); j++) {
      final_set.insert({coords.at(i), coords.at(j)});
    }
  }
  return final_set;
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

  std::set<Rectangle, RectangleLessOnArea> le_rectangles_ordered =
      getRectanglesOrdered(le_coords);

  if (le_rectangles_ordered.size() == 0) {
    throw std::runtime_error("Error : no rectangles in set !");
  }

  auto largest_rectangle = *(le_rectangles_ordered.rbegin());
  // largest_rectangle = *(--rectangles_set.end());
  hlp::printLogEntries({"R"}, "Got largest rectangle !");
  auto &[pt_a, pt_b] = largest_rectangle;
  auto res_area = computeArea(pt_a, pt_b);

  hlp::printLogEntries(
      {"R"}, "Largest rectangle (Part 1) :", rectangleToStr(largest_rectangle));
  hlp::printLogEntries({"R"}, "Area :", res_area);

  // PART 2

  hlp::LOG_CATS_MAP["I"].first = false;
  hlp::LOG_CATS_MAP["D"].first = false;
  hlp::LOG_CATS_MAP["VIZ"] = std::make_pair(false, "VIZ");
  hlp::LOG_CATS_MAP["GROW"] = std::make_pair(false, "GROW");
  // hlp::LOG_CATS_MAP["BORD"] = std::make_pair(true, "BORDERS");

  // auto le_base_table = getBaseTable(le_coords);
  // auto le_borders_table = getBordersTable(le_coords);

  // hlp::printLogEntries({"I"}, "Base table :", '\n' + le_base_table.str());

  // hlp::printLogEntries({"I", {"VIZ"}}, "Got (not) borders table");

  // hlp::printLogEntries({"I", "VIZ"},
  //                      "Table with borders :", '\n' +
  //                      le_borders_table.str());

  // auto [le_rc, le_cc] = getTableSize(le_coords);
  // hlp::printLogEntries(
  //     {"I"}, "Row count and col count :", std::format("{}, {}", le_rc,
  //     le_cc));
  // auto le_borders = getBordersCells(le_coords);
  // auto le_edges = getEdges(le_rc, le_cc);
  // le_borders.insert(le_edges.begin(), le_edges.end());
  // hlp::printLogEntries({"I"}, "About to call growAreaToMax()");
  // auto le_test_area =
  //     checkIfWillSpill(le_borders, {{70000, 2000}}, {le_rc, le_cc});
  // auto [start_row, start_col] = std::make_pair<coord_num_t>(1, 1);
  // auto [start_row, start_col] =
  //     std::make_pair<coord_num_t>(97498 + 1, 51566 - 1);
  // auto le_test_area = checkIfWillSpillEff<DirectionalCellOrderings::TopLeft>(
  //     le_borders, {{start_row, start_col}}, {le_rc, le_cc});
  // CustomCellSetTypes::TopLeft le_candidates = {{start_row, start_col}};
  // auto will_spill = checkIfWillSpillEff_v2<
  //     CustomCellSetType_t<DirectionalCellOrderings::TopLeft>>(
  //     le_borders, le_candidates, std::make_pair(le_rc, le_cc));
  // std::cout << "le_test_area : " << std::boolalpha << will_spill <<
  // std::endl;

  // hlp::printLogEntries({"I"}, tableWithArea(le_test_area, le_rc,
  // le_cc).str());

  auto le_largest_rect_p2 =
      findLargestUnsplitRectangle(le_rectangles_ordered, le_coords);
  auto &[corner_a, corner_b] = le_largest_rect_p2;

  hlp::printLogEntries(
      {"R"}, "Found largest unsplit rectangle (Part 2) :",
      rectangleToStr(le_largest_rect_p2),
      std::format("Area : {}", computeArea(le_largest_rect_p2)));
  // CustomCellSetType_t<DirectionalCellOrderings::TopLeft> candidates_set = {
  //     {start_row, start_col}};
  // cell_set_t final_area{};
  // exploreBestCandidate<CustomCellSetType_t<DirectionalCellOrderings::TopLeft>>(
  //     le_borders, candidates_set, final_area);
}