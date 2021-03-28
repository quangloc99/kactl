#include "../utilities/template.h"

#include "../../content/various/QueueLikeUndo.hpp"

using uint = unsigned int;
using Upd = uint32_t;
class DS {
    vector<uint> st_and;
    vector<uint> st_sum;
public:
    DS() : st_and{(uint)-1}, st_sum{0} {}
    pair<uint, uint> get_ans() const {
        return {st_and.back(), st_sum.back()};
    }
    
    void accept(Upd x) {
        st_and.emplace_back(st_and.back() & x);
        st_sum.emplace_back(st_sum.back() + x);
    }
    
    void undo() {
        assert(sz(st_and) > 1);
        st_and.pop_back();
        st_sum.pop_back();
    }
    
    size_t size() const {
        return st_and.size() - 1; 
    }
};

using QueueDS = QueueUndo<DS, Upd>;

struct StupidDS {
    deque<uint> all_nums;
    void accept(Upd x) {
        all_nums.push_back(x);
    }
    
    void undo() {
        assert(sz(all_nums));
        all_nums.pop_front();
    }
    
    pair<uint, uint> get_ans() const {
        uint and_all = (uint)-1;
        uint sum_all = 0;
        for (auto i: all_nums) {
            and_all &= i;
            sum_all += i;
        }
        return {and_all, sum_all};
    }
};

// #define do_log 

#ifndef do_log 
#define clog if(0) cout
#endif
int main() {
    example();
    mt19937 rng;
#define rand() ((int)(rng() >> 1))
    rep(testcase, 0, 10) {
        clog << "===TEST" << endl;
        QueueDS qds;
        StupidDS sds;
        auto check = [&] { 
            clog << "and: " << qds.data.get_ans().first << ' ' << sds.get_ans().first << endl;
            clog << "sum: " << qds.data.get_ans().second << ' ' << sds.get_ans().second << endl;
            assert(qds.data.get_ans() == sds.get_ans());
        };
        
        check();
        rep(step, 0, 500) {
            int type = sz(qds.data) == 0 ? 1 : rand() % 2;
            if (type == 1) {  // add
                // doing so because after "not so much" step, "and" of all
                // numbers reduce a lot
                int cnt = rand() % 2 ? rand() % 1000 : rand() % 20;
                while (cnt--) {
                    uint x = (uint)rng();
                    clog << "Add " << x << endl;
                    qds.accept(x);
                    sds.accept(x);
                    check();
                }
            } else {  // remove
                int cnt = rand() % 50  == 0 ? sz(qds.data) : rand() % sz(qds.data);
                while (cnt--) {
                    clog << "Undo" << endl;
                    qds.undo();
                    sds.undo();
                    check();
                }
            }
        }
        cout << "OK" << endl; 
    }
	cout << "Tests passed!" << endl;
}
