#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <stack>
#include <string.h>

using namespace std;

#define ll long long
#define llu ll unsigned


template<typename IterVal>
class A {
public:
    A(IterVal& aa, int& bb) : a(aa), b(bb) {}

    IterVal& a;
    int& b;
};

template<typename Iter>
class B {
public:
    B(Iter pp) : p(pp) { }

    auto val() {
        return A(*p, counter);
    }

    decltype(*Iter{})  get() const { return *p; }
    decltype(*Iter{})& get()       { return *p; }

    void operator ++ () {
        p++;
        counter++;
    }


    Iter p;
    
    int counter = 0;
};

int main() {
    vector<int> vv{1, 6, 3, 4, 0, 7, 5};
    B b(vv.begin());

    while( b.p != vv.end() ) {
        cout << b.get();
        
        { // копирование значения
            auto c = b.get();
            c++;
            cout << " (" << c << ")";
        }
        cout << "  -> " << b.get();

        { // значение по ссылке
            auto& c = b.get();
            c++;
            cout << " (" << c << ")";
        }
        cout << "  -> " << b.get() << "  | ";

        
        auto [e, i] = b.val();
        cout << i << " " << e << endl;

        ++b;
    }    
}
