#ifndef S_BOARD_H
#define S_BOARD_H
#include "arr.h"
#include "type.h"
#include <stdbool.h>

typedef struct s_board {
  s_el *board;
  s_size __side_sqrt;
} s_board_t;

#ifdef S_USING_INLINE
static inline s_size index_from_coordinates_maybe_inlined(s_size r, s_size c,
                                                          s_size side) {
  return r * side + c;
}

inline s_size s_board_side_sqrt_maybe_inlined(const s_board_t *board) {
  return board->__side_sqrt;
}

inline s_size s_board_side_maybe_inlined(const s_board_t *board) {
  s_size side_sqrt = board->__side_sqrt;
  return side_sqrt * side_sqrt;
}
inline s_size s_board_side_squared_maybe_inlined(const s_board_t *board) {
  s_size side_sqrt = board->__side_sqrt;
  return side_sqrt * side_sqrt * side_sqrt * side_sqrt;
}

inline s_el s_board_get_at_maybe_inlined(const s_board_t *board, s_size r,
                                         s_size c) {
  return board->board[index_from_coordinates_maybe_inlined(
      r, c, s_board_side_maybe_inlined(board))];
}
inline s_el s_board_get_at_i_maybe_inlined(const s_board_t *board, s_size i) {
  return board->board[i];
}

inline void s_board_set_at_i_maybe_inlined(const s_board_t *board, s_size i,
                                           s_el value) {
  board->board[i] = value;
}

inline s_board_t s_stack_board_from_buff_maybe_inlined(s_el *board,
                                                       s_size side_sqrt) {
  s_board_t result = {
      .board = board,
      .__side_sqrt = side_sqrt,
  };
  return result;
}
#else
s_size index_from_coordinates_maybe_inlined(s_size r, s_size c, s_size side);

s_size s_board_side_sqrt_maybe_inlined(const s_board_t *board);

s_size s_board_side_maybe_inlined(const s_board_t *board);
s_size s_board_side_squared_maybe_inlined(const s_board_t *board);

s_el s_board_get_at_maybe_inlined(const s_board_t *board, s_size r, s_size c);
s_el s_board_get_at_i_maybe_inlined(const s_board_t *board, s_size i);

void s_board_set_at_i_maybe_inlined(s_board_t *board, s_size i, s_el value);

s_board_t s_stack_board_from_buff_maybe_inlined(s_el *board, s_size side_sqrt);
#endif

s_board_t s_stack_board_from_buff(s_el *board, s_size side_sqrt);

void s_board_print(const s_board_t *board);

bool s_board_is_value_safe(const s_board_t *board, s_size r, s_size c,
                           s_el attempt);

void s_board_set_at(s_board_t *board, s_size r, s_size c, s_el value);

void s_board_copy_into(s_board_t *dest, const s_board_t *src);

bool s_board_equals(const s_board_t *a, const s_board_t *b);

void s_board_initialize_to_zero(s_board_t *board);

s_board_t *s_board_new(s_size side_sqrt);

void s_board_free(s_board_t *board);

#endif