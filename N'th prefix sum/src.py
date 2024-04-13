from fractions import Fraction




def show_powers(table, n):
    ii = 0
    jj = 0

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
        # print(poww, ": ", pascal)

        # print("SUM_[1,n] i^", poww, ' = ', sep='', end='')
        sum = Fraction(0)

        print('{', end='')
        for i in range(poww-1):
            val = pascal[i]*bernuli[i]
            sum += val

            sign = 1 - 2 * int(i == 1)
            # print(Fraction(sign*val, poww), '*n^', poww - i, ' + ', end='', sep='')
            table[ii+1][jj] = Fraction(sign*val, poww)
            jj += 1
            print(Fraction(sign*val, poww), ". , ", end='', sep='')

        bernuli[poww-1] = -sum / poww

        sign = 1 - 2 * int(poww-1 == 1)
        print(Fraction(sign * pascal[poww-1] * bernuli[poww-1], poww), '.},', sep='')
        table[ii+1][jj] = Fraction(sign * pascal[poww-1] * bernuli[poww-1], poww)

        ii += 1
        jj  = 0



# ===== [ EXAMPLE ] ====================================================================

N = 10
table = [[Fraction(0)]*N for _ in range(N)]
table[0][0] = Fraction(1)

show_powers(table, N)
for i in range(len(table)):
    print(table[i])


ray = [Fraction(0)]*N
ray[0] = Fraction(1)
curr = 1
for i in range(1, N):
    next = [Fraction(0)]*N
    for j in range(curr):
        for k in range(curr-j+1):
            next[j+k] += ray[j] * table[curr-j][k]

    curr += 1
    sum = Fraction(0)
    for j in range(curr):
        ray[j] = next[j]
        sum   += next[j]

    print(sum, "|", ray)
