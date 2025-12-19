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
  for i in range(len(buttons)) :
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


def findTotalMin(lines) :
  total = 0
  for line in lines :
    diagram, buttons = parseDiagram(line), parseButtons(line)
    total += findMinButtonPresses(diagram, buttons)[1]
  return total

######
# PART 2
######

