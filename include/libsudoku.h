#include <stdbool.h>

typedef unsigned char s_el;
typedef unsigned char s_size;
typedef struct s_board s_board_t;

void s_rand_set_state(int state);
void s_board_initialize_to_zero(s_board_t *board);
s_size s_board_side_sqrt(const s_board_t *board);
s_size s_board_side(const s_board_t *board);
s_el s_board_get_at(const s_board_t *board, s_size r, s_size c);
void s_board_set_at(s_board_t *board, s_size r, s_size c, s_el value);
void s_board_copy_into(s_board_t *dest, const s_board_t *src);
s_board_t *s_board_new(s_size side_sqrt);
void s_board_free(s_board_t *board);
void s_sudoku_generate(s_board_t *board);
bool s_sudoku_has_many_sols(s_board_t *board);
