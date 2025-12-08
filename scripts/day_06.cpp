#include "helpers.hpp"

#include <cstddef>
#include <format>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace hlp = helper;

std::size_t getPart1Results(std::vector<std::string> lines) {
  std::size_t row_count = lines.size();
  hlp::printLogEntries({"I"}, std::format("row_count : {}", row_count));
  std::vector<std::size_t> operands{};
  std::size_t col_count{};
  // First, read operands
  auto it = lines.begin();
  for (; it != lines.end(); it++) {
    std::stringstream line_strm{*it};
    std::istream_iterator<std::size_t> begin_iter{line_strm}, end_iter{};
    operands.insert(std::end(operands), begin_iter, end_iter);
  }
  hlp::printLogEntries({"I"}, "Number of operands : ", operands.size());
  hlp::printLogEntries({"II"}, "Operands :", hlp::vectorToString(operands));
  if (operands.size() % row_count == 0) {
    col_count = operands.size() / (row_count - 1);
    hlp::printLogEntries({"I"}, std::format("Row count : {}, col_count : {}",
                                            row_count, col_count));
  } else {
    throw std::runtime_error(
        "Error : each line should have the same number of operands !");
  }
  // Then, read operators
  auto &operators_lines = *(--it);
  std::vector<char> operators{};
  constexpr char SPACE_CHAR{' '}, MUL_CHAR{'*'}, ADD_CHAR{'+'};
  for (auto operator_iter = operators_lines.begin();
       operator_iter != operators_lines.end(); operator_iter++) {
    if ((*operator_iter) == MUL_CHAR || (*operator_iter) == ADD_CHAR) {
      operators.emplace_back(*operator_iter);
    }
  }
  if (operators.size() != col_count) {
    std::string msg{std::format(
        "Error : there should be as many operators ({}) as columns ({}) !",
        operators.size(), col_count)};
    throw std::runtime_error(msg);
  }
  hlp::printLogEntries({"I"}, "Preparing for calculations..");
  // Prepare for calculations
  // auto operand_begin_iter = operands.begin();
  // std::vector<decltype(operands)::iterator> operands_iters{};
  // for (std::size_t row{0}; row < row_count - 1; row++) {
  //   operands_iters.emplace_back(operands.begin() + row * col_count);
  // }
  // auto operator_iter = operators.begin();
  // Finally, do the calculations
  hlp::printLogEntries({"I"}, "Doing calculations..");
  std::size_t res{0};
  for (std::size_t col{0}; col < col_count; col++) {
    std::size_t sub_res;
    switch (operators[col]) {
    case '*':
      sub_res = 1;
      for (std::size_t row{0}; row < row_count - 1; row++) {
        sub_res *= operands[col + row * col_count];
      }
      // operand_begin_iter++;
      break;
    case '+':
      sub_res = 0;
      for (std::size_t row{0}; row < row_count - 1; row++) {
        sub_res += operands[col + row * col_count];
      }
      // operand_begin_iter++;
      break;
    }
    res += sub_res;
    hlp::printLogEntries({"II"}, std::format("Found sub_res : {}", sub_res));
  }
  return res;
}

int main(int argc, char *argv[]) {
  const std::string filename{"../inputs/day_06_input.txt"};
  auto lines = hlp::getFileContentAslines(filename);
  auto test_lines = hlp::rawToLines(R"(123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +)");

  // lines = test_lines;

  // PART 1

  hlp::LOG_CATS_MAP["I"].first = true;
  hlp::LOG_CATS_MAP["II"] = std::make_pair("DETAILED INFO", false);

  auto res = getPart1Results(lines);

  hlp::printLogEntries({"R"}, "Result for Part 1 :", res);

  hlp::printLogEntries({"R", "T"}, "Results");
}