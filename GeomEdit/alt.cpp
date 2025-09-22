// ЧЕРЕЗ СЕТКУ HxH
// (халтурка)

#include <bits/stdc++.h>
using namespace std;

#define N 4


// вообще наверное есть операция "<" на vector<vector<int>>, 
// но чет не хочется такое сложное использовать
map<string, vector<vector<int>>> history;

string miku(vector<vector<int>> Matr);
vector<vector<int>> deobfuscate(vector<vector<int>> stamp);


int paint[N][N];

vector<vector<int>> save_paint() {
    vector Matr(N, vector<int>(N));
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            Matr[i][j] = paint[i][j];
    return deobfuscate(Matr);
}


// красим k-ый прямоугольник
// возвращает: смогли ли хоть как-то докрасить (хотя всё равно не будем это использовать...)
// асимптотика всего решиния: ~O( (N^2) ^ N )
bool produce(int k) {
    // находим самый верхний, левый, непокрашенный элемент
    int   y = 0, x;  
    for(; y < N; y++) 
        for(x = 0; x < N; x++)
            if( !paint[y][x] ) goto breakcore;
    // простите за goto
    return 0;
    breakcore:

    // граница этого непокрашенного сегмента
    int xout = x;
    while( xout < N && !paint[y][xout] ) xout++;

    if( k == 1 ) {  // ладно, напишем шорткат
        // кринж, душат goto
        vector<vector<int>> vv;
        string mk;

        bool ans = 0;
        for(int i = y; i < N; i++)
            for(int j = x; j < xout; j++) 
                if ( paint[i][j] ) goto clearing;
                else paint[i][j] = 1;

        // проверяем, что всё покрашенно
        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++) 
                if( !paint[i][j] ) goto clearing;

        // это победа, друзья! мы смоги найти какую-то раскраску!
        ans = 1;
        vv = save_paint();
        mk = miku(vv);
        if( !history.count(mk) ) history[mk] = vv;

        clearing:
        for(int i = y; i < N; i++)
            for(int j = x; j < xout; j++)
                if( paint[i][j] == 1 ) paint[i][j] = 0;
        
        return ans;
    }

    // перебор
    bool collect = 0;
    for(int i = y; i < N; i++) {
        for(int j = x; j < xout; j++) {
            // красим весь столбец
            for(int ii = y; ii <= i; ii++)
                paint[ii][j] = k;
        
            collect |= produce(k-1);

            if( k == N ) {
                cout << "."; fflush(stdout);
            }
        }

        if( k == N ) {
            cout << "Final  y=" << i << "!" << endl;
        }

        // возвращаем холст к "началу"
        for(int ii = y; ii <= i; ii++)
            for(int j = x; j < xout; j++)
                paint[ii][j] = 0;
    }

    return collect;
}


vector<vector<int>> deobfuscate(vector<vector<int>> stamp) {
    bool xxx = 0;
    vector<bool> xcords(N, xxx), ycords(N, xxx);
    xcords[0] = ycords[0] = 1;

    for(int i = 0; i < N; i++)
        for(int j = 1; j < N; j++)
            if( /*stamp[i][j] &&*/ stamp[i][j-1] != stamp[i][j] )
                xcords[j] = 1;

    for(int j = 0; j < N; j++)
        for(int i = 1; i < N; i++)
            if( /*stamp[i][j] &&*/ stamp[i-1][j] != stamp[i][j] )
                ycords[i] = 1;

    int n = 0, m = 0;
    for(int i = 0; i < N; i++) {
        n += ycords[i];
        m += xcords[i];
    }

    vector Matr(n, vector<int>(m));
    int xpos = 0, ypos = 0;
    for(int i = 0; i < N; i++) {
        if( !ycords[i] ) continue;

        for(int j = 0; j < N; j++) {
            if( !xcords[j] ) continue;
            Matr[ypos][xpos] = stamp[i][j];
            xpos++;
        }
        xpos = 0;
        ypos++;
    }

    return Matr;
}
 
string miku(vector<vector<int>> Matr) {
    int n = Matr.size(), m = Matr[0].size();
    
    // ВНИМАНИЕ !!!!!
    if( n > m ) return "[NOT HORIZONTAL]\n";

    string result;
    for(int i = 0; i < n; i++) {
        string lain(m, '#');
        for(int j = 0; j < m; j++)
            lain[j] = Matr[i][j] ? 'A' + (Matr[i][j]-1) : '.';
        result += lain + '\n';
    }    
    return result;
}





// мммм, вот бы имба была если понимать структуру матричного равенства:  A*A = B*B
// а ещё интересно, умеет ли человечество решать системы уравнений второй степени
// а ещё, как wolfram аналетически(?) считает:  f(A)
using   Rect = tuple<int, int, int, int>;
using paving = pair<vector<Rect>, pair<int, int>>;

paving make_crush(vector<vector<int>> Matr) {
    int n = Matr.size(), m = Matr[0].size();
    // assert( n >= m );
    vector<bool> was(N);
    vector<Rect> ans(N);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) {
            int clr = Matr[i][j];
            if( was[clr-1] ) continue;
        
            int yout = i+1, xout = j+1;
            while( yout < n && Matr[yout][j] == clr ) yout++;
            while( xout < m && Matr[i][xout] == clr ) xout++;
            was[clr-1] = 1;
            ans[clr-1] = {j, xout-1, i, yout-1};
        }
    return {ans, {n, m}};
}
auto xreflect        (paving components) {
    auto [n, m] = components.second;
    for(auto& [xmin, xmax, ymin, ymax] : components.first) {
        int t = xmin;
        xmin  = m-1 - xmax;
        xmax  = m-1 - t; 
    }
    return components;
}
auto counterclockwise(paving components) {
    auto [n, m] = components.second;
    for(auto& [xmin, xmax, ymin, ymax] : components.first) {
        int t1 = xmin, t2 = xmax;
        xmin   = ymin;
        xmax   = ymax;
        ymin   = m-1 - t2;
        ymax   = m-1 - t1;
    }
    components.second = {m, n};
    return components;
}


bool equal(paving A, paving B) {
    auto cA = A.first;
    auto cB = B.first;
    for(int i = 0; i < N; i++) {
        bool find = 0;
        for(int j = 0; !find && j < N; j++)
            find = cA[i] == cB[j];
        if( !find ) return 0;
        // вот здесь круто было бы по питоновски сделать else после того как цикл полностью завершился:
        //   for(...)
        //   else return 0;
        // а не эти всякие создания новых переменных
    }
    return 1;
}

bool similar(paving A, paving B) {
    auto [nA, mA] = A.second;
    auto [nB, mB] = B.second;
    if( nA != nB || mA != mB ) return 0;

    auto saveA = A;

    // ===================================
    if( equal(A, B) ) return 1;
    
    bool square = nA == mA;
    A = counterclockwise(A);
    if( square && equal(A, B) ) return 1;

    A = counterclockwise(A);
    if( equal(A, B) ) return 1;

    if( square ) {
        A = counterclockwise(A);
        if( equal(A, B) ) return 1;
    }
    // ===================================

    A = xreflect(saveA);
    if( equal(A, B) ) return 1;
    
    A = counterclockwise(A);
    if( square && equal(A, B) ) return 1;

    A = counterclockwise(A);
    if( equal(A, B) ) return 1;

    if( square ) {
        A = counterclockwise(A);
        if( equal(A, B) ) return 1;
    }
    // ===================================
    return 0;
}



vector<pair< string, paving >> althitory;
void convert() {
    for(auto [s, vv] : history)
        althitory.push_back( {s, make_crush(vv)} );
}


vector<string> alt;
void unique() {  // сравнивает все пары
    int k = althitory.size();
    vector<int> indx;
    for(int i = 0; i < k; i++) {
        bool find = 0;
        for(auto j: indx) {
            find |= similar( althitory[i].second, althitory[j].second );
        }
        if(!find) indx.push_back(i);
    }
    for(auto j : indx) alt.push_back(althitory[j].first);
}




void print(paving A) {
    auto [ff, ss] = A;
    printf("RECT:  n=%d x m=%d\n", ss.first, ss.second);
    for(auto [a, b, c, d] : ff) {
        printf("  x: [%d %d] , y: [%d %d]\n", a, b, c, d);
    }
}

void test_transformation(paving A) {
    print(A);

    cout << "\nTurn counterclockwise:  90\n";
    A = counterclockwise(A);
    print(A);

    cout << "\nTurn counterclockwise: 180\n";
    A = counterclockwise(A);
    print(A);

    cout << "\nTurn counterclockwise: 270\n";
    A = counterclockwise(A);
    print(A);


    A = counterclockwise(A);

    cout << "\nX - reflect\n";
    A = xreflect(A);
    print(A);

    cout << "\nTurn counterclockwise:  90 + (x-reflect)\n";
    A = counterclockwise(A);
    print(A);

    cout << "\nTurn counterclockwise: 180 + (x-reflect)\n";
    A = counterclockwise(A);
    print(A);

    cout << "\nTurn counterclockwise: 270 + (x-reflect)\n";
    A = counterclockwise(A);
    print(A);
}


int main() {
    produce(N);
    
    history.erase( "[NOT HORIZONTAL]\n");
    convert();
    unique();
    
    cout << history.size() << " / " << alt.size() << endl;
    for(auto s : alt) {
        cout << s << endl;
    }
}