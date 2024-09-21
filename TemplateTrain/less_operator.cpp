#include <iostream>
#include <array>

// USING g++
// (because in macro I use construction > ,##__VA_ARGS__ < which supported in g++ compiler)

using namespace std;

template<size_t M = 0, typename T1, typename T2, size_t N>
bool operator < (const array<T1, N>& a, const array<T2, N>& b) {
    if constexpr (N == M) return 0;
    else return a[M] != b[M] ? a[M] < b[M] : operator< <M+1, T1, T2, N>(a, b);
}




// Дальше идет тестирование (для удобства заведём макросы)
// ---------------------------------------------------------------------------------
// TEST( $a_1, $a_2, result)
//   $a_i = (x1, x2, ...)    -> array<decltype(     x1   ),  n> a_i {x1, x2, ...}
//   $a_i = no_bracket       -> array<decltype(no_bracket),  0> a_i;

#define unwrap(...) __VA_ARGS__
#define catch_bracket(...) ~, ~

#define FIRST(a,       ...) a
#define THIRD(a, b, c, ...) c

#define GET6(a, b, c, d, e, f, ...) f
#define NARG(...) GET6( ,##__VA_ARGS__, 4, 3, 2, 1, 0)

#define zerroARRAY(name, a, ...) array<decltype(      a{}),                0 > name
#define     nARRAY(name, a, ...) array<decltype(FIRST a  ), NARG(__VA_ARGS__)> name{__VA_ARGS__}
#define      ARRAY(name, a, ...) THIRD(__VA_ARGS__, nARRAY, zerroARRAY) (name, a, unwrap a)

#define TEST_LESS( a, b, result ) { \
    ARRAY(a1, a, catch_bracket a);  \
    ARRAY(a2, b, catch_bracket b);  \
    if( (a1 < a2) != result ) {     \
        error_count++;              \
        cout << "ERROR in " << #a << " and " << #b << "  - excepted: " << result << endl; \
    }                               \
} 


int main() {
    int error_count = 0;
    TEST_LESS(string, string, 0)  // check's array<string, 0>
    TEST_LESS(int   , double, 0)  // check's array<int, 0> and array<double, 0>

    TEST_LESS((9), (9), 0)
    TEST_LESS((8), (9), 1)
    TEST_LESS((9), (8), 0)

    TEST_LESS((3, 7), (3, 7), 0)
    TEST_LESS((4, 0), (3, 8), 0)
    TEST_LESS((3, 7), (5, 0), 1)
    TEST_LESS((3, 7), (3, 8), 1)

    TEST_LESS((string{"aa"}, "aab", "x"), (string{"aa"}, "aab", "y"), 1)

    cout << "ALL errors: " << error_count << endl;
}