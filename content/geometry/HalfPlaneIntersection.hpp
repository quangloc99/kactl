/**
 * Author: Tran Quang Loc
 * Date: 2021-03-27
 * License: CC0
 * Source: Me
 * Description: find half plane intersection by maintaining
 *   the set of both back and front of the convex hull.
 *   Works on both integer and double.
 * Usage:
 *  using HP = HalfPlane<long double>; 
 *  vector<HP> planes = {...};
 *  auto inter = find_halfplane_intersection(planes);
 *  if (inter) { use *inter here; }
 * Time: O(n\log n)
 * Status: Tested with timus 1062 and 1520
 */
#pragma once
const long double eps = 1e-10;
template<class T> int sgn(T x) { return x < 0 ? -1 : x > 0; } 
template<class T> struct HalfPlane {
	using num_t = T;
	T a, b, c;
	T operator()(T x, T y, T z = 1) const
		{ return a * x + b * y + c * z; }
	friend bool parallel(const HalfPlane& u, const HalfPlane& v)
		{ return abs(u.a * v.b - u.b * v.a) <= T(eps); }
	friend T cross(const HalfPlane& obj, const HalfPlane& u,
			const HalfPlane& v) {
		T z = u.a * v.b - u.b * v.a;  
		return obj((u.b * v.c - v.b * u.c) / z,
				(u.c * v.a - v.c * u.a) / z);  // for double  
		// return obj(u.b * v.c - v.b * u.c,
		//	   u.c * v.a - v.c * u.a, z) * sgn(z); // for int
	}
	friend int cross_norm_sgn(
					const HalfPlane& u, const HalfPlane& v)
	{ return sgn(u.a * v.b - u.b * v.a); }
};
template<class HP = HalfPlane<long long>>
optional<deque<HP>> find_halfplane_intersection(vector<HP> hps)
{ // we can also not sort this
	// E.g finding convex polygon intersection can be done in
	// O(n) with merge sort before calling this function.
	sort(all(hps), [&](const HP& u, const HP& v) {
			int qu = u.b > 0 or (abs(u.b) <= eps and u.a > 0);
			int qv = v.b > 0 or (abs(v.b) <= eps and v.a > 0);
			if (qu != qv) return qu < qv;
			return cross_norm_sgn(u, v) > 0;
	});
	using dq = deque<HP>;
	dq qu;
	bool empty = false;
	enum ParallelRes {NotParallel=0xf0, PopFir=1, PopSec=2};
	auto check_prl = [&](const HP& u, const HP& v) -> int {
		if (!parallel(u, v)) return NotParallel;
		HP p {-u.b, u.a, 0};
		auto x = cross(u, p, v);
		if (sgn(u.a) == sgn(v.a) and sgn(u.b) == sgn(v.b))
			return ((x <= 0) * PopSec) | ((x >= 0) * PopFir);
		if (x <= 0) empty = true;
		return 0;
	};
	auto needPopMid = [&](const HP& prv, const HP& mid,
			const HP& nxt) -> bool {
		if (parallel(prv, mid) or parallel(mid, nxt))
			return false;
		if (auto p = check_prl(prv, nxt); p != NotParallel)
			return false;
		auto x = cross(mid, prv, nxt);
		if (abs(x) <= eps) { 
			auto u = prv, v = nxt;
			if (cross_norm_sgn(u, v) > 0) swap(u, v);
			auto side_u = cross_norm_sgn(mid, u);
			auto side_v = cross_norm_sgn(mid, v);
			if (side_u == side_v) return false;
			if (side_u < 0) empty = true;
			return true;
		}
		bool mid_at_back = cross(prv, mid, nxt) < 0 and 
			cross(nxt, mid, prv) < 0;
		if (x > 0) return mid_at_back;
		if (mid_at_back) empty = true;
		return false;
	};
	for (auto& h: hps) {
		bool needed = true;
		auto cut_off = [&](auto end, auto pop) {
			while (needed and !empty and qu.size()) {
				auto& last = end(qu)[-1];
				if (auto p = check_prl(h, last); p != NotParallel) {
					if (p & PopSec) pop(qu);
					else if (p & PopFir) needed = false;
					else break;
				}
				else if (qu.size() > 1 and
						needPopMid(h, last, end(qu)[-2])) pop(qu);
				else break;
			}
		};
		cut_off(mem_fn(&dq::cend), mem_fn(&dq::pop_back));
		cut_off(mem_fn(&dq::crend), mem_fn(&dq::pop_front));
		if (qu.size() > 1 and needPopMid(qu.front(), h, qu.back()))
			needed = false;
		if (empty) return {};
		if (needed) qu.push_back(h);
	}
	if (qu.size() < 3) return qu;
	// (optional)
	// make the first open line be the starting for open-hull.
	auto cur = --qu.end(), prv = prev(cur), nxt = qu.begin();
	for (; nxt != qu.end(); prv = cur, cur = nxt++)
		if (parallel(*prv, *cur) or cross(*nxt, *cur, *prv) < 0)
			break;
	if (nxt != qu.end()) rotate(qu.begin(), cur, qu.end());
	return qu;
}
