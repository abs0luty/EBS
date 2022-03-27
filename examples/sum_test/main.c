#include <stdio.h>

extern int sum(int, int);

int main() {
  int a, b;
  scanf("%d %d", &a, &b);
  printf("%d\n", sum(a, b));
  return 0;
}