.intel_syntax noprefix

// System V AMD64 ABI:
//   args: RDI, RSI, RDX, RCX, R8, R9
//   callee-saved: RBX, RBP, R12, R13, R14, R15
//   caller-saved: otherwise

/* This macro avoids symbol mismatching in test binary on macOS */
#if defined(__APPLE__)
# define cdecl(s) _##s
#else
# define cdecl(s) s
#endif

.global cdecl(ABIRegTestHelper)
cdecl(ABIRegTestHelper):
  // Args:
  //   RDI: function(RDI, RSI, RDX, RCX, R8, R9)
  // This function returns:
  // 0: PASSed
  // 1: the function returned non-zero value
  // 2: the function broke RBX
  // 3: the function broke RBP
  // 4: the function broke R12
  // 5: the function broke R13
  // 6: the function broke R14
  // 7: the function broke R15
  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15
  // [rsp + 8 * 5] == rbx
  // [rsp + 8 * 4] == rbp
  // [rsp + 8 * 3] == r12
  // [rsp + 8 * 2] == r13
  // [rsp + 8 * 1] == r14
  // [rsp + 8 * 0] == r15

  // Set initial values
  mov rax, 0x636d70696c69756d // "cmpilium"
  mov rbx, rax
  mov rbp, rax
  mov r12, rax
  mov r13, rax
  mov r14, rax
  mov r15, rax
  // Call a given function
  call rdi
  // Check a return value
  cmp rax, 0
  setne al
  movsx rax, al
  // Check if registers are not modified
  mov rdi, 0x636d70696c69756d // "cmpilium"
  // Check rbx
  cmp rdi, rbx
  je rbx_ok
  or rax, 0b10
rbx_ok:
  // Check rbp
  cmp rdi, rbp
  je rbp_ok
  or rax, 0b100
rbp_ok:
  // Check r12
  cmp rdi, r12
  je r12_ok
  or rax, 0b1000
r12_ok:
  // Check r13
  cmp rdi, r13
  je r13_ok
  or rax, 0b10000
r13_ok:
  // Check r14
  cmp rdi, r14
  je r14_ok
  or rax, 0b100000
r14_ok:
  // Check r15
  cmp rdi, r15
  je r15_ok
  or rax, 0b1000000
r15_ok:
  // Revert regs
  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx
  ret

.global cdecl(TargetReturns0)
cdecl(TargetReturns0):
  mov rax, 0
  ret

.global cdecl(TargetReturns1)
cdecl(TargetReturns1):
  mov rax, 1
  ret

.global cdecl(TargetBreakAllRegs)
cdecl(TargetBreakAllRegs):
  mov rax, 0
  neg rbx
  neg rbp
  neg r12
  neg r13
  neg r14
  neg r15
  ret
