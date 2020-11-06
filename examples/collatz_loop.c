#include <stdio.h>

static int collatz(int n) {
  // printf("%d->", n);
  if (n == 1) return 1;
  if (n & 1) {
    n = 3 * n + 1;
  } else {
    n /= 2;
  }
  return collatz(n) + 1;
}

static int collatz_loop(int n) {
  int sum = 0;

  while(1) {
    if (n == 1) {
      sum = sum + 1;
      break;
    }
    if (n & 1) {
      n = 3 * n + 1;
    } else {
      n /= 2;
    }
    n = n;
    sum = sum + 1;
  }
  return sum;
}

int main() {
  for (int k = 0; k < 10; k++) {
    for (int i = 1; i < 100000; i++) {
      printf("%d: ", i);
      //int cycle = collatz(i);
      int cycle = collatz_loop(i);
      printf("OK! cycle = %d\n", cycle);
    }
  }
  return 0;
}
