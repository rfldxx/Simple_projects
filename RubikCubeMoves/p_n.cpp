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
#define DOTS(a, b) , a( N*N, decltype(a::value) (b) )
    cube(unsigned N) : n(N) FORSIDE(DOTS) {} 

    // bool operator !  ()              const { }
    // bool operator != (const cube& b) const { }


    friend ostream& operator << (ostream&, const cube&);
private:
    const unsigned n;
    enum class CLR {
           RED,   ORANGE,
         GREEN,     BLUE,
        YELLOW,    WHITE
    };

#define DECLARE(a, ...) vector<CLR> a;
FORSIDE(DECLARE)

    template <typename Cont>
    stringstream lineout(const Cont& a, int i, int lvl, int add) const;

    string map_color(CLR _) const {
        // ((int)CLR::RED);
        CLR(1);
        //     colors[] : [ красный,  оранжевый, зелёный,  синий,     жёлтый,      белый ]
        string colors[] = {   "101", "48;5;208",   "102",  "104", "48;5;255", "48;5;226" };
        return string() + "\033[" + (colors[(int)_]) + "m" + "  " + "\033[0m"; 
    }

    size_t lineout_size() const { return n*2;  /* n*map_color(0).size(); */  }

    template <typename Cont>
    void spin(const Cont* pos[], int l, bool one_side = 0) {
        for(int i = 0; i < l - one_side; i++)
            cycle_swap( pos[0][i], pos[1][i], pos[2][i], pos[3][i] );
    }


};



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
    cout << x;
    return 0;

#define filling(side, num) init(side, num);
FORSIDE(filling)
    
    fs[0][0] = 1; fs[0][1] = 2; fs[0][2] = 3;
    fs[1][0] = 4; fs[1][1] = 5; fs[1][2] = 6;
    fs[2][0] = 7; fs[2][1] = 8; fs[2][2] = 9;

    // P();
    Fd(); P();

}


ostream& operator << (ostream& out, const cube& _) /* const */ {
    #define CRINGEITERATION for(int i = 0, lvl = 0, add = 1; i < _.n; (i == (_.n+1)/2-1 ? (add = -1, lvl += !(_.n&1)) : (int)0 ), lvl += add, i++)

    //    | us | 
    out << string(_.lineout_size()+2, ' ') << string(_.lineout_size(), '-') << "\n";
    CRINGEITERATION {
        out << string(_.lineout_size()+1, ' ') << "|" << _.lineout(_.us, i, lvl, add).str() << "|\n";
    }
    for(int i = 0; i < 4; i++) out << " " << string(_.lineout_size(), '-');
    out << "\n";

    // ls | fs | rs | bs
    CRINGEITERATION {
        for(auto side : ((const decltype(_.ls)* []){&_.ls, &_.fs, &_.rs, &_.bs}))
            out << "|" << _.lineout(*side, i, lvl, add).str();
        out << "|\n"; 
    }

    //    | ds |
    for(int i = 0; i < 4; i++) out << " " << string(_.lineout_size(), '-');
    out << "\n";
    CRINGEITERATION {
        out << string(_.lineout_size()+1, ' ') << "|" << _.lineout(_.ds, i, lvl, add).str() << "|\n";
    }
    out << string(_.lineout_size()+2, ' ') << string(_.lineout_size(), '-') << "\n";

    return out;
    #undef CRINGEITERATION
}

template <typename Cont>
stringstream  cube::lineout(const Cont& a, int i, int lvl, int add) const {
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
