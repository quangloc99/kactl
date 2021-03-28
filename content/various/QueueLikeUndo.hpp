/**
 * Author: Noam *Noam527* Gutterman (https://codeforces.com/profile/Noam527)
 * Date: 2020-08-08
 * License: none
 * Source: https://codeforces.com/blog/entry/83467
 * Description: Transform a data structure with stack-like undo operation to
 * ds with ONLINE queue-like undo operation.
 * Time: Amortized $O(\log N \times C)$ ($C$ = cost of the inner DS).
 * Seem to be faster in practice (from the actual author).
 * Status: Stress-tested
 */
#pragma once
template<class DS, class Upd> struct QueueUndo {
	DS data;
	QueueUndo(): bot(0) {}
	QueueUndo(const DS& d): data(d), bot(0) {}
	void accept(const Upd& up) {
		data.accept(up);
		s.emplace_back('B', up);
	}
	void undo() {
		inc_bot();
	  /// no more A's, let's reverse and begin again.
		if (bot == sz(s)) rev_upd();
		fix(); data.undo(); s.pop_back();
	}
private:	
	vector<pair<char, Upd>> s;
	int bot; /// bot of the stack: S[0..bot-1] is entirely B's.
	void inc_bot() {
		while (bot < sz(s) && s[bot].first == 'B') bot++;
	}
	void fix() {
		if (!sz(s) || s.back().first == 'A') return;
		inc_bot();
		deque<pair<char, Upd>> saveB, saveA;
		saveB.push_back(s.back());
		s.pop_back(), data.undo();
		while (sz(saveA) != sz(saveB) && sz(s) > bot) {
			(s.back().first=='A' ? saveA:saveB).push_front(s.back());
			s.pop_back(), data.undo();
		}
		for (auto& arr: {saveB, saveA})
		  for (const auto& u: arr) {
				s.push_back(u);
				data.accept(u.second);
			}
		inc_bot();
	}
	void rev_upd() {
		rep(i, 0, sz(s)) data.undo();
		for (int i = sz(s); i--; ) {
			data.accept(s[i].second);
			s[i].first = 'A';
		}
		reverse(all(s));
		bot = 0;
	}
};

void example() {
	using Upd = uint32_t; // can be a struct if more params needed.
  // data structure that maintains `and` of all contained numbers.
	struct DS {
		vector<uint32_t> st;
		DS(): st{(uint32_t)-1} {}
		// must have accept and undo
		void accept(Upd x) { st.push_back(st.back() & x); }
		void undo() { st.pop_back(); }
		// additional ops for any purpose
		uint get_ans() const { return st.back(); }
	};
	QueueUndo<DS, Upd> ds;
	ds.accept(1); ds.accept(3);  // also accept Upd
	ds.undo();
	// additional operations via .data
	assert(ds.data.get_ans() == 3);
}
/// vim: cc=63 ts=2 noet
