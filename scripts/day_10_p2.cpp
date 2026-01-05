#include "helpers.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace hlp = helper;

using count_t = std::size_t;
using incr_or_nots_t = std::uint16_t;
using press_or_nots_t = std::uint16_t;
using button_t = incr_or_nots_t;
using press_counts_t = std::vector<count_t>;
using joltages_t = std::vector<count_t>;

count_t countPresses(press_or_nots_t press_or_nots) {
  count_t res{0};
  while (press_or_nots > 0) {
    res += (press_or_nots & 1);
    press_or_nots >>= 1;
  }
  return res;
}

count_t countIncrements(incr_or_nots_t incr_or_nots) {
  count_t res{0};
  while (incr_or_nots > 0) {
    res += (incr_or_nots & 1);
    incr_or_nots >>= 1;
  }
  return res;
}

int main(int argc, char *argv[]) {
  const std::string le_filename{"../inputs/day_10_input.txt"};
  // auto le_content = hlp::getFileContentRaw(le_filename);
  auto le_lines = hlp::getFileContentAslines(le_filename);
  // auto le_table = hlp::getFileContentAsTable(le_filename);

  // node : binary
  // heuristic : countOnes(node) + (countOnes(goal) - countOnes(current)) /
  // countOnes(biggest_button)
  // candidates : {count : } start : 0b00000
}