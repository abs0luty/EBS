#include <assert.h>

extern int sum(int, int);

void TEST1(void) { assert(sum(1, 2) == 3); }

int main() { TEST1(); }