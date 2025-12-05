#include <klib-tests.h>
#include <klib-tests-utils.h>
#include <assert.h>

void test_strncmp() {
  puts("[test_strncmp] start...\n");

  struct {
    const char *s1;
    const char *s2;
    size_t n;
    int expected;     // 0: equal; <0: s1 < s2; >0: s1 > s2
  } cases[] = {
    // --- n=0 和空串边界 ---
    {"", "", 0, 0},
    {"", "", 5, 0},
    {"abc", "abc", 3, 0},
    {"a", "b", 0, 0}, // n=0 时必为 0
    
    // --- 差异点与 n 边界的交互 (原优秀测试) ---
    {"abc", "abcd", 3, 0},  // 差异在 n 之后，n 限制生效
    {"abc", "abcd", 4, -1}, // 差异在 \0 处，\0 < d
    {"abcd", "abc", 4, 1},  // 差异在 \0 处，d > \0
    {"abc", "abd", 1, 0},  // 差异在 n 之后
    {"abc", "abd", 2, 0},  // 差异在 n 之后
    {"abc", "abd", 3, -1}, // 差异在 n 处
    
    // --- 长度边界 ---
    {"abc", "", 1, 1},
    {"", "abc", 1, -1},
    
    // --- 大小写敏感测试 (ASCII) ---
    {"Hello", "hello", 5, -1},
    {"hello", "Hello", 5, 1},
    
    // -----------------------------------------------------------------
    // --- 关键：无符号字符测试 (Unsigned Character Test) ---
    // -----------------------------------------------------------------
    // 比较 0xFF (255) vs 0x01 (1)，n=1
    // 预期：255 > 1，结果应为 > 0
    {"\xff a", "\x01 b", 1, 1}, 
    
    // 比较 0x7f (127) vs 0xff (255)，n=2
    // 预期：在 n=1 处，127 < 255，结果应为 < 0
    {"\x7f z", "\xff y", 2, -1},
    
    // 比较到 n=2 时，高位字节相等，低位字节 z > y
    {"\xff z", "\xff y", 2, 1},
    
    // n 限制在高位字节之后，结果应为相等
    {"\xff z", "\xff y", 1, 0}, 
  };

  int ncases = sizeof(cases) / sizeof(cases[0]);

  for (int i = 0; i < ncases; i++) {
    int r = strncmp(cases[i].s1, cases[i].s2, cases[i].n);

    if (cases[i].expected == 0) {
      assert(r == 0);
    } else if (cases[i].expected < 0) {
      assert(r < 0);
    } else {
      assert(r > 0);
    }
  }

  puts("[test_strncmp] done!\n");
}