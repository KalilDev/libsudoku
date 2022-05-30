#include "bits/board.h"
#include "bits/type.h"
#define debugging_fuck_this
#ifndef debugging_fuck_this
int main() {
  s_el __board[9][9];
  s_board_t board = s_board_from_buff((s_el *)__board, 3);
  print_board(board);
}
#else
#include "bits/board.h"
#include "bits/dbg.h"
#include "bits/generate.h"
#include "bits/read.h"
#include "bits/solve.h"
#include "bits/type.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main_generate() {
  const s_size side_sqrt = 3;
  // int err = fscanf(stdin, "%u", &side_sqrt);
  // if (err != 1 || side_sqrt < 2 || side_sqrt > 4) {
  //   return 1;
  // }
  const s_size side = side_sqrt * side_sqrt;
  s_el __solved_board[side][side];
  s_board_t solved_board = s_board_from_buff((s_el *)__solved_board, side_sqrt);
  s_el __board[side][side];
  s_board_t board = s_board_from_buff((s_el *)__board, side_sqrt);
  (void)board;
  printf("gonna print board at main\n");
  fflush(stdout);
  print_board(solved_board);
  printf("\n");
  s_sudoku_generate(solved_board);
  printf("gonna print board at main\n");
  fflush(stdout);
  print_board(solved_board);
  fflush(stdout);
  return 0;
}

int main_read() {
  const s_size side_sqrt = 3;
  // int err = fscanf(stdin, "%u", &side_sqrt);
  // if (err != 1 || side_sqrt < 2 || side_sqrt > 4) {
  //   return 1;
  // }
  const s_size side = side_sqrt * side_sqrt;
  s_size side_squared = side * side;
  (void)side_squared;
  // initialize the board to zero
  s_el __solved_board[side][side];
  s_board_t solved_board = s_board_from_buff((s_el *)__solved_board, side_sqrt);
  s_el __board[side][side];
  s_board_t board = s_board_from_buff((s_el *)__board, side_sqrt);
  {
    s_read_result_t result;
    printf("Digite a missão\n");
    fflush(stdout);
    fscanf(stdin, "mission: ");
    if ((result = s_read(board)) != s_read_success) {
      return result;
    }
    for (char c = getc(stdin);; c = getc(stdin)) {
      if (c != '\n') {
        ungetc(c, stdin);
        break;
      }
    }
    printf("Digite o resultado\n");
    fflush(stdout);
    fscanf(stdin, "solution: ");
    if ((result = s_read(solved_board)) != s_read_success) {
      return result;
    }
  }
  printf("p1\n");
  print_board(board);
  fflush(stdout);
  printf("p2\n");
  fflush(stdout);
  // solve the sudoku for this initial state
  bool could_solve = s_sudoku_solve(board);
  print_board(board);
  fflush(stdout);
  dbg_printf("sol");
  dbg_print_board(solved_board);
  dbg_flush();
  assert(could_solve);
  return 0;
}

int main() {
#define S_INPUT_GENERATE
#ifdef S_INPUT_READ
#ifdef S_INPUT_GENERATE
  static_assert(0, "you need to select either read or generate!");
#endif
  return main_read();
#else
#ifndef S_INPUT_GENERATE
  static_assert(0, "you need to select either read or generate!");
#endif
  return main_generate();
#endif
}
#endif
