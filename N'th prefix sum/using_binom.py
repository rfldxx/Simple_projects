def sum_of_nth_iteration(data, N):
    nc = N
    kc = 0
    C  = 1

    ans = 0
    for i in range(len(data)):
        ans += C * data[len(data)-1 - i]

        nc += 1
        kc += 1
        C   = (nc*C) // kc

    return ans

data = [9, 11, 4, 6, 4, 2]
ans = sum_of_nth_iteration(data, 7)
print('\nFOR data: ', data, '\nANS:      ', ans)
