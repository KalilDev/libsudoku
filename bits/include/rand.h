#ifndef S_RAND_H
#define S_RAND_H
#define rand() s_rand()
void s_rand_set_state(int state);
int s_rand();
#endif