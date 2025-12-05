#include <klib-tests.h>
#include <klib-tests-utils.h>

/*
 data 内容来自 reset():
   data[i] = i + 1,  i ∈ [0, N)
*/

void test_memmove() {
  int dst, src, len;

  // len = 拷贝长度
  for (len = 1; len <= N; len++) {
    for (src = 0; src + len <= N; src++) {
      for (dst = 0; dst + len <= N; dst++) {

        reset(); // refresh data[i] = i+1

        // 记录 memmove 之前的 data
        uint8_t before[N];
        for (int i = 0; i < N; i++) before[i] = data[i];

        // 执行 memmove
        memmove(data + dst, data + src, len);

        // 验证：memmove 效果应该和手动拷贝一致
        for (int i = 0; i < N; i++) {
          uint8_t expected;

          // 1) [dst, dst+len) 这段应该等于 before[src, src+len)
          if (i >= dst && i < dst + len) {
            expected = before[src + (i - dst)];
          }
          // 2) 非目标区间的数据应该保持原来 before[i] 的内容
          else {
            expected = before[i];
          }

          if (data[i] != expected) {
            char val[2];
            val[0] = data[i] % 10;
            val[1] = data[i] / 10 % 10;
            puts("data["); putch(i+48); puts("]: ");
            putch(val[1]+48); putch(val[0]+48);
            puts(", expected: "); 
            val[0] = expected % 10;
            val[1] = expected / 10 % 10;
            putch(val[1]+48); putch(val[0]+48); putch('\n');
            return;
          }
        }
      }
    }
  }
  puts("test_memmove pass.\n");
}
