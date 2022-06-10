#include "arr.h"
#include "inline.h"
#include "type.h"
#include <string.h>

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

#endif

void remove_from_el_arr(s_el_array_t *array, s_el value) {
  if (value == 0) {
    return;
  }
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

static const s_el s_el_arr_1_to_4[] = {1, 2, 3, 4};
static const s_el s_el_arr_1_to_9[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
static const s_el s_el_arr_1_to_16[] = {1, 2,  3,  4,  5,  6,  7,  8,
                                        9, 10, 11, 12, 13, 14, 15, 16};

void s_el_arr_initialize_1_to_4(s_el_array_t *arr) {
  memcpy(arr->arr, s_el_arr_1_to_4, sizeof(s_el) * 4);
}
void s_el_arr_initialize_1_to_9(s_el_array_t *arr) {
  memcpy(arr->arr, s_el_arr_1_to_9, sizeof(s_el) * 9);
}
void s_el_arr_initialize_1_to_16(s_el_array_t *arr) {
  memcpy(arr->arr, s_el_arr_1_to_16, sizeof(s_el) * 16);
}

void s_el_arr_initialize_1_to_len(s_el_array_t *arr) {
  for (s_size i = 0; i < arr->length; i++) {
    // [i] must fit in an s_el
    arr->arr[i] = i + 1;
  }
}