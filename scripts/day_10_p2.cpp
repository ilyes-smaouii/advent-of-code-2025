#include "helpers.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <ios>
#include <numeric>
#include <ostream>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace hlp = helper;

using count_t = std::size_t;
using small_count_t = std::uint16_t;
using joltage_t = count_t;
using incr_or_nots_t = std::uint16_t;
using press_or_nots_t = std::uint16_t;
// using button_t = incr_or_nots_t;
using button_t = std::vector<small_count_t>;
using buttons_t = std::vector<button_t>;
using press_counts_t = std::vector<count_t>;
using joltages_t = std::vector<joltage_t>;

constexpr std::size_t MAX_LIGHT_COUNT{16};
constexpr std::size_t MAX_BUTTON_COUNT{32};
// constexpr std::size_t MAX_PRESS_COUNT{16};

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

count_t countIncrements_v2(const button_t &button) {
  count_t res{0};
  for (const auto &incr_or_not : button) {
    res += incr_or_not;
  }
  return res;
}

std::pair<std::vector<button_t>, joltages_t>
parseLine_p2(const std::string &line) {
  auto res = std::make_pair<std::vector<button_t>, joltages_t>({}, {});
  auto &[buttons, goal_jolts] = res;
  buttons.reserve(MAX_BUTTON_COUNT);
  goal_jolts.reserve(MAX_LIGHT_COUNT);
  auto line_iter = line.begin();
  while (*(line_iter++) != ' ') { // skip light diagram
  }
  while (*line_iter != '{') { // process buttons_str
    button_t curr_button{};
    curr_button.reserve(MAX_LIGHT_COUNT);
    if (*line_iter != '(') {
      throw std::runtime_error("parseLine_p2() error : expected '(' !");
    }
    while (*line_iter != ')') {
      small_count_t curr_num{0};
      while ('0' <= *(++line_iter) && *line_iter <= '9') {
        curr_num *= 10;
        curr_num += (*line_iter) - '0';
        // line_iter++;
      }
      curr_button.push_back(curr_num);
    }
    buttons.push_back(curr_button);
    line_iter +=
        2; // skip to next unit, whether it's another button_str or jolt_str
  }
  if (*line_iter != '{') {
    throw std::runtime_error("parseLine_p2() error : expected '{' !");
  }
  while (*line_iter != '}') {
    count_t curr_jolt{0};
    while ('0' <= *(++line_iter) && *line_iter <= '9') {
      curr_jolt *= 10;
      curr_jolt += *line_iter - '0';
    }
    goal_jolts.push_back(curr_jolt);
    if (*line_iter == ',') {
      // line_iter++;
    }
  }
  return res;
}

// std::ostream &operator<<(std::ostream &os,
//                          const std::vector<std::vector<std::uint8_t>>&
//                          buttons) {
//   for (const auto &button : buttons) {
//     os << '{' << hlp::vectorToString(button) << "} ";
//   }
//   return os;
// }

using node_t =
    std::tuple<press_counts_t, count_t, joltages_t, count_t, count_t>;
constexpr std::size_t PRESSES{0}, PRESSES_SUM{1}, JOLT_LVLS{2}, JOLTS_SUM{3},
    EST{4};

struct NodeLess {
  bool operator()(const node_t &a, const node_t &b) const {
    auto &a_press_counts = std::get<PRESSES>(a),
         &b_press_counts = std::get<PRESSES>(b);
    auto &a_presses_sum = std::get<PRESSES_SUM>(a);
    auto &b_presses_sum = std::get<PRESSES_SUM>(b);
    auto &a_est = std::get<EST>(a), &b_est = std::get<EST>(b);
    // if (std::equal(a_press_counts.begin(), a_press_counts.end(),
    //                b_press_counts.begin(), b_press_counts.end())) {
    //   return false;
    // } else {
    //   return std::less()(a_presses_sum + a_est, b_presses_sum + b_est) ||
    //          (a_presses_sum + a_est == b_presses_sum + b_est && );
    // }
    const auto a_score = a_presses_sum + a_est, b_score = b_presses_sum + b_est;
    [[unlikely]] if (a_score == b_score) {
      return std::lexicographical_compare(
          a_press_counts.begin(), a_press_counts.end(), b_press_counts.begin(),
          b_press_counts.end());
    } else {
      return std::less()(a_score, b_score);
    }
  }
};
struct NodeHash {
  std::size_t operator()(const node_t &a) const {
    auto &presses_a = std::get<PRESSES>(a);
    // Code below copied from here : https://stackoverflow.com/a/72073933
    std::size_t seed = presses_a.size();
    for (auto x : presses_a) {
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = (x >> 16) ^ x;
      seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
  }
};

std::pair<button_t, count_t> findBiggestButton(const buttons_t &buttons) {
  count_t max_size{0};
  button_t res{};
  for (const auto &button : buttons) {
    auto curr_size = std::accumulate(button.begin(), button.end(), 0,
                                     std::plus<small_count_t>());
    if (curr_size > max_size) {
      max_size = curr_size;
      res = button;
    }
  }
  return {res, max_size};
}

// candidates = {start_presses: {"total_presses": 0, "jolt_levels":
// start_joltage_levels, "est_to_goal": goal_jolt_sum}}

joltages_t tryButtonPresses(const buttons_t &buttons,
                            const press_counts_t &press_counts) {
  joltages_t res{buttons.size(), static_cast<joltages_t::value_type>(0)};
  if (buttons.size() != press_counts.size()) {
    std::stringstream msg_appendix{};
    hlp::operator<<(hlp::operator<<(msg_appendix << "Buttons :\n", buttons)
                        << "\nPress counts :\n",
                    press_counts);
    throw std::runtime_error("tryButtonPresses() error - buttons and "
                             "press_counts should have the same size !\n" +
                             msg_appendix.str());
  }
  for (std::size_t i{0}; i < buttons.size(); i++) {
    for (const auto &light_idx : buttons[i]) {
      res[light_idx] += press_counts[i];
    }
  }
  return res;
}

count_t getButtonSize(const button_t &button) {
  return std::accumulate(button.begin(), button.end(), 0, std::plus());
}

node_t findMinButtonPresses(const buttons_t &buttons,
                            const joltages_t &goal_joltages) {
  const count_t light_count{goal_joltages.size()};
  const count_t button_count{buttons.size()};
  auto initial_press_counts = press_counts_t(button_count, 0);
  auto initial_jolt_levels = joltages_t(light_count, 0);
  auto [biggest_button, biggest_button_size] = findBiggestButton(buttons);
  count_t goal_jolt_sum = std::accumulate(
      goal_joltages.begin(), goal_joltages.end(), 0, std::plus<joltage_t>());
  //
  std::set<node_t, NodeLess> candidates{};
  std::unordered_set<node_t, NodeHash> explored{};
  count_t initial_estimate = goal_jolt_sum / biggest_button_size;
  candidates.insert(std::tuple(initial_press_counts, 0, initial_jolt_levels, 0,
                               initial_estimate));
  std::cout << "about to enter loop" << std::endl; // [debugging]
  std::size_t loop_iter{0};
  while (candidates.size() > 0) {
    std::cout << "in loop (iter n°" << loop_iter++ << ")"
              << std::endl; // [debugging]
    std::cout << "Candidates count : " << candidates.size()
              << std::endl; // [debugging]
    // auto best_candidate_iter = candidates.begin();
    auto best_candidate = *(candidates.begin());
    std::cout << "[candidate] Best candidate : "
              << hlp::vectorToString(std::get<PRESSES>(best_candidate))
              << std::endl; // [debugging]
    auto attempt = tryButtonPresses(buttons, std::get<PRESSES>(best_candidate));
    if (std::equal(attempt.begin(), attempt.end(), goal_joltages.begin(),
                   goal_joltages.end())) {
      return best_candidate;
    } else {
      node_t candidate_copy = best_candidate;
      if (!candidates.erase(best_candidate)) {
        throw std::runtime_error("Error : couldn't remove candidate !");
      } else {
        std::cout << "Removed best candidate !" << std::endl; // [debugging]
      }
      explored.insert(candidate_copy);
      std::cout << "erase and insert done" << std::endl; // [debugging]
      for (std::size_t i{0}; i < button_count; i++) {
        std::cout << "adding neighbor from button n°" << i << "/"
                  << button_count << std::endl; // [debugging]
        auto curr_neighbor = candidate_copy;
        {
          std::get<PRESSES>(curr_neighbor)[i] +=
              1; // adding relevant button press
          std::get<PRESSES_SUM>(curr_neighbor) +=
              getButtonSize(buttons[i]); // adding to presses sum
          for (const auto &light_idx : buttons[i]) {
            std::get<JOLT_LVLS>(curr_neighbor)[light_idx] += 1;
            std::get<JOLTS_SUM>(curr_neighbor) += 1;
          }
          std::get<EST>(curr_neighbor) =
              (goal_jolt_sum - std::get<JOLTS_SUM>(curr_neighbor)) /
              biggest_button_size;
        }
        std::cout << "set up neighbor n°" << i << std::endl; // [debugging]
        bool is_valid_candidate{true};
        for (std::size_t i{0}; i < light_count; i++) {
          if (std::get<JOLT_LVLS>(curr_neighbor)[i] > goal_joltages[i]) {
            is_valid_candidate = false;
            break;
          }
        }
        std::cout << "neighbor's validity as candidate : " << std::boolalpha
                  << is_valid_candidate << std::endl; // [debugging]
        if (is_valid_candidate && !(explored.contains(curr_neighbor))) {
          candidates.insert(curr_neighbor);
          std::cout << "Just added a new candidate to explore"
                    << std::endl;                        // [debugging]
          std::cout << "neighbor inserted" << std::endl; // [debugging]
        }
      }
    }
  }
  std::cout << "loop finished" << std::endl; // [debugging]
  throw std::runtime_error(
      "findMinButtonPresses() error : didn't find solution !");
}

int main(int argc, char *argv[]) {
  const std::string le_filename{"../inputs/day_10_input.txt"};
  // auto le_content = hlp::getFileContentRaw(le_filename);
  auto le_lines = hlp::getFileContentAslines(le_filename);
  // auto le_table = hlp::getFileContentAsTable(le_filename);

  auto le_test_lines =
      hlp::rawToLines(R"([.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5})");

  le_lines = le_test_lines;

  auto [buttons, goal_jolts] = parseLine_p2(le_lines[0]);
  hlp::LOG_CATS_MAP["T"].first = true;
  hlp::printLogEntries(
      {"T"}, std::format("buttons.size() : {}, goal_jolts.size() : {}",
                         buttons.size(), goal_jolts.size()));
  hlp::printLogEntries({"T"}, "buttons :", buttons);
  hlp::printLogEntries({"T"}, "goal_jolts :", hlp::vectorToString(goal_jolts));

  auto res = findMinButtonPresses(buttons, goal_jolts);

  // node : binary
  // heuristic : countOnes(node) + (countOnes(goal) - countOnes(current)) /
  // countOnes(biggest_button)
  // candidates : {count : } start : 0b00000
}