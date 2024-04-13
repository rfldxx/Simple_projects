from fractions import Fraction

def save_powers(table, n):
    pascal = [0]*(n+1)
    pascal[0] = 1
    pascal[1] = 1

    bernuli = [Fraction(1)]*n

    for poww in range(2, n+1):
        jj = 0
        temp = pascal[0]

        for j in range(1, poww+1):
            tt = temp + pascal[j]
            temp = pascal[j]
            pascal[j] = tt

        sum = Fraction(0)
        for i in range(poww-1):
            val = pascal[i]*bernuli[i]
            sum += val

            sign = 1 - 2 * int(i == 1)
            table[poww-1][jj] = Fraction(sign*val, poww)
            jj += 1

        bernuli[poww-1] = -sum / poww

        sign = 1 - 2 * int(poww-1 == 1)
        table[poww-1][jj] = Fraction(sign * pascal[poww-1] * bernuli[poww-1], poww)




N = 10
table = [[Fraction(0)]*N for _ in range(N)]
table[0][0] = Fraction(1)

save_powers(table, N)


ray = [Fraction(0)]*N
ray[0] = Fraction(1)
for curr in range(1, N):
    next = [Fraction(0)]*N
    for j in range(curr):
        for k in range(curr-j+1):
            next[j+k] += ray[j] * table[curr-j][k]

    for j in range(curr+1):
        ray[j] = next[j]

    print(curr+1, "'th iteration:   ", sep ='', end = '')
    for t in range(curr):
        print(ray[t], "*n^", curr+1-t, end = ' + ', sep ='')
    print(ray[curr], "*n", sep='')
    print()
