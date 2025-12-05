#include <klib-tests.h>
#include <klib-tests-utils.h>

void test_memcpy() {
  uint8_t src[N];
  for (int i = 0; i < N; i++) src[i] = i + 10; // 源数组不同于data
  int l, r;
  for (l = 0; l < N; l++) {
    for (r = l + 1; r <= N; r++) {
      reset();
      // 避免源目标重叠
      if ((l >= 0 && l < N) && ((r-l) <= N)) {
        memcpy(data + l, src + l, r - l);
        for (int i = l; i < r; i++) {
          if (data[i] != src[i]) {
            puts("memcpy test failed");
            return;
          }
        }
      }
    }
  }
  puts("test_memcpy() pass.\n");
}