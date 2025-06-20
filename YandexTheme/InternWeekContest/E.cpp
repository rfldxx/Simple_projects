// подготовка...
// вроде решается методом - разделяй и влавствуй

// В крацее, количество прямоугольников, пересекающихся с данной прямоугольной областью можно находить через преффексные суммы (только их понадобиться не одна а целых 4).
//
// 1. Заведем по преффексной сумме для каждой разновидности угла прямоугольника отдельно:
//     X --------- B
//     |           |
//     |           |
//     A --------- Y
//    prfxA[x][y], prfxB[x][y], prfxX[x][y], prfxY[x][y]
//
// 2. Если мы рассматриваем область:
//    (x1, y2) ----------------- (x2, y2)           ( тоже надо будет записать,           )
//       |                          |               ( вроде это не стандартное применение )
//       |                          |                           |
//    (x1, y1) ----------------- (x2, y1)                       V
//    То ответом для неё будет: prfxA[x2][y2] - prfxX[x2][y1] - prfxY[x1][y2] + prfxB[x1][y1]
//
// 3. На самом деле мы не сможем по честному хранить преффексные суммы - так что от них останутся только запросы.
//    Можно моделировать преффекс сумму через дерево отрезков. Но это кажется пушкой по воробьям.
//    И действительно в InterestingTasks.md показано как можно решить следующую задачу по аналогии с merge-sort. 
//         Задача. Дан массив a_i. Для каждого i найти кол-во j < i, таких что a_j < a_i.
//
// 
// Наконец-то добил эту задачу ! . .



#include <iostream>
#include <map>
#include <array>

using namespace std;

// решил на ООПешить
template<typename Cont>
class Interval {
public:
    Interval(Cont& a_) : p(a_.begin()), a(a_) {}
    
    typename Cont::iterator  p;
    Cont& a;
    
    operator int() const { return p != a.end(); }
    // bool operator !  () { return  p == a.end(); }

    auto operator *  () { return *p; }
    auto operator -> () { return  p; }
    Interval operator ++ (int) { Interval temp = *this; p++; return temp; }
};

template<typename Cont>
void combine(Cont& a, Cont& b) {
    array<Interval<Cont>, 2> p = {a, b};

    while( 1 ) {
        bool t = !p[0] || ( p[1] && p[1]->first < p[0]->first );
        
        if( !p[t] ) return;
        
        auto [k, v] = *p[t]++;
        cout << k << " : " << v << endl;
    }
}

int main() {
    map<int, int> a = { {1, 2}, {9, 3}, {3, 5} };
    map<int, int> b = { {4, 7}, {2, 0} };
    
    combine(a, b);
}
