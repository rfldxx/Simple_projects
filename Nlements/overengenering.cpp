#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <set>
#include <iomanip>

using namespace std;

// using mref = map<pair<mref, mref>, int>*;

struct specturm {
    struct mref;

    struct triple {
        string result;
        const string* a;
        const string* b;

        bool operator < (const triple& b) const { 
            return tuple{this->result, this->a, this->b} < tuple{b.result, b.a, b.b}; 
        }
    };


    using prev = set<pair<const string*, const string*>>;

    map< pair<mref, mref>, map<string, prev> > data;

    struct mref {
        mref(const void* pp) { p = (decltype(p)) pp; }

        // vv НАДО СДЕЛАТЬ  value_type* , А НЕ  key_type*  vv
        const decltype(data)::key_type* p;

        bool operator < (const mref& b) const { return this->p < b.p; }
    };

    friend ostream& operator << (ostream& out, const specturm& a);
    
    
    struct pos {
        mref p;
        const string* n; 
    };

    mref combine(pos a, pos b, string result) {
        data[ {{a.p}, {b.p}} ][result].insert( {a.n, b.n} );
        return {&data.find({{a.p}, {b.p}})->first};
    }

    pos get(mref p, string name) { 
        auto pp = data[ *p.p ].find(name);
        if( pp == data[ *p.p ].end() ) throw runtime_error("Can't find " + name); 
        return pos{p, &pp->first}; 
    }

    // void print_mref(ostream& out, )
};


int main() {
    specturm x;

    auto init_p = x.combine( {0, 0}, {0, 0},  "water" );
                  x.combine( {0, 0}, {0, 0},   "dirt" );
                  x.combine( {0, 0}, {0, 0}, "photon" );


    // элемент уровня  "1" := "0" + "0"
    auto p1 = x.combine( x.get(init_p, "photon"), x.get(init_p,   "dirt"),  "bacteria" );
              x.combine( x.get(init_p, "photon"), x.get(init_p,   "dirt"),     "plant" );
              x.combine( x.get(init_p,   "dirt"), x.get(init_p, "photon"),      "dust" );
              x.combine( x.get(init_p, "photon"), x.get(init_p,   "dirt"),      "dust" );
              x.combine( x.get(init_p,   "dirt"), x.get(init_p,  "water"),     "algae" );
              x.combine( x.get(init_p,   "dirt"), x.get(init_p,  "water"),      "kelp" ); 

    
    // новый элемент уровня "0" :
                  x.combine( {0, 0}, {0, 0}, "time" );


    // новый элемент уровня "1" 
              x.combine( x.get(init_p,   "time"), x.get(init_p,  "water"),  "cohaesus" ); 
              x.combine( x.get(init_p,   "time"), x.get(init_p,  "water"),      "cell" ); 
              


    // элемент уровня "2" := "0" + "1"
    auto p2 = x.combine( x.get(init_p,     "time"), x.get(p1, "bacteria"),  "kelp" );
              x.combine( x.get(init_p,     "time"), x.get(p1,     "dust"),  "kelp" );

    // элемент уровня "3" := "1" + "1"
    auto p3 = x.combine( x.get(p1, "cohaesus"), x.get(p1, "bacteria"),              "kelp" ); 
              x.combine( x.get(p1, "cohaesus"), x.get(p1,     "cell"),  "Quantum observer" ); 


    cout << x;
}

ostream& operator << (ostream& out, const specturm& a) {
    for(auto& [k, v] : a.data) {
        auto& [k1, k2] = k;
        
        out << "[" << k1.p << " , " << k2.p << "] :\n";
        for(auto& [str, prv] : v) {
            std::ostringstream sname;
            sname << ">" << str << "< = " << &str << " ";
            string name = sname.str();
            out << "\t" << name;

            bool nft = 0;
            for(auto [pa, pb] : prv) {
                if(nft) out << "\t" << setw(name.size()) << " ";
                out << " <- {";

                for(auto& xp : {pa, pb})
                    if(xp) out << "  >" << *xp << "< = " << xp;
                   
                out << "  }" << endl;
                nft = 1;
            }

            if( prv.begin() == prv.end() )
             out << endl;
        }
    }

    return out;
}

void operator + (const specturm::mref& a, const specturm::mref& b) {

}