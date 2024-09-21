#ifndef E
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <set>
#include <iomanip>
#include <array>
#include <exception>
#include <optional>
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

// tmp(size_t M, T, size_t N)
// void extract(array<T, N>& a) {
//     if constexpr (M < N) {
//         cout << a[M] << endl;
//         extract<M+1>(a);
//     }
//         // return tuple<T>{a[N-1], extract<T, N, M+1>(a)};
//     else
//         return;
// }

// int sum() { return 0; }



struct LAST { };

tmp(T, V) 
struct collector {
    T neww;
    V oldd;

    tmp(size_t Npos)
    auto& get() {
        if constexpr (Npos == 0) return neww;
        else return oldd.template get<Npos-1>();
    }

    tmp(size_t Npos, K)
    auto push(K a) {
        if constexpr (Npos == 0) return collector<K, my_type> {a, *this};
        else {
            auto tail = oldd.template push<Npos-1>(a);
            return collector<T, decltype(tail)>{neww, tail};
        }
    }

    tmp(size_t Npos, K, L)
    auto push(collector<K, L> a) {
        return this->push<Npos>(a.neww).template push<Npos+1>(a.oldd);
    }

    tmp(size_t Npos)
    auto push(collector<LAST, LAST> a) { return *this; }

    using my_type = collector<T, V>;

    tmp(K, L)
    friend ostream& operator << (ostream& out, collector<K, L> cltr);
};


// struct LAST {
//     // tmp(size_t N)
//     // auto& get() { 
//     //     static_assert(0, "get behiend the end!");
//     //     return *this;
//     // }
    
//     // tmp(size_t Npos, K)
//     // auto push(K a) {
//     //     static_assert(Npos == 0, "push behiend the end!");
//     //     return collector<K, LAST> {a};
//     // }

//     // tmp(size_t Npos, K, L)
//     // auto push(collector<K, L> a) {
//     //     static_assert(Npos == 0, "push behiend the end!");
//     //     return this->push<Npos>(a.neww).template push<Npos+1>(a.oldd);
//     // }
// };

// // tmp(T, V)
// // tmp(size_t Npos)
// // auto collector<T, V>::push(collector<LAST, LAST> a) { return *this; }

// // tmp(size_t Npos)
// // auto collector<LAST, LAST>::push(collector<LAST, LAST> a) { return *this; }


tmp(T, V)
ostream& operator << (ostream& out, collector<T, V> cltr) {
    return out //<< "{"
               << cltr.neww << " " << cltr.oldd //<< "}"
               ;
}

ostream& operator << (ostream& out, collector<LAST, LAST> cltr) {
    return out << " END!" << endl;
}


collector<LAST, LAST> make_collector() {
    return {};
}

tmp(T, x Args) 
auto make_collector(T a, xAA) {
    auto tail = make_collector(args...);
    return collector<T, decltype(tail)>{a, tail};
}


tmp(T, size_t N, size_t M = 0)
auto array_to_my_collector(array<T, N>& a) {
    if constexpr (M == N) return make_collector();
    else return array_to_my_collector<T, N, M+1>(a).template push<0>(a[M]);
}

int main() {
    // array<int, 4> a{5, -3, 4, 7};
    // auto x = extract(a);
    // extract<0>(a);
    // auto p = sum(2, 4, 1, 9);
    // cout << get<0>(p) << endl;
    // void a;

    // EXAMPLE 1:
    auto p = make_collector( 1, 4, string{"hi"});
    p.get<1>() *= 2;
    p.get<2>()[0] += 'A'-'a';
    // cout << p.get<1>();
    auto xp = p.push<3>(9).push<0>(10);
    cout << xp;

    // EXAMPLE 2:
    array<string, 3> t{"hi", "mweow", "sad"};
    auto q = array_to_my_collector(t);
    cout << q;

    // EXAMPLE 3:
    auto r = xp.push<3>(q);
    cout << r;

    // COMBINE EXAMPLE:
    array<string, 3> tt{"hi", "mweow", "sad"};
    cout << make_collector(2.3).push<0>("START").push<1>(array_to_my_collector(tt));
}
