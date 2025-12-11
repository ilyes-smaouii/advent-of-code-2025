#include "helpers.hpp"

#include <cstdint>
#include <format>
#include <functional>
#include <istream>
#include <limits>
#include <map>
#include <numeric>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
// #include <utility>
#include <utility>
#include <vector>
// #include <tuple>

namespace hlp = helper;

using num_t = std::uint32_t;

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
  num_t x{};
  num_t y{};
  num_t z{};
};

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

int main(int argc, char *argv[]) {
  const std::string le_filename{"../inputs/day_08_input.txt"};
  // auto le_content = hlp::getFileContentRaw(le_filename);
  auto le_lines = hlp::getFileContentAslines(le_filename);
  // auto le_table = hlp::getFileContentAsTable(le_filename);

  // PART 1

  hlp::LOG_CATS_MAP["I"].first = true;

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

  std::map<std::pair<std::size_t, std::size_t>, num_t, box_pairs_compare>
      shortest_distances_map{};

  num_t curr_dist{0};
  for (std::size_t i{0}; i < le_coordinates_vec.size(); i++) {
    for (std::size_t j{i + 1}; j < le_coordinates_vec.size(); j++) {
      curr_dist =
          computeDistanceSquared(le_coordinates_vec[i], le_coordinates_vec[j]);
      shortest_distances_map.insert(
          std::make_pair(std::make_pair(i, j), curr_dist));
    }
  }

  std::size_t iteration_count(std::stoi(argv[1]));

  std::unordered_map<std::size_t, std::size_t> group_sizes_map{};
  // group_sizes_map.reserve(iteration_count);
  std::unordered_map<std::size_t, std::size_t> box_to_group_map{};
  // box_to_group_map.reserve(iteration_count);

  auto shortest_iter = shortest_distances_map.begin();
  std::size_t unused_group_id{0};

  hlp::LOG_CATS_MAP["D"].first = true;

  for (std::size_t i{0}; i < std::stoi(argv[1]); i++) {

    hlp::printLogEntries({"I"}, "Current box pair :",
                         std::format("{}, {}", shortest_iter->first.first,
                                     shortest_iter->first.second));

    auto &[box_a, box_b] = shortest_iter->first;

    auto box_a_group_iter = box_to_group_map.find(box_a);
    auto box_b_group_iter = box_to_group_map.find(box_b);
    // bool a_has_group = box_a_group_iter != box_to_group_map.end();
    // bool b_has_group = box_b_group_iter != box_to_group_map.end();

    // First, make sure A and B are assigned to groups if it's not already the
    // case
    if (box_a_group_iter == box_to_group_map.end()) {
      hlp::printLogEntries({"D"}, "A has no group");
      const auto a_group_id = unused_group_id++;
      hlp::printLogEntries({"D"}, std::format("Making group {} for A box {}",
                                              unused_group_id - 1, box_a));
      group_sizes_map.insert(std::make_pair(a_group_id, 1));
      box_to_group_map.insert(std::make_pair(box_a, a_group_id));
      box_a_group_iter = box_to_group_map.find(box_a);
    }
    if (box_b_group_iter == box_to_group_map.end()) {
      hlp::printLogEntries({"D"}, "B has no group");
      const auto b_group_id = unused_group_id++;
      hlp::printLogEntries({"D"}, std::format("Making group {} for B box {}",
                                              unused_group_id - 1, box_a));
      group_sizes_map.insert(std::make_pair(b_group_id, 1));
      box_to_group_map.insert(std::make_pair(box_b, b_group_id));
      box_b_group_iter = box_to_group_map.find(box_b);
    }

    // Then, fuse the corresponding groups

    auto a_group = box_a_group_iter->second;
    auto b_group = box_b_group_iter->second;
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
      for (auto &box_group_iter : box_to_group_map) {
        if (box_b_group_iter->second == b_group) {
          box_b_group_iter->second = a_group;
        }
      }
      hlp::printLogEntries(
          {"D"},
          std::format("Erasing b_group {} from group_sizes_map", b_group));
      group_sizes_map.erase(b_group_size_iter);
    } else {
      // i--;
    }

    shortest_iter++;
  }

  hlp::printLogEntries(
      {"I"}, std::format("Got {} distinct groups with more than 1 member",
                         group_sizes_map.size()));
  auto group_sizes_sum = std::accumulate(
      group_sizes_map.begin(), group_sizes_map.end(), 0,
      [](std::size_t acc, const auto &gs) { return acc + gs.second; });
  hlp::printLogEntries(
      {"I"},
      std::format("Total number of boxes in groups : {}", group_sizes_sum));
}