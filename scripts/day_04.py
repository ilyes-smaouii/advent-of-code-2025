import helpers
import numpy as np

# open file, collect content
le_filename = "../inputs/day_04_input.txt"
le_file_content = helpers.get_file_content_raw(le_filename)
# le_char_table = helpers.get_file_content_as_table(le_filename)

le_test_content = """..@@.@@@@.
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

le_table = helpers.raw_to_table(le_file_content)
le_test_table = helpers.raw_to_table(le_test_content)

def get_accessible_rolls(rolls_table) :
  # row_count, col_count = helpers.get_size(rolls_table)
  row_count, col_count = np.size(rolls_table)
  
  

######
# PART 2
######
