#include "board.h"
#include "arr.h"
#include "dbg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static s_size index_from_coordinates(s_size r, s_size c, s_size side) {
  return r * side + c;
}
s_size s_board_side_sqrt(const s_board_t *board) { return board->__side_sqrt; }

s_size s_board_side(const s_board_t *board) {
  s_size side_sqrt = board->__side_sqrt;
  return side_sqrt * side_sqrt;
}
s_size s_board_side_squared(const s_board_t *board) {
  s_size side_sqrt = board->__side_sqrt;
  return side_sqrt * side_sqrt * side_sqrt * side_sqrt;
}

s_el s_board_get_at(const s_board_t *board, s_size r, s_size c) {
  return board
      ->board[index_from_coordinates(r, c, s_board_side_maybe_inlined(board))];
}
s_el s_board_get_at_i(const s_board_t *board, s_size i) {
  return board->board[i];
}

void s_board_set_at_i(s_board_t *board, s_size i, s_el value) {
  board->board[i] = value;
}
s_board_t s_stack_board_from_buff(s_el *board, s_size side_sqrt) {
  s_board_t result = {
      .board = board,
      .__side_sqrt = side_sqrt,
  };
  return result;
}

#ifndef S_USING_INLINE
s_size index_from_coordinates_maybe_inlined(s_size r, s_size c, s_size side) {
  return index_from_coordinates(r, c, side);
}
s_size s_board_side_sqrt_maybe_inlined(const s_board_t *board) {
  return s_board_side_sqrt(board);
}

s_size s_board_side_maybe_inlined(const s_board_t *board) {
  return s_board_side(board);
}
s_size s_board_side_squared_maybe_inlined(const s_board_t *board) {
  return s_board_side_squared(board);
}

s_el s_board_get_at_maybe_inlined(const s_board_t *board, s_size r, s_size c) {
  return s_board_get_at(board, r, c);
}
s_el s_board_get_at_i_maybe_inlined(const s_board_t *board, s_size i) {
  return s_board_get_at_i(board, i);
}

void s_board_set_at_i_maybe_inlined(s_board_t *board, s_size i, s_el value) {
  s_board_set_at_i(board, i, value);
}
s_board_t s_stack_board_from_buff_maybe_inlined(s_el *board, s_size side_sqrt) {
  return s_stack_board_from_buff(board, side_sqrt);
}
#endif

void print_board(const s_board_t *board) {
  for (s_size c = 0; c < s_board_side_maybe_inlined(board); c++) {
    printf("{");
    for (s_size r = 0; r < s_board_side_maybe_inlined(board); r++) {
      printf(r == 0 ? "%u" : " %u", s_board_get_at_maybe_inlined(board, r, c));
    }
    printf("}\n");
  }
}

bool s_board_is_value_safe(const s_board_t *board, s_size r, s_size c,
                           s_el attempt) {
  for (s_size c = 0; c < s_board_side_maybe_inlined(board); c++) {
    s_el value = s_board_get_at_maybe_inlined(board, r, c);
    if (value == attempt) {
      return false;
    }
  };
  for (s_size r = 0; r < s_board_side_maybe_inlined(board); r++) {
    s_el value = s_board_get_at_maybe_inlined(board, r, c);
    if (value == attempt) {
      return false;
    }
  };
  {
    s_size side_sqrt = s_board_side_sqrt_maybe_inlined(board);
    s_size side = s_board_side_maybe_inlined(board);
    s_size sq_r = r / side_sqrt;
    s_size sq_c = c / side_sqrt;
    for (s_size sq_i = 0; sq_i < side; sq_i++) {
      s_size r_in_sq = sq_i / side_sqrt;
      s_size c_in_sq = sq_i % side_sqrt;
      s_size r = sq_r * side_sqrt + r_in_sq;
      s_size c = sq_c * side_sqrt + c_in_sq;
      s_el value = s_board_get_at_maybe_inlined(board, r, c);
      if (value == attempt) {
        return false;
      }
    }
  }
  return true;
}

void s_board_set_at(s_board_t *board, s_size r, s_size c, s_el value) {
  board
      ->board[index_from_coordinates(r, c, s_board_side_maybe_inlined(board))] =
      value;
}

void s_board_copy_into(s_board_t *dest, const s_board_t *src) {
  assert(s_board_side_sqrt_maybe_inlined(dest) ==
         s_board_side_sqrt_maybe_inlined(src));
  // s_size side = s_board_side(dest);
  memcpy(dest->board, src->board,
         sizeof(s_el) * s_board_side_squared_maybe_inlined(src));
  /*for (s_size i = 0; i < side; i++) {
    for (s_size j = 0; j < side; j++) {
      s_board_set_at(dest, i, j, s_board_get_at(src, i, j));
    }
  }*/
}

bool s_board_equals(const s_board_t *a, const s_board_t *b) {
  s_size a_len = s_board_side_squared(a);
  s_size b_len = s_board_side_squared(b);
  if (a_len != b_len) {
    return false;
  }
  if (memcmp(a->board, b->board, a_len * sizeof(s_el)) == 0) {
    return true;
  }
  return false;
}

void s_board_initialize_to_zero(s_board_t *board) {
  memset(board->board, 0,
         sizeof(s_el) * s_board_side_squared_maybe_inlined(board));
}

s_board_t *s_board_new(s_size side_sqrt) {
  s_size side = side_sqrt * side_sqrt;
  s_size side_squared = side * side;
  void *buffer = malloc(sizeof(s_board_t) + side_squared * sizeof(s_el));
  if (buffer == NULL) {
    return NULL;
  }
  s_board_t *board = (s_board_t *)buffer;
  s_el *board_els = (s_el *)board + 1;
  // s_el board_els[side_squared]
  board->board = board_els;
  board->__side_sqrt = side_sqrt;
  return board;
}

void s_board_free(s_board_t *board) { free(board); }
