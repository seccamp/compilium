#include <stdio.h>

#define add1 (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10)
#define add2 (add1 * add1)
#define add3 (add2 * add2)
#define add4 (add3 * add3)
#define add5 (add4 * add4)
int main() {
    int ans = 0;
    for (int i = 0; i <= 10000000;i++) {
       ans = add1 + add2 + (add2 + add3) + (add4 + add5);
    } 
  return 0;
}


