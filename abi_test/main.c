#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// System V AMD64 ABI:
//  args:
//    RDI, RSI, RDX, RCX, R8, R9
//  callee-saved(should be kept on return):
//    RBX, RBP, R12, R13, R14, R15
//  caller-saved(can be destroyed):
//    otherwise

// @helper.S
uint64_t ABIRegTestHelper(void *, uint64_t, uint64_t, uint64_t, uint64_t,
                          uint64_t);
uint64_t TargetReturns0();
uint64_t TargetReturns1();
uint64_t TargetBreakAllRegs();

static void PrintTestResult(const char *expr, uint64_t actual,
                            uint64_t expected, const char *s, int line) {
  int satisfied = (actual == expected);
  printf("%s @ %s:%d == 0x%02llX %s\n", expr, s, line, expected,
         satisfied ? "PASS" : "FAIL");
  if (!satisfied) {
    printf("  Expected 0x%llX\n", expected);
    printf("   but got 0x%llX\n", actual);
    exit(EXIT_FAILURE);
  }
}

#define assert(expr, expected) \
  PrintTestResult(#expr, (expr), (expected), __FILE__, __LINE__)

int main() {
  assert(ABIRegTestHelper(TargetReturns0, 0, 0, 0, 0, 0), 0);
  assert(ABIRegTestHelper(TargetReturns1, 0, 0, 0, 0, 0), 1);
  assert(ABIRegTestHelper(TargetBreakAllRegs, 0, 0, 0, 0, 0), 0x7E);
  puts("PASS");
  return 0;
}
