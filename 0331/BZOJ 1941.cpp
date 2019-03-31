/*
自己yy了KDtree的模板..
枚举每个点找最近点，最远点更新答案
还是个板子题...但是KDtree是真的难...
*/
#include <cmath>
#include <cstdio>
#include <complex>
#include <cstring>
#include <iostream>
#include <algorithm>
#define ll long long
#define inf 1000000000
#define mod 1000000007
#define sqr(x) x*x

using namespace std;

int read() {
	int x = 0, f = 1; char ch = getchar();
	while(ch < '0' || ch > '9') {
		if(ch == '-')f = -1;
		ch = getchar();
	}
	while(ch >= '0' && ch <= '9') {
		x = x * 10 + ch - '0';
		ch = getchar();
	}
	return x * f;
}

int x[500005], y[500005];
int n, F, rt, ans = inf;

struct P {
	int d[2], mn[2], mx[2], l, r;
	int &operator[](int x) {
		return d[x];
	}
	friend bool operator<(P a, P b) {
		return a[F] < b[F];
	}
	friend int dis(P a, P b) {
		return abs(a[1] - b[1]) + abs(a[0] - b[0]);
	}
} p[500005];
struct kdtree {
	P t[500005], T;
	int ans;
	void update(int k) {
		int l = t[k].l, r = t[k].r;
		for(int i = 0; i < 2; i++) {
			t[k].mn[i] = t[k].mx[i] = t[k][i];
			if(l)t[k].mn[i] = min(t[k].mn[i], t[l].mn[i]);
			if(r)t[k].mn[i] = min(t[k].mn[i], t[r].mn[i]);
			if(l)t[k].mx[i] = max(t[k].mx[i], t[l].mx[i]);
			if(r)t[k].mx[i] = max(t[k].mx[i], t[r].mx[i]);
		}
	}
	int build(int l, int r, int now) {
		F = now;
		int mid = (l + r) >> 1;
		nth_element(p + l, p + mid, p + r + 1);
		t[mid] = p[mid];
		for(int i = 0; i < 2; i++)
			t[mid].mn[i] = t[mid].mx[i] = t[mid][i];
		if(l < mid)t[mid].l = build(l, mid - 1, now ^ 1);
		if(r > mid)t[mid].r = build(mid + 1, r, now ^ 1);
		update(mid);
		return mid;
	}
	int getmn(P a) {
		int ans = 0;
		for(int i = 0; i < 2; i++) {
			ans += max(T[i] - a.mx[i], 0);
			ans += max(a.mn[i] - T[i], 0);
		}
		return ans;
	}
	int getmx(P a) {
		int ans = 0;
		for(int i = 0; i < 2; i++)
			ans += max(abs(T[i] - a.mx[i]), abs(T[i] - a.mn[i]));
		return ans;
	}
	void querymx(int k) {
		ans = max(ans, dis(t[k], T));
		int l = t[k].l, r = t[k].r, dl = -inf, dr = -inf;
		if(l)dl = getmx(t[l]); if(r)dr = getmx(t[r]);
		if(dl > dr) {
			if(dl > ans)querymx(l);
			if(dr > ans)querymx(r);
		} else {
			if(dr > ans)querymx(r);
			if(dl > ans)querymx(l);
		}
	}
	void querymn(int k) {
		int tmp = dis(t[k], T);
		if(tmp)ans = min(ans, tmp);
		int l = t[k].l, r = t[k].r, dl = inf, dr = inf;
		if(l)dl = getmn(t[l]); if(r)dr = getmn(t[r]);
		if(dl < dr) {
			if(dl < ans)querymn(l);
			if(dr < ans)querymn(r);
		} else {
			if(dr < ans)querymn(r);
			if(dl < ans)querymn(l);
		}
	}
	int query(int f, int x, int y) {
		T[0] = x; T[1] = y;
		if(f == 0)ans = inf, querymn(rt);
		else ans = -inf, querymx(rt);
		return ans;
	}
} kdtree;
int main() {
	n = read();
	for(int i = 1; i <= n; i++) {
		x[i] = read(), y[i] = read();
		p[i][0] = x[i]; p[i][1] = y[i];
	}
	rt = kdtree.build(1, n, 0);
	for(int i = 1; i <= n; i++) {
		int mn = kdtree.query(0, x[i], y[i]), mx = kdtree.query(1, x[i], y[i]);
		ans = min(ans, mx - mn);
	}
	printf("%d\n", ans);
	return 0;
}
/*
4
0 0
1 0
0 1
1 1
*/