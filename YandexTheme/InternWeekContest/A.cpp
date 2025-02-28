// For GET() macro's and include's
#include "my_macro.h"

int main() {
    GET(N, prev)
    N--;
    int ans = 0;

    while( (ans != -1) && N-- ) {
        GET(a)

        if( a < prev ) ans = -1;
        else ans += a - prev;

        prev = a;
    }

    cout << ans << endl;
}
