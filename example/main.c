#include "libsudoku.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main_generate() {
  const s_size side_sqrt = 3;
  // int err = fscanf(stdin, "%u", &side_sqrt);
  // if (err != 1 || side_sqrt < 2 || side_sqrt > 4) {
  //   return 1;
  // }
  const s_size side = side_sqrt * side_sqrt;
  s_el __solved_board_state[9][9] = {
      {1, 0, 0, 8, 0, 0, 3, 0, 0}, {3, 0, 0, 6, 0, 0, 7, 0, 0},
      {8, 0, 0, 1, 0, 0, 6, 0, 0}, {6, 0, 0, 2, 0, 0, 4, 0, 0},
      {4, 0, 0, 9, 0, 0, 1, 0, 0}, {5, 0, 0, 7, 0, 0, 2, 0, 0},
      {2, 0, 0, 3, 0, 0, 5, 0, 0}, {7, 0, 0, 4, 0, 0, 9, 0, 0},
      {9, 0, 0, 5, 0, 0, 8, 0, 0},
  };
  s_board_t *solved_board = s_board_new(side_sqrt);
  if (solved_board == NULL) {
    return 1;
  }
  for (s_size i = 0; i < side; i++) {
    for (s_size j = 0; j < side; j++) {
      s_board_set_at(solved_board, i, j, __solved_board_state[j][i]);
    }
  }
  printf("gonna print board at main\n");
  fflush(stdout);
  s_board_print(solved_board);
  printf("\n");
  // s_sudoku_generate(solved_board);
  printf("gonna print board at main\n");
  s_sudoku_solve(solved_board);
  s_board_print(solved_board);
  fflush(stdout);
  s_board_set_at(solved_board, 6, 5, 0);
  puts("");
  s_board_print(solved_board);
  fflush(stdout);
  printf("has one soln: %i\n", !s_sudoku_has_many_sols(solved_board));
  s_board_print(solved_board);
  fflush(stdout);
  s_board_free(solved_board);
  return 0;
}

int main_read() {
  const s_size side_sqrt = 3;
  /*
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
  assert(could_solve);*/
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
