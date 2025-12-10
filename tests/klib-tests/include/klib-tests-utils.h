#ifndef __KLIB_TESTS_UTILS_H__
#define __KLIB_TESTS_UTILS_H__

#include <stdint.h>

#define N 32
extern uint8_t data[N];

/*
* Reset the array `data[N]` to contain the sequence 1, 2, 3, ...
*/
void reset();

/*
* Verify that the values in the interval [l, r)
* form the sequence val, val+1, val+2, ...
*/
void check_seq(int l, int r, int val);

/*
* Verify that the values in the interval [l, r) are equal to val
*/
void check_eq(int l, int r, int val);

/*
* Put string
*/
void puts(const char *s);

#endif