// Создана специальная структура Storage - которую надо только один раз предпосчитать для заданных X_train, X_test.
// Для этого при первом запуске должен быть задан макрос FIRST_LAUNCH.
// 
// ИТОГО:
//  * первый запуск - компилируем:  g++ naive2.cpp -std=c++17 -O3 -DFIRST_LAUNCH
//  * далее компилируем как:        g++ naive2.cpp -std=c++17 -O3

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cassert>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>

using namespace std;

// ===== [ СЧИТЫВАНИЕ ТАБЛИЦЫ ] =========================================================
// НУЖНО УКАЗЫВАТЬ КОЛ-ВО ПРИЗНАКОВ
// whitch == 0/1 - считываются соответственно только +/- объекты
// whitch == -1  - считываются все объекты (в таком случае Yfilename гоняется в холостую)
vector<vector<int>> read(string Xfilename, int M, string Yfilename, int whitch) {
    ifstream Xin(Xfilename);
    ifstream Yin(Yfilename);

    // скипаем первую строчку
    string s;
    getline(Xin, s);
    getline(Yin, s);

    vector<vector<int>> table;
    string first;
    while( Xin >> first ) {
        vector<int> neww(M);
        neww[0] = (first[0] == 'T');  // True / False

        for(int j = 1; j < M; j++) {
            string a; Xin >> a;
            neww[j] = (a[0] == 'T');
        }

        bool result;
        Yin >> result;

        if( whitch == -1 || result == whitch ) table.push_back(neww);
    }

    return table;
}

vector<int> readY(string test_file) {
    ifstream Yin(test_file);

    // скипаем первую строчку
    string s; getline(Yin, s);

    vector<int> ans;
    for(int value; Yin >> value; ) ans.push_back(value);
    return ans;
}

double f1_score(const vector<int>& real_value, const vector<int>& predicted) {
    assert( real_value.size() == predicted.size() );
    int TP = 0, FP = 0, FN = 0;

    for(int i = 0; i < real_value.size(); i++) {
        TP += (predicted[i] == 1 && real_value[i] == 1);
        FP += (predicted[i] == 0 && real_value[i] == 1);
        FN += (predicted[i] == 1 && real_value[i] == 0);
    }

    double precision = 1. * TP / (TP + FP);
    double recall    = 1. * TP / (TP + FN);
    return 2.*precision*recall / (precision + recall); 
}
// --------------------------------------------------------------------------------------


// Находит кол-во вложений строки intersection в table
inline int count(const vector<int>& intersection, const vector<vector<int>>& table) {
    int count = 0;
    for(const auto& line : table) {
        bool ok = 1;

        for(int j = 0; j < line.size(); j++) 
            ok &= (!intersection[j] | line[j]);  // intersection -> line 
        
        count += ok;
    }

    return count;
}


// 1. Пересекает sample со всеми объектами из table
// 2. Находит кол-во вхождений intersection-а в table и other_table
// 3. На основе этого делает вердикт для данного объекта из table о принадлежности к классификатору
vector<pair<int, int>> IntersectionTableCount(const vector<int>& sample, 
            const vector<vector<int>>& table, const vector<vector<int>>& other_table) {
    int M = sample.size();
    assert(table[0].size() == M);

    vector<pair<int, int>> ans;
    ans.reserve( table.size() );

    for(const auto& line : table) {
        vector<int> intersection(M);
        for(int j = 0; j < M; j++) 
            intersection[j] = sample[j] & line[j];
        
        int positive_count = count(intersection, table);
        int negative_count = count(intersection, other_table);

        ans.push_back( {positive_count, negative_count} );
    }

    return ans;
}

// --------------------------------------------------------------------------------------


class Storage {
public:
Storage(const vector<vector<int>>& positive_table, const vector<vector<int>>& negative_table,
        const vector<vector<int>>& test_table) {
    int Ntest = test_table.size(); 
    positive_pairwise.reserve(Ntest);
    negative_pairwise.reserve(Ntest);

    for(const auto& neww : test_table) {
        positive_pairwise.push_back( IntersectionTableCount(neww, positive_table, negative_table) );
        negative_pairwise.push_back( IntersectionTableCount(neww, negative_table, positive_table) );
    }
}

// Чтение из файла
Storage(string filename) {
    ifstream in(filename);
    int Ntest, Ntrain_positive, Ntrain_negative;
    in >> Ntest >> Ntrain_positive >> Ntrain_negative;

    positive_pairwise.resize(Ntest);
    negative_pairwise.resize(Ntest);

    for(auto& neww : positive_pairwise) {
        neww.resize(Ntrain_positive);
        for(auto& [same, other] : neww) in >> same >> other;
    }

    for(auto& neww : negative_pairwise) {
        neww.resize(Ntrain_negative);
        for(auto& [same, other] : neww) in >> same >> other;
    }
}


// Запись в файл
void load_to(string filename) {
    ofstream out(filename);

    // Ntest  Ntrain_positive  Ntrain_negative
    out << positive_pairwise.size() << " " << positive_pairwise[0].size() << " " << negative_pairwise[0].size() << "\n";

    for(const auto& neww : positive_pairwise)
        for(auto [same, other] : neww) 
            out << same << " " << other << "\n";

            
    for(const auto& neww : negative_pairwise)
        for(auto [same, other] : neww) 
            out << same << " " << other << "\n";
}

// Матрицы из элементов pair<int, int>
//   pairwise[i]          - относится к i-ому тестовому объекту
//   pairwise[i][j]       - пересекаем  i-ый  тестовый  объект с j-ым тренировочным
//                         (j-ый берется из соответсвующей таблицы - positive или negative)
//  .first элемент в pair - кол-во вложений пересечения i и j в соответсвующую таблицу
//                         (в positive_table для positive_pairwise и в negative_table для negative_pairwise)
//  .second               - кол-во вложений пересечения i и j в противоположную таблицу
//                         (в negative_table для positive_pairwise и в positive_table для negative_pairwise)

    vector<vector< pair<int, int> >> positive_pairwise;
    vector<vector< pair<int, int> >> negative_pairwise;
};


void precalculating(const vector<vector<int>>& positive_table, const vector<vector<int>>& negative_table,
                    const vector<vector<int>>& test_table, string destination_filename) {
    cout << "Start fill Storage" << endl;
    auto start = chrono::high_resolution_clock::now();
    Storage Calculate(positive_table, negative_table, test_table);
    auto stop  = chrono::high_resolution_clock::now();
    cout << "# FINISH  " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << "ms" << endl;
    
    Calculate.load_to(destination_filename);
    cout << "LOAD TO FILE: " << destination_filename << endl << endl;
}

// --------------------------------------------------------------------------------------


template<typename ClassifierGen, typename PredictionGen>
vector<int> GetPrediction(ClassifierGen IsClassifierFunc, PredictionGen PredictorFunc, const Storage& Storage) {
    int Ntest = Storage.positive_pairwise.size();
    vector<int> ans;
    ans.reserve(Ntest);
    
    int mxp = 0, mxn = 0;
    for(int i = 0; i < Ntest; i++) {
        int cnt_positive_classifier = 0;
        for(auto [same, other] : Storage.positive_pairwise[i]) 
            cnt_positive_classifier += IsClassifierFunc(same, other);
    
        
        int cnt_negative_classifier = 0;
        for(auto [same, other] : Storage.negative_pairwise[i]) 
            cnt_negative_classifier += IsClassifierFunc(same, other);
    
        ans.push_back( PredictorFunc(cnt_positive_classifier, cnt_negative_classifier) );

        mxp = max(mxp, cnt_positive_classifier);
        mxn = max(mxn, cnt_negative_classifier);
    }

    cout << "Max count of positive classifier: " << mxp << endl;
    cout << "Max count of negative classifier: " << mxn << endl;

    return ans;
}

// --------------------------------------------------------------------------------------



class AlphaWeakClassifier {
public:
    int operator() (int positive_count, int negative_count) {
        return negative_count < alpha * (negative_count + positive_count);
    }

    // Некоторая память / настройки для функции / место для гиперпараметров
    double alpha = 0.1;
};

class Predictor {
public:
    int operator() (int positive_classifiers, int negative_classifiers) {
        int result = 1;
        if( negative_classifiers*positive_cnt > positive_classifiers*negative_cnt ) 
            result = 0;
        return result;
    }

    // Некоторая память / настройки для функции / место для гиперпараметров
    int positive_cnt, negative_cnt;
};



#ifndef FIRST_LAUNCH
#define FIRST_LAUNCH 0
#else
#undef  FIRST_LAUNCH
#define FIRST_LAUNCH 1
#endif

int main() {
    int M = 92;    
    auto positive_table = read ("X_train.csv", M, "y_train.csv",  1);
    auto negative_table = read ("X_train.csv", M, "y_train.csv",  0);
    auto     test_table = read ("X_test.csv" , M, "y_test.csv" , -1);
    auto     test_answr = readY("y_test.csv");

    if( FIRST_LAUNCH ) 
        precalculating(positive_table, negative_table, test_table, "Storage.txt");

    Storage MyStorage("Storage.txt");

    int positive_cnt = positive_table.size();
    int negative_cnt = negative_table.size();
    cout << "All positive objects : " << positive_cnt << "  |  All negative objects : " << negative_cnt << endl;
    
    Predictor MyPredictor{positive_cnt, negative_cnt};

    for(auto alpha : {0.01, 0.03, 0.05, 0.1, 0.15}) {
        cout << "Alpha weak hypothesis: a = " << alpha << endl;
        
        AlphaWeakClassifier MyClassifier{alpha};
        auto my_predicted_value = GetPrediction(MyClassifier, MyPredictor, MyStorage);
        cout << "F1-score : " << setprecision (15) << f1_score(test_answr, my_predicted_value) << endl << endl;
    }
}
