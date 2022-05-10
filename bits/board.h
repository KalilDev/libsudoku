#ifndef S_BOARD_H
#define S_BOARD_H
#include "arr.h"
#include "type.h"

typedef struct s_board {
  s_el *board;
  s_size __side_sqrt;
} s_board_t;

#ifdef S_USING_INLINE
inline s_size index_from_coordinates(s_size r, s_size c, s_size side) {
  return r * side + c;
}

inline s_size s_board_side_sqrt(s_board_t board) { return board.__side_sqrt; }

inline s_size s_board_side(s_board_t board) {
  s_size side_sqrt = board.__side_sqrt;
  return side_sqrt * side_sqrt;
}
inline s_size s_board_side_squared(s_board_t board) {
  s_size side_sqrt = board.__side_sqrt;
  return side_sqrt * side_sqrt * side_sqrt * side_sqrt;
}

inline s_el s_board_get_at(s_board_t board, s_size r, s_size c) {
  return board.board[index_from_coordinates(r, c, s_board_side(board))];
}
inline s_el s_board_get_at_i(s_board_t board, s_size i) {
  return board.board[i];
}

inline void s_board_set_at_i(s_board_t board, s_size i, s_el value) {
  board.board[i] = value;
}

inline s_board_t s_board_from_buff(s_el *board, s_size side_sqrt) {
  s_board_t result = {
      .board = board,
      .__side_sqrt = side_sqrt,
  };
  return result;
}
#else
s_size index_from_coordinates(s_size r, s_size c, s_size side);

s_size s_board_side_sqrt(s_board_t board);

s_size s_board_side(s_board_t board);
s_size s_board_side_squared(s_board_t board);

s_el s_board_get_at(s_board_t board, s_size r, s_size c);
s_el s_board_get_at_i(s_board_t board, s_size i);

void s_board_set_at_i(s_board_t board, s_size i, s_el value);

s_board_t s_board_from_buff(s_el *board, s_size side_sqrt);
#endif

void print_board(const s_board_t board);
void remove_already_present_on_row(s_board_t board, s_size row,
                                   s_el_array_t *array);

void remove_already_present_on_column(s_board_t board, s_size column,
                                      s_el_array_t *array);
void remove_already_present_on_square(s_board_t board, s_size r, s_size c,
                                      s_el_array_t *array);

void s_board_set_at(s_board_t board, s_size r, s_size c, s_el value);

void s_board_copy_into(s_board_t dest, const s_board_t src);
#endif