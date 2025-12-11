import helpers

# open file, collect content
le_filename = "../inputs/day_08_input.txt"
# le_file_content = helpers.get_file_content_raw(le_filename)
le_lines = helpers.get_file_content_as_lines(le_filename)
# le_char_table = helpers.get_file_content_as_table(le_filename)

######
# PART 1
######

NO_POS = (-1, -1, -1)
MAX_DIST_SQ = 1 << 31


def parseCoordinates(line):
  x_str, y_str, z_str = line.split(",")
  return (int(x_str), int(y_str), int(z_str))


def parseCoordinates_s(lines):
  coordinatess = dict()
  for line in lines:
    coord = parseCoordinates(line)
    coordinatess[coord] = (NO_POS, MAX_DIST_SQ)
  return coordinatess


def getDistanceSquared(coord_a, coord_b):
  a_x, a_y, a_z = coord_a
  b_x, b_y, b_z = coord_b
  return (a_x - b_x)**2 + (a_y - b_y)**2 + (a_z - b_z)**2


def getCoordDict(coordinates_s):
  coord_dict = dict()
  pass


le_coordinates = parseCoordinates_s(le_lines)


######
# PART 2
######
