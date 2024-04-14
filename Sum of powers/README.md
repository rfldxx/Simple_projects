Show formula for:

$\hspace{30pt}$ $\Large{\sum\limits_{i=1}^n i^k \hspace{5pt}=\hspace{5pt} \text{..}\cdot n^{k+1} \text{ }+\text{ } \text{..}\cdot n^k \text{ }+\text{ ...}}$

$\text{ }$


```
# ===== [ EXAMPLE ] ====================================================================
show_powers(10)
# PRINTED:
#   SUM_[1,n] i^1 = 1/2*n^2 + 1/2*n
#   SUM_[1,n] i^2 = 1/3*n^3 + 1/2*n^2 + 1/6*n
#   SUM_[1,n] i^3 = 1/4*n^4 + 1/2*n^3 + 1/4*n^2 + 0*n
#   SUM_[1,n] i^4 = 1/5*n^5 + 1/2*n^4 + 1/3*n^3 + 0*n^2 + -1/30*n
#   SUM_[1,n] i^5 = 1/6*n^6 + 1/2*n^5 + 5/12*n^4 + 0*n^3 + -1/12*n^2 + 0*n
#   SUM_[1,n] i^6 = 1/7*n^7 + 1/2*n^6 + 1/2*n^5 + 0*n^4 + -1/6*n^3 + 0*n^2 + 1/42*n
#   SUM_[1,n] i^7 = 1/8*n^8 + 1/2*n^7 + 7/12*n^6 + 0*n^5 + -7/24*n^4 + 0*n^3 + 1/12*n^2 + 0*n
#   SUM_[1,n] i^8 = 1/9*n^9 + 1/2*n^8 + 2/3*n^7 + 0*n^6 + -7/15*n^5 + 0*n^4 + 2/9*n^3 + 0*n^2 + -1/30*n
#   SUM_[1,n] i^9 = 1/10*n^10 + 1/2*n^9 + 3/4*n^8 + 0*n^7 + -7/10*n^6 + 0*n^5 + 1/2*n^4 + 0*n^3 + -3/20*n^2 + 0*n
```



----
Thank's to:
* [Faulhaber's formula](https://en.m.wikipedia.org/wiki/Faulhaber%27s_formula)
* [Interesting Note](https://dev.mccme.ru/~merzon/pscache/bernoulli-howto-pre.pdf)
