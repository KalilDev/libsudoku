#include "dbg.h"
#include "board.h"
#include <assert.h>
#include <stdio.h>

#ifdef DBG_SUDOKU
void dbg_print_board(const s_board_t board) {
  for (s_size c = 0; c < s_board_side(board); c++) {
    printf("{");
    for (s_size r = 0; r < s_board_side(board); r++) {
      dbg_printf("%u ", s_board_get_at(board, r, c));
    }
    printf("}\n");
  }
}
#endif

void dbg_print_possibilities(s_el_array_t poss) {
  dbg_printf("possibilities: [");
  for (s_size i = 0; i < poss.length; i++) {
    dbg_printf(i == 0 ? "%d" : ", %d", poss.arr[i]);
  }
  dbg_printf("]\n");
  dbg_flush();
}
