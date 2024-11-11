#include <iostream>
#include <vector>
#include <map>
#include <functional>

using namespace std;

// СПАСИБО: https://habr.com/ru/articles/419579/



// === HELP MACRO's ===========================================================
#define tSAMEFUNC(witch1, witch2, ...) auto witch1() { return data.witch2(); }
#define  SAMEFUNC(...) tSAMEFUNC(__VA_ARGS__, __VA_ARGS__)

#define SAMEOBVES(name)                 \
class MyObserver_##name{};              \
template<typename MyRange> class name;  \
template<typename Other>                \
auto        operator <<= (MyObserver_##name observe, Other v) { \
    return name<Other>(v);              \
}                                       \
                                        \
template<typename MyRange>              \
name<MyRange> operator <<= (MyObserver observe, name<MyRange> other) {  \
    return other;                       \
}                                       \
                                        \
template<typename MyRange>              \
class name {                            \
    MyRange data;                       \
public:                                 \
    SAMEFUNC(size)                      \
    name(MyRange  x) : data(x) {}

#define ENDOBVES  };



// === INIT ===================================================================
class MyObserver{};

template<typename Cont>
class MyRangeInit {
    Cont& data;
public:
    MyRangeInit(Cont& v) : data(v) {}
    auto& val() { return data; }

    SAMEFUNC( begin)   SAMEFUNC( end) 
    SAMEFUNC(rbegin)   SAMEFUNC(rend)
    SAMEFUNC(  size)
};

template<typename Cont>
MyRangeInit<Cont> operator <<= (MyObserver observe, Cont& v) { return {v}; }



// ===== A1 ===================================================================
// REVERSE
#define   reverse MyObserver_A1{} <<= MyObserver{} <<=
SAMEOBVES(A1)
    SAMEFUNC( begin, rbegin)   SAMEFUNC( end, rend) 
    SAMEFUNC(rbegin,  begin)   SAMEFUNC(rend,  end)
ENDOBVES



// ===== A2 ===================================================================
// ZIP
#define enumerate MyObserver_A2{} <<= MyObserver{} <<=
SAMEOBVES(A2)
    template<int Adder, typename DataIter>
    class Iterator {
        DataIter p;
        int counter = 0;
    public:
        Iterator(DataIter newp, int start_count = 0) : p(newp), counter(start_count) {}

        bool      operator != (Iterator other) { return p != other.p; }
        Iterator  operator ++ () { Iterator temp = *this; ++p; counter += Adder; return temp; }
        auto      operator *  () { return pair{*p, counter}; }
    };

    auto  begin() { return Iterator<+1, decltype(data. begin()) >(data. begin()); }
    auto  end  () { return Iterator<+1, decltype(data. begin()) >(data. end  ()); }

    auto rbegin() { return Iterator<-1, decltype(data.rbegin()) >{data.rbegin(), (int)data.size()-1}; }
    auto rend  () { return Iterator<-1, decltype(data.rbegin()) >{data.rend  (), (int)data.size()-1}; }
ENDOBVES



#undef tSAMEFUNC
#undef  SAMEFUNC
#undef  SAMEOBVES
#undef   ENDOBVES
// ============================================================================



int main() {
    map<int, int> data = { {0, 3}, {2, 7}, {5, -1}, {11, 10}};

    cout << "\n           NORMAL data |  ";
    for(auto [key, val] : data) cout << "{" << key << " : " << ++val << "}   ";

    // cout << "\n          REVERSE data |  ";
    // for(auto  [key, val] : reverse data) cout << "{" << key << " : " << ++val << "};   ";

    // cout << "\n        ENUMERATE data |  ";
    // for(auto [elem,  i] : reverse enumerate reverse  data) {
    //     auto [key, val] = elem;
    //     cout << "[" << i << "] {" << key << " : " << ++val << "}   ";
    // }

    // cout << "\nENUMERATE REVERSE data |  ";
    // for(auto [elem,  i] : enumerate reverse data) {
    //     auto [key, val] = elem;
    //     cout << "[" << i << "] {" << key << " : " << val << "}   ";
    // }

    // cout << "\nREVERSE ENUMERATE data |  ";
    // for(auto [elem,  i] : reverse enumerate data) {
    //     auto [key, val] = elem;
    //     cout << "[" << i << "] {" << key << " : " << val << "}   ";
    // }
    // cout << endl;


    cout << "\nENUMERATE ENUMERATE |  ";
    for(auto [elem_i1,  i2] : reverse enumerate  enumerate  data) {
        auto [elem, i1] = elem_i1;
        auto [key, val] = elem;
        cout << "[" << i1 << "][" << i2 << "] {" << key << " : " << val << "}   ";
    }
}
   