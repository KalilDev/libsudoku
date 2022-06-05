#ifndef S_DBG_H
#define S_DBG_H
#include "arr.h"
#include "board.h"
#include <assert.h>

#ifdef DBG_SUDOKU
#define dbg_printf(string, ...) fprintf(stderr, string, ##__VA_ARGS__);
#define dbg_flush() fflush(stderr);
#else
#define dbg_printf(string, ...) ;
#define dbg_flush() ;
#endif

#ifdef DBG_SUDOKU
void dbg_print_board(const s_board_t board);
#else
#define dbg_print_board(board) ;
#endif

#ifdef DBG_SUDOKU
#ifndef DBG_SUDOKU_NOT_NOISY
#define dbg_print_removed(row_column_or_square, value)                         \
  if (value != 0) {                                                            \
    dbg_printf("removed %d because it was on " #row_column_or_square "\n",     \
               value);                                                         \
    dbg_flush();                                                               \
  }
#endif
#else
#define dbg_print_removed(_, __) ;
#endif

void dbg_print_possibilities(s_el_array_t poss);

#endif