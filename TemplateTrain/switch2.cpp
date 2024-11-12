#include <iostream>
#include <vector>
#include <map>
#include <functional>

using namespace std;

// TODO:
//  * сделать  auto&  operator *  ()  для самопального класса A2::Iterator
//    (пока что шиза, как детектировать что нужна сслыка, как её возвращать)
//    (чето, какик-то шрихи можно посмотреть в switch2_draft.cpp)
//  * это в свою очередь (возможно) вызовет резню в классе A2 
//    со всякими auto begin() / auto end() / ...
// 
//  * сделать A3 - оператор zip который итерируется по двум контейнерам одновременно:
//    vector vv1, vv2;
//    for(auto& [e1, e2] : vv1 zip vv2) e1 = e2;


// СПАСИБО: https://habr.com/ru/articles/419579/


// === HELP MACRO's ===========================================================
#define tSAME_FUNC(witch1, witch2, ...) auto witch1() { return data.witch2(); }
#define  SAME_FUNC(...) tSAME_FUNC(__VA_ARGS__, __VA_ARGS__)

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
    SAME_FUNC(size)                     \
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

    SAME_FUNC( begin)   SAME_FUNC( end) 
    SAME_FUNC(rbegin)   SAME_FUNC(rend)
    SAME_FUNC(  size)
};

template<typename Cont>
MyRangeInit<Cont> operator <<= (MyObserver observe, Cont& v) { return {v}; }



// ===== A1 ===================================================================
// REVERSE
#define   reverse MyObserver_A1{} <<= MyObserver{} <<=
SAMEOBVES(A1)
    SAME_FUNC( begin, rbegin)   SAME_FUNC( end, rend) 
    SAME_FUNC(rbegin,  begin)   SAME_FUNC(rend,  end)
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
        
        auto      operator *  () const { return pair{*p, counter}; }

        // ?? pair<DataIterVal&, int&> -> pair<DataIterVal&, int&>&
        // auto&  operator *  () { /* хрен занет */ return wtf; }
    };

    auto  begin() { return Iterator<+1, decltype(data. begin()) >(data. begin()); }
    auto  end  () { return Iterator<+1, decltype(data. begin()) >(data. end  ()); }

    auto rbegin() { return Iterator<-1, decltype(data.rbegin()) >{data.rbegin(), (int)data.size()-1}; }
    auto rend  () { return Iterator<-1, decltype(data.rbegin()) >{data.rend  (), (int)data.size()-1}; }
ENDOBVES



#undef tSAME_FUNC
#undef  SAME_FUNC
#undef  SAMEOBVES
#undef   ENDOBVES
// ============================================================================



int main() {
    map<int, int> data = { {0, 3}, {2, 7}, {5, -1}, {11, 10}};

    cout << "\n           NORMAL data |  ";
    for(auto [key, val] : data) cout << "{" << key << " : " << val << "}   ";
// >           NORMAL data |  {0 : 3}   {2 : 7}   {5 : -1}   {11 : 10}


    cout << "\n          REVERSE data |  ";
    for(auto [key, val] : reverse data) cout << "{" << key << " : " << val << "};   ";
// >          REVERSE data |  {11 : 10};   {5 : -1};   {2 : 7};   {0 : 3};


    cout << "\n        ENUMERATE data |  ";
    for(auto [elem,  i] : reverse enumerate reverse  data) {
        auto [key, val] = elem;
        cout << "[" << i << "] {" << key << " : " << val << "}   ";
    }
// >        ENUMERATE data |  [3] {0 : 3}   [2] {2 : 7}   [1] {5 : -1}   [0] {11 : 10}


    cout << "\nENUMERATE REVERSE data |  ";
    for(auto [elem,  i] : enumerate reverse data) {
        auto [key, val] = elem;
        cout << "[" << i << "] {" << key << " : " << val << "}   ";
    }
// >ENUMERATE REVERSE data |  [0] {11 : 10}   [1] {5 : -1}   [2] {2 : 7}   [3] {0 : 3}


    cout << "\nREVERSE ENUMERATE data |  ";
    for(auto [elem,  i] : reverse enumerate data) {
        auto [key, val] = elem;
        cout << "[" << i << "] {" << key << " : " << val << "}   ";
    }
// >REVERSE ENUMERATE data |  [3] {11 : 10}   [2] {5 : -1}   [1] {2 : 7}   [0] {0 : 3}

    cout << endl;
}
   