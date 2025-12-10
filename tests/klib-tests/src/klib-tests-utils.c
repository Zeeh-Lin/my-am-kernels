#include <klib-tests-utils.h>
#include <klib-tests.h>
#include <assert.h>

uint8_t data[N];

/*
* Reset the array `data[N]` to contain the sequence 1, 2, 3, ...
*/
void reset() {
  int i;
  for (i = 0; i < N; i ++) {
    data[i] = i + 1;
  }
}

/*
* Verify that the values in the interval [l, r)
* form the sequence val, val+1, val+2, ...
*/
void check_seq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val + i - l);
  }
}

/*
* Verify that the values in the interval [l, r) are equal to val
*/
void check_eq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val);
  }
}

void puts(const char *s) {
  while (*s) putch(*s++);
}