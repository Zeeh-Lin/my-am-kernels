#include <klib-tests.h>
#include <klib-tests-utils.h>

void (*entry)() = NULL; // mp entry

static const char *tests[256] = {
  ['H'] = "display this help message",
  ['0'] = "memset()  test",
  ['1'] = "memcpy()  test",
  ['2'] = "memmove() test",
  ['3'] = "memcmp()  test",
  ['4'] = "strcat()  test",
  ['5'] = "strcpy()  test",
  ['6'] = "strncpy() test",
  ['7'] = "strcmp()  test",
  ['8'] = "strncmp() test"
};

int main(const char *args) {
  switch (args[0]) {
    CASE('0', test_memset);
    CASE('1', test_memcpy);
    CASE('2', test_memmove);
    CASE('3', test_memcmp);
    CASE('4', test_strcat);
    CASE('5', test_strcpy);
    CASE('6', test_strncpy);
    CASE('7', test_strcmp);
    CASE('8', test_strncmp);
    case 'H':
    default:
      puts("Usage: make run mainargs=*\n");
      for (int ch = 0; ch < 256; ch++) {
        if (tests[ch]) {
          puts("  "); putch(ch); puts(": ");
          puts(tests[ch]); putch('\n');
        }
      }
  }
  return 0;
}
