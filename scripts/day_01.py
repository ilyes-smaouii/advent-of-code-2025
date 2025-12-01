import helpers

# open file, collect content
le_filename = "../inputs/day_01_input.txt"
# le_file_content = helpers.get_file_content_raw(le_filename)
le_lines = helpers.get_file_content_as_lines(le_filename)
# le_char_table = helpers.get_file_content_as_table(le_filename)

le_test_lines = helpers.raw_to_lines("""L68
L30
R48
L5
R60
L55
L1
L99
R14
L82""")

######
# PART 1
######

def lines_to_transformations(lines) :
  transformations = []
  for line in lines :
    if line[0] == "R" :
      try :
        transformations.append(int(line[1:]))
      except ValueError :
        prev = helpers.LOG_DICT["D"][0]
        helpers.LOG_DICT["D"][0] = True
        helpers.print_log_entries("Couldn't decode line {}".format(line), log_cats={"D"})
        helpers.LOG_DICT["D"][0] = prev
    elif line[0] == "L" :
      try :
        transformations.append(-(int(line[1:])))
      except ValueError :
        prev = helpers.LOG_DICT["D"][0]
        helpers.LOG_DICT["D"][0] = True
        helpers.print_log_entries("Couldn't decode line {}".format(line), log_cats={"D"})
        helpers.LOG_DICT["D"][0] = prev
    else :
      raise RuntimeError("Error : line should start with \"L\" or \"R\" ! (line : {})".format(line))
  return transformations

def count_occurrences(transformations, initial_state = 50, to_count = 0, state_count = 100) :
  total_count = 0
  curr_state = initial_state
  for t in transformations :
    curr_state += t
    curr_state %= state_count
    if curr_state == to_count :
      total_count += 1
  return total_count

le_transformations = lines_to_transformations(le_lines)
le_occurence_count = count_occurrences(le_transformations, 50, 0)

helpers.print_log_entries("Number of times the arrow lands on \"0\" after a full rotation :", le_occurence_count, log_cats={"R"})

######
# PART 2
######

def count_occurrences_p2(transformations, initial_state = 50, to_count = 0, state_count = 100) :
  total_count = 0
  curr_state = initial_state
  for t in transformations :
    for i in range(abs(t)) :
      curr_state += (t/abs(t))
      curr_state %= state_count
      if curr_state == to_count :
        total_count += 1
  return total_count

le_occurence_count_p2 = count_occurrences_p2(le_transformations, 50, 0)
le_test_transformations = lines_to_transformations(le_test_lines)
le_occurence_count_p2_test = count_occurrences_p2(le_test_transformations, 50, 0)

helpers.LOG_DICT["T"][0] = True
helpers.print_log_entries("Number of times the arrow lands on \"0\" after a click :", le_occurence_count_p2_test, log_cats={"R"})
helpers.print_log_entries("Number of times the arrow lands on \"0\" after a click :", le_occurence_count_p2, log_cats={"T"})