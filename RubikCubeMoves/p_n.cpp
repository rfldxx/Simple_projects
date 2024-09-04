// Вращение сторон "кубика-рубика" NxNxN
// + отображение его "физической" развёртки

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "utils.h"

using namespace std;

#define FORSIDE(F) F(fs, 0) F(bs, 1) F(ls, 2) F(rs, 3) F(us, 4) F(ds, 5)

class cube {
public:
#define DOTS(a, b) , a( N*N, (decltype(a)::value_type)(b) )
    cube(unsigned N) : n(N) FORSIDE(DOTS) {} 

    // Проверка что кубик ещё не собран
    // bool operator !  ()              const { }

    // Проверка двух кубиков на схожесть
    // bool operator != (const cube& b) const { 
        // Если n - нечёт, то проверяем через центры
        // Иначе находим одинаковые углы и далее ...
    // }

    // Значение на сколько кубик собран
    // double status() const {
    //     // Надо наэфективно раставить центры
    // }


    friend ostream& operator << (ostream&, const cube&);
// private:
    const unsigned n;
    enum class CLR {
        RED, ORANGE,   GREEN, BLUE,   WHITE, YELLOW, 
    };

#define DECLARE(a, ...) vector<CLR> a;
FORSIDE(DECLARE)

// ВЫВОД РАЗВЁРТКИ НА ЭКРАН
// ------------------------
    template <typename Cont>
    stringstream lineout(const Cont& a, int i, int lvl, int add) const;
    size_t       lineout_size() const { return n*2;  /* n*map_color(0).size(); */ }
    string       map_color(CLR _) const;


// ВРАЩЕНИЕ СТОРОН
// ---------------
    // вращение "спирали" 
    template <typename Cont>
    inline void spin(Cont* pos[], int l) {
        // cursed макрос:  RCY([pos][i])  ->  0[pos][i], ...  <->  pos[0][i]
        #define RCY(a) 0 a, 1 a, 2 a, 3 a 

        // стартуем с конца участка, так сделано чтобы spin_relaxation отрабатывал эффективнее:
        //    в таком случае в spin_relaxation "иголка" перебрасывается только для случая c t == 0
        // for(size_t ii = l; ii != 0; ii--)  
        while(l--) cycle_swap( RCY([pos][l]) );
    }


    // поворот одной стороны
    template <typename Cont>
    void spin_face(Cont& ws) {
        for(int l = 0, N = n-1; N > 0; N -= 2) {
            // typename Cont::value_type*  _; <- чтоб дальше красиво использовать REPEAT_4
            typename Cont::value_type *_, *cringe[] = { REPEAT(4)( (_ = &ws[l], l += N, _), ) 0 };
            spin( cringe , N );
        }
    }
    
    // поворот примыкающих сторон
    template <typename Cont>
    void spin_relaxation(Cont* pos[], const int t[], int h) {
        for(int i = 0; i < 4; i++) pos[i] += t[i] * (n-1); 

        for(int N = n; h >= 0; N -= 2, h--) {
            cout << endl << "START AGAIN!!!" << endl;
            // дебилская t = 3
            for(int i = 0; i < 4; i++) pos[i] += (N-1) - (t[i]==3)*4*(N-1);
            spin( pos, h+1 );
            cout << "spin 1/3 with h = " << h << endl << *this << endl;

            for(int i = 0; i < 4; i++) pos[i] -= (N-1) - (t[i]==3)*4*(N-1); 
            

            spin( pos, N-1 );
            cout << "spin 1/2 with h = " << h << endl << *this << endl;
            cout << endl << "FINISH???" << endl;

            for(int i = 0; i < 4; i++) pos[i] += ( !t[i])*4*(N-1) - h;
    
            cout << endl << "FINISH?? !" << endl;

            if(h) spin( pos,  h-1 );
    
            cout << endl << "FINISH?!!" << endl;

            for(int i = 0; i < 4; i++) pos[i] += (!!t[i])*4*(N-1) + h;
            cout << endl << "FINISH!!!" << endl;
        }
    }

};



#define inizilization(side, ...) int side[3][3];
FORSIDE(inizilization)


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

// INIT_AXIS_MOVE(F, B, fs, bs,  ds, 0, ls, 1, us, 2, rs, 3)
// INIT_AXIS_MOVE(R, L, rs, ls,  bs, 3, ds, 3, fs, 3, us, 3)
// INIT_AXIS_MOVE(U, D, us, ds,  ls, 0, bs, 2, rs, 0, fs, 0)



int main() {
    cube x(4);
    x.rs[ 1] = cube::CLR::GREEN;
    x.rs[ 6] = cube::CLR::RED;
    x.rs[ 9] = cube::CLR::YELLOW;
    x.rs[13] = cube::CLR::WHITE;    
    x.rs[12] = cube::CLR::ORANGE;

    // x.us[ 5] = cube::CLR::YELLOW;
    cout << "INITIAL CUBE:\n" << x;
    x.spin_face(x.rs);
    cout << "AFTER MOVING rs:\n" << x;
    
    // x.spin_face(x.rs);
    // cout << "AFTER MOVING rs:\n" << x;

    // x.spin_face(x.rs);
    // cout << "AFTER MOVING rs:\n" << x;
    // x.spin_face(x.rs);
    // cout << "AFTER MOVING rs:\n" << x;


    #define  stp(a, b, c, d) &(a[0]), &(b[0]), &(c[0]), &(d[0])
    #define  RTA(...) wRTA(__VA_ARGS__)
    #define wRTA(name, a, ...) decltype(a) name[] = {a, __VA_ARGS__}

    RTA(pp1, stp(x.ds, x.fs, x.us, x.bs));
    RTA(pp2, 1, 1, 1, 3);
    x.spin_relaxation(pp1, pp2, 1);
    cout << "AFTER SPIN BELT rs:\n" << x;

    return 0;
}


ostream& operator << (ostream& out, const cube& _) {
#define CRINGEITERATION for(int i = 0, lvl = 0, add = 1; i < (int)_.n; ++i, (i == (int)(_.n+1)/2 ? (add = -1, lvl += !(_.n&1)) : (int)0 ), lvl += add)

    //    | us | 
    out << string(_.lineout_size()+2, ' ') << string(_.lineout_size(), '-') << "\n";
    CRINGEITERATION {
        out << string(_.lineout_size()+1, ' ') << "|" << _.lineout(_.us, i, lvl, add).str() << "|\n";
    }
    for(size_t i = 0; i < 4; i++) out << " " << string(_.lineout_size(), '-');
    out << "\n";

    // ls | fs | rs | bs
    CRINGEITERATION {
        for(auto side : {&_.ls, &_.fs, &_.rs, &_.bs} )
            out << "|" << _.lineout(*side, i, lvl, add).str();
        out << "|\n"; 
    }

    //    | ds |
    for(size_t i = 0; i < 4; i++) out << " " << string(_.lineout_size(), '-');
    out << "\n";
    CRINGEITERATION {
        out << string(_.lineout_size()+1, ' ') << "|" << _.lineout(_.ds, i, lvl, add).str() << "|\n";
    }
    out << string(_.lineout_size()+2, ' ') << string(_.lineout_size(), '-') << "\n";

    return out;
    #undef CRINGEITERATION
}

template <typename Cont>
stringstream cube::lineout(const Cont& a, int i, int lvl, int add) const {
    stringstream  out;
    for(int t = 0; t < lvl; t++) out << map_color(a[4*(t+1)*(n-t-1)-i+t]);

    for(int j = 0; j < (int)n-2*lvl; j++) 
        if( add == +1 )  out << map_color(a[4*lvl*(n-lvl)   + j]);
        else  out << map_color(a[-4*lvl*lvl+4*n*lvl+3*n-6*lvl-3 - j]);

    for(int t = lvl; t > 0; t--) out << map_color(a[-4*t*t+4*n*t-3*n+5*t-2+i]);
    return out;
}

string cube::map_color(CLR _) const {
    // decltype(t)::value_type; //::value_type;
    // ((int)CLR::RED);
    //     colors[] : [ красный,  оранжевый, зелёный,  синий,     жёлтый,      белый ]
    string colors[] = {   "101", "48;5;208",   "102",  "104", "48;5;255", "48;5;226" };
    return string() + "\033[" + (colors[(int)_]) + "m" + "  " + "\033[0m"; 
}

    