[**LeetCode 1358**](https://leetcode.com/problems/number-of-substrings-containing-all-three-characters/description/). \
Given a string `s` consisting only of characters $a$, $b$ and $c$. \
Return the number of substrings containing **at least** one occurrence of all these characters $a$, $b$ and $c$.

**Решение:** (не эффективно, зато красиво)  (нашёл [здесь](https://leetcode.com/problems/number-of-substrings-containing-all-three-characters/editorial/comments/2891380/))\
Пусть $A$ - подстроки, в которых содержится символ `'a'`. \
Аналогично множества $B$, $C$ - для символов `'b'`, `'c'`, соответственно.

Мы можем находить кол-во подстрок, состоящих **только** из определенных элементов `valid_elem` :
```c++
int f(set<char> valid_elem) {   // count of substrings with restriction
	int ans = 0;
	for(int l = 0; auto e : s)
		if( !valid_elem.count(e) ) l = 0;
		else ans += ++l;

	return ans;
}
```
Например:
1) подсроки состоящие только из элемента `"a"` - это на самом деле будет множество: $\text{}$ $(B \cup C)^\complement$ \
где: $X^\complement$ - дополнение множества $X$

2) подсроки состоящие только из элементов `"a"` или `"b"` - это будет: $\text{}$ $C^\complement$

$\Rightarrow$ $|A \cap B \cap C|$ $=$ $|\varnothing^\complement| - |A^\complement| - |B^\complement| - |C^\complement| + |(A \cup B)^\complement| + |(B \cup C)^\complement| + |(C \cup A)^\complement|$

Т.е.: ``` retrun f("abc") - f("bc") - f("ca") - f("ab") + f("c") + f("a") + f("b") ```

---

$\text{}$
$\text{}$

[**G. Студенческие советы**](https://codeforces.com/edu/course/2/lesson/6/2/practice/contest/283932/problem/G) \
Дано число $k$ - размер студенческого совета, и массив `a[1..n]` — количество студентов в $i$-й группе. \
Каждый студент состоит в какой-то (одной) группе, и студенты из одной группы не могут оказаться в одном совете. \
Также, каждый студент входит максимум в один совет.

Какое максимальное число советов может быть образовано?

**Решение:** бин-поиск по ответу \
Однако, как проверить возможность создания `cnt` советов?

Можно убедится, что следующий код позволяет это сделать:
```c++
bool can(ll cnt) {
    ll acc = 0;
    for(auto e : a)
        acc += min(cnt, (ll)e);
    return acc >= k*cnt;
}
```

---

$\text{}$
$\text{}$


[**146. LRU Cache**](https://leetcode.com/problems/lru-cache/description/) \
todo: \
\+ можно не просто прямой и "обратной" map-ой решить ([submissions](https://leetcode.com/problems/lru-cache/submissions/1576668167/)), \
а можно моделировать время последнего взаимодействия через двусвязный список (см. solution)

 
---

$\text{}$
$\text{}$


[**I. Отрезок с нужным подмножеством**](https://codeforces.com/edu/course/2/lesson/9/3/practice/contest/307094/problem/I) \
Задано число $s$ ($s \le 1000$). Дан массив из $n$ чисел $a_i$ ($n \le 10^5$ , $1 \le a_i \le s$). \
Скажем, что отрезок этого массива $a[l..r]$ хороший, если на этом отрезке можно выбрать некоторый набор чисел, сумма которых равна $s$. \
Ваша задача — найти самый короткий хороший отрезок.

**Решение:** Рассмотрим отрезок $a[l..r]$, давайте узнаем какие суммы можно получить, выбрав некоторые числа в нём. \
Для этого можно воспользоваться DP: пусть $dp[i]$ - кол-во способов выбрать набор чисел (из рассматриваемого отрезка), чтобы сумма чисел набора равнялась $i$.

При добавлении нового элемента $x = a[r+1]$, мы можем в каждый "прошлый" набор чисел (из отрезка $a[l..r]$) как включить, так и невключить $x$. Итого обновляем состояния $dp$:
```cpp
for(int i = s-x; i >= 0; i--)  // добавили x
	dp[i+x] += dp[i];

for(int i = 0; i <= s-x; i++)  // "отемнили" добавление x
	dp[i+x] -= dp[i]; 
```
Для полноты покажем, что не важно в каком порядке "создавать" $dp$ - можно добавлять элементы в любом порядке (а не только в порядке $a[l], a[l+1], ..., a[r], a[r+1]$) 

<img src="InterestingTasks_photo_I_task.png" alt="Покажем, что порядок при построении dp не важен " style="width:550px;"/>

**Итоговый код:**
```cpp
long long unsigned dp[1001] = {1};
for(int l = 0, r = 0; r < n; r++) {
    for(int x = a[r], i = s-x; i >= 0; i--)
	dp[i+x] += dp[i];

    while( dp[s] ) {
	best = min(best, r-l+1);
	for(int y = a[l++], i = 0; i <= s-y; i++)  // <- "удаление"
	    dp[i+y] -= dp[i];                
    }
}
```
Однако это **лажа** и удивительно, что такое решение заходит. Предположим, что в массиве $a$ одни $1$ - тогда состояния $dp$ что-то вроде биномиальных коэффициентов, которые могут принимать значения вплоть до $\text{ } 1000! / 500!^2 \text{ }$ - что явно не влезет в long long unsigned.

**Другие DP:**
 - [comment](https://codeforces.com/edu/course/2/lesson/9/3/practice?#comment-754998): Hey, my approach for Segment with the Required Subset was: I used the two stacks trick that was in the tutorials. Now we only have to recalculate the subset sum when we add an element. We can store the intermediate DP-table of a subset sum as a bitset of size $1001$, where $b[i]$ means that you can reach sum $i$. Then if you add an element to the set, the bitset gets updated like: `b = (b | b<<val)` . Last thing left is to check if current segment is good. We have to merge the top bitset of the first and second stack. This can be done with the bitset and-operation. Only then you have to store one of the bitsets backwards, so the elements line up. The runtime will be $O(n \cdot s / \text{wordsize})$. \
Никак не могу понять?! Как мы "удаляем" самый левый элемент, без полного пересчета отрезка $[l, r]$?

 - [comment](https://codeforces.com/edu/course/2/lesson/9/3/practice?#comment-764700): `dp[j]` is the maximum index of the beginning subsequence whose sum is equal to `j`.
```python
dp = [-1 for i in range(s)]
for i in range(0, n, 1):
    # Update dp array
    for j in range(s, a[i], -1):
        dp[j] = max(dp[j], dp[j-a[i]])
    dp[a[i]] = i
  
    if dp[s] != -1:
	ans = min(ans, i - dp[s] + 1)
```

---

$\text{}$
$\text{}$

[**784. Эволюция**](https://acmp.ru/asp/do/index.asp?main=task&id_course=2&id_section=21&id_topic=48&id_problem=1009) \
(вообще уже скатился, всё подряд сую) \
Прикол, что для вершины $N$ предком является вершина $(N \text{ >> } 1)$.
```cpp
    llu a, b; cin >> a >> b;
    while( a != b )
        *(llu*[]){&a, &b}[b > a] >>= 1;
    cout << a;
```


---

$\text{}$
$\text{}$

Иногда полезно рассматривать массив по "Лебегу".

Допустим у нас есть массив `arr[i]` - давайте для каждого значения сохраним индексы, при которых достигается это значение.
```c++
map<ll, vector<int>> heights;
// heights[0] = {-1};  // <- когда как, возможно и не нужно
for(ll prev = 0, i = 0; auto e : arr)
    heights[ prev += e ].push_back(i++);
```
**Примеры использования:** \
[1124. Longest Well-Performing Interval](https://leetcode.com/problems/longest-well-performing-interval/description/) : [submission](https://leetcode.com/problems/longest-well-performing-interval/submissions/1570442269/) \
[862. Shortest Subarray with Sum at Least K](https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/description/) : [submission](https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/submissions/1569002067/) \
(не особо, но чем-то схоже: [1590. Make Sum Divisible by P](https://leetcode.com/problems/make-sum-divisible-by-p/description/) : [submission](https://leetcode.com/problems/make-sum-divisible-by-p/submissions/1568969029/))

**Задача.** Дан массив $a_i$. Для каждого $i$ найти кол-во $j < i$, таких что $a_j < a_i$. \
Хочется сразу сделать онлайн решение: будем накапливать значения в `multiset before`, тогда, при получении следующего $a_i$ нужно узнать его (упорядоченную) позицию в multiset-е - это и будет кол-во таких $j$. Оказывается в g++ есть функция order_of_key() - [см.](https://codeforces.com/blog/entry/11080) . \
Можно, также моделировать (упорядоченную) позицию в `before` и через "массив": для каждого j, мы прибавляем по $+1$ на всем интервале $[a_j, MAX a]$. Делать эффективно такие запросы можно например через дерево отрезков.

Однако, хочется решить такую задачу более прозрачнее. Если достаточно оффлайн решения, то можно поступить так: \
$\text{1}$. Отсортируем все значения по уровням \
$\text{2}$. На каждом шаге выгружаем текущий уровень в облако уже рассмотренных точек (в коде `indx`). При этом _находя_ их позиции в этом облаке. 
А как мы эффективно можем находить позиции без техник выше? Видимо как всегда, рассмотрим метод разделяй и влавствуй. . . В итоге получилось подобие merge-sort ( надеюсь тоже с асимптотикой $O(N \text{ ln} N)$ ):

<details>
	
<summary>кринж код</summary>
	
```cpp
vector<int> ans(n, 0);  // ans[i] = размер множества {j : j < i && a[j] < a[i]}

map<int, int> was; // <значение элемента массива, "уровень">
for(auto e : a) was[e];
for(int prev = 0; auto& [k, v] : was) v = prev++;

vector<vector<int>> level(was.size());
for(int i = 0; auto e : a) level[was[e]].push_back(i++);

vector<int> merge(int l, int r) {  // merge-м векторы из отрезка [l, r]
    if( l == r ) return level[l];
    int m = (l+r)/2;

    vector<int> a = merge(l, m), b = merge(m+1, r), result(a.size() + b.size());
    int acc_below = 0;  // <- самая важная переменная!
    for(int i = 0, j = 0, l = 0; l < result.size(); )
        if( j == b.size() || (i < a.size() && a[i] < b[j]) ) {  // выбераем из массива a
            acc_below++;  // - это тоже самое что и переменная i
            result[l++] = a[i++];
        } else {  // выбераем из массива b
            ans[b[j]]  += acc_below;
            result[l++] = b[j++];
        }
    return result;
}

merge(0, was.size()-1);
```

</details>

Ёмае, можно было по адеватному, по длинне длине разделять - [Подсчёт инверсий](https://education.yandex.ru/handbook/algorithms/article/podschet-inversij) (чёт всё подгоняю под _Лебега_).

---

$\text{}$
$\text{}$


Топ сайт, для поиска латехных символов: https://detexify.kirelabs.org/classify.html
