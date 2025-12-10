#include <klib-tests.h>
#include <klib-tests-utils.h>

static void check_sign(const char *msg, int v, int expected) {
  // expected: -1 (<0), 0 (==0), 1 (>0)
  if ((v < 0 && expected == -1) ||
      (v == 0 && expected == 0) ||
      (v > 0 && expected == 1)) {
    return; // OK
  }
  puts("FAIL: ");
  puts(msg);
  putch('\n');
}

void test_memcmp() {
  puts("Running test_memcmp...\n");

  uint8_t ref[N]; // 引入参照数组

  // ----------------------------------------------------
  // 1. 基本相等性测试
  //    比较两个内容相同但地址不同的数组
  // ----------------------------------------------------
  reset();
  // 手动复制 data 到 ref，不依赖 memcpy
  for (int i = 0; i < N; i++) ref[i] = data[i];

  for (int len = 0; len <= N; len++) {
    int ret = memcmp(data, ref, len);
    if (ret != 0) {
      puts("ERROR: identical content in diff buffers should be 0\n");
    }
  }

  // ----------------------------------------------------
  // 2. 修改 s1 (data) 使其大于 s2 (ref)
  // ----------------------------------------------------
  for (int pos = 0; pos < N; pos++) {
    // 重置状态
    reset(); 
    for (int i = 0; i < N; i++) ref[i] = data[i];

    data[pos] += 1; // 修改 data，使其在 pos 处大于 ref

    // 比较差异点之前：应相等
    int ret = memcmp(data, ref, pos);
    if (ret != 0) {
      puts("ERROR: prefix before diff should be equal\n");
    }

    // 包含差异点：应返回 > 0
    ret = memcmp(data, ref, pos + 1);
    check_sign("s1 > s2", ret, 1);
  }

  // ----------------------------------------------------
  // 3. 修改 s1 (data) 使其小于 s2 (ref)
  // ----------------------------------------------------
  for (int pos = 0; pos < N; pos++) {
    reset();
    for (int i = 0; i < N; i++) ref[i] = data[i];

    data[pos] -= 1; // 修改 data，使其在 pos 处小于 ref

    // 包含差异点：应返回 < 0
    int ret = memcmp(data, ref, pos + 1);
    check_sign("s1 < s2", ret, -1);
  }

  // ----------------------------------------------------
  // 4. n = 0 的情况
  // ----------------------------------------------------
  reset();
  // 即使指针不同，n=0 也应返回 0
  if (memcmp(data, ref, 0) != 0) 
      puts("ERROR: memcmp n=0 must return 0\n");

  // ----------------------------------------------------
  // 5. 移位比较 (验证非对齐比较)
  //    data: 1, 2, 3...
  //    data vs data+1 => 1 vs 2 => -1
  // ----------------------------------------------------
  reset();
  int ret_shift = memcmp(data, data + 1, N - 1);
  check_sign("shifted comparison (1 vs 2)", ret_shift, -1);

  // ----------------------------------------------------
  // 6. 关键测试：无符号比较 (Unsigned Comparison)
  //    验证是否作为 unsigned char 比较
  // ----------------------------------------------------
  reset();
  for (int i = 0; i < N; i++) ref[i] = data[i];
  
  // 构造边界情况：
  // s1 = 0xFF (255), s2 = 0x01 (1)
  // 如果错误地用 signed char 比较： -1 < 1 (结果 -1，错误)
  // 如果正确地用 unsigned char 比较：255 > 1 (结果 1，正确)
  data[0] = 0xFF; 
  ref[0]  = 0x01;
  
  int ret_unsigned = memcmp(data, ref, 1);
  check_sign("unsigned char logic (0xFF vs 0x01)", ret_unsigned, 1);

  puts("test_memcmp finished.\n");
}