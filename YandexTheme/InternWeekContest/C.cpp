// данное решение НЕ СДАВАЛОСЬ
// результат для n = 5000 совпадает с результатом из примера (так что скорее всего пройдет тесты...)

// For GET() macro's and include's
#include "my_macro.h"


int main() {
    GET(n)

    const int p = 1'000'000'007;

    // thx: http://e-maxx.ru/algo/reverse_element
    vector<int> inv_elem(n+1, 0); inv_elem[1] = 1;
    
    vector<int> dp(n+1); dp[1] = 1;
    int pow2 = 1;  // = 2^{n-1}
    int peko = 1;  // = 2^{n*(n-1)/2} - кол-во различных графов с n вершинами

    for( int i = 2; i <= n; i++ ) {
        inv_elem[i] = (p - (p/i) * (llu)inv_elem[p%i] % p) % p;

        llu acc = 0;
        for(int j = i; j > 0; j--) {
            dp[j] = ( ((i * (llu)inv_elem[j])%p) * dp[j-1] ) % p;
            acc += dp[j];
            acc %= p;  // <- если n*p вмещается в llu, то можно выполнять эту операцию после этого for-а
        }

        pow2  = ((llu)pow2*2) % p;
        peko  = ((llu)pow2*peko) % p;
        dp[0] = ( (peko - acc) + p ) % p;
    }
    
    cout << dp[1] << endl;
}