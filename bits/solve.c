#include "solve.h"
#include "board.h"
#include "dbg.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef bool (*sudoku_solver)(s_board_t board);

#define BACKTRACK_SOLVE_IMPL_NO_DIRECTION(_side, _side_sqrt)                   \
  BACKTRACK_SOLVE_SIGNATURE(_side) {                                           \
    assert(direction == 1);                                                    \
    if (direction != 1) {                                                      \
      exit(1);                                                                 \
      return false;                                                            \
    }                                                                          \
    assert(s_board_side(board) == _side &&                                     \
           s_board_side_sqrt(board) == _side_sqrt);                            \
    if (s_board_side(board) != _side ||                                        \
        s_board_side_sqrt(board) != _side_sqrt) {                              \
      exit(1);                                                                 \
      return false;                                                            \
    }                                                                          \
    const s_size side_sqrt = _side_sqrt;                                       \
    const s_size side = _side;                                                 \
    /* steps: */                                                               \
    /* for each value */                                                       \
    for (s_size r = start_r; r < side; r++) {                                  \
      for (s_size c = start_c; c < side; c++) {                                \
        /* if the value is solved, continue */                                 \
        if (s_board_get_at(board, r, c) != 0) {                                \
          continue;                                                            \
        }                                                                      \
        dbg_printf("found empty r: %d, c: %d\n", r, c);                        \
        dbg_flush();                                                           \
        /* otherwise, find every possible possible value for it. */            \
        /* steps: */                                                           \
        /* create an array with every possible value */                        \
        s_el __poss[_side];                                                    \
        s_el_array_t poss = s_el_arr_from_buff(__poss, _side);                 \
        s_el_arr_initialize_1_to_len(&poss);                                   \
        /* remove the values already present in this row, col and square */    \
        remove_already_present_on_row(board, r, &poss);                        \
        remove_already_present_on_column(board, c, &poss);                     \
        remove_already_present_on_square(board, r, c, &poss);                  \
        /* if there are no possibilities left, the sudoku is at an unsolvable  \
         * state */                                                            \
        if (poss.length == 0) {                                                \
          dbg_printf("no available at r: %d, c: %d\n", r, c);                  \
          dbg_flush();                                                         \
          return false;                                                        \
        }                                                                      \
        /* if there is only one possibility, commit it. this avoids creating   \
         * a*/                                                                 \
        /*new board and all the other expensive stuff*/                        \
        if (poss.length == 1) {                                                \
          s_board_set_at(board, r, c, poss.arr[0]);                            \
          dbg_printf("found single at r: %d, c: %d = %d\n", r, c,              \
                     poss.arr[0]);                                             \
          dbg_flush();                                                         \
          continue;                                                            \
        }                                                                      \
        dbg_print_possibilities(poss);                                         \
        dbg_flush();                                                           \
        /* otherwise, try every possibility until we find an valid one. */     \
        /* steps: create an attempt board*/                                    \
        s_el __attempt_board[_side][_side];                                    \
        s_board_t attempt_board =                                              \
            s_board_from_buff((s_el *)__attempt_board, side_sqrt);             \
        /* steps: for each possible value*/                                    \
        for (s_size i = 0; i < poss.length; i++) {                             \
          /* reset the attempt board to the current state*/                    \
          s_board_copy_into(attempt_board, board);                             \
          /* commit the chosen possibility to the attempt board*/              \
          s_board_set_at(attempt_board, r, c, poss.arr[i]);                    \
          dbg_printf("trying r: %d, c: %d = %d\n", r, c, poss.arr[i]);         \
          dbg_flush();                                                         \
          /* try solving the attempt board with the chosen number*/            \
          /* this should be r, c, but for some reason i do not know, i messed  \
           * it*/                                                              \
          /* up. though, its not that biggie, as we waste 8 small iterations   \
           * at*/                                                              \
          /* most*/                                                            \
          /* TODO: fix it*/                                                    \
          dbg_printf("gonna start backtracking with board:\n");                \
          dbg_print_board(attempt_board);                                      \
          dbg_flush();                                                         \
          if (!BACKTRACK_SOLVE_WITH_START(_side, attempt_board, r, 0)) {       \
            /* reset the board before continuing*/                             \
            continue;                                                          \
          }                                                                    \
          /* if we found, commit this change to the board and continue the     \
           * other*/                                                           \
          /* cols and rols until we fill the board*/                           \
          s_board_set_at(board, r, c, poss.arr[i]);                            \
          dbg_printf("found at backtrack r: %d, c: %d = %d\n", r, c,           \
                     poss.arr[i]);                                             \
          dbg_flush();                                                         \
          goto found_value;                                                    \
        }                                                                      \
        /* we didnt find an suitable value, so this sudoku isnt solvable at    \
         * the */                                                              \
        /* current state. */                                                   \
        dbg_printf("didnt find at backtrack r: %d, c: %d\n", r, c, );          \
        dbg_flush();                                                           \
        return false;                                                          \
      found_value:                                                             \
        /* continue until we fill the board*/                                  \
        continue;                                                              \
      }                                                                        \
    }                                                                          \
    dbg_printf("solved\n");                                                    \
    dbg_flush();                                                               \
    return true;                                                               \
  }

BACKTRACK_SOLVE_SIGNATURE(4);
BACKTRACK_SOLVE_SIGNATURE(9);
BACKTRACK_SOLVE_SIGNATURE(16);

BACKTRACK_SOLVE_IMPL_NO_DIRECTION(4, 2);

BACKTRACK_SOLVE_IMPL_NO_DIRECTION(9, 3);

BACKTRACK_SOLVE_IMPL_NO_DIRECTION(16, 4);

bool solve_sudoku_1(s_board_t board) {
  board.board[0] = 1;
  return true;
}

bool solve_sudoku_4(s_board_t board) {
  assert(s_board_side(board) == 4);
  if (s_board_side(board) != 4) {
    return false;
  }
  return BACKTRACK_SOLVE(4, board);
}

bool solve_sudoku_9(s_board_t board) {
  assert(s_board_side(board) == 9);
  if (s_board_side(board) != 9) {
    return false;
  }
  return BACKTRACK_SOLVE(9, board);
}

bool solve_sudoku_16(s_board_t board) {
  assert(s_board_side(board) == 16);
  if (s_board_side(board) != 16) {
    return false;
  }
  return BACKTRACK_SOLVE(16, board);
}

const sudoku_solver solvers[] = {
    solve_sudoku_1,
    solve_sudoku_4,
    solve_sudoku_9,
    solve_sudoku_16,
};
const size_t solvers_length = sizeof(solvers) / sizeof(solvers[0]);

bool solve_sudoku(s_board_t board) {
  if (s_board_side_sqrt(board) <= solvers_length) {
    printf("solver index %d\n", s_board_side_sqrt(board) - 1);
    return solvers[s_board_side_sqrt(board) - 1](board);
  }
  // it would take waaaaaaaaaaaaaaaaaaay too long
  assert(false);
  return false;
}