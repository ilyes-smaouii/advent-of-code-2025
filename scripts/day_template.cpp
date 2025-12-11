#include "helpers.hpp"

#include <string>

namespace hlp = helper;

int main(int argc, char *argv[]) {
  const std::string le_filename{"../inputs/day_XX_input.txt"};
  auto le_content = hlp::getFileContentRaw(le_filename);
  auto le_lines = hlp::getFileContentAslines(le_filename);
  auto le_table = hlp::getFileContentAsTable(le_filename);
}