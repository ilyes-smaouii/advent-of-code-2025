import helpers

# open file, collect content
le_filename = "../inputs/day_05_input.txt"
le_file_content = helpers.get_file_content_raw(le_filename)
# le_lines = helpers.get_file_content_as_lines(le_filename)
# le_char_table = helpers.get_file_content_as_table(le_filename)

le_test_content = """3-5
10-14
16-20
12-18

1
5
8
11
17
32"""

le_test_ranges_str, le_test_ingredients_str = le_test_content.split("\n\n")

le_test_ranges_lines = le_test_ranges_str.split("\n")
le_test_ingredients_lines = le_test_ingredients_str.split("\n")

le_ranges_str, le_ingredients_str = le_file_content.split("\n\n")

le_ranges_lines = le_ranges_str.split("\n")
le_ingredients_lines = le_ingredients_str.split("\n")

######
# PART 1
######


def get_ingredients(ingredients_lines):
  ingredients = set()
  for ing_str in ingredients_lines:
    ingredients.add(int(ing_str))
  return ingredients


def get_ranges(ranges_lines):
  ranges = set()
  for range_str in ranges_lines:
    start_str, end_str = range_str.split("-")
    start, end = int(start_str), int(end_str)
    ranges.add((start, end))
  return ranges


def get_fresh_ingredients(ranges, ingredients):
  fresh_ingredients = set()
  # i = 0
  # for (start, end) in ranges:
  #   if i % 5 == 0:
  #     if i % 10 == 0:
  #       helpers.print_log_entries(
  #         "Currently at iteration n°{}".format(i), log_cats={"iter1"})
  #     else:
  #       helpers.print_log_entries(
  #         "Currently at iteration n°{}".format(i), log_cats={"iter2"})
  #   for id in range(start, end + 1):
  #     if id in ingredients:
  #       fresh_ingredients.add(id)
  #   i += 1
  for ingredient in ingredients:
    for (start, end) in ranges:
      if start <= ingredient <= end:
        fresh_ingredients.add(ingredient)
        break
  return fresh_ingredients


# helpers.LOG_DICT["iter1"] = [True, "[ITER]"]
# helpers.LOG_DICT["iter2"] = [True, "[ITER]"]

le_test_ranges = get_ranges(le_test_ranges_lines)
le_test_ingredients = get_ingredients(le_test_ingredients_lines)

le_ranges = get_ranges(le_ranges_lines)
le_ingredients = get_ingredients(le_ingredients_lines)


le_fresh_ingredients = get_fresh_ingredients(le_ranges, le_ingredients)
le_fresh_ingredients_count = len(le_fresh_ingredients)

helpers.print_log_entries("Total number of fresh ingredients :",
                          le_fresh_ingredients_count, log_cats={"R"})

######
# PART 2
######


def fuse_ranges(ranges):
  fused_ranges = set()
  for (start, end) in ranges:
    min_start = start
    max_end = end
    for (start_f, end_f) in fused_ranges.copy():
      if start <= end_f and end >= start_f:
        fused_ranges.discard((start_f, end_f))
        min_start = min(start_f, min_start)
        max_end = max(end_f, max_end)
    fused_ranges.add((min_start, max_end))
  return fused_ranges


def count_fresh_ids_p2(ranges):
  total_count = 0
  fused_ranges = fuse_ranges(ranges)
  for (start, end) in fused_ranges:
    total_count += (1 + end - start)
  return total_count


le_fresh_count_p2 = count_fresh_ids_p2(le_ranges)

helpers.print_log_entries("Total number of fresh ingredients (Part 2) :",
                          le_fresh_count_p2, log_cats={"R"})

helpers
