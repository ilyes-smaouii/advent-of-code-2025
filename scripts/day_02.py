import helpers

# open file, collect content
le_filename = "../inputs/day_02_input.txt"
le_file_content = helpers.get_file_content_raw(le_filename)
le_test_content = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,"\
    "1698522-1698528,446443-446449,38593856-38593862,565653-565659,"\
    "824824821-824824827,2121212118-2121212124"

######
# PART 1
######


def get_ranges(input):
  ranges_str = input.split(",")
  ranges = []
  for range_str in ranges_str:
    ranges.append(tuple(range_str.split("-")))
  return ranges


def is_invalid(id):
  id_str = str(id)
  str_len = len(id_str)
  return id_str[:(str_len // 2)] == id_str[(str_len // 2):]


# def is_invalid(id):
#   st_len = len(str(id))
#   if st_len > 1:
#     modulo = 1 + pow(10, st_len // 2)
#     return ((pow(10, st_len // 2 - 1) <= (id // modulo) < pow(10, st_len // 2)) and ((id % modulo) == 0))
#   else:
#     return True


def sum_invalid_ids_in_range(le_range):
  start, end = le_range
  start, end = int(start), int(end)
  total_sum = 0
  for i in range(start, end + 1):
    if is_invalid(i):
      total_sum += i
  return total_sum


def sum_invalid_ids_brute(ranges):
  total_sum = 0
  for range in ranges:
    total_sum += sum_invalid_ids_in_range(range)
  return total_sum


helpers.LOG_DICT["T"][0] = True

le_test_ranges = get_ranges(le_test_content)
le_test_sum = sum_invalid_ids_brute(le_test_ranges)
helpers.print_log_entries("Sum of invalid IDs :",
                          le_test_sum, log_cats={"T"})

le_ranges = get_ranges(le_file_content)
le_sum_of_invalid = sum_invalid_ids_brute(le_ranges)

helpers.print_log_entries("Sum of invalid IDs :",
                          le_sum_of_invalid, log_cats={"R"})

######
# PART 2
######
