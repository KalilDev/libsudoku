#include "generate.h"
#include "arr.h"
#include "board.h"
#include "dbg.h"
#include "rand.h"
#include "solve.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void s_sudoku_generate(s_board_t *board) {
  s_size side = s_board_side_maybe_inlined(board);
  // initialize the board to zero
  s_board_initialize_to_zero(board);
  printf("gonna print zeroed board at generate with memset\n");
  s_board_print(board);
  fflush(stdout);
  printf("\n");
  // generate the first row of each square. steps:
  // for each col with step of side_sqrt
  s_size selected_r = rand() % side;
  // for each column
  // create an array with the possible values
  s_el __poss[side];
  s_el_array_t poss = s_el_arr_from_buff(__poss, side);
  s_el_arr_initialize_1_to_len(&poss);
  for (s_size c = 0; c < side; c++) {
    // find an safe value
    s_el safe = 0;
    do {
      s_el value = poss.arr[rand() % poss.length];
      if (s_board_is_value_safe(board, selected_r, c, value)) {
        safe = value;
      }
    } while (!safe);
    // remove it from the row possibilities
    remove_from_el_arr(&poss, safe);
    // commit it to the board
    s_board_set_at(board, selected_r, c, safe);
  }

  s_board_print(board);
  fflush(stdout);
  // now, we are left with an sudoku that has side_sqrt^3 filled cells and
  // side_sqrt^4 - side_sqrt^3 empty cells.
  // this is way less empty cells than what we would have if we only generated a
  // single row/column/diagonal, and it is guaranteed to be solvable.
  // use flipflopping so that the board pattern is less predictable
  bool solved = s_sudoku_solve_flipflopping(board);
  dbg_print_board(*board);
  dbg_flush();

  assert(solved);
}