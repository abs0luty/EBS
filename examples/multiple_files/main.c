#include <stdio.h>

extern int sum(int, int);

int main() {
  printf("%d\n", sum(1, 2));
  return 0;
}