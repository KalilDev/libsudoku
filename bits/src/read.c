#include "read.h"
#include "board.h"
#include "type.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

s_read_result_t s_read(s_board_t *board) {
  {
    const size_t solved_string_size =
        s_board_side_squared_maybe_inlined(board) + 1;
    char solved_string[solved_string_size];
    if (fgets(solved_string, solved_string_size, stdin) != solved_string) {
      return s_read_e_too_few;
    }
    if (strlen(solved_string) != s_board_side_squared_maybe_inlined(board)) {
      return s_read_e_too_few;
    }
    for (s_size i = 0; i < s_board_side_squared_maybe_inlined(board); i++) {
      char char_at_i = solved_string[i];
      s_el char_as_number = char_at_i - '0';
      if (char_as_number > 9) {
        fprintf(stderr, "baz\n");
        return s_read_e_nan;
      }
      s_board_set_at_i_maybe_inlined(board, i, char_as_number);
    }
  }
  return s_read_success;
}
