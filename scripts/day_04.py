import helpers
# import numpy as np

# open file, collect content
le_filename = "../inputs/day_04_input.txt"
le_file_content = helpers.get_file_content_raw(le_filename)
# le_char_table = helpers.get_file_content_as_table(le_filename)

le_test_content = \
    """..@@.@@@@.
@@@.@.@.@@
@@@@@.@.@@
@.@@@@..@.
@@.@@@@.@@
.@@@@@@@.@
.@.@.@.@@@
@.@@@.@@@@
.@@@@@@@@.
@.@.@@@.@."""


######
# PART 1
######

ROLL_CHAR = "@"
EMPTY_CHAR = "."

le_table = helpers.raw_to_table(le_file_content)
le_test_table = helpers.raw_to_table(le_test_content)


def init_levels(full_table):
  row_count, col_count = helpers.get_size(full_table)
  zeroed_rolls_dict = dict()
  for row_idx in range(row_count):
    for col_idx in range(col_count):
      if full_table[row_idx][col_idx] == ROLL_CHAR:
        zeroed_rolls_dict[(row_idx, col_idx)] = 0
  return zeroed_rolls_dict


def get_levels_dict(full_table):
  row_count, col_count = helpers.get_size(full_table)
  rolls_dict = init_levels(full_table)
  # row_count, col_count = np.size(rolls_table)
  # final_table = [[0 for i in range(col_count)]
  #                for j in range(row_count)]
  for (row_idx, col_idx) in rolls_dict:
    for n_row, n_col in helpers.get_neighbors_8(row_idx, col_idx, row_count, col_count):
      if (n_row, n_col) in rolls_dict:
        rolls_dict[(n_row, n_col)] += 1
  return rolls_dict


def count_accessible_rolls(levels_table, threshold=4):
  sum = 0
  for level in levels_table.values():
    sum += (level < threshold)
  return sum


le_test_levels = get_levels_dict(le_test_table)
le_test_access_count = count_accessible_rolls(le_test_levels)

le_levels = get_levels_dict(le_table)
le_access_count = count_accessible_rolls(le_levels)

helpers.LOG_DICT["T"][0] = True

helpers.print_log_entries(
  "Total number of accessible rolls (test) :", le_test_access_count, log_cats={"T"})

helpers.print_log_entries(
  "Total number of accessible rolls :", le_access_count, log_cats={"R"})

######
# PART 2
######


def remove_next_rolls(zeroed_rolls_dict, threshold=5):
  # Compute levels
  next_rolls = dict()
  for (row_idx, col_idx) in zeroed_rolls_dict:
    for n_row, n_col in helpers.get_neighbors_9_permissive(row_idx, col_idx).intersection(zeroed_rolls_dict):
      zeroed_rolls_dict[(n_row, n_col)] += 1
      if zeroed_rolls_dict[(n_row, n_col)] >= threshold:
        next_rolls[(n_row, n_col)] = 0
  # return {cell : 0 for (cell, level) in zeroed_rolls_dict.items() if level >= threshold}
  return next_rolls


def count_removable_rolls_p2(full_table):
  zeroed_rolls_dict = init_levels(full_table)
  init_len = len(zeroed_rolls_dict)
  prev_len = init_len
  zeroed_rolls_dict = remove_next_rolls(zeroed_rolls_dict)
  curr_len = len(zeroed_rolls_dict)
  i = 0
  diff = prev_len - curr_len
  while diff > 0:
    helpers.print_log_entries("Removed {} rolls at iteration n°{}".format(
      diff, i), log_cats={"I"})
    prev_len = curr_len
    zeroed_rolls_dict = remove_next_rolls(zeroed_rolls_dict)
    curr_len = len(zeroed_rolls_dict)
    diff = prev_len - curr_len
    i += 1
  return init_len - curr_len


helpers.LOG_DICT["I"][0] = True

zd = init_levels(le_test_table)
gn = remove_next_rolls
