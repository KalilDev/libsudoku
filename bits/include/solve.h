#ifndef S_SOLVE_H
#define S_SOLVE_H
#include "board.h"
#include <stdbool.h>

#define BACKTRACK_SOLVE_NAME(n, _flipflopping)                                 \
  backtrack_solve_##n##_flipflopping
#define BACKTRACK_SOLVE_FORWARDS(n, board, _flip)                              \
  BACKTRACK_SOLVE_NAME(n)(board, 1, _flip)
#define BACKTRACK_SOLVE_BACKWARDS(n, board, _flip)                             \
  BACKTRACK_SOLVE_NAME(n)(board, -1, _flip)
#define BACKTRACK_SOLVE_SIGNATURE(n, _flipflopping)                            \
  bool BACKTRACK_SOLVE_NAME(n, _flipflopping)(s_board_t * board, int direction)

#define BACKTRACK_SOLVE(n, board, direction, _flipflopping)                    \
  BACKTRACK_SOLVE_NAME(n, _flipflopping)                                       \
  (board, #_flipflopping[0] ? (direction == -1 ? 1 : -1) : direction)

bool s_sudoku_solve(s_board_t *board);
bool s_sudoku_solve_flipflopping(s_board_t *board);
bool s_sudoku_has_many_sols(s_board_t *board);
#endif