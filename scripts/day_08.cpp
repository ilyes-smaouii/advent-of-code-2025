#include "helpers.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <format>
#include <functional>
#include <istream>
#include <limits>
#include <map>
#include <numeric>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
// #include <utility>
#include <utility>
#include <vector>
// #include <tuple>

namespace hlp = helper;

using num_t = std::uint64_t;

using box_id_t = std::size_t;
using group_id_t = std::size_t;
using group_size_t = std::size_t;

struct Coordinates {
  Coordinates() = default;
  Coordinates(num_t xx, num_t yy, num_t zz) : x(xx), y(yy), z(zz) {}

  Coordinates(const std::string &coordinates_str) {
    std::stringstream str_stream{coordinates_str};
    str_stream >> this->x;
    str_stream.ignore(1);
    str_stream >> this->y;
    str_stream.ignore(1);
    str_stream >> this->z;
  }

  std::string toStr() const { return std::format("{},{},{}", x, y, z); }

  bool operator==(const Coordinates &coord_b) const = default;
  auto operator<=>(const Coordinates &coord_b) const = default;
  num_t x{};
  num_t y{};
  num_t z{};
};

namespace std {
template <>
struct hash<Coordinates> {
  std::size_t operator()(const Coordinates &coord) const {
    auto hasher = std::hash<num_t>();
    auto res = hasher(coord.x);
    res ^= hasher(coord.y) + 0x9e3779b9 + (res << 6) + (res >> 2);
    res ^= hasher(coord.z) + 0x9e3779b9 + (res << 6) + (res >> 2);
    return res;
  }
};

template <>
struct less<Coordinates> {
  bool operator()(const Coordinates &coord_a,
                  const Coordinates &coord_b) const {
    std::array arr_a{coord_a.x, coord_a.y, coord_a.z};
    std::array arr_b{coord_b.x, coord_b.y, coord_b.z};
    return std::lexicographical_compare(arr_a.begin(), arr_a.end(),
                                        arr_b.begin(), arr_b.end());
  }
};
} // namespace std

std::ostream &operator<<(std::ostream &os, const Coordinates &coord) {
  os << coord.x << "," << coord.y << "," << coord.z;
  return os;
}

std::istream &operator>>(std::istream &is, Coordinates &coord) {
  is >> coord.x;
  is.ignore(1);
  is >> coord.y;
  is.ignore(1);
  is >> coord.z;
  return is;
}

void operator>>(const std::string &str, Coordinates &coord) {
  std::stringstream str_stream{str};
  str_stream >> coord;
}

// using Coordinates = std::tuple<num_t, num_t, num_t>;
// using Coordinates = Coordinates;

std::vector<Coordinates>
parseCoordinates(const std::vector<std::string> &lines) {
  std::vector<Coordinates> coordinates_vec{};
  for (const auto &line : lines) {
    coordinates_vec.emplace_back(line);
  }
  return coordinates_vec;
}

num_t computeDistanceSquared(const Coordinates &pos_a,
                             const Coordinates &pos_b) {
  return (pos_a.x - pos_b.x) * (pos_a.x - pos_b.x) +
         (pos_a.y - pos_b.y) * (pos_a.y - pos_b.y) +
         (pos_a.z - pos_b.z) * (pos_a.z - pos_b.z);
}

std::vector<Coordinates> le_coordinates_vec{};

struct box_pairs_compare {

  bool operator()(const std::pair<std::size_t, std::size_t> &pair_a,
                  const std::pair<std::size_t, std::size_t> &pair_b) const {
    const auto &[a_1, a_2] = pair_a;
    const auto &[b_1, b_2] = pair_b;
    [[unlikely]] if (std::max({a_1, a_2, b_1, b_2}) >=
                     le_coordinates_vec.size()) {
      throw std::runtime_error("Compare by ID error : couldn't find IDs !");
    } else {
      auto dist_a = computeDistanceSquared(le_coordinates_vec[a_1],
                                           le_coordinates_vec[a_2]);
      auto dist_b = computeDistanceSquared(le_coordinates_vec[b_1],
                                           le_coordinates_vec[b_2]);
      return std::less<num_t>()(dist_a, dist_b);
    }
  }
};

struct box_pairs_compare_stateless {
  bool operator()(const std::pair<Coordinates, Coordinates> &coords_a,
                  const std::pair<Coordinates, Coordinates> &coords_b) const {
    const auto &[a_1, a_2] = coords_a;
    const auto &[b_1, b_2] = coords_b;
    auto const dist_a = computeDistanceSquared(a_1, a_2);
    auto const dist_b = computeDistanceSquared(b_1, b_2);
    return std::tie(dist_a, a_1, a_2) < std::tie(dist_b, b_1, b_2);
  }
};

struct group_sizes_compare {
  bool operator()(const std::pair<group_id_t, group_size_t> &group_a,
                  const std::pair<group_id_t, group_size_t> &group_b) const {
    auto &[id_a, size_a] = group_a;
    auto &[id_b, size_b] = group_b;
    return std::less()(size_a, size_b) ||
           (size_a == size_b && std::less()(id_a, id_b));
  }
};

int main(int argc, char *argv[]) {
  const std::string le_filename{"../inputs/day_08_input.txt"};
  // auto le_content = hlp::getFileContentRaw(le_filename);
  auto le_lines = hlp::getFileContentAslines(le_filename);
  auto le_test_lines = hlp::rawToLines(R"(162,817,812
57,618,57
906,360,560
592,479,940
352,342,300
466,668,158
542,29,236
431,825,988
739,650,466
52,470,668
216,146,977
819,987,18
117,168,530
805,96,715
346,949,466
970,615,88
941,993,340
862,61,35
984,92,344
425,690,689)");

  // le_lines = le_test_lines;

  // auto le_table = hlp::getFileContentAsTable(le_filename);

  // PART 1

  hlp::LOG_CATS_MAP["I"].first = true;
  hlp::printLogEntries({"I"}, "Got lines");

  le_coordinates_vec = parseCoordinates(le_lines);
  hlp::printLogEntries(
      {"I"}, "Number of coordinates in vec :", le_coordinates_vec.size());

  // auto &coord = le_coordinates_vec.back();
  // std::cout << coord << std::endl;

  // std::unordered_map<std::size_t, std::pair<std::size_t, num_t>>
  //     box_to_closest_map{};
  // for (std::size_t i{0}; i < le_coordinates_vec.size(); i++) {
  //   num_t min_dist = std::numeric_limits<num_t>::max();
  //   std::size_t min_idx{std::numeric_limits<std::size_t>::max()};
  //   num_t curr_dist = min_dist;
  //   for (std::size_t j{i + 1}; j < le_coordinates_vec.size(); j++) {
  //     curr_dist =
  //         computeDistanceSquared(le_coordinates_vec[i],
  //         le_coordinates_vec[j]);
  //     if (curr_dist < min_dist) {
  //       min_dist = curr_dist;
  //       min_idx = j;
  //     }
  //     min_dist = std::min(min_dist, curr_dist);
  //   }
  //   box_to_closest_map[i] = {min_idx, min_dist};
  // }

  // std::map<std::pair<box_id_t, box_id_t>, num_t, box_pairs_compare>
  //     shortest_distances_map{};
  std::map<std::pair<Coordinates, Coordinates>, num_t,
           box_pairs_compare_stateless>
      shortest_distances_map_2{};

  // num_t curr_dist{0};
  // for (std::size_t i{0}; i < le_coordinates_vec.size(); i++) {
  //   for (std::size_t j{i + 1}; j < le_coordinates_vec.size(); j++) {
  //     shortest_distances_map.insert(std::make_pair(
  //         std::make_pair(i, j), computeDistanceSquared(le_coordinates_vec[i],
  //                                                      le_coordinates_vec[j])));
  //   }
  // }

  for (std::size_t i{0}; i < le_coordinates_vec.size(); i++) {
    for (std::size_t j{i + 1}; j < le_coordinates_vec.size(); j++) {
      auto &box_a{le_coordinates_vec[i]}, &box_b{le_coordinates_vec[j]};
      shortest_distances_map_2.insert(std::make_pair(
          std::make_pair(box_a, box_b), computeDistanceSquared(box_a, box_b)));
    }
  }

  std::size_t iteration_count(std::stoi(argv[1]));

  std::unordered_map<group_id_t, group_size_t> group_sizes_map{};
  // group_sizes_map.reserve(iteration_count);
  // std::unordered_map<box_id_t, group_id_t> box_to_group_map{};
  std::unordered_map<Coordinates, group_id_t> box_to_group_map_2{};
  // box_to_group_map.reserve(iteration_count);

  // auto shortest_iter = shortest_distances_map.begin();
  auto shortest_iter_2 = shortest_distances_map_2.begin();
  std::size_t lowest_unused_group_id{0};

  hlp::LOG_CATS_MAP["D"].first = true;

  for (box_id_t box_id{0}; box_id < le_coordinates_vec.size(); box_id++) {
    // box_to_group_map.insert(std::make_pair(box_id, lowest_unused_group_id));
    box_to_group_map_2.insert(
        std::make_pair(le_coordinates_vec[box_id], lowest_unused_group_id));
    group_sizes_map.insert(std::make_pair(lowest_unused_group_id, 1));
    lowest_unused_group_id++;
  }

  for (std::size_t i{0}; i < iteration_count &&
                         shortest_iter_2 != shortest_distances_map_2.end();) {

    hlp::printLogEntries({"I"}, "Current box pair :",
                         std::format("({}), ({})",
                                     (shortest_iter_2->first.first).toStr(),
                                     (shortest_iter_2->first.second).toStr()));

    // auto &[box_a, box_b] = shortest_iter->first;
    auto &[box_a, box_b] = shortest_iter_2->first;

    // auto a_group = box_to_group_map.find(box_a)->second;
    // auto b_group = box_to_group_map.find(box_b)->second;
    auto a_group = box_to_group_map_2.find(box_a)->second;
    auto b_group = box_to_group_map_2.find(box_b)->second;
    if (a_group != b_group) {
      hlp::printLogEntries({"D"}, "A group != B group");
      auto a_group_size_iter = group_sizes_map.find(a_group);
      auto b_group_size_iter = group_sizes_map.find(b_group);
      if (a_group_size_iter == group_sizes_map.end() ||
          b_group_size_iter == group_sizes_map.end()) {
        throw std::runtime_error(
            "Error : A and B's groups should have sizes !");
      }

      a_group_size_iter->second += b_group_size_iter->second;
      for (auto &[box, group] : box_to_group_map_2) {
        if (group == b_group) {
          group = a_group;
        }
      }
      hlp::printLogEntries(
          {"D"},
          std::format("Erasing b_group {} from group_sizes_map", b_group));
      group_sizes_map.erase(b_group_size_iter);
    } else {
      // i--;
    }
    i++;

    shortest_iter_2++;
  }

  hlp::printLogEntries(
      {"I"}, std::format("Got {} distinct groups", group_sizes_map.size()));
  auto group_sizes_sum = std::accumulate(
      group_sizes_map.begin(), group_sizes_map.end(), 0,
      [](std::size_t acc, const auto &gs) { return acc + gs.second; });
  hlp::printLogEntries(
      {"I"},
      std::format("Total number of boxes in groups : {}", group_sizes_sum));

  std::set<std::pair<group_id_t, group_size_t>, group_sizes_compare>
      group_sizes{};
  for (const auto &[id, size] : group_sizes_map) {
    group_sizes.insert(std::make_pair(id, size));
  }
  auto it = group_sizes.end();
  std::size_t res = std::accumulate(
      group_sizes.rbegin(), std::next(group_sizes.rbegin(), 3),
      static_cast<std::uint64_t>(1), [](std::uint64_t acc, auto it_b) {
        auto res = acc * (it_b.second);
        std::cout << "acc * " << (it_b.second) << " : " << res << std::endl;
        return res;
      });
  hlp::printLogEntries({"R"}, "Final result :", res);

  group_sizes_map.clear();
  // group_sizes_map.reserve(iteration_count);
  // std::unordered_map<box_id_t, group_id_t> box_to_group_map{};
  box_to_group_map_2.clear();
  // box_to_group_map.reserve(iteration_count);

  // auto shortest_iter = shortest_distances_map.begin();
  shortest_iter_2 = shortest_distances_map_2.begin();
  lowest_unused_group_id = 0;

  for (box_id_t box_id{0}; box_id < le_coordinates_vec.size(); box_id++) {
    // box_to_group_map.insert(std::make_pair(box_id, lowest_unused_group_id));
    box_to_group_map_2.insert(
        std::make_pair(le_coordinates_vec[box_id], lowest_unused_group_id));
    group_sizes_map.insert(std::make_pair(lowest_unused_group_id, 1));
    lowest_unused_group_id++;
  }

  std::pair<Coordinates, Coordinates> latest_pair{};
  auto &[box_a, box_b] = latest_pair;
  while (group_sizes_map.size() > 1) {
    latest_pair = shortest_iter_2->first;
    auto &[box_a, box_b] = latest_pair;

    // auto a_group = box_to_group_map.find(box_a)->second;
    // auto b_group = box_to_group_map.find(box_b)->second;
    auto a_group = box_to_group_map_2.find(box_a)->second;
    auto b_group = box_to_group_map_2.find(box_b)->second;
    if (a_group != b_group) {
      hlp::printLogEntries({"D"}, "A group != B group");
      auto a_group_size_iter = group_sizes_map.find(a_group);
      auto b_group_size_iter = group_sizes_map.find(b_group);
      if (a_group_size_iter == group_sizes_map.end() ||
          b_group_size_iter == group_sizes_map.end()) {
        throw std::runtime_error(
            "Error : A and B's groups should have sizes !");
      }

      a_group_size_iter->second += b_group_size_iter->second;
      for (auto &[box, group] : box_to_group_map_2) {
        if (group == b_group) {
          group = a_group;
        }
      }
      hlp::printLogEntries(
          {"D"},
          std::format("Erasing b_group {} from group_sizes_map", b_group));
      group_sizes_map.erase(b_group_size_iter);
    }

    shortest_iter_2++;
  }

  auto res_p2 = box_a.x * box_b.x;

  hlp::printLogEntries({"R"}, "Found result (part 2) :", res_p2);
}