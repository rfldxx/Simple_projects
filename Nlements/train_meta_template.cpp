#ifndef E
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <set>
#include <iomanip>
#include <array>
#endif

using namespace std;


// Чтобы не писать template <typename A, ... >
//-----------------------------------------------------------------------------
// tmp(T, size_t N, x V)  ->  template< typename T, size_t N, typename... V >
//                  xAA   ->  Args... args 
//                   AA   ->  args...

#define xAA Args... args
#define  AA args...

#define  REP0(...)
#define  REP1(f, a, ...) f(a)  REP0(f, __VA_ARGS__)
#define  REP2(f, a, ...) f(a), REP1(f, __VA_ARGS__)
#define  REP3(f, a, ...) f(a), REP2(f, __VA_ARGS__)
#define  REP4(f, a, ...) f(a), REP3(f, __VA_ARGS__)
#define tREP(nn, ...) REP##nn (__VA_ARGS__)
#define  REP(...) tREP(__VA_ARGS__)

#define tSECOND(a, b, ...)  b
#define  SECOND(...) tSECOND(__VA_ARGS__)
#define  GET4(a, b, c, d, e, ...) e
#define  NARG(...) GET4(__VA_ARGS__, 4, 3, 2, 1)

#define CATCH_size_t ~, size_t
#define CATCH_x      ~, typename...
#define to_typename(a) SECOND(CATCH_##a, typename a)
#define tmp(...) template< REP(NARG(__VA_ARGS__), to_typename, __VA_ARGS__) >
// ----------------------------------------------------------------------------

tmp(size_t M, T, size_t N)
void extract(array<T, N>& a) {
    if constexpr (M < N) {
        cout << a[M] << endl;
        extract<M+1>(a);
    }
        // return tuple<T>{a[N-1], extract<T, N, M+1>(a)};
    else
        return;
}

// int sum() { return 0; }

tmp(T, V) 
struct collector {
    T neww;
    V oldd;

    tmp(size_t Npos)
    auto get() {
        if constexpr (Npos == 0) {
            return neww;
        }
        else return oldd.template get<Npos-1>();
    }
};

struct LAST {
    tmp(size_t N)
    auto get() { return -1; }
};


tmp(T)
collector<T, LAST> f(T a) {
    return collector<T, LAST>{a};
}

tmp(T, x Args) 
auto f(T a, xAA) {
    auto tail = f(args...);
    return collector<T, decltype(tail)>{a, tail};
}


tmp(x Args)
// tuple<Args...>
auto  sum(Args... args) {
    return tuple{ (..., args) };
}

int main() {
    array<int, 4> a{5, -3, 4, 7};
    // auto x = extract(a);
    // extract<0>(a);
    // auto p = sum(2, 4, 1, 9);
    // cout << get<0>(p) << endl;
    // void a;
    auto p = f(-7, 2, 3);
    cout << p.get<1>();
}
