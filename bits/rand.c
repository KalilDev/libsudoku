#include "rand.h"

int s_rand_state = 1;
int s_rand() {
  int result = s_rand_state;
  result ^= result << 13;
  result ^= result >> 17;
  result ^= result << 5;
  s_rand_state = result;
  return result;
}