#ifndef S_SOLVE_H
#define S_SOLVE_H
#include "board.h"
#include <stdbool.h>

#define BACKTRACK_SOLVE_NAME(n) backtrack_solve_##n
#define BACKTRACK_SOLVE(n, board, direction)                                   \
  BACKTRACK_SOLVE_NAME(n)(board, direction)
#define BACKTRACK_SOLVE_FORWARDS(n, board) BACKTRACK_SOLVE_NAME(n)(board, 1)
#define BACKTRACK_SOLVE_BACKWARDS(n, board) BACKTRACK_SOLVE_NAME(n)(board, -1)
#define BACKTRACK_SOLVE_SIGNATURE(n)                                           \
  bool BACKTRACK_SOLVE_NAME(n)(s_board_t board, int direction)

bool s_sudoku_solve(s_board_t board);
bool s_sudoku_has_many_sols(s_board_t board);
#endif