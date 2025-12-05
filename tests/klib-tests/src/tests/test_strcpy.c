#include <klib-tests.h>
#include <klib-tests-utils.h>

static void err(const char *msg) {
  puts("strcpy FAIL: ");
  puts(msg);
  putch('\n');
}

void test_strcpy() {
  puts("Running test_strcpy...\n");

  char *dst = (char *)data; 

  // ------------------------------------------------------------
  // 1. 基本拷贝：普通字符串 & 返回值检查
  // ------------------------------------------------------------
  {
    reset();
    const char *src = "GEMINI";
    
    char *ret = strcpy(dst, src);

    // 检查返回值必须指向 dst
    if (ret != dst) {
        err("return value should be equal to dst");
    }

    // 检查内容
    const char *p = src;
    char *q = dst;
    while (*p) {
      if (*q != *p) err("basic copy content mismatch");
      p++; q++;
    }
    if (*q != '\0') err("missing null terminator (basic)");
    
    // 检查写入后第一个字节是否是 \0 (即 data[6] = \0)
    // 哨兵：检查 data[7] 应该保持 reset() 后的值 (8)
    if (data[7] != 8) err("wrote past the null terminator");
  }

  // ------------------------------------------------------------
  // 2. 拷贝空字符串
  // ------------------------------------------------------------
  {
    reset();
    dst[0] = 'X'; dst[1] = 'Y'; dst[2] = 0; // 确保 dst 不为空
    
    char *ret = strcpy(dst, "");

    if (ret != dst) err("empty copy return value mismatch");
    if (dst[0] != '\0') err("copy empty string should produce \\0");
    if (dst[1] != 'Y') err("empty copy should not modify dst[1]"); 
  }

  // ------------------------------------------------------------
  // 3. 拷贝不同长度的字符串 (包含返回值检查)
  // ------------------------------------------------------------
  {
    const char *tests[] = { "A", "AB", "ABCDE", "1234567890", NULL };
    for (int i = 0; tests[i]; i++) {
      reset();
      const char *src = tests[i];
      char *ret = strcpy(dst, src);

      if (ret != dst) {
          err("multi-length copy return value mismatch");
          break;
      }

      const char *p = src;
      char *q = dst;
      while (*p) {
        if (*p != *q) err("multi-length copy content mismatch");
        p++; q++;
      }
      if (*q != '\0') err("missing null terminator (multi-length)");
    }
  }
  
  // ------------------------------------------------------------
  // 4. self-copy（src == dst） - 行为未定义，但测试稳定性
  // ------------------------------------------------------------
  {
    reset();
    dst[0] = 'A'; dst[1] = 'B'; dst[2] = 'C'; dst[3] = '\0';
    
    char *ret = strcpy(dst, dst);
    if (ret != dst) err("self-copy return value mismatch");

    if (dst[0] == '\0') err("self-copy corrupted data unexpectedly");
  }

  // ------------------------------------------------------------
  // 5. 移位重叠拷贝（src > dst） - 未定义行为，测试不崩溃
  // ------------------------------------------------------------
  {
    reset();
    dst[0] = 'A'; dst[1] = 'B'; dst[2] = 'C'; dst[3] = '\0';
    
    char *ret = strcpy(dst + 1, dst);

    if (ret != dst + 1) err("overlap copy return value mismatch");

    // 仅警告，不中断测试
    if (dst[0] != 'A' || dst[1] != 'A' || dst[2] != 'B' || dst[3] != 'C' || dst[4] != '\0') {
        puts("WARNING: Overlap copy behavior is UB. Result may be unexpected.");
    }
  }


  // ------------------------------------------------------------
  // 6. 拷贝 N-2 长度字符串 (Exact Fit with Sentinel)
  //    N=32。字符串长度 30 + \0 = 31。哨兵在 data[31]。
  // ------------------------------------------------------------
  {
    reset();
    int len = N - 2; // 30个字符 (0..29)
    int sentinel_idx = N - 1; // 31

    // 构造一个长度为 len 的字符串
    char large[N];
    for (int i = 0; i < len; i++) large[i] = 'Z';
    large[len] = '\0'; // large string length is 30

    // 设置哨兵： data[N-1] (即 data[31]) 应该保持原值
    data[sentinel_idx] = 0xAB; 
    
    // 修复未使用变量错误
    (void)strcpy(dst, large); 

    for (int i = 0; i < len; i++) {
      if (dst[i] != 'Z') err("large string copy content mismatch");
    }
    if (dst[len] != '\0') err("missing null terminator (large)");
    
    // 检查内部哨兵，确保没有写过头
    // 期望：strcpy 写入 data[0]..data[30]，不触碰 data[31]
    if (data[sentinel_idx] != 0xAB) err("wrote past expected boundary (internal sentinel)");
  }

  puts("test_strcpy finished.\n");
}