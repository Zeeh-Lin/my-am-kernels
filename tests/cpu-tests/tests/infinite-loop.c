#include "trap.h"

int a = 0;

int main() {
  while (1)
    for (int i = 0; i < 10000; i++)
      for (int j = 0; j < 10000; j++)
        printf("a = %d\n", a++);
  
  return 0;
}