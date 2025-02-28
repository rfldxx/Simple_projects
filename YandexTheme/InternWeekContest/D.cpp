// For GET() macro's and include's
#include "my_macro.h"

#define N 1'000'000

llu resheto[N+1] = {0};

llu   prime[N+1] = {2};
int   count      =  1 ;

void init() {
    for(int i = 3; i*i < N+1; i += 2) {
        if( resheto[i] ) continue;
        for(llu j = i*i; j < N+1; j += i) {
            resheto[j] = 1;
        }
    }

    for(int i = 3; i < N+1; i += 2)
        if( !resheto[i] ) prime[count++] = i;
}

llu Evklid(llu a, llu b) {
    if( a < b ) Evklid(b, a);

    while( b ) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// first  - из a удалили все простые множители, не превышающие N
// second - максимальный простой делитель (не превышающий N) числа a
pair<llu, llu> extract(llu a) {
    llu r = 0;
    for(int i = 0; i < count; i++) {
        while( a%prime[i] == 0 ) {
            a /= prime[i];
            r  = prime[i]; 
        }
    }
    return {a, r};
}

int main() {
    init();
    
    GET(T)
    while ( T-- ) {
        llu a[2];
        cin >> a[0] >> a[1];

        llu nod = Evklid(a[0], a[1]);

        llu p[2];
        for(int t = 0; t < 2; t++) p[t] = a[t] / nod;

        auto [p1, p2] = extract(p[0]);
        auto [p3, p4] = extract(p[1]);

        cout << nod * max( max(p1, p2), max(p3, p4) ) << endl;
    }
}
