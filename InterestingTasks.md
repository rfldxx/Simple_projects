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
\+ можно не просто прямой и "обратной" map-ой решить, \
а можно моделировать время последнего взаимодействия через двусвязный список (см. solution)

 
---

$\text{}$
$\text{}$

Иногда полезно рассматривать массив по "Лебегу".

Допустим у нас есть массив `arr[i]` - давайте для каждого значения сохраним индексы, при которых достигается это значение.
```c++
map<ll, vector<int>> heights;
heights[0] = {-1};  // <- когда как, возможно и не нужно
for(ll prev = 0, i = 0; auto e : arr)
    heights[ prev += e ].push_back(i++);
```
Примеры использования: \
[1124. Longest Well-Performing Interval](https://leetcode.com/problems/longest-well-performing-interval/description/) : [submission](https://leetcode.com/problems/longest-well-performing-interval/submissions/1570442269/) \
[862. Shortest Subarray with Sum at Least K](https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/description/) : [submission](https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/submissions/1569002067/) \
(не особо, но чем-то схоже: [1590. Make Sum Divisible by P](https://leetcode.com/problems/make-sum-divisible-by-p/description/) : [submission](https://leetcode.com/problems/make-sum-divisible-by-p/submissions/1568969029/))

---

$\text{}$
$\text{}$


Топ сайт, для поиска латехных символов: https://detexify.kirelabs.org/classify.html
