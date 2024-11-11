#include <iostream>
#include <vector>
#include <functional>

using namespace std;

// AIM:
//  DON'T write:      A1( A2( A3 (x) ) )
//  NOW   write:  ->  A1  A2  A3  x



// === INIT ===================================================================
template<typename Cont>
class MyRangeInit {
    Cont& data;
public:
    MyRangeInit(Cont& v) : data(v) {}
    auto& val() { return data; }
};
class MyObserver { };

template<typename Cont>
MyRangeInit<Cont> operator <<= (MyObserver observe, Cont& v) {
    return {v};
}


// ===  A_i ===================================================================
#define genAi(indx)                     \
template<typename MyRange>              \
class A_##indx {                        \
    MyRange data;                       \
public:                                 \
    A_##indx(MyRange x) : data(x) {  }  \
    auto& val() { return data.val(); }  \
};                                      \
class MyObserver_A_##indx { };          \
                                        \
template<typename Other>                \
auto operator <<= (MyObserver_A_##indx observe, Other v) { return A_##indx<Other>(v); } \
                                        \
template<typename MyRange>              \
A_##indx<MyRange> operator <<= (MyObserver observe, A_##indx<MyRange> other) { return other; }

genAi(1)
#define A1 MyObserver_A_1{} <<= MyObserver{} <<=

genAi(2)
#define A2 MyObserver_A_2{} <<= MyObserver{} <<=

genAi(3)
#define A3 MyObserver_A_3{} <<= MyObserver{} <<=





int main() {
    {
        int  t = 3;
        auto x = A1 A3 A1 A2 A2 A3 A3 A1 A1 t;

        cout << "Is  'val()'  and  't'  connected?" << endl;
        cout << "val(): " << x.val() << "    val()++" << endl;  x.val()++;
        cout << "t:     " << t       << "        t++" << endl;  t++;
        cout << "val(): " << x.val() << endl;
    }
}