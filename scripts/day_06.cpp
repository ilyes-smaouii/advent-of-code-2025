#include "helpers.hpp"

#include <cstddef>
#include <format>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace hlp = helper;

// PART 1

constexpr char SPACE_CHAR{' '}, MUL_CHAR{'*'}, ADD_CHAR{'+'};

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
  hlp::printLogEntries({"II", "OP"},
                       "Operands :", hlp::vectorToString(operands));
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

// PART 2

bool isDigit(const char some_char) {
  return '0' <= some_char && some_char <= '9';
}

bool isOperator(const char some_char) {
  return some_char == ADD_CHAR || some_char == MUL_CHAR;
}

std::vector<std::size_t> getOperands(std::size_t operand_pos,
                                     const std::vector<std::string> &lines) {
  hlp::printLogEntries(
      {"I"},
      std::format("getOperands() - called with operand_pos = {}", operand_pos));
  std::size_t row_count{lines.size()};
  std::size_t col_count{lines.at(0).length()};
  bool reached_end = false;
  std::vector<std::size_t> operands{};
  while (!reached_end && operand_pos < col_count) {
    reached_end = true;
    std::size_t sub_operand{0};
    for (std::size_t line_idx{0}; line_idx < row_count - 1; line_idx++) {
      const std::string &operand_line = lines.at(line_idx);
      if (isDigit(operand_line.at(operand_pos))) {
        reached_end = false;
        sub_operand *= 10;
        sub_operand += ((operand_line.at(operand_pos)) - '0');
      }
    }
    if (!reached_end) {
      operands.push_back(sub_operand);
    }
    operand_pos++;
  }
  // operands.pop_back();
  return operands;
}

std::size_t computeSubRes(std::size_t operand_pos, char opr,
                          const std::vector<std::string> &lines) {
  hlp::printLogEntries({"SR"},
                       std::format("computeSubRes() - operator : {}", opr));
  std::size_t res{};
  auto operands = getOperands(operand_pos, lines);
  hlp::printLogEntries({"I", "OP", "SR"}, "computeSubRes() - operands : ",
                       hlp::vectorToString(operands));
  if (opr == MUL_CHAR) {
    res = 1;
    for (const auto &operand : operands) {
      res *= operand;
    }
  } else if (opr == ADD_CHAR) {
    res = 0;
    for (const auto &operand : operands) {
      res += operand;
    }
  } else {
    std::string msg = std::format("Error : operator should be ADD or MULTIPLY "
                                  "! (got ASCII character n°{} instead)",
                                  static_cast<int>(opr));
    throw std::runtime_error(msg);
  }
  hlp::printLogEntries({"SR"},
                       std::format("computeSubRes() - Returning {}", res));
  return res;
}

std::pair<std::size_t, std::size_t>
getNextPositions(const std::vector<std::string> &lines,
                 std::pair<std::size_t, std::size_t> positions) {
  hlp::printLogEntries({"I"}, "getNextPositions() - starting..");
  // First, take care of operand cursor
  std::size_t &prev_oprd_pos{positions.first},
      &prev_oprtr_pos{positions.second};
  std::size_t row_count = lines.size();
  std::size_t col_count = lines.at(0).length();
  hlp::printLogEntries(
      {"I"}, std::format("getNextPositions() - col_count at {}", col_count));
  bool finished{false};
  while (!finished) {
    if (prev_oprd_pos >= col_count) {
      finished = true;
      break;
    }
    finished = true;
    for (std::size_t line_idx{0}; line_idx < row_count - 1; line_idx++) {
      if (isDigit(lines.at(line_idx).at(prev_oprd_pos))) {
        finished = false;
      }
    }
    if (!finished) {
      prev_oprd_pos++;
    }
  }
  finished = false;
  while (!finished) {
    if (prev_oprd_pos >= col_count) {
      finished = true;
      break;
    }
    for (std::size_t line_idx{0}; line_idx < row_count - 1; line_idx++) {
      if (isDigit(lines.at(line_idx).at(prev_oprd_pos))) {
        finished = true;
      }
    }
    if (!finished) {
      prev_oprd_pos++;
    }
  }

  // Then, operator
  hlp::printLogEntries({"I"}, "getNextPositions() - moving on to operator..");
  const std::string &operator_line = lines.at(row_count - 1);
  hlp::printLogEntries({"I"}, "getNextPositions() - operator - 1st at()");
  if (prev_oprtr_pos < col_count &&
      isOperator(operator_line.at(prev_oprtr_pos))) {
    prev_oprtr_pos++;
  } else {
    throw std::runtime_error("getNextPositions() error : prev_oprtr_pos should "
                             "be pointing at an operator !");
  }
  hlp::printLogEntries({"I"}, "getNextPositions() - operator - 2nd at()");
  while (prev_oprtr_pos < col_count &&
         operator_line.at(prev_oprtr_pos) == SPACE_CHAR) {
    prev_oprtr_pos++;
  }
  if (prev_oprtr_pos < col_count) {
    hlp::printLogEntries({"I"}, "getNextPositions() - operator - 3rd at()");
    hlp::printLogEntries(
        {"I"}, "getNextPositions() - operator_line.at(prev_oprtr_pos) :",
        static_cast<int>(operator_line.at(prev_oprtr_pos)));
  }

  hlp::printLogEntries({"I"},
                       std::format("getNextPositions() - returning ({}, {})",
                                   prev_oprd_pos, prev_oprtr_pos));
  return std::make_pair(prev_oprd_pos, prev_oprtr_pos);
}

std::size_t computeRes_p2(const std::vector<std::string> &lines) {
  std::size_t row_count = lines.size();
  std::size_t col_count = lines.at(0).length();
  hlp::printLogEntries(
      {"I"}, std::format("computeRes_p2() - col_count at {}", col_count));
  hlp::printLogEntries(
      {"I"}, std::format("computeRes_p2() - last character : ",
                         static_cast<int>(lines.at(4).at(col_count - 1))));
  std::pair<std::size_t, std::size_t> positions{0, 0};
  auto &operand_pos{positions.first}, &operator_pos{positions.second};
  const auto &operator_line = lines.at(row_count - 1);

  std::size_t res{0};
  while (operator_pos < col_count && operand_pos < col_count) {
    res += computeSubRes(operand_pos, operator_line.at(operator_pos), lines);
    positions = getNextPositions(lines, positions);
    hlp::printLogEntries({"I", "POS"}, "positions currently at : ",
                         std::format("{}, {}", operand_pos, operator_pos));
  }
  return res;
}

int main(int argc, char *argv[]) {
  const std::string filename{"../inputs/day_06_input.txt"};
  auto lines = hlp::getFileContentAslines(filename);
  auto test_lines = hlp::rawToLines(R"(123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  )");

  // lines = test_lines;

  // PART 1

  hlp::LOG_CATS_MAP["I"].first = false;
  hlp::LOG_CATS_MAP["II"] = std::make_pair(false, "DETAILED INFO");

  auto res = getPart1Results(lines);

  hlp::printLogEntries({"R"}, "Result for Part 1 :", res);

  // PART 2

  hlp::LOG_CATS_MAP["POS"] = std::make_pair(false, "POS");
  hlp::LOG_CATS_MAP["OP"] = std::make_pair(false, "OP");
  hlp::LOG_CATS_MAP["SR"] = std::make_pair(false, "SubRes");

  std::string lines_length_msg{};
  for (auto &line : lines) {
    lines_length_msg += "Line length : " + std::to_string(line.length()) + "\n";
  }
  hlp::printLogEntries({"I"}, lines_length_msg);

  auto res_p2 = computeRes_p2(lines);
  hlp::printLogEntries({"R"}, "Result for Part 2 :", res_p2);
}