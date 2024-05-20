#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <map>
#include <set>

using namespace std;


template <size_t N>
class unit {
public:
    struct bond { int i, j, q = 1; };
    struct intr { char a; int start, end; };

    unit(const char* atoms, vector<bond> pairs) {
        map<char, vector<int>> reflection;
        for(int i = 0; i < N; i++) reflection[atoms[i]].push_back(i);

        int mapping[N], counter = 0;
        for(const auto& [k, v] : reflection) {
            int temp = counter;
            for(const auto ii : v) {
                mapping[ii] = counter;
                counter++;
            }
            intervals.push_back({k, temp, counter});
        }

        for(int i = 0; i < N; i++) atom[mapping[i]] = atoms[i];
        
        for(int t = 0; t < pairs.size(); t++) {
            int i = mapping[pairs[t].i-1], j = mapping[pairs[t].j-1];
            matrix[i][j] += pairs[t].q;
            matrix[j][i] += pairs[t].q;
        }
    }

    char   atom[N];
    vector<intr> intervals; // atom_type : [start, end)
    int  matrix[N][N]{};
};

template <size_t N>
ostream& operator << (ostream& out, const unit<N>& x) {
    out << ' ';
    for(int i = 0; i < N; i++) out << ' ' << x.atom[i];
    out << "  | ";
    for(const auto& e : x.intervals) out << e.a << ":[" << e.start << "," << e.end << ")  ";
    for(int i = 0; i < N; i++) {
        out << endl << x.atom[i] << string(1 + 2*i, ' ') << '#';
        for(int j = i+1; j < N; j++) 
            out << ' ' << "* 123456789"[x.matrix[i][j]+1]; // зато без if-а
    }
    return out << endl;
}

template<size_t N, size_t M>
bool test(const unit<N>& mol, const unit<M>& pattern, const array<int, M>& pos) {
    { // проверка что не рассмотрен дважды один и тот же атом
        set<int> considered;
        for(auto ii : pos) 
            if  (considered.count (ii)) return 0;
            else considered.insert(ii);
    }

    // проверка что "под-матрица" равна шаблонной
    for(int i = 0; i < M; i++)
        for(int j = i+1; j < M; j++)
            if( mol.matrix[pos[i]][pos[j]] != pattern.matrix[i][j] ) return 0;

    return 1;
}

//  Находит кол-во паттернов (самым тупым перебором: ~N^M)
// (без учёта пересечения паттернов: finds(H-O-H, -O-H) вернёт значение 2)
template<size_t N, size_t M>
int finds(const unit<N>& mol, const unit<M>& pattern) {
    int ans = 0, counter = 0;
    array<pair<int, int>, M> diaposon;
    array<int, M> pos;
    for(auto pastel : pattern.intervals) {
        pair<int, int> positions = {0, N};
        char find_atom = pastel.a;
        bool is_find = (find_atom == '_');

        if(!is_find)
            for(auto& e : mol.intervals) 
                if(e.a == find_atom) {
                    positions = {e.start, e.end};
                    is_find = 1;
                    break;
                }

        if(!is_find) { cout << "Diffetent elements" << endl; return 0; } // атома из шаблона нет в молекуле
        if(positions.second-positions.first < pastel.end-pastel.start) return 0; // в молекуле меньше требуемых атомов чем в шаблоне
        
        for(int i = 0; i < pastel.end-pastel.start; i++, counter++) {
            diaposon[counter] = positions;
            pos[counter] = diaposon[counter].first;
        }
    }


    // перебираем pos в своих диапозонах
    while (1) {
        for(int i = 0; i < M; i++) {
            if(pos[i] < diaposon[i].second) break;

            if(i == M-1) return ans;

            pos[i] = diaposon[i].first;
            pos[i+1]++;
        }

        ans += test(mol, pattern, pos);
        pos[0]++;
    }
}


#define TEST(name, test, ans) if(finds(name, test) != ans) { cout << "Error in " #name " in: " #test << endl; return 0; }
#define   OK(name) printf("Test %10s  - OK\n", #name); fflush(stdout); return 1;

bool test_water() {
    auto water = unit<3>("HOH", {{1, 2}, {2, 3}});
    auto   OH  = unit<3>("HO_", {{1, 2}, {2, 3}});
    TEST(water, OH, 2)
      OK(water)
}

bool test_benzol() { // типо бензол, но вместо -H подвещено -O-H
    auto benzol = unit<18>("COCOCOCOCOCOHHHHHH", 
                    {{1, 2},    {3, 4},  {5, 6},    {7, 8},  {9, 10},    {11, 12},  // C-O
                     {1, 3, 2}, {3, 5},  {5, 7, 2}, {7, 9},  {9, 11, 2}, {11, 1 },  // C-C или C=C
                     {2, 13},   {4, 14}, {6, 15},   {8, 16}, {10, 17},   {12, 18}}  // O-H
                          ); 
                                                    
    //   |
    // - C =    -  должно быть 12 штук (!! вообще не учитываются симметрии шаблона)
    auto test1 = unit<4>("C___", {{1, 2}, {1, 3}, {1, 4, 2}});
    TEST(benzol, test1, 12)

    //   O
    //   |
    // - C =    -  должно быть  6 штук
    auto test2 = unit<4>("CO__", {{1, 2}, {1, 3}, {1, 4, 2}});
    TEST(benzol, test2, 6)

    //   H -    -  должно быть  6 штук
    auto test3 = unit<2>("H_", {{1, 2}});
    TEST(benzol, test3, 6)

    //   H-O-   -  должно быть  6 штук
    auto test4 = unit<3>("HO_", {{1, 2}, {2, 3}});
    TEST(benzol, test4, 6)

    //   C = C  -  должно быть  6 штук (!! вообще не учитываются симметрии шаблона)
    auto test5 = unit<2>("CC", {{1, 2}});
    TEST(benzol, test5, 6)
      OK(benzol)
}

int main() {
    test_water();
    test_benzol();
    
    return 0;
}
