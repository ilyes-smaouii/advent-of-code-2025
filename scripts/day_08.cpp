#include "helpers.hpp"

#include <cstdint>
#include <istream>
#include <limits>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
// #include <utility>
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

int main(int argc, char *argv[]) {
  const std::string le_filename{"../inputs/day_08_input.txt"};
  // auto le_content = hlp::getFileContentRaw(le_filename);
  auto le_lines = hlp::getFileContentAslines(le_filename);
  // auto le_table = hlp::getFileContentAsTable(le_filename);

  // PART 1

  auto le_coordinates_vec = parseCoordinates(le_lines);
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
  //         computeDistanceSquared(le_coordinates_vec[i], le_coordinates_vec[j]);
  //     if (curr_dist < min_dist) {
  //       min_dist = curr_dist;
  //       min_idx = j;
  //     }
  //     min_dist = std::min(min_dist, curr_dist);
  //   }
  //   box_to_closest_map[i] = {min_idx, min_dist};
  // }

  std::map<num_t, std::pair<std::size_t, std::size_t>> shortest_distances_map{};

  num_t curr_dist{0};
  for (std::size_t i{0}; i < le_coordinates_vec.size(); i++) {
    for (std::size_t j{i + 1}; j < le_coordinates_vec.size(); j++) {
      curr_dist =
          computeDistanceSquared(le_coordinates_vec[i], le_coordinates_vec[j]);
      shortest_distances_map[curr_dist] = {i, j};
    }
  }
  for (std::size_t i{0}; i < 1000; i++) {
    auto shortest_iter = shortest_distances_map.begin();

    // TO-DO : Do stuff here

    shortest_iter++;
  }
}