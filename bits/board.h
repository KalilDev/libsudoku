#ifndef S_BOARD_H
#define S_BOARD_H
#include "arr.h"
#include "type.h"

typedef struct s_board {
  s_el *board;
  const s_size __side_sqrt;
} s_board_t;

#ifdef S_USING_INLINE
inline s_size index_from_coordinates_maybe_inlined(s_size r, s_size c,
                                                   s_size side) {
  return r * side + c;
}

inline s_size s_board_side_sqrt_maybe_inlined(s_board_t board) {
  return board.__side_sqrt;
}

inline s_size s_board_side_maybe_inlined(s_board_t board) {
  s_size side_sqrt = board.__side_sqrt;
  return side_sqrt * side_sqrt;
}
inline s_size s_board_side_squared_maybe_inlined(s_board_t board) {
  s_size side_sqrt = board.__side_sqrt;
  return side_sqrt * side_sqrt * side_sqrt * side_sqrt;
}

inline s_el s_board_get_at_maybe_inlined(s_board_t board, s_size r, s_size c) {
  return board.board[index_from_coordinates_maybe_inlined(
      r, c, s_board_side_maybe_inlined(board))];
}
inline s_el s_board_get_at_i_maybe_inlined(s_board_t board, s_size i) {
  return board.board[i];
}

inline void s_board_set_at_i_maybe_inlined(s_board_t board, s_size i,
                                           s_el value) {
  board.board[i] = value;
}

inline s_board_t s_board_from_buff_maybe_inlined(s_el *board,
                                                 s_size side_sqrt) {
  s_board_t result = {
      .board = board,
      .__side_sqrt = side_sqrt,
  };
  return result;
}
#else
static s_size index_from_coordinates_maybe_inlined(s_size r, s_size c,
                                                   s_size side);

static s_size s_board_side_sqrt_maybe_inlined(s_board_t board);

static s_size s_board_side_maybe_inlined(s_board_t board);
static s_size s_board_side_squared_maybe_inlined(s_board_t board);

static s_el s_board_get_at_maybe_inlined(s_board_t board, s_size r, s_size c);
static s_el s_board_get_at_i_maybe_inlined(s_board_t board, s_size i);

static void s_board_set_at_i_maybe_inlined(s_board_t board, s_size i,
                                           s_el value);

static s_board_t s_board_from_buff_maybe_inlined(s_el *board, s_size side_sqrt);
#endif

s_board_t s_board_from_buff(s_el *board, s_size side_sqrt);

void print_board(const s_board_t board);
void remove_already_present_on_row(s_board_t board, s_size row,
                                   s_el_array_t *array);

void remove_already_present_on_column(s_board_t board, s_size column,
                                      s_el_array_t *array);
void remove_already_present_on_square(s_board_t board, s_size r, s_size c,
                                      s_el_array_t *array);

void s_board_set_at(s_board_t board, s_size r, s_size c, s_el value);

void s_board_copy_into(s_board_t dest, const s_board_t src);

void s_board_initialize_to_zero(s_board_t board);

#endif