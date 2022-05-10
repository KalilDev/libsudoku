#include "arr.h"
#include "inline.h"
#include "type.h"

void swap_el(s_el *left, s_el *right) {
  s_el old_left = *left;
  *left = *right;
  *right = old_left;
}

// Only implement this if we are not going to inline it
#ifndef S_USING_INLINE
s_el_array_t s_el_arr_from_buff(s_el *buff, s_size length) {
  s_el_array_t result = {
      .arr = buff,
      .length = length,
  };
  return result;
}

void s_el_arr_initialize_1_to_len(s_el_array_t *arr) {
  for (s_size i = 0; i < arr->length; i++) {
    // [i] must fit in an s_el
    arr->arr[i] = i + 1;
  }
}
#endif

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
