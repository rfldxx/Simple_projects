#include <bits/stdc++.h>
using namespace std;

const int N = 5;

template<typename T>
T shift(T& a, T& b, T c) {
    T t = a;
    a = b;
    b = c;
    return t;
}

// Боль . . .
// лол, реализовываю связный список в 4ый раз
template<typename T>
struct myanimelist<T> {
    vector<T> data;

    vector<int> prev, next;
    
    int  free = 0, size = 0;
    void push_back(T val) { insert_before(n+1, val); }

    void insert_before(int i, T val) {
        shift(prev[free], prev[i]   , free);
        shift(free      , next[free], i   );
        size++;
    }

    void erase (int i) {
        // лол страшно проверять (так было бы без бедроковых вершин)
        // ( i == start ? start : next[prev[i]] ) = next[i];
        // ( i ==  last ?  last : prev[next[i]] ) = prev[i];
        // next[free = i] = free;
        
        next[prev[i]]  = next[i];
        prev[next[i]]  = prev[i];
        next[free = i] = free;
        size--;
    }

    // в начале и в конце виртуальные не удаляемые (бедроковая) вершины: "0" и "n+1"
    myanimelist(int reserve) : data(reserve+2), prev(reserve+2), next(reserve+2) {
        assert(reverse > 1);  // какая культурная проверка
        for(int i = 1; i <= reserve; i++) {
            next[i] = i+1;
        }
        // бедроковые вершины:
        next[0] = reserve+1;
        prev[reserve+1] = 0;
    } 
};

// struct Lain {
//     vector<pair<int, int>> ab; // отрезки
// };

void meowsh() {  // -> sh+ow+me
    // произведём градацию высоты
    vector<int> h(N);
    for(int i = yorder.start, H = 1; i != yorder.end; i = yorder.next[i], H++) {
        h[i] = H;
    }
    
    vector<int> cross;
    for(int i = xorder.start i != xorder.end; i = xor.next[i]) {
        cross.push_back( h[ xorder.data[i] ] );
    }

    for(int i = 0; i < cross.size(); i++) {
        cout << string(cross[i], 'A'+i) << endl;
    }
}

myanimelist<int> xorder(N);
myanimelist<vector<int>> yorder(N);

int main() {
    yorder.push_back({})
    xorder.push_back()





}