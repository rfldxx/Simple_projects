// ПОФИГУ РЕАЛИЗОВАТЬ БЫ ХОТЬ КАК-ТО СНАЧАЛА ЭТОТ ТРЕШАК

#include <bits/stdc++.h>
using namespace std;


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



const int N = 5;

struct Section {
    int x1, x2, y;
    vector<int> subs;  // точки внутри
    // => количество отсеков = 2*subs.size()+1
};

vector<Section> lain;

struct Nothing { };
myanimelist<Nothing> xcords(N), ycords(N);
myanimelist<Nothing> possibly(N); // допустимые для выбора y



struct Rect { int x1, y1, x2, y2; };
vector<Rect> history;

// печать всех прямоугольников
void Miku(const vector<Rect> history) {
    int n = 0, m = 0;
    vector<int> xx(N), yy(N);
    for(int i = 0, I = 0; 1; i = xcords.next[i]) {
        xx[i] = I++;
        m++;
        if( i == xcords.n+1 ) break;
    }
    for(int i = 0, I = 0; 1; i = ycords.next[i]) {
        yy[i] = I++;
        n++;
        if( i == ycords.n+1 ) break;
    }
    
    vector field(n, vector<int>(m));
    for(int id = 0; auto [xx1, yy1, xx2, yy2] : history) {
        int x1 = xx[xx1], x2 = xx[xx2];
        int y1 = yy[yy1], y2 = yy[yy2];
        for(int i = y1; i <= y2; i++)
            for(int j = x1; j <= x2; j++)
                field[i][j] = id;

        id++;
    }
    for(auto& vv : field) {
        for(auto e : vv) cout << ((char)('A'+e));
        cout << endl;
    }
    cout << endl;    
}


void test2(int n, int k) {
    vector<int> pos(k);
    pos[0] = n;
    while( 1 ) {
        {   // печать
            for(auto e : pos) cout << e << ' ';
            cout << endl;
        }

        // обновляем состояние
        if( pos[k-1] == n ) break;

        int i = k-2;
        while( pos[i] == 0 ) i--;
        int x = pos[k-1];
        pos[k-1]  = 0;
        pos[i]   -= 1;
        pos[i+1] += 1 + x;
    }   
}

// s - количество внутрених точек на сегменте
// => 2*s + 1 отсек
bool test3(int n, int s) {
    int cnt = 0;
    int k = 2*s+1;
    vector<int> pos(k);
    pos[0] = n;
    while( 1 ) {
        {   // печать
            // for(auto e : pos) cout << e << ' ';
            // cout << endl;
            cnt++;
        }

        // обновляем состояние
        int x = pos[k-1];
        pos[k-1] = 0;
        if( x == n ) break;        
        
        int i = k-2;
        while( pos[i] == 0 ) i--;
        pos[i]   -= 1;
        pos[i+1] += 1;
        if( (i+1)&1 ) pos[i+2] += x;
        else          pos[i+1] += x;
    }

    // должно быть sum_{t=0}^{min(s, n)} (n+s-t)! / (n-t)! / (s-t)! / t!
    int need = 0;
    int A = min(n,s), B = max(n, s);
    long long mlt = 1;
    for(int x = A; x > 0; x--) 
        mlt = (mlt*(B+x)) / x; 
    
    for(int t = 0; t <= A; t++) {
        need +=  mlt;
        mlt   = (mlt * (A-t) * (B-t)) / (A+B - t) / (t+1);
    }
    // cout << cnt << endl;
    // cout << need << endl;
    return need == cnt;
}


int main() {
    bool ok = 1;
    for(int n = 1; n <= 10; n++)
    for(int s = 1; s <= 10; s++)
        ok &= test3(2, 2);
    cout << ok << endl;
    // lain.push_back( {0, N, 0} );

    // {   // получили сегмент
    //     Section yoko;
    //     auto [x1, x2, y, subs] = yoko;

    //     int k = 2*subs.size()+1; // количество отсеков
        

    // }

}