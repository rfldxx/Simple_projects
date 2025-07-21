## [Задачи с "олимпиадного" стенда на youngcon](https://yandex.ru/youngcon/olympics)

[3.](https://new.contest.yandex.ru/contests/78953/problem) Вас наняли организовать новую выставку алмазов. Сейчас на складе хранится $n \le 10^5$ алмазов, $i$-й из которых имеет размер $a_i$ и стоимость $b_i$ ($a_i , b_i \le 10^9$). \
На выставке будет два стеллажа с алмазами, каждый из которых может вместить любое количество алмазов, но для красоты необходимо, чтобы максимальный и минимальный размеры алмазов на стеллаже отличались не более чем на $k \le 10^9$. \
Определите максимальную суммарную стоимость алмазов, которые можно одновременно взять на выставку.

<details>

<summary> Решение </summary>

Тогда сделал пушкой по воробьям - через дерево отрезков. \
Сейчас кажется что можно было и без него, через структуру в которую можно добавлять/удалять последовательно возрастающие элементы, и извлекать отрезок с максимальной суммой ($\sum b_i$) и необходимыми ограничениями $\max\limits_{a_i} - \min\limits_{a_i} \le k$.

ОФИГЕТЬ, что GPT выдал:
* Для каждого возможного интервала (подмассива) с разницей между минимальным и максимальным размером не более $k$, найти сумму стоимости.
* После этого — выбрать два интервала, **не пересекающихся**, чтобы максимизировать сумму их стоимости. Можно за $O(n)$.

Вообще гений - очень понятное решение. (по сути схоже с предложенным мною _упрощенным_ решением).

<details>

<summary> Через ДО </summary>

```cpp
#include <bits/stdc++.h>
using namespace std;

#define int long long
struct diamond {
    int a, b, i;
};

int n, k;


// кол-во элументов на расстоянии <= k
vector<int> right_count(const vector<int>& b) {
    vector<int> count(n);

    int l = 0, r = 0;
    while( l < n ) {
        int pr = r;
        
        while( r < n && abs(b[r] - b[l]) <= k ) {
            r++;
        }

        count[l] = r-l;
        l++;
    }

    return count;
}

// границы включительно
vector<pair<int, int>> diap(vector<int>& b) {
    vector<int> right = right_count(b);

    reverse(b.begin(), b.end());
    vector<int> left = right_count(b);
    reverse(left.begin(), left.end());
    reverse(b.begin(), b.end());

    vector<pair<int, int>> result(n);
    for(int i = 0; i < n; i++) {
        result[i] = { i - (left[i]-1), i + (right[i]-1) };
    }
    
    return result;
}

int t[1'000'000];  // == segment_b_max !!!!!
int lazy[1'000'000];

// соре угнал душноту из https://cp-algorithms.com/data_structures/segment_tree.html#more-complex-queries
void build(int a[], int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = a[tl];
        return;
    }

    int tm = (tl + tr) / 2;
    build(a, v*2, tl, tm);
    build(a, v*2+1, tm+1, tr);
    t[v] = max(t[v*2], t[v*2 + 1]);
}

void push(int v) {
    t[v*2]    += lazy[v];
    lazy[v*2] += lazy[v];
    t[v*2+1]    += lazy[v];
    lazy[v*2+1] += lazy[v];
    lazy[v] = 0;
}

void update(int v, int tl, int tr, int l, int r, int addend) {
    if (l > r) return;

    if (l == tl && tr == r) {
        t[v] += addend;
        lazy[v] += addend;
        return;
    }

    push(v);
    int tm = (tl + tr) / 2;
    update(v*2, tl, tm, l, min(r, tm), addend);
    update(v*2+1, tm+1, tr, max(l, tm+1), r, addend);
    t[v] = max(t[v*2], t[v*2+1]);
}

int query(int v, int tl, int tr, int l, int r) {
    if (l > r) return 0;
    if (l == tl && tr == r) return t[v];

    push(v);
    int tm = (tl + tr) / 2;
    return max(query(v*2, tl, tm, l, min(r, tm)), query(v*2+1, tm+1, tr, max(l, tm+1), r));
}

int find_best_b() {
    return query(1, 0, n-1, 0, n-1);
}

#undef int
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

#define int long long
    cin >> n >> k;

    vector<diamond> ab(n);

    for(int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        ab[i] = {a, b, i};
    }

    // сортируем по a
    sort(ab.begin(), ab.end(), [](auto& a, auto& b) { 
        return tuple{a.a, a.b, a.i} < tuple{b.a, b.b, b.i};
    });
    
    vector<int> copy_b(n), copy_a(n);
    for(int i = 0; i < n; i++) {
        copy_a[i] = ab[i].a;
        copy_b[i] = ab[i].b;
    }
    
    // ультра пофиш на эффективность
    vector<int> prfx_b(n+1);
    for(int i = 0; i < n; i++) {
        prfx_b[i+1] = prfx_b[i] + copy_b[i];
    }

// ===========================================================
    // сразу соберем дерево отрезков
    vector<pair<int, int>> a_diap = diap(copy_a);

    // от точки вправо: [i, left_i]
    int data_a[1'000'000] = {};
    for(int i = 0; i < n; i++) {
        int l = a_diap[i].first;
        int r = a_diap[i].second;
        data_a[i] = prfx_b[r+1] - prfx_b[i];
    }

    // ужас, пора делать дерево . . .
    build(data_a, 1, 0, n-1);

// ===========================================================
    



// ===========================================================
    int ans = 0, acc_a = 0;
    int l = 0, r = 0;
    while( l < n ) {
        while( r < n && ab[r].a - ab[l].a <= k ) {
            auto [lb, rb] = a_diap[r];

            update(1, 0, n, lb, r, -ab[r].b); 
            acc_a += ab[r].b;
            
            r++;
        }

        int acc_b = find_best_b();
        ans = max( ans, acc_a + acc_b );

        auto [lb, rb] = a_diap[l];

        update(1, 0, n, lb, l, +ab[l].b);
        acc_a -= ab[l].b;
        
        l++;
    }
    
    cout << ans << endl;
    return 0;
}
```

</details>

</details>


## Задачи с coderun

[7271 Языковой барьер.](https://coderun.yandex.ru/seasons/2025-summer/tracks/common/problem/language-barier) Похоже, кто-то умышленно вмешался в систему коммуникации: теперь одни гости говорят на привычном языке, а другие используют укороченные версии слов. \
Однако есть список полных и укороченных слов, но всё друг с другом перемешано. Необходимо восстановить соответствие между словами. \
Гарантируется, что ответ существует. Суммарная длина слов и их количество в пределах $10^5$.

[7502 Пустые сети.](https://coderun.yandex.ru/seasons/2025-summer/tracks/common/problem/prob-win) (хорошая задача - есть простое решение, дающее верный ответ, но при этом не правильное) \
Рассмотрим $A$ - случайные $k$ различных (натуральных) чисел из $[1, a]$ (любой набор равновероятен). \
Аналогично $B$ - случайные $n$ различных чисел из $[1, a+n]$. \
Определить вероятность того, что все числа из множества $A$ будут строго меньше, чем все числа из множества $B$. \
Ограничения $a, k, n \le 10^9$, но гарантируется, что: $min(n, k) \le 10^5$.

[7503 Разрушение дворцовой стены.](https://coderun.yandex.ru/seasons/2025-summer/tracks/common/problem/city-wall-destruction) \
Дан массив различных чисел $a_i$, размера $n \le 10^5$. Для каждого подмассива вычисляется разность между максимальным и минимальным значением: $max| a_i - a_j |$, где $i, j \in$ подмассив. Надо найти сумму всех полученных значений.
