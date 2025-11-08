### DSU

<details>

<summary> Текущий шаблон DSU </summary>

```cpp

// ===== [ DSU ] ==============================================================
template<typename... AccounterTypes>
struct DSU {
    vector<int> pprev, sz;
    tuple<AccounterTypes...> accounters;

    template<size_t N>  // получения счетчиков
    auto& accounter(int a) { return std::get<N>(accounters)[get(a)]; }

    DSU(int n) : pprev(n), sz(n, 1), accounters(AccounterTypes(n)...) {
        for(int i = 0; i < n; i++) pprev[i] = i;
    }

    int get(int a) { return pprev[a] == a ? a : pprev[a] = get(pprev[a]); }

    int unite(int a, int b) {
        int id1 = get(a), id2 = get(b);
        if( id1 == id2 ) return id1;
        
        auto temp = pair{id1, id2};
        if( sz[id1] < sz[id2] ) swap(id1, id2);
        
        sz[id1] += sz[id2];
        pprev[id2] = id1;
        
        // вызываем .uniting() для каждого accounter-а
        auto curr = pair{id1, id2};
        std::apply( 
            [curr, temp](auto&... args) { (args.uniting(curr, temp), ...); }, 
            accounters
        );

        return id1;
    }
};

template<typename Operation>
struct Tracker {
    vector<int> var;
    int& operator[] (int i) { return var[i]; }

    Tracker(int n) : var(n) {
        for(int i = 0; i < n; i++) var[i] = i;
    }

    void uniting(pair<int, int> unite_order, pair<int, int> input_order) {
        auto [id1, id2] = unite_order;
        auto [ida, idb] = input_order;
        var[id1] = Operation{}( var[id1], var[id2], var[ida], var[idb]);
    }
};
// ============================================================================


// > создать обычное dsu <
//   DSU rem(n);


// > создать dsu c поискум максимума и минимума в каждом множестве <
//   struct MinElem {
//       int operator () (int v1, int v2, int va, int vb) {
//           return min(v1, v2);
//       }
//   };
//   struct MaxElem {
//       int operator () (int v1, int v2, int va, int vb) {
//           return max(v1, v2);
//       }
//   };
//
//   DSU<Tracker<MinxElem>, Tracker<MaxElem>> rem(n);
//   rem.accounter<0>(a);  // получили min в множестве с элементом a 
//   rem.accounter<1>(a);  // получили max в множестве с элементом a
```

</details>

$\text{ }$


Задачи (из вкладки EDU на codeforces):

<details>

<summary> 1C. Подсчет опыта </summary>

Изначально у всех игроков $0$ опыта и каждый из них состоит в клане состоящем из него одного.\
Даны запросы трех типов:
* $\text{join}$ $X$ $Y$ — объединить кланы, в которые входят игроки $X$ и $Y$ (если они уже в одном клане, то ничего не меняется)
* $\text{add}$ $X$ $V$ — добавить $V$ единиц опыта всем участникам клана, в который входит игрок $X$
* $\text{get}$ $X$ — вывести текущий опыт игрока $X$

<details>

<summary> Решение </summary>

Чтобы узнать опыт участника, надо просуммировать все `delta[v]` на пути до корня.\
В запросах мы увеличиваем `delta` только для корневой вершины, так что можем сжимать пути в СНМ.

Асимптотика: $O(N \cdot \alpha(N))$

```cpp
#include <bits/stdc++.h>
using namespace std;

// СНМ через дерево
int pprev[200001];
int    sz[200001];
int delta[200001];

int get(int a) {
    if( pprev[a] == a ) return a;  // корень
    
    // получили корень и обновили delta[v] на всех вершинах до корня
    int root = get(pprev[a]);      

    if( pprev[a] != root )
        delta[a] += delta[pprev[a]];
    
    return pprev[a] = root;
}

void unite(int id1, int id2) {
    if( sz[id1] < sz[id2] ) swap(id1, id2);

    sz[id1] += sz[id2];

    pprev[id2]  = id1;
    delta[id2] -= delta[id1];
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;

    for(int i = 0; i < n; i++) {
        pprev[i] = i;
        sz   [i] = 1;
    }

    while( m-- ) {
        string t; int u, v;
        cin >> t >> u;

        int id1 = get(u-1);

        if( t[0] == 'g' ) {       // get
            cout << delta[u-1] + (u-1 != id1 ? delta[id1] : 0) << "\n";
            continue;
        }
        else if( t[0] == 'a' ) {  // add
            int x; cin >> x;
            delta[id1] += x;
            continue;
        }

        cin >> v;
        int id2 = get(v-1);
        if( id1 != id2 ) unite(id1, id2);
    }
}
```

</details>

</details>


$\text{ }$


<details>

<summary> 2C. Реструктуризация компании </summary>

Изначально каждый человек работает в своём собственном отделе, пронумерованном от $1$ до $n$. \
Обозначим за $team(\text{person})$ команду, в которой работает человек $\text{person}$. \
Даны запросы трех типов (ого какое совпадение):

* Объединить отделы $team(x)$ и $team(y)$. (если они уже в одном отделе, то ничего не меняется)

* Объединить отделы $team(x) , team (x+1), \dots, team(y)$ , где $x$ и $y$  ( $1 \le x \le y \le n$ ) — номера каких-то двух сотрудников компании.

* Работают ли сотрудники $x$ и $y$ в одном отделе.

<details>

<summary> Решение </summary>

Вся сложность - это понять какие сейчас множества есть на отрезке $[x, y]$ для $\text{II}$-го типа запроса. 

Давайте заведем дополнительное DSU для "числовой прямой": это будут отрезки $[x_1, x_2]$ в которых все люди в одной команде. \
Асимптотика: $O(N \cdot \alpha(N))$

```cpp
// ===== [ DSU ] =====
// ...
// ===================

struct MaxElem {
    int operator () (int v1, int v2, int va, int vb) {
        return max(v1, v2);
    }
};
 
int main() {
    int n, Q; cin >> n >> Q;
    DSU team(n);
    DSU<Tracker<MaxElem>> line_segment(n+1);
 
    while( Q-- ) {
        char t; int x1,   x2;
        cin >> t >> x1 >> x2;
        x1--; x2--;
 
        // будем лениво merg-ить отрезки -> только при диапозонном union
        if( t == '1' ) team.unite(x1, x2);
        if( t == '2' ) {
            int curr_x = x1;
            while( curr_x <= x2 ) {
                int next_x = line_segment.accounter<0>(curr_x) + 1;
                team        .unite(x1, curr_x);
                line_segment.unite(x1, curr_x);  // <- merg-им отрезки
                curr_x = next_x;
            }
        }

        if( t == '3' ) cout << (team.get(x1) == team.get(x2) ? "YES\n" : "NO\n");
    }
}
```

*Решения других людей:*

Прикольная идея с `set<int> actived`:
```cpp
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 200010;

int n, q;
int anc[MAXN], rnk[MAXN];

set<int> actived;

int get(int cur) { return anc[cur] == cur ? cur : anc[cur] = get(anc[cur]); }

void join(int i, int j) {
	i = get(i); j = get(j);
	if( i == j ) return;

	if( rnk[i] < rnk[j] ) swap( i , j );

	anc[j] = i;
	if( rnk[i] == rnk[j] ) rnk[i]++;
}

int main() {
    cin >> n >> q;
    
	for(int i = 1 ; i <= n ; i++) {
		anc[i] = i;
		actived.insert( i );
	}

	while( q-- ) {
		int type, x, y; cin >> type >> x >> y;
        
		if( type == 1 ) join( x , y );
		if( type == 2 ) {
			auto it = actived.upper_bound( x );

			while( it != actived.end() && *it <= y ) {
				join( x , *it );
				it = actived.erase(it);
			}
		}
		if( type == 3 ) cout << ( get(x) == get(y) ? "YES\n" : "NO\n");
	}
}
```

Какая-то мега идея. С указателями `r[]` происходит что-то очень похожее на сжатие путей, но может в данном случае будет $O(N)$ суммарно ?? что-то не могу оценить асимптотику
```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 200005;
int fa[N], r[N];
int get(int x) { x == fa[x] ? x : fa[x] = get(fa[x]); }

int main() {
	int n, Q, type, u, v, tmp;
    сin >> n >> Q;
	for(int i = 1 ; i <= n ; ++ i) fa[i] = i, r[i] = i + 1;

	while( Q-- ) {
        cin >> type >> u >> v;
		if( type == 1 ) fa[get(u)] = get(v);
		if( type == 2 ) {
			while(u < v) {
				fa[get(u)] = get(v);
				tmp = r[u], r[u] = r[v], u = tmp;
			}
		}
        if( type == 3 ) cout << ( get(x) == get(y) ? "YES\n" : "NO\n");
	}
}
```

</details>

</details>

