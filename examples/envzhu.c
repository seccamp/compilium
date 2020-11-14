#include <stdio.h>

void PrimeFactorization(int n) {
  int i;

  if (n == 1) {
    return;
  }

  for (i = 2; i < n; i++) {
    if (n % i == 0) {
      break;
    }
  }

  if (i == n) {
    printf("%d\n", n);
    return;
  }

  printf("%d\n", i);
  PrimeFactorization(n / i);
  return;
}

int main() {
  // int num[] = {
  //  366384294, 1082565612, 1771807709, 282873727, 398189471,
  //  1988054611, 1495237291, 927243533, 1213357382, 60362733
  //};
  int num[10];
  num[0] = 366384294;
  num[1] = 1082565612;
  num[2] = 71807709;
  num[3] = 282873727;
  num[4] = 398189471;
  num[5] = 1988054611;
  num[6] = 1495237291;
  num[7] = 927243533;
  num[8] = 1213357382;
  num[9] = 60362733;

  for (int i = 0; i < 10; i++) {
    printf("Factor of %d:\n", num[i]);
    PrimeFactorization(num[i]);
  }
  return 0;
}
