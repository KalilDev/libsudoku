#ifndef S_READ_H
#define S_READ_H
#include "board.h"
typedef enum {
  s_read_success,
  s_read_e_too_few,
  s_read_e_nan,
} s_read_result_t;

s_read_result_t s_read(s_board_t *board);
#endif