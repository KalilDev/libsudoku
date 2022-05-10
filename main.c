#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __OPTIMIZE__
#define __INLINE_ON_OPTIMIZE inline
#else
#define __INLINE_ON_OPTIMIZE
#endif

int s_rand_state = 1;
int s_rand() {
  int result = s_rand_state;
  result ^= result << 13;
  result ^= result >> 17;
  result ^= result << 5;
  s_rand_state = result;
  return result;
}
#define rand() s_rand()

typedef unsigned int s_size;
typedef unsigned char s_el;
#ifdef DBG_SUDOKU
#define dbg_printf(string, ...) fprintf(stderr, string, ##__VA_ARGS__);
#define dbg_flush() fflush(stderr);
#else
#define dbg_printf(string, ...) ;
#define dbg_flush() ;
#endif

void swap_el(s_el *left, s_el *right) {
  s_el old_left = *left;
  *left = *right;
  *right = old_left;
}

typedef struct {
  s_el *arr;
  s_size length;
} s_el_array_t;

__INLINE_ON_OPTIMIZE s_el_array_t s_el_arr_from_buff(s_el *buff,
                                                     s_size length) {
  s_el_array_t result = {
      .arr = buff,
      .length = length,
  };
  return result;
}

__INLINE_ON_OPTIMIZE void s_el_arr_initialize_1_to_len(s_el_array_t *arr) {
  for (s_size i = 0; i < arr->length; i++) {
    // [i] must fit in an s_el
    arr->arr[i] = i + 1;
  }
}

void remove_from_el_arr(s_el_array_t *array, s_el value) {
  s_size length = array->length;
  if (length == 0) {
    return;
  }
  s_el *arr = array->arr;
  // move the value (if existent) to the end
  for (s_size i = 0; i < length - 1; i++) {
    if (arr[i] == value) {
      swap_el(arr + i, arr + i + 1);
    }
  }
  // update the length
  if (arr[length - 1] == value) {
    array->length = length - 1;
  }
}

void remove_from_arr(s_el *arr, s_size *len, s_el value) {
  s_size length = *len;
  if (length == 0) {
    return;
  }
  // move the value (if existent) to the end
  for (s_size i = 0; i < length - 1; i++) {
    if (arr[i] == value) {
      swap_el(arr + i, arr + i + 1);
    }
  }
  // update the length
  if (arr[length - 1] == value) {
    *len = length - 1;
  }
}

__INLINE_ON_OPTIMIZE s_size index_from_coordinates(s_size r, s_size c,
                                                   s_size side) {
  return r * side + c;
}
typedef struct s_board {
  s_el *board;
  s_size __side_sqrt;
} s_board_t;

__INLINE_ON_OPTIMIZE s_size s_board_side_sqrt(s_board_t board) {
  return board.__side_sqrt;
}

__INLINE_ON_OPTIMIZE s_size s_board_side(s_board_t board) {
  s_size side_sqrt = board.__side_sqrt;
  return side_sqrt * side_sqrt;
}
__INLINE_ON_OPTIMIZE s_size s_board_side_squared(s_board_t board) {
  s_size side_sqrt = board.__side_sqrt;
  return side_sqrt * side_sqrt * side_sqrt * side_sqrt;
}

__INLINE_ON_OPTIMIZE s_el s_board_get_at(s_board_t board, s_size r, s_size c) {
  return board.board[index_from_coordinates(r, c, s_board_side(board))];
}
void print_board(const s_board_t board) {
  for (s_size c = 0; c < s_board_side(board); c++) {
    printf("{");
    for (s_size r = 0; r < s_board_side(board); r++) {
      printf(r == 0 ? "%u" : " %u", s_board_get_at(board, r, c));
    }
    printf("}\n");
  }
}
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

void remove_already_present_on_row(s_board_t board, s_size row,
                                   s_el_array_t *array) {
  for (s_size c = 0; c < s_board_side(board); c++) {
    s_el value = s_board_get_at(board, row, c);
    remove_from_el_arr(array, value);
    dbg_print_removed(row, value);
  };
}

void remove_already_present_on_column(s_board_t board, s_size column,
                                      s_el_array_t *array) {
  for (s_size r = 0; r < s_board_side(board); r++) {
    s_el value = s_board_get_at(board, r, column);
    remove_from_el_arr(array, s_board_get_at(board, r, column));
    dbg_print_removed(column, value);
  };
}
void remove_already_present_on_square(s_board_t board, s_size r, s_size c,
                                      s_el_array_t *array) {
  s_size side_sqrt = s_board_side_sqrt(board);
  s_size side = s_board_side(board);
  s_size sq_r = r / side_sqrt;
  s_size sq_c = c / side_sqrt;
  for (s_size sq_i = 0; sq_i < side; sq_i++) {
    s_size r_in_sq = sq_i / side_sqrt;
    s_size c_in_sq = sq_i % side_sqrt;
    s_size r = sq_r * side_sqrt + r_in_sq;
    s_size c = sq_c * side_sqrt + c_in_sq;
    s_el value = s_board_get_at(board, r, c);
    remove_from_el_arr(array, value);
    dbg_print_removed(square, value);
  }
}
void dbg_print_possibilities(s_el_array_t poss) {
  dbg_printf("possibilities: [");
  for (s_size i = 0; i < poss.length; i++) {
    dbg_printf(i == 0 ? "%d" : ", %d", poss.arr[i]);
  }
  dbg_printf("]\n");
  dbg_flush();
}

__INLINE_ON_OPTIMIZE s_el s_board_get_at_i(s_board_t board, s_size i) {
  return board.board[i];
}

__INLINE_ON_OPTIMIZE void s_board_set_at_i(s_board_t board, s_size i,
                                           s_el value) {
  board.board[i] = value;
}

void s_board_set_at(s_board_t board, s_size r, s_size c, s_el value) {
  board.board[index_from_coordinates(r, c, s_board_side(board))] = value;
}

__INLINE_ON_OPTIMIZE s_board_t s_board_from_buff(s_el *board,
                                                 s_size side_sqrt) {
  s_board_t result = {
      .board = board,
      .__side_sqrt = side_sqrt,
  };
  return result;
}

void s_board_copy_into(s_board_t dest, const s_board_t src) {
  assert(s_board_side_sqrt(dest) == s_board_side_sqrt(src));
  s_size side = s_board_side(dest);
  for (s_size i = 0; i < side; i++) {
    for (s_size j = 0; j < side; j++) {
      s_board_set_at(dest, i, j, s_board_get_at(src, i, j));
    }
  }
}

typedef enum {
  s_read_success,
  s_read_e_too_few,
  s_read_e_nan,
} s_read_result_t;

s_read_result_t s_read(s_board_t board) {
  {
    const size_t solved_string_size = s_board_side_squared(board) + 1;
    char solved_string[solved_string_size];
    if (fgets(solved_string, solved_string_size, stdin) != solved_string) {
      fprintf(stderr, "foo\n");
      return s_read_e_too_few;
    }
    if (strlen(solved_string) != s_board_side_squared(board)) {
      fprintf(stderr, "bar %s %lu\n", solved_string, strlen(solved_string));
      return s_read_e_too_few;
    }
    for (s_size i = 0; i < s_board_side_squared(board); i++) {
      char char_at_i = solved_string[i];
      s_el char_as_number = char_at_i - '0';
      if (char_as_number > 9) {
        fprintf(stderr, "baz\n");
        return s_read_e_nan;
      }
      s_board_set_at_i(board, i, char_as_number);
    }
  }
  return s_read_success;
}

bool solve_sudoku(s_board_t board);

void generate(s_board_t board) {
  s_size side = s_board_side(board);
  s_size side_sqrt = s_board_side_sqrt(board);
  // initialize the board to zero
  for (s_size r = 0; r < side; r++) {
    for (s_size c = 0; c < side; c++) {
      s_board_set_at(board, r, c, 0);
    }
  }
  // create an arr to store every possibility in the row. outside the loop to
  // avoid messing with the stack too much.
  s_el __arr[side];
  s_el_array_t arr = s_el_arr_from_buff(__arr, side);
  // create an arr to store every possibility in the cell. outside the loop to
  // avoid messing with the stack too much.
  s_el __poss[side];
  s_el_array_t poss = s_el_arr_from_buff(__arr, side);
  // generate the first row of each square. steps:
  // for each column with step of side_sqrt
  for (s_size r = 0; r < side; r += side_sqrt) {
    // reset the row possibilities array
    arr.length = side;
    s_el_arr_initialize_1_to_len(&arr);
    // for each column
    for (s_size c = 0; c < side; c++) {
      // in each cell
      // initialize the possibilities to be the remaining values in the row
      poss.length = arr.length;
      for (s_size i = 0; i < arr.length; i++) {
        poss.arr[i] = arr.arr[i];
      }
      // remove the values already on the column
      remove_already_present_on_column(board, c, &poss);

      // chose an value from the possibilities
      s_el chosen = poss.arr[rand() % poss.length];
      // remove it from the row possibilities
      remove_from_el_arr(&arr, chosen);
      // commit it to the board
      s_board_set_at(board, r, c, chosen);
    }
  }

  print_board(board);
  fflush(stdout);
  // now, we are left with an sudoku that has side_sqrt^3 filled cells and
  // side_sqrt^4 - side_sqrt^3 empty cells.
  // this is way less empty cells than what we would have if we only generated a
  // single row/column/diagonal, and it is guaranteed to be solvable.
  bool solved = solve_sudoku(board);
  print_board(board);
  fflush(stdout);

  assert(solved);
}

typedef bool (*sudoku_solver)(s_board_t board);

#define BACKTRACK_SOLVE_NAME(n) backtrack_solve_##n
#define BACKTRACK_SOLVE(n, board) BACKTRACK_SOLVE_NAME(n)(board, 0, 0, 1)
#define BACKTRACK_SOLVE_WITH_START(n, board, r, c)                             \
  BACKTRACK_SOLVE_NAME(n)(board, r, c, 1)
#define BACKTRACK_SOLVE_BACKWARDS(n, board)                                    \
  BACKTRACK_SOLVE_NAME(n)(board, n - 1, n - 1, -1)
#define BACKTRACK_SOLVE_SIGNATURE(n)                                           \
  bool BACKTRACK_SOLVE_NAME(n)(s_board_t board, s_size start_r,                \
                               s_size start_c, int direction)

#define BACKTRACK_SOLVE_IMPL_NO_DIRECTION(_side, _side_sqrt)                   \
  BACKTRACK_SOLVE_SIGNATURE(_side) {                                           \
    assert(direction == 1);                                                    \
    if (direction != 1) {                                                      \
      exit(1);                                                                 \
      return false;                                                            \
    }                                                                          \
    assert(s_board_side(board) == _side &&                                     \
           s_board_side_sqrt(board) == _side_sqrt);                            \
    if (s_board_side(board) != _side ||                                        \
        s_board_side_sqrt(board) != _side_sqrt) {                              \
      exit(1);                                                                 \
      return false;                                                            \
    }                                                                          \
    const s_size side_sqrt = _side_sqrt;                                       \
    const s_size side = _side;                                                 \
    const s_size side_squared = _side * _side;                                 \
    /* steps: */                                                               \
    /* for each value */                                                       \
    for (s_size r = start_r; r < side; r++) {                                  \
      for (s_size c = start_c; c < side; c++) {                                \
        /* if the value is solved, continue */                                 \
        if (s_board_get_at(board, r, c) != 0) {                                \
          continue;                                                            \
        }                                                                      \
        dbg_printf("found empty r: %d, c: %d\n", r, c);                        \
        dbg_flush();                                                           \
        /* otherwise, find every possible possible value for it. */            \
        /* steps: */                                                           \
        /* create an array with every possible value */                        \
        s_el __poss[_side];                                                    \
        s_el_array_t poss = s_el_arr_from_buff(__poss, _side);                 \
        s_el_arr_initialize_1_to_len(&poss);                                   \
        /* remove the values already present in this row, col and square */    \
        remove_already_present_on_row(board, r, &poss);                        \
        remove_already_present_on_column(board, c, &poss);                     \
        remove_already_present_on_square(board, r, c, &poss);                  \
        /* if there are no possibilities left, the sudoku is at an unsolvable  \
         * state */                                                            \
        if (poss.length == 0) {                                                \
          dbg_printf("no available at r: %d, c: %d\n", r, c);                  \
          dbg_flush();                                                         \
          return false;                                                        \
        }                                                                      \
        /* if there is only one possibility, commit it. this avoids creating   \
         * a*/                                                                 \
        /*new board and all the other expensive stuff*/                        \
        if (poss.length == 1) {                                                \
          s_board_set_at(board, r, c, poss.arr[0]);                            \
          dbg_printf("found single at r: %d, c: %d = %d\n", r, c,              \
                     poss.arr[0]);                                             \
          dbg_flush();                                                         \
          continue;                                                            \
        }                                                                      \
        dbg_print_possibilities(poss);                                         \
        dbg_flush();                                                           \
        /* otherwise, try every possibility until we find an valid one. */     \
        /* steps: create an attempt board*/                                    \
        s_el __attempt_board[_side][_side];                                    \
        s_board_t attempt_board =                                              \
            s_board_from_buff((s_el *)__attempt_board, side_sqrt);             \
        /* steps: for each possible value*/                                    \
        for (s_size i = 0; i < poss.length; i++) {                             \
          /* reset the attempt board to the current state*/                    \
          s_board_copy_into(attempt_board, board);                             \
          /* commit the chosen possibility to the attempt board*/              \
          s_board_set_at(attempt_board, r, c, poss.arr[i]);                    \
          dbg_printf("trying r: %d, c: %d = %d\n", r, c, poss.arr[i]);         \
          dbg_flush();                                                         \
          /* try solving the attempt board with the chosen number*/            \
          /* this should be r, c, but for some reason i do not know, i messed  \
           * it*/                                                              \
          /* up. though, its not that biggie, as we waste 8 small iterations   \
           * at*/                                                              \
          /* most*/                                                            \
          /* TODO: fix it*/                                                    \
          dbg_printf("gonna start backtracking with board:\n");                \
          dbg_print_board(attempt_board);                                      \
          dbg_flush();                                                         \
          if (!BACKTRACK_SOLVE_WITH_START(_side, attempt_board, r, 0)) {       \
            /* reset the board before continuing*/                             \
            continue;                                                          \
          }                                                                    \
          /* if we found, commit this change to the board and continue the     \
           * other*/                                                           \
          /* cols and rols until we fill the board*/                           \
          s_board_set_at(board, r, c, poss.arr[i]);                            \
          dbg_printf("found at backtrack r: %d, c: %d = %d\n", r, c,           \
                     poss.arr[i]);                                             \
          dbg_flush();                                                         \
          goto found_value;                                                    \
        }                                                                      \
        /* we didnt find an suitable value, so this sudoku isnt solvable at    \
         * the */                                                              \
        /* current state. */                                                   \
        dbg_printf("didnt find at backtrack r: %d, c: %d\n", r, c, );          \
        dbg_flush();                                                           \
        return false;                                                          \
      found_value:                                                             \
        /* continue until we fill the board*/                                  \
        continue;                                                              \
      }                                                                        \
    }                                                                          \
    dbg_printf("solved\n");                                                    \
    dbg_flush();                                                               \
    return true;                                                               \
  }

BACKTRACK_SOLVE_SIGNATURE(4);
BACKTRACK_SOLVE_SIGNATURE(9);
BACKTRACK_SOLVE_SIGNATURE(16);

BACKTRACK_SOLVE_IMPL_NO_DIRECTION(4, 2);

BACKTRACK_SOLVE_IMPL_NO_DIRECTION(9, 3);

BACKTRACK_SOLVE_IMPL_NO_DIRECTION(16, 4);

bool solve_sudoku_1(s_board_t board) {
  board.board[0] = 1;
  return true;
}

bool solve_sudoku_4(s_board_t board) {
  assert(s_board_side(board) == 4);
  if (s_board_side(board) != 4) {
    return false;
  }
  return BACKTRACK_SOLVE(4, board);
}

bool solve_sudoku_9(s_board_t board) {
  assert(s_board_side(board) == 9);
  if (s_board_side(board) != 9) {
    return false;
  }
  return BACKTRACK_SOLVE(9, board);
}

bool solve_sudoku_16(s_board_t board) {
  assert(s_board_side(board) == 16);
  if (s_board_side(board) != 16) {
    return false;
  }
  return BACKTRACK_SOLVE(16, board);
}

const sudoku_solver solvers[] = {
    solve_sudoku_1,
    solve_sudoku_4,
    solve_sudoku_9,
    solve_sudoku_16,
};
const size_t solvers_length = sizeof(solvers) / sizeof(solvers[0]);

bool solve_sudoku(s_board_t board) {
  if (s_board_side_sqrt(board) <= solvers_length) {
    printf("solver index %d\n", s_board_side_sqrt(board) - 1);
    return solvers[s_board_side_sqrt(board) - 1](board);
  }
  // it would take waaaaaaaaaaaaaaaaaaay too long
  assert(false);
  return false;
}

int main_generate() {
  const s_size side_sqrt = 3;
  // int err = fscanf(stdin, "%u", &side_sqrt);
  // if (err != 1 || side_sqrt < 2 || side_sqrt > 4) {
  //   return 1;
  // }
  const s_size side = side_sqrt * side_sqrt;
  s_size side_squared = side * side;
  // initialize the board to zero
  s_el __solved_board[side][side];
  s_board_t solved_board = s_board_from_buff((s_el *)__solved_board, side_sqrt);
  s_el __board[side][side];
  s_board_t board = s_board_from_buff((s_el *)__board, side_sqrt);
  generate(solved_board);
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
  bool could_solve = solve_sudoku(board);
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