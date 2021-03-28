/**
 * Author: various (see source)
 * Date: 2009-10-30
 * License: CC-BY-SA-4.0 
 * Source: cp-algorithms.com (https://github.com/e-maxx-eng/e-maxx-eng/blob/master/src/geometry/convex_hull_trick.md)
 * Description: Act like line container but for a given range.
 * Works well with limited integer range.
 * Time: Both operations are $O(\log N)$.
 * Status: Trusted
 */
#pragma once
typedef int ftype;
typedef complex<ftype> point;
#define x real
#define y imag
const int maxn = 2e5;
ftype dot(point a, point b) { return (conj(a) * b).x(); }
ftype f(point a,  ftype x) { return dot(a, {x, 1}); }
point line[4 * maxn];
void add_line(point nw, int v = 1, int l = 0, int r = maxn) {
	int m = (l + r) / 2;
	bool lef = f(nw, l) < f(line[v], l);
	bool mid = f(nw, m) < f(line[v], m);
	if(mid) {
		swap(line[v], nw);
	}
	if(r - l == 1) {
		return;
	} else if(lef != mid) {
		add_line(nw, 2 * v, l, m);
	} else {
		add_line(nw, 2 * v + 1, m, r);
	}
}
int get(int x, int v = 1, int l = 0, int r = maxn) {
	int m = (l + r) / 2;
	if(r - l == 1) {
		return f(line[v], x);
	} else if(x < m) {
		return min(f(line[v], x), get(x, 2 * v, l, m));
	} else {
		return min(f(line[v], x), get(x, 2 * v + 1, m, r));
	}
}

/// vim: cc=63 ts=2
