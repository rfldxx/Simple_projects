#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n = 8;
    int lvl = 0, add = 1;
    for(int i = 0; i < n; lvl += add, i++) {
        int t = 0;
        printf("%5d:  ", lvl);

        for( ; t < lvl; t++) printf("%5d", 4*(t+1)*(n-t-1)-i+t);

        for(int j = 0; j < n-2*t; j++) 
            if( add == +1 ) printf("%5d", 4*t*(n-t)   + j);
            else printf("%5d", -4*t*t+4*n*t+3*n-6*t-3 - j);

        for( ; t > 0; t--) printf("%5d", -4*t*t+4*n*t-3*n+5*t-2+i);

        printf("\n");
        if( i == (n+1)/2 - 1 ) {
            add = -1;
            lvl += !(n&1);
        }
    }
}