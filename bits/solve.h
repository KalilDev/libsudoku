#ifndef S_SOLVE_H
#define S_SOLVE_H
#include "board.h"
#include <stdbool.h>

#define BACKTRACK_SOLVE_NAME(n) backtrack_solve_##n
#define BACKTRACK_SOLVE(n, board) BACKTRACK_SOLVE_NAME(n)(board, 0, 0, 1)
#define BACKTRACK_SOLVE_WITH_START(n, board, r, c)                             \
  BACKTRACK_SOLVE_NAME(n)(board, r, c, 1)
#define BACKTRACK_SOLVE_BACKWARDS(n, board)                                    \
  BACKTRACK_SOLVE_NAME(n)(board, n - 1, n - 1, -1)
#define BACKTRACK_SOLVE_SIGNATURE(n)                                           \
  bool BACKTRACK_SOLVE_NAME(n)(s_board_t board, s_size start_r,                \
                               s_size start_c, int direction)

bool s_sudoku_solve(s_board_t board);
bool s_sudoku_has_many_sols(s_board_t board);
#endif