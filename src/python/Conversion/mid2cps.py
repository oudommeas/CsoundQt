#import event sheet module
import qutesheet as q
from math import pow

# get all cells by row
cells = q.cells_selection_by_row()
new_data = []

for c in cells:
    if type(c) != str:
        new_val = 440.0 * pow(2, (c - 69)/12.0)
        new_data.append(new_val)
    else:
        new_data.append(c)


# set output
q.set_cells_by_row(new_data)
