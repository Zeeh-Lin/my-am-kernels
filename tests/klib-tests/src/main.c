#include <klib-tests.h>
#include <klib-tests-utils.h>

void (*entry)() = NULL; // mp entry

static const char *tests[256] = {
  ['H'] = "display this help message",
  ['s'] = "memset() test",
  ['c'] = "memcpy() test"
};

int main(const char *args) {
  switch (args[0]) {
    CASE('c', test_memcpy);
    CASE('s', test_memset);
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
