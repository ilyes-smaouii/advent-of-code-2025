import copy
# import time

# Helper stuff

LOG_DICT = {
  "D": [False, "DEBUG"],
  "T": [False, "TESTING"],
  "I": [True, "INFO"],
  "R": [True, "RESULTS"],
}


def get_file_content_raw(filename):
  _file = open(filename, "r")
  _file_content = _file.read()
  return _file_content


def id_func(arg):
  return arg


def table_to_raw(table, line_transformation_func=None, cell_transformation_func=None):
  if line_transformation_func == None:
    line_transformation_func = id_func
  if cell_transformation_func == None:
    cell_transformation_func = id_func
  final_str = ""
  for line in table:
    line_str = ""
    for cell in line:
      line_str += str(cell_transformation_func(cell))
    final_str += line_transformation_func(line_str) + "\n"
  return final_str


def raw_to_table(raw_content, cell_transformation_func=None):
  if cell_transformation_func == None:
    cell_transformation_func = id_func
  _lines = raw_content.split("\n")
  _char_table = []
  for _line in _lines:
    _char_table.append([cell_transformation_func(char) for char in _line])
  return _char_table


def raw_to_lines(raw_content, line_transformation_func=None):
  _lines = []
  if line_transformation_func == None:
    line_transformation_func = id_func
  for _line in raw_content.split("\n"):
    _lines.append(line_transformation_func(_line))
  return _lines


def get_file_content_as_lines(filename, line_transformation_func=None):
  if line_transformation_func == None:
    line_transformation_func = id_func
  return [line_transformation_func(line) for line in get_file_content_raw(filename).split("\n")]


def get_file_content_as_table(filename, cell_transformation_func=None):
  if cell_transformation_func == None:
    cell_transformation_func = id_func
  _file_content = get_file_content_raw(filename)
  return raw_to_table(_file_content, cell_transformation_func)


def concat_table_as_str(table, func=None):
  if func == None:
    func = str
  return "".join(func(elem) for elem in table)


def get_log_entries(*to_print, log_cats={"I"}):
  """
  Log categories : Result, Info, Debug
  """
  should_log = False
  prefix = ""
  for log_cat in log_cats:
    if log_cat in LOG_DICT and LOG_DICT[log_cat][0]:
      should_log = True
      # if len(LOG_DICT[log_cat]) > 1:
      #   prefix += LOG_DICT[log_cat][1]
      # else:
      # prefix += "[" + str(log_cat) + "]"
      prefix += "[" + LOG_DICT[log_cat][1] + "]"
  # print("get_log_entries() - to_print : ", to_print) # [debugging]
  if should_log:
    return [prefix + " " + str(_str) for _str in to_print]
  else:
    return []


def print_log_entries(*to_print, log_cats={"I"}):
  log_entries = get_log_entries(*to_print, log_cats=log_cats)
  # print("log_entries : ", log_entries) # [debugging]
  for log_entry in log_entries:
    print(log_entry)


def eval_and_print(expr, log_cats={"I"}):
  print_log_entries(expr, eval(expr), log_cats)


def check_table_type_and_size(table):
  if not type(table) is list:
    return (False, "Bad type on table (got " + str(type(table)) + ")")
  if not type(table[0]) is list:
    return (False, "Bad type on table[0] (got " + str(type(table[0])) + ")")
  row_count = len(table)
  col_count = len(table[0])
  for row_idx in range(row_count):
    if len(table[row_idx]) != col_count:
      return (False, "Bad len on table[" + str(row_idx) + "]")
  return (True, "All good")


def set_log_by_parts():
  LOG_DICT["P1"] = [True, "PART 1"]
  LOG_DICT["P2"] = [True, "PART 2"]


class SimpleTableView():
  #
  def __init__(self, table, initial_pos=(0, 0)):
    self._table_rep = []
    if type(table) is list:
      print_log_entries(
        "SimpleTableView.__init__() : got list argument", log_cats={"D"})
      self._table_rep = table
    elif type(table) is SimpleTableView:
      print_log_entries(
        "SimpleTableView.__init__() : got SimpleTableView argument", log_cats={"D"})
      self._table_rep = table._table_rep
      print_log_entries(
        "SimpleTableView.__init__() : used SimpleTableView._table_rep", log_cats={"D"})
      pass
    else:
      raise Exception(
        "SimpleTableView() construction error : unsupported type " + str(type(table)))
    table_check = check_table_type_and_size(self._table_rep)
    if not table_check[0]:
      print_log_entries(
        "table_check[1] :\n" + str(table_check[1]), log_cats={"D"})  # [debugging]
      raise Exception(
        "SimpleTable construction error : table has inconsistent type or size !")
    self._row_count = len(self._table_rep)
    self._col_count = len(self._table_rep[0])
    self._row_pos = min(self._row_count - 1, max(0, initial_pos[0]))
    self._col_pos = min(self._col_count - 1, max(0, initial_pos[1]))
  #

  def get_cell_at(self, pos):
    return self._table_rep[pos[0]][pos[1]]
  #

  def get_current_cell(self):
    return self._table_rep[self._row_pos][self._col_pos]
  #

  def get_pos(self):
    return (self._row_pos, self._col_pos)
  #

  def get_pos_and_cell(self):
    pos, cell = (self._row_pos, self._col_pos), self.get_cell_at(
      (self._row_pos, self._col_pos))
    return (pos, cell)
  #

  def laxist_set_pos(self, new_pos):
    print_log_entries("set_pos() - called with " +
                      str(new_pos), log_cats={"D"})  # [debugging]
    self._row_pos = min(self._row_count - 1, max(0, new_pos[0]))
    self._col_pos = min(self._col_count - 1, max(0, new_pos[1]))
    print_log_entries("set_pos() - returning " +
                      str(self.get_pos()), log_cats={"D"})  # [debugging]
    return self.get_pos_and_cell()

  def set_pos(self, new_pos):
    print_log_entries("set_pos() - called with " +
                      str(new_pos), log_cats={"D"})  # [debugging]
    if 0 <= new_pos[0] <= self._row_count - 1 and 0 <= new_pos[1] <= self._col_count:
      self._row_pos = new_pos[0]
      self._col_pos = new_pos[1]
    else:
      raise Exception(
        "SimpleTableView.set_pos() error : invalid position (" + str(new_pos) + ")")
    print_log_entries("set_pos() - returning " +
                      str(self.get_pos()), log_cats={"D"})  # [debugging]
    return self.get_pos_and_cell()
  #

  def move_up(self):
    return self.laxist_set_pos((self._row_pos - 1, self._col_pos))
  #

  def move_down(self):
    return self.laxist_set_pos((self._row_pos + 1, self._col_pos))
  #

  def move_left(self):
    return self.laxist_set_pos((self._row_pos, self._col_pos - 1))
  #

  def move_right(self):
    return self.laxist_set_pos((self._row_pos, self._col_pos + 1))
  #

  def get_size(self):
    return (self._row_count, self._col_count)


class SimpleTable(SimpleTableView):
  _table_copy_rep = []
  #

  def __init__(self, table, initial_pos=(0, 0)):
    self._table_copy_rep = copy.deepcopy(table)
    SimpleTableView.__init__(self._table_copy_rep)


class SimpleTableViewCursor():
  def __init__(self, simple_table_view):
    self._pos = (0, -1)
    self._simple_table_view = simple_table_view

  def __iter__(self):
    self._has_started = False
    self._pos, self._viewed_cell = self._simple_table_view.set_pos((0, 0))
    self._table_size = self._simple_table_view.get_size()
    return self

  def __next__(self):
    if not self._has_started:
      self._has_started = True
      self._pos, self._viewed_cell = self._simple_table_view.set_pos((0, 0))
      return (self._pos, self._viewed_cell)
    # check if we're at the end of the current row
    if self._pos[1] >= self._table_size[1] - 1:
      # check if we're at the end of the table (i.e. current row is the last row)
      if self._pos[0] >= self._table_size[0] - 1:
        raise StopIteration
      else:
        self._pos, self._viewed_cell = self._simple_table_view.set_pos(
          (self._pos[0] + 1, 0))
    else:
      self._pos, self._viewed_cell = self._simple_table_view.move_right()
    # return (self._pos, self._simple_table_view.get_cell_at(self._pos))
    return (self._pos, self._viewed_cell)
    return self._simple_table_view.get_pos_and_cell()
#
