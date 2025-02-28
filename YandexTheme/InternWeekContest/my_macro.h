#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <stack>

using namespace std;

// ----- [ REPEAT] ----------------------------------------------------------------------
#define wCAT(a, b) a##b
#define  CAT( ...) wCAT(__VA_ARGS__)

#define    R_1(F, a)      F(a)
#define    R_2(F, a, ...) F(a) R_1(F, __VA_ARGS__)
#define    R_3(F, a, ...) F(a) R_2(F, __VA_ARGS__)
#define    R_4(F, a, ...) F(a) R_3(F, __VA_ARGS__)
#define    R_5(F, a, ...) F(a) R_4(F, __VA_ARGS__)
#define    R_6(F, a, ...) F(a) R_5(F, __VA_ARGS__)
#define getNN(a1, a2, a3, a4, a5, a6, a7, ...) a7
#define    NN(...) getNN(__VA_ARGS__, 6, 5, 4, 3, 2, 1)
#define REPEAT(F, ...) CAT( R_,  NN(__VA_ARGS__) ) (F,  __VA_ARGS__)
// --------------------------------------------------------------------------------------

#define VAL(a) int a; cin >> a;
#define GET(...) REPEAT(VAL, __VA_ARGS__)


#define llu long long unsigned

