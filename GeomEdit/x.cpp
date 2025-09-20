// ПОФИГУ РЕАЛИЗОВАТЬ БЫ ХОТЬ КАК-ТО СНАЧАЛА ЭТОТ ТРЕШАК

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

    T& operator [] (int i) { return data[i]; }
    
    void push_back(T val) { insert_before(n+1, val); }
    void insert_before(int i, T val) {
        data[free   ] =  val;
        next[prev[i]] = free;
        sync_shift(prev[free], prev[i], free, next[free], i);
        size++;
    }

    void erase(int i) {
        prev[next[i]] = prev[i];
        next[prev[i]] = next[i];
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



myanimelist<int> xorder(N);
myanimelist<vector<int>> yorder(N);

void meowsh() {  // -> sh+ow+me
    // произведём градацию высоты
    vector<int> h(N+1);
    for(int i = yorder.next[0], H = 1; i != yorder.n+1; i = yorder.next[i], H++) {
        h[i] = H;
    }
    
    vector<int> cross;
    for(int i = xorder.next[0]; i != xorder.n+1; i = xorder.next[i]) {
        cross.push_back( h[ xorder[i] ] );
    }

    for(int i = 0; i < cross.size(); i++) {
        cout << string(cross[i], 'A'+i) << endl;
    }
}

struct Rect { int x1, y1, x2, y2; };

vector<Rect> history;

struct Section { int x1, x2, y; };
vector<Section> Lain;

struct Nothing { };
myanimelist<Nothing> xcords(N);

// n - количество точек для растановки
// k - количество отсеков
void test(int n, int k) {
    vector<int> pos(n);
    while( 1 ) {
        {   // печать
            vector<int> sect(k);
            for(auto e : pos ) sect[e]++;
            for(auto e : sect) cout << e << ' ';
            cout << endl;
        }

        // обновляем состояние
        int i = n-1;
        while( i >= 0 && pos[i] == k-1 ) i--;
        if( i < 0 ) break;
        int blck = pos[i]+1;
        while( i < n ) pos[i++] = blck;
    }   
}


int main() {
    test(3, 4);



}