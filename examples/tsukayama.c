#include <stdio.h>
#define RECUESIVE_NUM 100

int sum;

//Simply return argv + 300
int pl(int a) {
    int b;
    int c;
    b = 100;
    c = 200;
    return a+b+c;
}

// Recursive Function
// coutinue calling myself until surpass modifed value
int recursive(int i, int tempsum){
    if (i > RECUESIVE_NUM)
        return 0;
    sum = tempsum + pl(i);
    return recursive(i+1, sum);
}

int main(){
    int i = 0;
    sum = 0;
    recursive(i, sum);
    printf("sum : %d\n", sum);
}
