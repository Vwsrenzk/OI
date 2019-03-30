/*
将包含关系建树,方法是将每个图形拆成上半边和下半边，从左往右扫描线，用Splay从下到上维护扫描线上所有图形。
每次加入一个新的图形x的时候，看看它下方第一个图形y，如果y是上半边，那么x的父亲就是y，否则是y的父亲。用同样的方法可以完成点定位。
然后每次相当于查询两点间的异或和，用树状数组维护dfs序即可。
时间复杂度为 O((n+m)logn)。
 */
#include <cmath>
#include <cstdio>
#include <algorithm>

using namespace std;

const int N = 100010;
const int M = N << 1;
const double inf = 1e20;
const double eps = 1e-9;

int n, m, x, y, ce, cb, ans;
int val[N], q[N][3]; 
double X;

struct Shape {
	int n;
	bool type;
	double x, y, r;
	double a[35][2];

	void read() {
		char t[5];
		scanf("%s", t);
		if(t[0] == 'C') type = 0, scanf("%lf%lf%lf", &x, &y, &r);
		else {
			type = 1;
			scanf("%d", &n);
			for(int i = 0; i < n; ++i) scanf("%lf%lf", &a[i][0], &a[i][1]);
			a[n][0] = a[0][0], a[n][1] = a[0][1];
		}
	}

	double getl() {
		if(!type) return x - r;
		double ret = inf;
		for(int i = 0; i < n; ++i)ret = min(ret, a[i][0]);
		return ret;
	}
	double getr() {
		if(!type) return x + r;
		double ret = -inf;
		for(int i = 0; i < n; ++i) ret = max(ret, a[i][0]);
		return ret;
	}

	double getd(double o) {
		if(!type) return y - sqrt(max(r * r - (o - x) * (o - x), 0.0));
		double ret = inf;
		for(int i = 0; i < n; ++i) {
			double A = a[i][0], B = a[i][1], C = a[i + 1][0], D = a[i + 1][1];
			if(A > C)swap(A, C), swap(B, D);
			if(o < A - eps || o > C + eps) continue;
			if(o < A + eps) {
				ret = min(ret, B);
				continue;
			}
			if(o > C - eps) {
				ret = min(ret, D);
				continue;
			}
			ret = min(ret, B + (D - B) / (C - A) * (o - A));
		}
		return ret;
	}

	double getu(double o) {
		if(!type) return y + sqrt(max(r * r - (o - x) * (o - x), 0.0));
		double ret = -inf;
		for(int i = 0; i < n; ++i) {
			double A = a[i][0], B = a[i][1], C = a[i + 1][0], D = a[i + 1][1];
			if(A > C)swap(A, C), swap(B, D);
			if(o < A - eps || o > C + eps)continue;
			if(o < A + eps) {
				ret = max(ret, B);
				continue;
			}
			if(o > C - eps) {
				ret = max(ret, D);
				continue;
			}
			ret = max(ret, B + (D - B) / (C - A) * (o - A));
		}
		return ret;
	}
} a[N];

struct E {
	double x;
	int y, t;
	E() {}
	E(double _x, int _y, int _t) {
		x = _x, y = _y, t = _t;
	}
} e[N >> 2];

inline bool cmp(const E &a, const E &b) {
	if(a.t < 2 && b.t < 2 && a.y == b.y)return a.t < b.t;
	return a.x < b.x;
}

struct P {
	double x, y;
} b[M];

int root, L, R, ed, dfn;
int from[M], f[M], fa[N], g[N], v[M], nxt[M], st[N], en[N], bit[M];
int son[M][2];
inline void rotate(int x) {
	int y = f[x], w = son[y][1] == x;
	son[y][w] = son[x][w ^ 1];
	if(son[x][w ^ 1]) f[son[x][w ^ 1]] = y;
	if(f[y]) {
		int z = f[y];
		if(son[z][0] == y) son[z][0] = x;
		if(son[z][1] == y) son[z][1] = x;
	}
	f[x] = f[y]; son[x][w ^ 1] = y; f[y] = x;
}
inline void splay(int x, int w) {
	while(f[x] != w) {
		int y = f[x];
		if(f[y] != w) {
			if((son[f[y]][0] == y) ^ (son[y][0] == x)) rotate(x);
			else rotate(y);
		}
		rotate(x);
	}
	if(!w) root = x;
}

inline bool bigger(int x, int y) {
	if(y == L) return 1;
	if(y == R) return 0;
	if(x + n == y || x == y + n) return x > y;
	double A = x <= n ? a[x].getd(X) : a[x - n].getu(X), B = y <= n ? a[y].getd(X) : a[y - n].getu(X);
	return A > B;
}

inline bool biggerp(double x, int y) {
	if(y == L) return 1;
	if(y == R) return 0;
	double B = y <= n ? a[y].getd(X) : a[y - n].getu(X);
	return x > B;
}

void ins(int x, int y) {
	int w = bigger(y, x);
	if(!son[x][w]) {
		son[x][w] = y;
		f[y] = x;
		return;
	}
	ins(son[x][w], y);
}

int ask(int x, double y) {
	if(!x) return 0;
	if(biggerp(y, x)) {
		int t = ask(son[x][1], y);
		return t ? t : x;
	}
	return ask(son[x][0], y);
}

inline void add(int x) {
	ins(root, x);
	ins(root, x + n);
	splay(x, 0);
	int y;
	for(y = son[x][0]; son[y][1]; y = son[y][1]);
	if(y < L) fa[x] = y <= n ? y : fa[y - n];
	splay(y, 0);
}

inline void del(int x) {
	int y;
	splay(x, 0);
	for(y = son[x][0]; son[y][1]; y = son[y][1]);
	splay(y, x);
	son[y][1] = son[x][1];
	f[son[x][1]] = y;
	f[root = y] = 0;
}

inline void getpos(int x) {
	int y = ask(root, b[x].y);
	if(y < L) from[x] = y <= n ? y : fa[y - n];
	splay(y, 0);
}

void dfs(int x) {
	st[x] = ++dfn;
	for(int i = g[x]; i; i = nxt[i]) dfs(i);
	en[x] = ++dfn;
}

inline void modify(int x, int y) {
	for(; x <= dfn; x += x & -x) bit[x] ^= y;
}

inline int sum(int x) {
	int t = 0;
	for(; x; x -= x & -x) t ^= bit[x];
	return t;
}

int main() {
	scanf("%d%d", &n, &m);
	for(int i = 1; i <= n; ++i) {
		a[i].read();
		scanf("%d", &val[i]);
		e[++ce] = E(a[i].getl(), i, 0);
		e[++ce] = E(a[i].getr(), i, 1);
	}
	for(int i = 1; i <= m; ++i) {
		char op[5];
		scanf("%s", op);
		if(op[0] == 'Q') {
			cb++;
			scanf("%lf%lf", &b[cb].x, &b[cb].y);
			e[++ce] = E(b[cb].x, cb, 2);
			cb++;
			scanf("%lf%lf", &b[cb].x, &b[cb].y);
			e[++ce] = E(b[cb].x, cb, 2);
			q[i][1] = cb - 1, q[i][2] = cb;
		} else {
			q[i][0] = 1;
			scanf("%d%d", &q[i][1], &q[i][2]);
		}
	}
	sort(e + 1, e + ce + 1, cmp);
	L = n * 2 + 1; R = L + 1;
	son[L][1] = R, f[R] = L;
	root = L;
	for(int i = 1; i <= ce; ++i) {
		X = e[i].x;
		if(!e[i].t) add(e[i].y);
		else if(e[i].t == 1) del(e[i].y), del(e[i].y + n);
		else getpos(e[i].y);
	}
	for(int i = 1; i <= n; ++i) if(fa[i]) nxt[i] = g[fa[i]], g[fa[i]] = i;
	for(int i = 1; i <= n; ++i) if(!fa[i]) dfs(i);
	for(int i = 1; i <= n; ++i) modify(st[i], val[i]), modify(en[i], val[i]);
	for(int i = 1; i <= m; ++i) if(q[i][0]) {
			x = q[i][1], y = q[i][2] ^ val[x];
			val[x] = q[i][2];
			modify(st[x], y), modify(en[x], y);
		} else printf("%d\n", ans ^= sum(st[from[q[i][1]]]) ^ sum(st[from[q[i][2]]]));
	return 0;
}
/*
2 4
C 0 0 2 1
P 4 -1 -1 -1 1 1 1 1 -1 2
Q -2 -2 2 2
Q -1.5 0 0.0 0.0
C 1 1005
Q -1.5 0 0.0 0.0
 */