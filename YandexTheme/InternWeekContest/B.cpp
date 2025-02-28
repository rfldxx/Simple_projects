// > https://codeforces.com/problemset/gymProblem/102262/A
// Спасибо М* за cссылку на контест

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct interval {
    interval() {}
    interval(int start_, int end_, int dy_) : start(start_), end(end_), dy(dy_) {
        dx = (end_ == -1) ? 1 : max(start_, end_) - min(start_, end_);
    }

    int start, end;
    int dx, dy;
};

struct angle {
    int dx, dy;
    bool operator < (const angle& other) const {
        return dy*other.dx < dx*other.dy;
    }
};


// return: < <P_j of min_pos>, <dwn_error, upp_error> >
pair< int, array<vector<interval>, 2> > read_group() {
// считывание данных
    int m; cin >> m;
    vector<int> p_j(m);
    for(auto& e : p_j) cin >> e;

    sort(p_j.begin(), p_j.end());
    
    vector< pair<int, int> > uniq;  // <p_j, count of this value>
    for(int i = 0; i < m; i++) {
        int count = 1;
        while( i+1 < m && p_j[i+1] == p_j[i] ) count++, i++;
        uniq.push_back({p_j[i], count});
    }
    int n = uniq.size();

    

// подсчет ошибки для разных "высот" (P-шек)
    vector<int> error(n); error[0] = 0;  // ошибка для каждой высоты из uniq
    int acc = 0, prefix = 0;
    for(int i = 1; i < n; i++) {  // префиксная "ошибка"
        acc += uniq[i-1].second;
        error[i] = prefix += acc*(uniq[i].first - uniq[i-1].first);
    }

    int min_pos = n-1;
    int suffix  = acc = 0;
    for(int i = n-2; i >= 0; i--) {  // суффиксная "ошибка"
        acc += uniq[i+1].second;
        error[i] += suffix += acc*(uniq[i+1].first - uniq[i].first);

        if( error[i] < error[min_pos] ) min_pos = i;
    }



// подгатавливаем для вывода
    vector<interval> upp(n - min_pos);
    upp.back() = {uniq.back().first, -1, m};  // края
    for(int i = min_pos, pos = 0; pos < upp.size()-1; pos++, i++)
        upp[pos] = {uniq[i].first, uniq[i+1].first, error[i+1] - error[i]};
    
   
    vector<interval> dwn(min_pos+1);
    dwn.back() = {uniq.front().first, -1, m};  // края
    for(int i = min_pos, pos = 0; pos < dwn.size()-1; pos++, i--)
        dwn[pos] = {uniq[i].first, uniq[i-1].first, error[i-1] - error[i]};
    
    return { uniq[min_pos].first, {dwn, upp} };
}


int main() {
    int n; cin >> n;
    vector<int> T(n);
    int current_T = 0;
    vector<array<vector<interval>, 2>> direction(n);
    
// изначально раставляем все значения на "медианы"
    for(int i = 0; i < n; i++) {
        tie(T[i], direction[i]) = read_group();
        current_T += T[i];
    }

    int needed_T; cin  >> needed_T;
    bool dir = (current_T < needed_T);


// сортируем интервалы по возрастанию угла наклона
    vector<int> poses(n, 0);  // номер интервала для каждой группы 
    map<angle, vector<int>> angles;
    for(int i = 0; i < n; i++) {
        interval cur = direction[i][dir][ poses[i] ];
        angles[{cur.dx, cur.dy}].push_back(i);
    }


// наконец, основной цикл "балансировки"
    while( current_T != needed_T ) {
        auto p = angles.begin();
        int i = p->second.back();
        p->second.pop_back();
        if( p->second.empty() ) angles.erase(p);

        interval cur = direction[i][dir][ poses[i]++ ];
        
        int  delta_T = (cur.end == -1) ? (needed_T - current_T) : (cur.end - cur.start);

        // не может быть отрицательных T
        if( dir == 0 && cur.end == -1 ) delta_T = -cur.start;
      

        // пофиг, харкодим
        if( dir ? (delta_T >= needed_T - current_T) : (delta_T <= needed_T - current_T) ) {
            T[i] += (needed_T - current_T);
            break;
        } 

        // иначе мутотень (оказалось не особо)
        current_T += delta_T;
        T[i]      += delta_T;
 
        if( cur.end != -1 ) { // по идеи можно заменить только на dir == 0
            interval next = direction[i][dir][ poses[i] ];
            angles[{next.dx, next.dy}].push_back(i);
        }
    }

    for(auto t : T) cout << t << " ";
}
