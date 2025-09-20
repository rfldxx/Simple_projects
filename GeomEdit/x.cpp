#include <bits/stdc++.h>
using namespace std;

const int N = 5;

template<typename T>
void sync_shift(T) {}

template<typename T, typename... Args>
void sync_shift(T& a, T& b, Args&&... args) {
    a = b;
    sync_shift(b, args...);
}


// Боль . . .
// лол, реализовываю связный список в 4ый раз
template<typename T>
struct myanimelist {
    int n, free = 1, size = 0;
    vector<  T> data;
    vector<int> prev, next;
    
    void push_back(T val) { insert_before(n+1, val); }
    void insert_before(int i, T val) {
        data[free   ] =  val;
        next[prev[i]] = free;
        sync_shift(prev[free], prev[i], free, next[free], i);
        size++;
    }

    void erase(int i) {
        prev[next [i]] = prev[i];
        next[prev [i]] = next[i];
        sync_shift(next[i], free, i);
        size--;
    }

    // в начале и в конце виртуальные не удаляемые (бедроковая) вершины: "0" и "n+1"
    // + регистр для free (чтобы if не писать): "n+2" - ай пофиг, без него можно, проблемы user-а если не уследил
    myanimelist(int reserve) : n(reserve), data(n+2), prev(n+2), next(n+2) {
        for(int i = 1; i <= n; i++) next[i] = i+1;
        // бедроковые вершины:
        next[0] = n+1;
        prev[n+1] = 0;
    }
};


template<typename T>
ostream& operator << (ostream& out, const myanimelist<T> x) {
    out << "(Size:" << x.size << " / free:" << x.free << ")  "; 
    if( !x.size ) return out << "empty" << endl;
    int i = x.next[0];
    while( 1 ) {
        out << "[" << i << "]: " << x.data[i];
        i = x.next[i];
        if( i == x.n + 1 ) return out << endl;
        out << " , ";
    }
}

// struct Lain {
//     vector<pair<int, int>> ab; // отрезки
// };

// void meowsh() {  // -> sh+ow+me
//     // произведём градацию высоты
//     vector<int> h(N);
//     for(int i = yorder.start, H = 1; i != yorder.end; i = yorder.next[i], H++) {
//         h[i] = H;
//     }
    
//     vector<int> cross;
//     for(int i = xorder.start i != xorder.end; i = xor.next[i]) {
//         cross.push_back( h[ xorder.data[i] ] );
//     }

//     for(int i = 0; i < cross.size(); i++) {
//         cout << string(cross[i], 'A'+i) << endl;
//     }
// }

// myanimelist<int> xorder(N);
// myanimelist<vector<int>> yorder(N);

int main() {
    // yorder.push_back({})
    // xorder.push_back()


    myanimelist<int> x(3);

    cout << x;

    x.push_back(1);
    cout << x;
    x.push_back(2);
    cout << x;
    x.push_back(3);
    cout << x;

    x.erase(2);
    cout << x;
    x.erase(1);
    cout << x;
    x.erase(3);
    cout << x;

    cout << "///////////////////////////////\n";

    x.push_back(5);
    cout << x;
    x.push_back(4);
    cout << x;
    x.push_back(6);
    cout << x;

    x.erase(3);
    cout << x;
    x.erase(1);
    cout << x;
    x.erase(2);
    cout << x;
}