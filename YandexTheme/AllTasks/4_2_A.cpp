// ЗАДАЧА: https://contest.yandex.ru/contest/53030/problems/A/

// Реализованно хеширование сразу по нескольким (x, p).
// Сделано через макросы, [ЗАЧЁРКНУТО] т.к. более красивая версия с использованием
// tuple для различных polyhash<x, p> оказалась медленее в 4 раза: [\ЗАЧЁРКНУТО] -> такая же скорость я оказывается использовал endl, вместо "\n"
// auto yuki = tuple{ polyhash<257, (int)1e9+7>{}, polyhash<357, (1<<20)>{} };
// ...
// bool ok = (get<0>(yuki) ... )

#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define alls(a) a.begin(), a.end()

template<int x, int p>
struct polyhash {
    vector<int> prfx, xpow;

    polyhash(const string& s) {
        int n = s.size();
        prfx.resize(n+1, 0);
        xpow.resize(n+1, 1);

        for(int i = 0; i < n; i++) {
            prfx[i+1] = ((ll)prfx[i]*x + (s[i]-'a'+1)) % p;
            xpow[i+1] = ((ll)xpow[i]*x) % p;
        }
    }

    // i - с нуль-нумерации
    int get(int i, int l) {
        return ( prfx[i+l] - (((ll)prfx[i]*xpow[l])%p) + 2*p ) % p;
    }
};

// ------------------------------------------------------------------------------------------------
#define  CAT(a, b) a##b
#define wCAT(...) CAT(__VA_ARGS__)

#define  OPN(...) __VA_ARGS__

#define GET8(a, b, c, d, e, f, g, h, ...) h
#define CNT_ARG(...) GET8(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1)

#define  REPEAT7(F, data, a, ...) F(7, data, a) wREPEAT6(F, data, __VA_ARGS__)
#define  REPEAT6(F, data, a, ...) F(6, data, a) wREPEAT5(F, data, __VA_ARGS__)
#define  REPEAT5(F, data, a, ...) F(5, data, a) wREPEAT4(F, data, __VA_ARGS__)
#define  REPEAT4(F, data, a, ...) F(4, data, a) wREPEAT3(F, data, __VA_ARGS__)
#define  REPEAT3(F, data, a, ...) F(3, data, a) wREPEAT2(F, data, __VA_ARGS__)
#define  REPEAT2(F, data, a, ...) F(2, data, a) wREPEAT1(F, data, __VA_ARGS__)
#define  REPEAT1(F, data, a, ...) F(1, data, a)
#define wREPEAT7(...) REPEAT7(__VA_ARGS__)
#define wREPEAT6(...) REPEAT6(__VA_ARGS__)
#define wREPEAT5(...) REPEAT5(__VA_ARGS__)
#define wREPEAT4(...) REPEAT4(__VA_ARGS__)
#define wREPEAT3(...) REPEAT3(__VA_ARGS__)
#define wREPEAT2(...) REPEAT2(__VA_ARGS__)
#define wREPEAT1(...) REPEAT1(__VA_ARGS__)


#define INTanize(a, b) (int) (a), (int) (b)
#define INIT_YUKI_HASH(indx, data, x_p) polyhash<INTanize x_p> yuki##indx(data);
#define POLYinit(data) wCAT(wREPEAT, CNT_ARG(POLYhash))(INIT_YUKI_HASH, data, POLYhash);

#define  YUKI_CHECKER2(yuki, a, b, l)  & (yuki.get(a, l) == yuki.get(b, l))
#define wYUKI_CHECKER2(...) YUKI_CHECKER2(__VA_ARGS__)
#define YUKI_CHECKER1(indx, abl, ...)  wYUKI_CHECKER2(yuki##indx, OPN abl)
#define POLYcheck(a, b, l) ( 1 wCAT(REPEAT, CNT_ARG(POLYhash))( YUKI_CHECKER1, (a,b,l), ,,,,,,,) )
// ------------------------------------------------------------------------------------------------



int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    string s; cin >> s;
    int n = s.size();

    #define POLYhash (257, 1e9+7), (257, 1<<20), (357, 1<<20), (457, 1<<20), (557, 1<<20)
    POLYinit(s)
      
    int t, l, a, b; cin >> t;
    while( t-- ) {
        cin >> l >> a >> b;
    
        bool ok = POLYcheck(a, b, l);
        cout << ( ok ? "yes" : "no" ) << "\n";
    }
}
