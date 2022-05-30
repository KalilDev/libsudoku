#include "generate.h"
#include "board.h"
#include "dbg.h"
#include "rand.h"
#include "solve.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void s_sudoku_generate(s_board_t board) {
  s_size side = s_board_side_maybe_inlined(board);
  s_size side_sqrt = s_board_side_sqrt_maybe_inlined(board);
  // initialize the board to zero
  memset(board.board, 0,
         sizeof(s_el) * s_board_side_squared_maybe_inlined(board));
  printf("gonna print zeroed board at generate with memset\n");
  print_board(board);
  fflush(stdout);
  printf("\n");
  // create an arr to store every possibility in the row. outside the loop to
  // avoid messing with the stack too much.
  s_el __arr[side];
  s_el_array_t arr = s_el_arr_from_buff(__arr, side);
  // create an arr to store every possibility in the cell. outside the loop to
  // avoid messing with the stack too much.
  s_el __poss[side];
  s_el_array_t poss = s_el_arr_from_buff(__arr, side);
  // generate the first row of each square. steps:
  // for each column with step of side_sqrt
  for (s_size r = 0; r < side; r += side_sqrt) {
    // reset the row possibilities array
    arr.length = side;
    s_el_arr_initialize_1_to_len(&arr);
    // for each column
    for (s_size c = 0; c < side; c++) {
      // in each cell
      // initialize the possibilities to be the remaining values in the row
      poss.length = arr.length;
      for (s_size i = 0; i < arr.length; i++) {
        poss.arr[i] = arr.arr[i];
      }
      // remove the values already on the column
      remove_already_present_on_column(board, c, &poss);

      // chose an value from the possibilities
      s_el chosen = poss.arr[rand() % poss.length];
      // remove it from the row possibilities
      remove_from_el_arr(&arr, chosen);
      // commit it to the board
      s_board_set_at(board, r, c, chosen);
    }
  }

  print_board(board);
  fflush(stdout);
  // now, we are left with an sudoku that has side_sqrt^3 filled cells and
  // side_sqrt^4 - side_sqrt^3 empty cells.
  // this is way less empty cells than what we would have if we only generated a
  // single row/column/diagonal, and it is guaranteed to be solvable.
  bool solved = s_sudoku_solve(board);
  dbg_print_board(board);
  dbg_flush();

  assert(solved);
}