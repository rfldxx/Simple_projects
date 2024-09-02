#include <iostream>
#include <vector>
#include <string>
#include <sstream>

template<typename T, typename... Args>
void cycle_swap(T& first, Args&... args) {
    (std::swap(first, args), ...);
}

using namespace std;

#define FORSIDE(F) F(fs, 0) F(bs, 1) F(ls, 2) F(rs, 3) F(us, 4) F(ds, 5)

class cube {
public:
#define DOTS(a, b) , a(N*N, b)
    cube(unsigned N) : n(N) FORSIDE(DOTS) {} 

    // bool operator ! () const {

    // }


    ostream& operator << (ostream& out) /* const */ {
        #define CRINGEITERATION for(int i = 0, lvl = 0, add = 1; i < n; (i == (n+1)/2-1 ? (add = -1, lvl += !(n&1)) : (int)0 ), lvl += add, i++)

        //    | us | 
        out << string(lineout_size()+2, ' ') << string(lineout_size(), '-') << "\n";
        CRINGEITERATION {
            out << string(lineout_size()+1, ' ') << "|" << lineout(us, i, lvl, add).str() << "|\n";
        }

        // ls | fs | rs | bs
        for(int i = 0; i < 4; i++) out << " " << string(lineout_size(), '-');
        out << "\n";
        CRINGEITERATION {
            for(auto side : ((vector<int>* []){&ls, &fs, &rs, &bs}))
                out << "|" << lineout(*side, i, lvl, add).str();
            out << "|\n"; 
        }
        for(int i = 0; i < 4; i++) out << " " << string(lineout_size(), '-');
        out << "\n";

        //    | ds | 
        CRINGEITERATION {
            out << string(lineout_size()+1, ' ') << "|" << lineout(ds, i, lvl, add).str() << "|\n";
        }
        out << string(lineout_size()+2, ' ') << string(lineout_size(), '-') << "\n";

        return out;
    }

private:
    const unsigned n;

#define DECLARE(a, ...) vector<int> a;
FORSIDE(DECLARE)

    template <typename Cont>
    stringstream  lineout(const Cont& a, int i, int lvl, int add) const {
        stringstream  out;
        for(int t = 0; t < lvl; t++)  // printf("%4d", 4*(t+1)*(n-t-1)-i+t);
            out << map_color(a[4*(t+1)*(n-t-1)-i+t]);

        for(int j = 0; j < n-2*lvl; j++) 
            if( add == +1 )  // printf("%4d", 4*lvl*(n-lvl)   + j);
                out << map_color(a[4*lvl*(n-lvl)   + j]);
            else  // printf("%4d", -4*lvl*lvl+4*n*lvl+3*n-6*lvl-3 - j);
                out << map_color(a[-4*lvl*lvl+4*n*lvl+3*n-6*lvl-3 - j]);

        for(int t = lvl; t > 0; t--)  // printf("%4d", -4*t*t+4*n*t-3*n+5*t-2+i);
            out << map_color(a[-4*t*t+4*n*t-3*n+5*t-2+i]);
        return out;
    }

    string map_color(int clr) const {
        //     colors[] : [ красный,  оранжевый, зелёный,  синий,     жёлтый,      белый ]
        string colors[] = {   "101", "48;5;208",   "102",  "104", "48;5;255", "48;5;226" };
        return string() + "\033[" + (colors[clr]) + "m" + "  " + "\033[0m"; 
    }

    size_t lineout_size() const {
        return n*2; // n*map_color(0).size();
    }
};

// попробовать от m[3][3] перейти к vector[3] или array[3]
//  -> уберает функцию init
//  -> можно убрать chbs если воспользоваться каким нибудь find_if
//     для поиска элемента отличного от первого в check()


#define inizilization(side, ...) int side[3][3];
FORSIDE(inizilization)

void init(int m[3][3], int colour) {
    for(int i = 0; i < 3 ; i++)
        for(int j = 0; j < 3 ; j++)
            m[i][j] = colour;
}

bool chbs(int m[3][3]) {
    for(int i = 0; i < 3 ; i++) 
        for(int j = 0; j < 3 ; j++)
            if(m[i][j] != m[0][0]) return 0;
    return 1;
}

bool check() {
    #define CHBS(side, ...) && chbs(side)
    return 1 FORSIDE(CHBS);
}

int& get(int m[3][3], int t, int i, int h = 0) {
    return *( (int* []) {&m[h][i], &m[i][2-h], &m[2-h][2-i], &m[2-i][h]} [t] );
}

void spin(int a[3][3], int at,  int b[3][3], int bt,
          int c[3][3], int ct,  int d[3][3], int dt, int h = 0, int imax = 3) {
    for(int i = 0; i < imax; i++)
        cycle_swap( get(a, at, i, h), get(b, bt, i, h), 
                    get(c, ct, i, h), get(d, dt, i, h) );
}


// Face Move (side)
#define  INV(...) tINV(__VA_ARGS__)
#define tINV(a, at, b, bt, c, ct, d, dt) a,(at+2)%4, d,(dt+2)%4, c,(ct+2)%4, b,(bt+2)%4
#define SIDE(a) a, 0, a, 1, a, 2, a, 3

// __VA_ARGS__ = a, at, b, bt, c, ct, d, dt
#define INIT_AXIS_MOVE(Fname, Bname, f, b, ...)                          \
void Fname   () { spin(    SIDE(f) , 0, 2); spin(    __VA_ARGS__    ); } \
void Bname   () { spin(    SIDE(b) , 0, 2); spin(INV(__VA_ARGS__)   ); } \
void Fname##d() { spin(INV(SIDE(f)), 0, 2); spin(INV(__VA_ARGS__), 2); } \
void Bname##d() { spin(INV(SIDE(b)), 0, 2); spin(    __VA_ARGS__ , 2); }

INIT_AXIS_MOVE(F, B, fs, bs,  ds, 0, ls, 1, us, 2, rs, 3)
INIT_AXIS_MOVE(R, L, rs, ls,  bs, 3, ds, 3, fs, 3, us, 3)
INIT_AXIS_MOVE(U, D, us, ds,  ls, 0, bs, 2, rs, 0, fs, 0)


// печать развертки
void P();

int main() {
    cube x(5);
    x << cout;
    return 0;

#define filling(side, num) init(side, num);
FORSIDE(filling)
    
    fs[0][0] = 1; fs[0][1] = 2; fs[0][2] = 3;
    fs[1][0] = 4; fs[1][1] = 5; fs[1][2] = 6;
    fs[2][0] = 7; fs[2][1] = 8; fs[2][2] = 9;

    // P();
    Fd(); P();

//    spin(fs, ds, 0, ls, 1, us, 2, rs, 3);
//    spin(bs, ls, 3, ds, 2, rs, 1, us, 0);
    
//     int count = 0;
//     do {
//         spin(fs, ds, 0, ls, 1, us, 2, rs, 3);
//         spin(ls, us, 3, fs, 3, ds, 3, bs, 3);
//         spin(bs, ls, 3, ds, 2, rs, 1, us, 0);
//         spin(ls, us, 3, fs, 3, ds, 3, bs, 3);
//         spin(ls, us, 3, fs, 3, ds, 3, bs, 3);
//         spin(ls, us, 3, fs, 3, ds, 3, bs, 3);
// //        spin(rs, bs, 1, ds, 1, fs, 1, us, 1);
//         count++;
//     } while(check() == 0);
//     printf("Moves need: %d\n", count);
}



// печать развертки
// ----------------------------------------
void pbs(int m[3][3]) {
    for(int i = 0; i < 3 ; i++) {
        printf("        | ");
        for(int j = 0; j < 3 ; j++)
            printf("%d ", m[i][j]);
        printf("|\n");
    }
}

void P() {
    printf("         -------\n");
    pbs(us);
    
    printf(" ------- ------- -------\n");
    for(int i = 0; i < 3 ; i++) {
        printf("| ");
        
        for(int j = 0; j < 3 ; j++)
            printf("%d ", ls[i][j]);
        
        printf("| ");
        
        for(int j = 0; j < 3 ; j++)
            printf("%d ", fs[i][j]);
        
        printf("| ");
        
        for(int j = 0; j < 3 ; j++)
            printf("%d ", rs[i][j]);
        
        printf("|\n");
    }
    printf(" ------- ------- -------\n");
    
    pbs(ds);
    printf("         -------\n");
    pbs(bs);
    printf("         -------\n\n");
}
