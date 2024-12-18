### N'th prefix sum
> [!NOTE]
> Кликбейт про in one pass $\text{}$ – $\text{}$ за один проход вычисляется только сумма всех элементов $n$-го префикса (или что тоже самое последний элемент ($n$+1)-го префикса) \
> Конечно можно таким способом найти все $N$ элементов заданого префикса, но это займет: $\frac{N(N+1)}{2}$
```
       data     |  a  |       b  |             c  |                   d
----------------|-----|----------|----------------|----------------------
  I prefix sum  |  a  |   a + b  |    a +  b + c  |    a +   b +  c + d
 II prefix sum  |  a  |  2a + b  |   3a + 2b + c  |   4a +  3b + 2c + d
III prefix sum  |  a  |  3a + b  |   6a + 3b + c  |  10a +  6b + 3c + d
 IV prefix sum  |  a  |  4a + b  |  10a + 4b + c  |  20a + 10b + 4c + d
```

---

$\text{ }$

По сути достаточно расмотреть следующую таблицу ```table``` (если нас интересует столбец ```j```, то для "буквы" из позиции ```k``` надо взять коэфициент расположений в столбце ```table[.][j-k]```  )
```
table:
     |  1  |  0  |   0  |   0  |   0
-----|-----|-----|------|------|------
  I  |  1  |  1  |   1  |   1  |   1
 II  |  1  |  2  |   3  |   4  |   5
III  |  1  |  3  |   6  |  10  |  15
 IV  |  1  |  4  |  10  |  20  |  35
  V  |  1  |  5  |  15  |  35  |  55
```

Понятно, что в этой таблице всякие $C_k^n$ (в силу ```table[i][j] = table[i][j-1]+table[i-1][j]```). На этом и покончено

---

$\text{ }$

Однако можно взягнуть по другому:
* в ```  I``` у нас на каждой позиции ```  I[i]``` $=$ $1$ $\hspace{50pt}$ $\hspace{17pt}$ $\Rightarrow$  $\hspace{5pt}$ значение ``` II[n]``` $=$ ```I[0] + ... + I[n]``` $=$ $\sum\limits_{i=1}^n i^0$ $=$ $n$
* в ``` II``` у нас на каждой позиции ``` II[i]``` $=$ $i$ $\hspace{50pt}$ $\hspace{18pt}$ $\Rightarrow$  $\hspace{5pt}$ значение ```III[n]```  $=$ $\hspace{8pt}$ $\sum\limits_{i=1}^n i^1$ $=$ $\frac{1}{2}n^2 + \frac{1}{2}n$
* в ```III``` у нас на каждой позиции ```III[i]``` $=$ $\frac{1}{2}i^2 + \frac{1}{2}i$  $\hspace{5pt}$ $\Rightarrow$  $\hspace{5pt}$ значение ``` IV[n]```  $=$ $\frac{1}{2}\sum\limits_{i=1}^n i^2 + \frac{1}{2}\sum\limits_{i=1}^n i^1$ $=$ ... 
#### $\hspace{50pt}$ $\Rightarrow$  $\hspace{5pt}$ в $n$ строчке многочлен степени $n-1$ (надо было нумеровать с нуля ...)
```
table:
     |  1  |  0  |   0  |   0  |   0    
-----|-----|-----|------|------|------  ( j - номер столбца )
  I  |  1  |  1  |   1  |   1  |   1    =     1
 II  |  1  |  2  |   3  |   4  |   5    =     j
III  |  1  |  3  |   6  |  10  |  15    = 1/2 j^2 + 1/2j
 IV  |  1  |  4  |  10  |  20  |  35    = 1/6 j^3 + 1/2j^2 +  1/3 j
  V  |  1  |  5  |  15  |  35  |  55    = 1/24j^4 + 1/4j^3 + 11/24j^2 + 1/4j
```

Если воспользоваться нахождением через биноминальные коэфициенты, то получаем, что многочлен на $n$ строчке равен: $C_{0 + (j-1)}^{n-1 + (j-1)} = \prod\limits_{t=0}^{n-2} \frac{t+j}{t+1} $ 

---

$\text{ }$

Если посмотреть на ```using_polinom.py``` и ```using_binom.py``` то понятно, что многочлены тут совершенно бесполезны (по эффективности / коду). Если только мы, не знаю, не интерполируем что-ли, беря что-то типа нецелых элементов массива . . . 
