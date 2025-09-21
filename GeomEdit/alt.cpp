// ЧЕРЕЗ СЕТКУ HxH
// (халтурка)

#include <bits/stdc++.h>
using namespace std;

#define N 4


set<string> history;

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

bool MIKU = 0;

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
    if(MIKU) cout << "!!! TERMINATE !!!" << endl;
    return 0;
    breakcore:

    // граница этого непокрашенного сегмента
    int xout = x;
    while( xout < N && !paint[y][xout] ) xout++;

    if( k == 1 ) {  // ладно, напишем шорткат
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
        history.insert( miku(save_paint()) );

        if(MIKU) {
            cout << "!!! FINAL: " << (ans ? "OK" : "SAD") <<  " !!!" << endl;
            cout << miku(deobfuscate( save_paint() ));
            cout << endl << endl;
        }

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
        
            if(MIKU) {
                cout << "COLOR: " << ((char)('A' + (k-1))) << endl;
                cout << miku(deobfuscate( save_paint() ));
                cout << " >>> ";
            }
        
            collect |= produce(k-1);
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
    if( n > m ) return "[NOT HORIZONTAL]";

    string result;
    for(int i = 0; i < n; i++) {
        string lain(m, '#');
        for(int j = 0; j < m; j++)
            lain[j] = Matr[i][j] ? 'A' + (Matr[i][j]-1) : '.';
        result += lain + '\n';
    }    
    return result;
}

// set<string> unique() {
//     set<string> result;
//     for(auto e : history)
//         result.insert( miku(deobfuscate(e)) );
//     return result;
// }


int main() {
    produce(N);
    cout << history.size() <<  endl;
    // for(auto s : history) {
    //     cout << s << endl;
    // }
}