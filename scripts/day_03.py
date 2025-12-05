import helpers

# open file, collect content
le_filename = "../inputs/day_03_input.txt"
le_lines = helpers.get_file_content_as_lines(le_filename)

le_test_lines = helpers.raw_to_lines("""987654321111111
811111111111119
234234234234278
818181911112111""")

######
# PART 1
######


def find_largest_joltage(jolt_list):
  tens_max = max(jolt_list[:-1])
  tens_idx = jolt_list.index(tens_max)
  ones_max = max(jolt_list[tens_idx + 1:])
  return 10 * int(tens_max) + int(ones_max)


def sum_joltages(banks):
  sum = 0
  for bank in banks:
    sum += find_largest_joltage(bank)
  return sum


le_total_output_joltage = sum_joltages(le_lines)

helpers.print_log_entries("Total output joltage :",
                          le_total_output_joltage, log_cats={"R"})

######
# PART 2
######


def find_largest_joltage_p2(jolt_list, digit_count=12):
  last_idx = -1
  curr_joltage = 0
  ll = len(jolt_list)
  start, end = 0, ll - digit_count
  for i in range(digit_count):
    end += 1
    curr_char = max(jolt_list[start:end])
    curr_joltage *= 10
    curr_joltage += int(curr_char)
    start = jolt_list[start:end].index(curr_char) + start + 1
  return curr_joltage


def sum_joltages_p2(banks, digit_count=12):
  sum = 0
  for bank in banks:
    sum += find_largest_joltage_p2(bank, digit_count)
  return sum


le_total_output_joltage_p2 = sum_joltages_p2(le_lines)
le_test_output_p2 = sum_joltages_p2(le_test_lines)

helpers.LOG_DICT["T"][0] = True

helpers.print_log_entries("Total output joltage (test) :",
                          le_test_output_p2, log_cats={"T"})
helpers.print_log_entries("Total output joltage :",
                          le_total_output_joltage_p2, log_cats={"R"})
