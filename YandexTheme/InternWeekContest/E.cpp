// подготовка...
// вроде решается методом - разделяй и влавствуй

#include <iostream>
#include <map>
#include <array>

using namespace std;

// решил на ООПешить
template<typename Cont>
class Interval {
public:
    Interval(Cont& a_) : p(a_.begin()), a(a_) {}
    
    typename Cont::iterator  p;
    Cont& a;
    
    operator int() const { return p != a.end(); }
    // bool operator !  () { return  p == a.end(); }

    auto operator *  () { return *p; }
    auto operator -> () { return  p; }
    Interval operator ++ (int) { Interval temp = *this; p++; return temp; }
};

template<typename Cont>
void combine(Cont& a, Cont& b) {
    array<Interval<Cont>, 2> p = {a, b};

    while( 1 ) {
        bool t = !p[0] || ( p[1] && p[1]->first < p[0]->first );
        
        if( !p[t] ) return;
        
        auto [k, v] = *p[t]++;
        cout << k << " : " << v << endl;
    }
}

int main() {
    map<int, int> a = { {1, 2}, {9, 3}, {3, 5} };
    map<int, int> b = { {4, 7}, {2, 0} };
    
    combine(a, b);
}