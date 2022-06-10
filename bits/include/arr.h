#ifndef S_ARR_H
#define S_ARR_H

#include "inline.h"
#include "type.h"

typedef struct {
  s_el *arr;
  s_size length;
} s_el_array_t;

#ifdef S_USING_INLINE
inline s_el_array_t s_el_arr_from_buff(s_el *buff, s_size length) {
  s_el_array_t result = {
      .arr = buff,
      .length = length,
  };
  return result;
}

#else
s_el_array_t s_el_arr_from_buff(s_el *buff, s_size length);
#endif

void remove_from_el_arr(s_el_array_t *array, s_el value);
void s_el_arr_initialize_1_to_4(s_el_array_t *arr);
void s_el_arr_initialize_1_to_9(s_el_array_t *arr);
void s_el_arr_initialize_1_to_16(s_el_array_t *arr);
void s_el_arr_initialize_1_to_len(s_el_array_t *arr);
#endif