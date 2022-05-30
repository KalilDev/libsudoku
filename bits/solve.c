#include "solve.h"
#include "board.h"
#include "dbg.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef bool (*sudoku_solver)(s_board_t board);

#define __BACKTRACK_SOLVE_BAIL_OUT                                             \
  /* if there are no possibilities left, the sudoku is at an unsolvable        \
   * state */                                                                  \
  if (poss.length == 0) {                                                      \
    dbg_printf("no available at r: %d, c: %d\n", r, c);                        \
    dbg_flush();                                                               \
    return false;                                                              \
  }                                                                            \
  /* if there is only one possibility, commit it. this avoids creating         \
   * a*/                                                                       \
  /*new board and all the other expensive stuff*/                              \
  if (poss.length == 1) {                                                      \
    s_board_set_at(board, r, c, poss.arr[0]);                                  \
    dbg_printf("found single at r: %d, c: %d = %d\n", r, c, poss.arr[0]);      \
    dbg_flush();                                                               \
    continue;                                                                  \
  }

#define BACKTRACK_SOLVE_IMPL_DIRECTION(_side, _side_sqrt)                      \
  BACKTRACK_SOLVE_SIGNATURE(_side) {                                           \
    assert(direction == 1 || direction == -1);                                 \
    assert(s_board_side_maybe_inlined(board) == _side &&                       \
           s_board_side_sqrt_maybe_inlined(board) == _side_sqrt);              \
    if (s_board_side_maybe_inlined(board) != _side ||                          \
        s_board_side_sqrt_maybe_inlined(board) != _side_sqrt) {                \
      exit(1);                                                                 \
      return false;                                                            \
    }                                                                          \
    const s_size side_sqrt = _side_sqrt;                                       \
    const s_size side = _side;                                                 \
    s_el __attempt_board[_side][_side];                                        \
    s_board_t attempt_board =                                                  \
        s_board_from_buff_maybe_inlined((s_el *)__attempt_board, side_sqrt);   \
    s_board_copy_into(attempt_board, board);                                   \
    /* steps: */                                                               \
    /* for each value */                                                       \
    for (s_size r = 0; r < side; r++) {                                        \
      for (s_size c = 0; c < side; c++) {                                      \
        /* if the value is solved, continue */                                 \
        if (s_board_get_at_maybe_inlined(board, r, c) != 0) {                  \
          continue;                                                            \
        }                                                                      \
        dbg_printf("found empty r: %d, c: %d\n", r, c);                        \
        dbg_flush();                                                           \
        /* Try every possibility until we find an valid one. */                \
        /* steps: for each value that may be possible*/                        \
        for (s_size v = direction == 1 ? 1 : side;                             \
             direction == 1 ? v <= side : v >= 1;                              \
             direction == 1 ? v++ : v--) {                                     \
          /* Check if the value is safe */                                     \
          if (!s_board_is_value_safe(attempt_board, r, c, v)) {                \
            continue;                                                          \
          }                                                                    \
          /* reset the attempt board to the current state*/                    \
          s_board_copy_into(attempt_board, board);                             \
          /* commit the chosen possibility to the attempt board*/              \
          s_board_set_at(attempt_board, r, c, v);                              \
          dbg_printf("trying r: %d, c: %d = %d\n", r, c, v);                   \
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
          if (!BACKTRACK_SOLVE(_side, attempt_board, direction)) {             \
            /* reset the board before continuing*/                             \
            continue;                                                          \
          }                                                                    \
          /* if we found, commit this change to the board and continue the     \
           * other*/                                                           \
          /* cols and rols until we fill the board*/                           \
          s_board_set_at(board, r, c, v);                                      \
          dbg_printf("found at backtrack r: %d, c: %d = %d\n", r, c, v);       \
          dbg_flush();                                                         \
          goto found_soln;                                                     \
        }                                                                      \
        /* we didnt find an solution , so this sudoku isnt solvable at         \
         * the */                                                              \
        /* current state. */                                                   \
        dbg_printf("didnt find at backtrack r: %d, c: %d\n", r, c);            \
        dbg_flush();                                                           \
        return false;                                                          \
      }                                                                        \
    }                                                                          \
  found_soln:                                                                  \
    dbg_printf("solved\n");                                                    \
    dbg_flush();                                                               \
    s_board_copy_into(board, attempt_board);                                   \
    return true;                                                               \
  }

BACKTRACK_SOLVE_SIGNATURE(4);
BACKTRACK_SOLVE_SIGNATURE(9);
BACKTRACK_SOLVE_SIGNATURE(16);

BACKTRACK_SOLVE_IMPL_DIRECTION(4, 2);

BACKTRACK_SOLVE_IMPL_DIRECTION(9, 3);

BACKTRACK_SOLVE_IMPL_DIRECTION(16, 4);

#define SOLVE_SUDOKU_N(n, direction_name, direction)                           \
  static bool solve_sudoku_##n##_##direction_name(s_board_t board) {           \
    assert(s_board_side_maybe_inlined(board) == n);                            \
    if (s_board_side_maybe_inlined(board) != n) {                              \
      return false;                                                            \
    }                                                                          \
    return BACKTRACK_SOLVE(n, board, direction);                               \
  }

bool solve_sudoku_1_forwards(s_board_t board) {
  board.board[0] = 1;
  return true;
}

bool solve_sudoku_1_backwards(s_board_t board) {
  board.board[0] = 1;
  return true;
}

SOLVE_SUDOKU_N(4, backwards, -1)
SOLVE_SUDOKU_N(4, forwards, 1)

SOLVE_SUDOKU_N(9, backwards, -1)
SOLVE_SUDOKU_N(9, forwards, 1)

SOLVE_SUDOKU_N(16, backwards, -1)
SOLVE_SUDOKU_N(16, forwards, 1)

#define S_SUDOKU_SOLVE(direction_name)                                         \
  static const sudoku_solver solvers_##direction_name[] = {                    \
      solve_sudoku_1_##direction_name,                                         \
      solve_sudoku_4_##direction_name,                                         \
      solve_sudoku_9_##direction_name,                                         \
      solve_sudoku_16_##direction_name,                                        \
  };                                                                           \
  static const size_t solvers_##direction_name##_length =                      \
      sizeof(solvers_##direction_name) / sizeof(solvers_##direction_name[0]);  \
                                                                               \
  bool s_sudoku_solve_##direction_name(s_board_t board) {                      \
    if (s_board_side_sqrt_maybe_inlined(board) <=                              \
        solvers_##direction_name##_length) {                                   \
      return solvers_##direction_name[s_board_side_sqrt_maybe_inlined(board) - \
                                      1](board);                               \
    }                                                                          \
    /* it would take waaaaaaaaaaaaaaaaaaay too long */                         \
    assert(false);                                                             \
    return false;                                                              \
  }

S_SUDOKU_SOLVE(forwards)

S_SUDOKU_SOLVE(backwards)

bool s_sudoku_solve(s_board_t board) { return s_sudoku_solve_forwards(board); }

bool s_sudoku_has_many_sols(s_board_t board) {
  s_size side = s_board_side_maybe_inlined(board);
  s_size side_sqrt = s_board_side_sqrt_maybe_inlined(board);
  s_el __left_board[side][side];
  s_board_t left_board = s_board_from_buff((s_el *)__left_board, side_sqrt);
  s_board_copy_into(left_board, board);
  s_el __right_board[side][side];
  s_board_t right_board = s_board_from_buff((s_el *)__right_board, side_sqrt);
  s_board_copy_into(right_board, board);

  if (!s_sudoku_solve_forwards(left_board)) {
    return false;
  }
  if (!s_sudoku_solve_backwards(right_board)) {
    assert(false);
    exit(EXIT_FAILURE);
    return false;
  }

  return !s_board_equals(left_board, right_board);
}