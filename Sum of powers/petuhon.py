from fractions import Fraction


def show_powers(n):
    pascal = [0]*(n+1)
    pascal[0] = 1
    pascal[1] = 1

    bernuli = [Fraction(1)]*n

    for poww in range(2, n+1):
        temp = pascal[0]

        for j in range(1, poww+1):
            tt = temp + pascal[j]
            temp = pascal[j]
            pascal[j] = tt
        # now have pascal : (a+b)^poww

        print("SUM_[1,n] i^", poww-1, ' = ', sep='', end='')
        sum = Fraction(0)
        for i in range(poww-1):
            val = pascal[i]*bernuli[i]
            sum += val

            sign = 1 - 2 * int(i == 1)
            print(Fraction(sign*val, poww), '*n^', poww - i, ' + ', end='', sep='')

        bernuli[poww-1] = -sum / poww

        sign = 1 - 2 * int(poww-1 == 1)
        print(Fraction(sign * pascal[poww-1] * bernuli[poww-1], poww), '*n', sep='')


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
