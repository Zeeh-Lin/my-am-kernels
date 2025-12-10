#include <klib-tests.h>
#include <klib-tests-utils.h>

// 辅助函数：手动比较两个字符串，替代 strcmp
static int check_str_eq(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    if (*s1 != *s2) return 0;
    s1++;
    s2++;
  }
  return (*s1 == '\0' && *s2 == '\0');
}

static void err(const char *msg) {
  puts("strcat FAIL: ");
  puts(msg);
  putch('\n');
}

void test_strcat() {
  puts("Running test_strcat...\n");

  char *dst = (char *)data; 

  // ------------------------------------------------------------
  // 1. 基本拼接 & 返回值检查
  // ------------------------------------------------------------
  {
    reset();
    dst[0] = 'A'; dst[1] = 'B'; dst[2] = 'C'; dst[3] = '\0';
    const char *src = "XYZ";

    char *ret = strcat(dst, src);

    // 检查返回值必须指向 dst
    if (ret != dst) {
        err("return value should be equal to dst");
    }

    // 检查内容
    if (!check_str_eq(dst, "ABCXYZ")) {
        err("basic concat content mismatch");
    }
  }

  // ------------------------------------------------------------
  // 2. dst 为空串
  // ------------------------------------------------------------
  {
    reset();
    dst[0] = '\0';
    const char *src = "HELLO";

    strcat(dst, src);

    if (!check_str_eq(dst, "HELLO")) {
        err("dst empty case mismatch");
    }
  }

  // ------------------------------------------------------------
  // 3. src 为空串
  // ------------------------------------------------------------
  {
    reset();
    dst[0] = '1'; dst[1] = '2'; dst[2] = '\0';
    
    strcat(dst, "");

    if (!check_str_eq(dst, "12")) {
        err("src empty should keep dst unchanged");
    }
  }

  // ------------------------------------------------------------
  // 4. 多次拼接 (Chaining)
  // ------------------------------------------------------------
  {
    reset();
    dst[0] = '\0';
    
    // 利用返回值进行链式调用 (如果实现支持)
    // strcat(strcat(dst, "A"), "B"); 
    // 为了保险起见，分开写，但检查逻辑一致
    strcat(dst, "Op");
    strcat(dst, "en");
    strcat(dst, "AI");

    if (!check_str_eq(dst, "OpenAI")) {
        err("multi concat mismatch");
    }
  }

  // ------------------------------------------------------------
  // 5. 哨兵测试 (Sentinel Check) - 安全性检查
  //    确保 strcat 仅仅复制了需要的字符，没有在其后多写垃圾数据
  // ------------------------------------------------------------
  {
    reset();
    // 构造: dst="A", src="B"
    // 预期: "AB\0" 占用 data[0]..data[2]
    // 哨兵: data[3] 应该保持 reset() 后的值 (4)
    dst[0] = 'A'; dst[1] = '\0';
    
    // 确认初始状态 (data 被 reset 为 1, 2, 3, 4...)
    // data[0]=1(被覆盖为A), data[1]=2(被覆盖为\0), data[2]=3, data[3]=4
    
    strcat(dst, "B");
    
    // 检查结果 "AB"
    if (dst[0] != 'A' || dst[1] != 'B' || dst[2] != '\0') {
        err("sentinel test logic error");
    }
    
    // 检查哨兵 data[3] 是否被破坏
    // 如果实现错误地多复制了字节，这里可能会变
    if (data[3] != 4) {
        err("buffer overflow: wrote past the end of string");
    }
  }

  // ------------------------------------------------------------
  // 6. 极限容量测试 (Exact Fit)
  //    测试填满 data[N] 的情况 (N=32)
  //    字符串长度 31 + '\0' = 32字节
  // ------------------------------------------------------------
  {
    reset();
    // 先填充 30 个 'X'
    for (int i = 0; i < N - 2; i++) dst[i] = 'X';
    dst[N - 2] = '\0'; // 长度 30

    // 再拼接 1 个 'Y'
    // 结果长度 31, 占用 0..30 是字符, 31 是 '\0'
    strcat(dst, "Y");

    if (dst[N-2] != 'X' || dst[N-1] != 'Y') {
        err("boundary exact fit content mismatch");
    }
    
    // 验证最后一个字节是 \0
    if (data[N-1] != '\0') {
        err("boundary exact fit missing null terminator");
    }
  }

  puts("test_strcat finished.\n");
}