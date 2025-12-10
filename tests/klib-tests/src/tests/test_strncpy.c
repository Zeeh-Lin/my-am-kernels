#include <klib-tests.h>
#include <klib-tests-utils.h>

static void err(const char *msg) {
  puts("strncpy FAIL: ");
  puts(msg);
  putch('\n');
}

void test_strncpy() {
  puts("Running test_strncpy...\n");

  char *dst = (char *)data; 

  // ------------------------------------------------------------
  // 1. 基本拷贝：n > strlen(src)，拷贝后要填充 '\0' (包含返回值)
  // ------------------------------------------------------------
  {
    reset();
    const char *src = "HELLO";
    int n = 10;

    char *ret = strncpy(dst, src, n);
    if (ret != dst) err("basic copy return value mismatch");

    // 前 5 个是 HELLO
    for (int i = 0; i < 5; i++) {
      if (dst[i] != src[i]) err("basic copy content mismatch");
    }
    // 后续必须全部填 '\0'
    for (int i = 5; i < n; i++) {
      if (dst[i] != '\0') err("basic padding mismatch");
    }
    // 检查 n 之外的第一个字节是否被破坏 (哨兵)
    // 假设 data[n] 保持 reset 后的值 (n+1)
    // 在 klib-tests-utils.h 中，reset() 初始化 data[i] = i+1
    if (data[n] != (n + 1) % 256) err("wrote past n limit"); 
  }

  // ------------------------------------------------------------
  // 2. n == strlen(src)：恰好复制，不追加 '\0' (包含返回值)
  // ------------------------------------------------------------
  {
    reset();
    const char *src = "ABCD";
    int n = 4;
    // 设置一个哨兵在 dst[4]，它应该保持原值
    data[4] = 0xFF; 

    char *ret = strncpy(dst, src, n);
    if (ret != dst) err("exact length return value mismatch");

    for (int i = 0; i < 4; i++) {
      if (dst[i] != src[i]) err("exact length copy mismatch");
    }
    // 关键点：这里不应有 '\0'！
    if (dst[4] == '\0')
      err("exact length: should NOT write null terminator");
    
    // 检查哨兵是否被破坏
    if (data[4] != 0xFF)
      err("exact length: wrote past n limit (corrupted sentinel)");
  }

  // ------------------------------------------------------------
  // 3. n < strlen(src)：只拷贝 n 个，不写 '\0' (包含返回值)
  // ------------------------------------------------------------
  {
    reset();
    const char *src = "ABCDEFG";
    int n = 3;
    // 原始值 data[3] = 4 (来自 reset)

    char *ret = strncpy(dst, src, n);
    if (ret != dst) err("prefix copy return value mismatch");

    for (int i = 0; i < n; i++) {
      if (dst[i] != src[i]) err("prefix copy mismatch");
    }
    
    // 检查 dst[n] 保持原值 (4)
    if (data[n] != 4)
      err("prefix copy: wrote past n limit");
  }

  // ------------------------------------------------------------
  // 4. 拷贝空串：n > 0 时必须全部填 '\0' (包含返回值)
  // ------------------------------------------------------------
  {
    reset();
    const char *src = "";
    int n = 8;

    char *ret = strncpy(dst, src, n);
    if (ret != dst) err("empty copy return value mismatch");

    for (int i = 0; i < n; i++) {
      if (dst[i] != '\0') err("copy empty string should fill zeros");
    }
  }

  // ------------------------------------------------------------
  // 5. n = 0：不应触碰 dst (包含返回值)
  // ------------------------------------------------------------
  {
    reset();
    dst[0] = 'X'; dst[1] = 'Y'; dst[2] = 'Z';
    
    char *ret = strncpy(dst, "HELLO", 0);
    if (ret != dst) err("n=0 return value mismatch");

    if (dst[0] != 'X' || dst[1] != 'Y' || dst[2] != 'Z')
      err("n=0 should do nothing");
  }

  // ------------------------------------------------------------
  // 6. self-copy（dst == src） — 未定义行为，确认不会崩 (包含返回值)
  // ------------------------------------------------------------
  {
    reset();
    dst[0] = 'A'; dst[1] = 'B'; dst[2] = 'C'; dst[3] = '\0';
    
    char *ret = strncpy(dst, dst, 4); 
    if (ret != dst) err("self-copy return value mismatch");

    // 不检查内容，只要求基本稳定性
    if (dst[0] == '\0') err("self-copy corrupted data unexpectedly");
  }

  // ------------------------------------------------------------
  // 7. 边界安全检查：保证只写 n 个字节
  // ------------------------------------------------------------
  {
    reset();
    int len = 5; // 目标拷贝 5 个字符
    
    // 在 data[len] 放置哨兵，它不应该被 strncpy 触碰
    data[len] = 0xAB; 
    data[len+1] = 0xCD;

    (void)strncpy(dst, "123456789", len); // strncpy(dst, src, 5)

    // 检查拷贝内容是否正确
    if (dst[4] != '5') err("n-limit copy content mismatch");

    // 检查哨兵是否被破坏
    if (data[len] != 0xAB || data[len+1] != 0xCD)
      err("boundary safety check: strncpy wrote past n limit");
  }

  // ------------------------------------------------------------
  // 8. 大字符串，不越界情况下的正常使用
  // ------------------------------------------------------------
  {
    reset();
    int n_copy = N - 5; // 复制一个较大的 n
    
    char src[N];
    for (int i = 0; i < n_copy; i++) src[i] = 'Z';
    src[n_copy] = '\0'; // 字符串长度为 n_copy

    char *ret = strncpy(dst, src, n_copy);
    if (ret != dst) err("large copy return value mismatch");

    for (int i = 0; i < n_copy; i++) {
      if (dst[i] != 'Z') err("large copy mismatch");
    }
    // 关键点：n_copy == strlen(src)，所以不应写额外 '\0'
    if (dst[n_copy] == '\0')
      err("large copy: should NOT auto-null-terminate");
  }

  puts("test_strncpy finished.\n");
}