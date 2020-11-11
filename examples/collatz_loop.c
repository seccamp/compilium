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
  int _X = 0;

  for(;;) {
    if (n == 1) {
      _X += 1;
      break;
    }
    if (n & 1) {
      n = 3 * n + 1;
    } else {
      n /= 2;
    }
    n = n;
    _X = _X + 1;
  }
  return _X;
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
