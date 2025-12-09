import helpers
import copy

# open file, collect content
le_filename = "../inputs/day_07_input.txt"
# le_file_content = helpers.get_file_content_raw(le_filename)
# le_lines = helpers.get_file_content_as_lines(le_filename)
le_char_table = helpers.get_file_content_as_table(le_filename)
le_test_table = helpers.raw_to_table(""".......S.......
...............
.......^.......
...............
......^.^......
...............
.....^.^.^.....
...............
....^.^...^....
...............
...^.^...^.^...
...............
..^...^.....^..
...............
.^.^.^.^.^...^.
...............""")

######
# PART 1
######

START_CHAR, BEAM_CHAR = ('S', '|')
SPLIT_CHAR = "^"
EMPTY_CHAR = "."


def add_sub_beams(table, row_idx, col_idx):
  row_count = len(table)
  col_count = len(table[0])
  res = 0
  if row_idx < row_count - 1:
    if col_idx < col_count - 1:
      if table[row_idx + 1][col_idx + 1] == EMPTY_CHAR:
        table[row_idx + 1][col_idx + 1] = BEAM_CHAR
        res = 1
    if col_idx > 0:
      if table[row_idx + 1][col_idx - 1] == EMPTY_CHAR:
        table[row_idx + 1][col_idx - 1] = BEAM_CHAR
        res = 1
  return res


def next_step(table):
  row_count = len(table)
  col_count = len(table[0])
  new_splits = 0
  _table = copy.deepcopy(table)
  for row_idx in range(row_count):
    for col_idx in range(col_count):
      curr = table[row_idx][col_idx]
      if curr in (START_CHAR, BEAM_CHAR):
        if row_idx < row_count - 1 and (table[row_idx + 1][col_idx] == SPLIT_CHAR):
          new_splits += add_sub_beams(_table, row_idx, col_idx)
        elif row_idx < row_count - 1 and table[row_idx + 1][col_idx] == EMPTY_CHAR:
          _table[row_idx + 1][col_idx] = BEAM_CHAR
  return (_table, new_splits)


def count_beam_chars(table):
  sum = 0
  for line in table:
    for cell in line:
      if cell == BEAM_CHAR:
        sum += 1
  return sum


le_row_count = len(le_char_table)
le_table = copy.deepcopy(le_char_table)
# le_table = copy.deepcopy(le_test_table)
le_split_count = 0
for i in range(le_row_count):
  le_table, new_splits = next_step(le_table)
  le_split_count += new_splits

print(helpers.table_to_raw(le_table))


######
# PART 2
######

def count_timelines_rec(table, row_idx, col_idx):
  row_count = len(table)
  col_count = len(table[0])
  if table[row_idx][col_idx] not in (EMPTY_CHAR, START_CHAR):
    return 0
  count = 0
  if row_idx < row_count - 1:
    if table[row_idx + 1][col_idx] != SPLIT_CHAR:
      return count_timelines_rec(table, row_idx + 1, col_idx)
    else:
      if col_idx < col_count - 1:
        count += count_timelines_rec(table, row_idx + 1, col_idx + 1)
      if col_idx > 0:
        count += count_timelines_rec(table, row_idx + 1, col_idx - 1)
      return count
  else:
    return 1
