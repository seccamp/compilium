#define M 2147483647
#define A M/M
#define B A*A*A*A*A*A*A*A*A*A*A*A*A
#define C B*B*B
#define ONE (C*C)

int _double(int n) {
    if (n == 0) return 0;
    n -= ONE;
    return _double(n) + 2;
}

int main() {
    for (int i = 0; i < 470; i++) {
        _double(10000);
    }
    return 0;
}