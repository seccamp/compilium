#!/bin/bash -e

function test_result {
  input="$1"
  expected="$2"
  expected_stdout="$3"
  testname="$4"
  printf "$expected_stdout" > expected.stdout
  ./compilium --target-os `uname` <<< "$input" > out.S || { \
    echo "Source Input : $input"
    echo "$input" > failcase.c; \
    echo "Compilation failed."; \
    exit 1; }
  gcc out.S
  actual=0
  ./a.out > out.stdout || actual=$?
  if [ $expected = $actual ]; then
      diff -u expected.stdout out.stdout \
        && echo "PASS $testname returns $expected" \
				|| {
				echo "FAIL $testname: stdout diff (actual / expected)";
							echo "expected:";
							echo "----";
							cat expected.stdout;
							echo "----";
							echo "actual:";
							echo "----";
							cat out.stdout;
							echo "----";
							exit 1;
						}
  else
    echo "FAIL $testname: expected $expected but got $actual"; echo $input > failcase.c; exit 1; 
  fi
}

function test_expr_result {
  test_result "int main(){return $1;}" "$2" "" "$1"
}

function test_stmt_result {
  test_result "int main(){$1}" "$2" "" "$1"
}

function test_src_result {
  test_result "$1" "$2" "$3" "$1"
}

# pointer addition https://github.com/seccamp/compilium/issues/1
test_src_result "`cat << EOS
int printf(const char *, ...);
int main() {
  int n = 10;
  int *a;
  int b[10];
  a = b;
  for (int i = 0; i < n; ++i) {
    a[i] = i * 16 + i;
  }
  for (int i = 0; i < n; ++i) {
    printf("    a[%d]=0x%08x\n", i, a[i]);
  }
  for (int i = 0; i < n; ++i) {
    printf("*(a + %d)=0x%08x\n", i, *(a + i));  // bug
  }
  //printf("\n");
  return 0;
}
EOS
`" 0 "`cat << EOS
    a[0]=0x00000000
    a[1]=0x00000011
    a[2]=0x00000022
    a[3]=0x00000033
    a[4]=0x00000044
    a[5]=0x00000055
    a[6]=0x00000066
    a[7]=0x00000077
    a[8]=0x00000088
    a[9]=0x00000099
*(a + 0)=0x00000000
*(a + 1)=0x00000011
*(a + 2)=0x00000022
*(a + 3)=0x00000033
*(a + 4)=0x00000044
*(a + 5)=0x00000055
*(a + 6)=0x00000066
*(a + 7)=0x00000077
*(a + 8)=0x00000088
*(a + 9)=0x00000099\n
EOS
`"

# nested func call with args
test_src_result "`cat << EOS
int g() {
  int a;
  a = 3;
  int b;
  b = 5;
  return a + b;
}

int f() {
  int v;
  v = 2;
  int r;
  r = g();
  return v + r;
}

int main() {
  return f();
}
EOS
`" 10 ""

# func args should be visible
test_src_result "`cat << EOS
int sum(int a, int b) {
  return a + b;
}

int main() {
  return sum(3, 5);
}
EOS
`" 8 ''

# same symbols in diffrent scope shadows previous one
test_src_result "`cat << EOS
int main() {
  int result;
  result = 1;
  int duplicated_var;
  duplicated_var = 2;
  result = result * duplicated_var;
  if(1) {
    int duplicated_var;
    duplicated_var = 3;
    result = result * duplicated_var;
  }
  result = result * duplicated_var;
  return result;
}
EOS
`" 12 ''

# for stmt
test_src_result "`cat << EOS
int main() {
  int i;
  int sum;
  sum = 0;
  for(i = 0; i <= 10; i = i + 1) {
    sum = sum + i;
  }
  return sum;
}
EOS
`" 55 ''

# return with no expression
test_src_result "`cat << EOS
void func_returns_void() {
  return;
}
int main() {
  func_returns_void();
  return 3;
}
EOS
`" 3 ''

test_src_result "`cat << EOS
int three() {
  return 3;
}
int main() {
  return three();
}
EOS
`" 3 ''

test_src_result "`cat << EOS
int puts(char *s);
int main() {
  puts("Hello, world!");
  return 0;
}
EOS
`" 0 'Hello, world!\n'

test_src_result "`cat << EOS
int putchar(int c);
int main() {
  putchar('C');
  return 0;
}
EOS
`" 0 'C'

test_stmt_result 'return 3 + 5 + 7;' 15

test_stmt_result 'return 3 + 5 * 7;' 38

test_stmt_result 'return 3 * 5 / 7;' 2

test_stmt_result 'return 3 * 5 % 7;' 1

test_stmt_result 'int a; int b; int c; a = 3; b = 5; c = 7; return a + b + c;' 15

test_stmt_result 'int a; int b; int c; a = 3; b = 5; c = 7; return a + b * c;' 38

test_stmt_result 'int a; int b; int c; a = 3; b = 5; c = 7; return a * b / c;' 2

test_stmt_result 'int a; int b; int c; a = 3; b = 5; c = 7; return a * b % c;' 1

test_stmt_result 'int a;a = 3; return 3 + 5 + a + 2 * 5;' 21

test_stmt_result 'int a;a = 2 - 5; return a+8;' 5

test_stmt_result 'int a = 6; a /= 2; return a;' 3
test_stmt_result 'int a = 7; a /= 2; return a;' 3
test_stmt_result 'int a = 15; a /= 4; return a;' 3
test_stmt_result 'int a = 16; a /= 4; return a;' 4
test_stmt_result 'int a = 17; a /= 4; return a;' 4
test_stmt_result 'int a = 262143; a /= 32768; return a;' 7
test_stmt_result 'int a = 262144; a /= 32768; return a;' 8
test_stmt_result 'int a = 262145; a /= 32768; return a;' 8

test_stmt_result 'int a = 262145; a /= 7; return a==37449;' 1

test_stmt_result 'int a ; int b = 7; a =  b/2; return a;' 3
test_stmt_result 'int a ; int b = 8; a =  b/2; return a;' 4
test_stmt_result 'int a ; int b = 9; a =  b/2; return a;' 4

test_stmt_result 'int a ; int b = 15; a =  b/4; return a;' 3
test_stmt_result 'int a ; int b = 16; a =  b/4; return a;' 4
test_stmt_result 'int a ; int b = 17; a =  b/4; return a;' 4

test_stmt_result 'int a ; int b = 262143; a =  b/32768; return a;' 7
test_stmt_result 'int a ; int b = 262144; a =  b/32768; return a;' 8
test_stmt_result 'int a ; int b = 262145; a =  b/32768; return a;' 8


# Non-printable
test_expr_result ' 0 ' 0

# Unary Prefix
test_expr_result '+ +1' 1
test_expr_result '- -17' 17

echo "All tests passed."
