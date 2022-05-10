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

inline void s_el_arr_initialize_1_to_len(s_el_array_t *arr) {
  for (s_size i = 0; i < arr->length; i++) {
    // [i] must fit in an s_el
    arr->arr[i] = i + 1;
  }
}
#else
s_el_array_t s_el_arr_from_buff(s_el *buff, s_size length);
void s_el_arr_initialize_1_to_len(s_el_array_t *arr);
#endif

void remove_from_el_arr(s_el_array_t *array, s_el value);
#endif