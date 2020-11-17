/* Calulate Fibonacci Sequence */

#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]) {
    const int n = 1000000;
    int ar1[n];
    ar1[0] = 0, ar1[1] = 1;
    for (int i = 2; i < n; ++i) {
        ar1[i] = (ar1[i - 1] + ar1[i - 2]) & 0x7fffffff;
    }
    float ar2[n];
    for (int i = 0; i < n; ++i) {
        ar2[i] = 1. / (ar1[i] + 1.0);
    }
    return 0;
}

/* LLVM IR
ret     1
br      5998
add     2996
fadd    1000
sub     1996
fdiv    1000
and     998
alloca  8
load    11988
store   4004
getelementptr   4996
sext    4994
sitofp  1000
fptrunc 1000
icmp    2000

load: load
store: store
int: add+sub+and+getelementptr+sext
fp: fadd+fdiv+sitofp+fptrunc
branch: ret+br+icmp
unknown: alloca
*/

