#include <klib-tests.h>
#include <klib-tests-utils.h>

void test_strcmp() {
  puts("[test_strcmp] start...\n");

  struct {
    const char *s1;
    const char *s2;
    int expected;     // 0: equal; <0: s1 < s2; >0: s1 > s2
  } cases[] = {
    // --- 边界和基本测试 ---
    {"", "", 0},
    {"a", "a", 0},
    {"abc", "abc", 0},
    {"", "a", -1},
    {"a", "", 1},
    {"a", "b", -1},
    {"b", "a", 1},
    {"abc", "abd", -1},
    {"abd", "abc", 1},
    
    // --- 子串/前缀测试 ---
    {"abc", "abcd", -1},
    {"abcd", "abc", 1},
    {"hello", "hell", 1},
    {"hell", "hello", -1},
    
    // --- 大小写敏感测试 (ASCII) ---
    {"Hello", "hello", -1}, 
    {"hello", "Hello", 1},
    
    // -----------------------------------------------------------------
    // --- 关键：无符号字符测试 (Unsigned Character Test) ---
    // -----------------------------------------------------------------
    // 比较 0xFF (255) vs 0x01 (1)
    // 预期：255 > 1，结果应为 > 0
    // 如果实现按有符号比较 (-1 vs 1)，则结果会是 < 0 (错误)
    {"\xff", "\x01", 1},
    
    // 比较 0x7f (127) vs 0xff (255)
    // 预期：127 < 255，结果应为 < 0
    {"\x7f", "\xff", -1},
    
    // 两个高位字节比较 (255 vs 254)
    {"\xff", "\xfe", 1},
  };

  int n = sizeof(cases) / sizeof(cases[0]);
  for (int i = 0; i < n; i++) {
    int r = strcmp(cases[i].s1, cases[i].s2);

    // 标准 strcmp 只保证符号正确，不保证具体值
    if (cases[i].expected == 0) {
      assert(r == 0);
    } else if (cases[i].expected < 0) {
      assert(r < 0);
    } else {
      assert(r > 0);
    }
  }

  puts("[test_strcmp] done!\n");
}