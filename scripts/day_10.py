import numpy as np
import helpers

# open file, collect content
le_filename = "../inputs/day_10_input.txt"
# le_file_content = helpers.get_file_content_raw(le_filename)
le_lines = helpers.get_file_content_as_lines(le_filename)
# le_char_table = helpers.get_file_content_as_table(le_filename)

le_test_lines = helpers.raw_to_lines("""[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}""")

######
# PART 1
######


ON_CHAR = '#'


def parseDiagram(line_str):
  diagram_str = line_str.split(" ")[0]
  lights = []
  res = 0
  d_len = len(diagram_str)
  for i in range(1, d_len - 1):
    if diagram_str[i] == ON_CHAR:
      lights.append(diagram_str[i] == ON_CHAR)
      res += (1 << (i - 1))
  return res


def parseButton(button_str):
  lights_set = set(button_str[1:-1].split(","))
  res = 0
  for light in lights_set:
    res += (1 << int(light))
  return res


def parseButtons(line_str):
  elements = line_str.split(" ")
  return [parseButton(e) for e in elements[1:-1]]


def parseLines(lines):
  data = []
  for line in lines:
    elements = line.split(" ")
    diagram = parseDiagram(elements[0])
    buttons = [parseButton(e) for e in elements[1:-1]]
    data.append((diagram, buttons))
  return data


def countOnes(bin_num):
  res = 0
  while (bin_num > 0):
    res += bin_num % 2
    bin_num //= 2
  return res


def findCandidatesAux(buttons, perm):
  res = 0
  i = 0
  for i in range(len(buttons)):
    if (perm % 2 == 1):
      res ^= buttons[i]
    perm //= 2
  return res


def findCandidates(diagram, buttons):
  candidates = set()
  for i in range(pow(2, len(buttons))):
    if findCandidatesAux(buttons, i) == diagram:
      candidates.add(i)
  return candidates


def findMinButtonPresses(diagram, buttons):
  candidates = findCandidates(diagram, buttons)
  min_presses = len(buttons) + 1
  sol = 0
  for candidate in candidates:
    curr_presses = countOnes(candidate)
    if curr_presses < min_presses:
      min_presses = curr_presses
      sol = candidate
  return (sol, min_presses)


def findTotalMin(lines):
  total = 0
  for line in lines:
    diagram, buttons = parseDiagram(line), parseButtons(line)
    total += findMinButtonPresses(diagram, buttons)[1]
  return total

######
# PART 2
######


def tryButtonPress(button_bin_rep, press_count, diagram_size):
  res = np.zeros((diagram_size), dtype=np.uint)
  i = 0
  while button_bin_rep > 0:
    res[i] += press_count * (button_bin_rep % 2)
    i += 1
    button_bin_rep //= 2
  return res


def tryButtonPresses(buttons_bin_rep, press_counts, diagram_size):
  res = np.zeros((diagram_size), dtype=np.uint)
  if len(buttons_bin_rep) != len(press_counts):
    raise RuntimeError(
      "tryButtonPresses() error : buttons and press_counts should have the same length !")
  for i in range(len(buttons_bin_rep)):
    res += tryButtonPress(buttons_bin_rep[i], press_counts[i])
  return res


bt = 0b1101
pc = 3
dsz = 4


def findLargestButton(buttons_bin_rep):
  max_count = 0
  res = 0
  for button_bin_rep in buttons_bin_rep:
    curr_count = countOnes(button_bin_rep)
    if curr_count > max_count:
      max_count = curr_count
      res = button_bin_rep
  return res


def estimateToGoal(largest_button_size, joltage_levels, goal_joltage_levels):
  total_diff = sum(goal_joltage_levels - joltage_levels)
  return total_diff / largest_button_size


def findMinButtonPresses(buttons_bin_rep, goal_joltage_levels):
  # data structure : {press_counts : (press_counts_sum, joltage_levels, estimate_to_goal)}
  largest_button_size = countOnes(findLargestButton(buttons_bin_rep))
  light_count = len(goal_joltage_levels)
  start_joltage_levels = np.zeros((light_count), dtype=np.uint)
  start_presses = tuple(np.zeros((light_count), dtype=np.uint8))
  candidates = {start_presses: {"total_presses": 0, "jolt_levels": start_joltage_levels, "est_to_goal": estimateToGoal(
    largest_button_size, start_joltage_levels, goal_joltage_levels)}}
  while (len(candidates) > 0):
    # find best candidate, then :
    best_candidate = next(iter(candidates.items()))
    best_score = best_candidate[1]["total_presses"] + best_candidate[1]["est_to_goal"]
    for candidate in candidates:
      curr_score = candidate[1]["total_presses"] + candidate[1]["est_to_goal"]
    # if best candidate works :
    #   return result
    # else :
    #   add neighbors using heuristic (remember : each candidate has to be < goal !)
    #   remove best candidate from candidates
    pass
  # no working candidate --> raise error ?
  pass

# node : binary
# heuristic : countOnes(node) + (countOnes(goal) - countOnes(current)) // countOnes(biggest_button)
# candidates : {count : }
# start : 0b00000
