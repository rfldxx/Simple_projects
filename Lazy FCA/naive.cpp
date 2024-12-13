#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cassert>
#include <algorithm>
#include <chrono>
#include <fstream>

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

double f1_score(string test_file, const vector<int>& predicted) {
    ifstream Yin(test_file);

    // скипаем первую строчку
    string s; getline(Yin, s);

    int TP = 0, FP = 0, FN = 0;

    for(auto pred_val : predicted) {
        bool real_val; Yin >> real_val;
        TP += (pred_val == 1 && real_val == 1);
        FP += (pred_val == 0 && real_val == 1);
        FN += (pred_val == 1 && real_val == 0);
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
template<typename ClassifierGen>
int CountOfClassifier(const vector<int>& sample, const vector<vector<int>>& table, 
                      const vector<vector<int>>& other_table, ClassifierGen IsClassifierFunc) {
    int M = sample.size();
    assert(table[0].size() == M);

    int count_of_classifier = 0;

    for(const auto& line : table) {
        vector<int> intersection(M);
        for(int j = 0; j < M; j++) 
            intersection[j] = sample[j] & line[j];
        
        int positive_count = count(intersection, table);
        int negative_count = count(intersection, other_table);

        count_of_classifier += IsClassifierFunc(positive_count, negative_count);
    }

    return count_of_classifier;
}
// --------------------------------------------------------------------------------------


class AlphaWeakClassifier {
public:
    int operator() (int positive_count, int negative_count) {
        return negative_count < alpha * (negative_count + positive_count);
    }

    // Некоторая память / настройки для функции
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

    // Некоторая память / настройки для функции
    int positive_cnt, negative_cnt;
};

void TestAlpha(const vector<vector<int>>& positive_table, const vector<vector<int>>& negative_table,
               const vector<vector<int>>&     test_table, double alpha) {
    int M = positive_table[0].size();
    int positive_cnt = positive_table.size();
    int negative_cnt = negative_table.size();
    cout << "All positive objects : " << positive_cnt << "  |  All negative objects : " << negative_cnt << endl;


    AlphaWeakClassifier MyClassifier{alpha};
    Predictor           MyPredictor {positive_cnt, negative_cnt};
    
    vector<int> my_predicted_value;

    for( const auto neww : test_table ) {
        int positive = CountOfClassifier(neww, positive_table, negative_table, MyClassifier);
        int negative = CountOfClassifier(neww, negative_table, positive_table, MyClassifier);

        my_predicted_value.push_back( MyPredictor(positive, negative) );
    }

    cout << "F1 score : " << f1_score("y_test.csv", my_predicted_value) << endl;
}

int main() {
    int M = 92;
    auto positive_table = read("X_train.csv", M, "y_train.csv",  1);
    auto negative_table = read("X_train.csv", M, "y_train.csv",  0);
    auto     test_table = read("X_test.csv" , M, "y_test.csv" , -1);

    for(auto alpha : {0.01, 0.03, 0.05, 0.1, 0.15}) {
        cout << "Alpha weak hypothesis: a = " << alpha << endl;
        TestAlpha(positive_table, negative_table, test_table, alpha);
        cout << endl;
    }
}
