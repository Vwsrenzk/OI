/*
首先找一个初始凸包内部的点作为极点，然后按极角放入set
注意旋转超过180度的情况要特判。
每加入一个点，就在set中找到它的前驱和后继（注意是环形的）
先判断该点是否在已有凸包内，再判断它的前驱和后继是否在新的凸包上
不断删点直到满足为止，同时维护一下面积。
 */
#include<set>
#include<cmath>
#include<ctime>
#include<vector>
#include<cstdio>
#include<string>
#include<cstring>
#include<iostream>
#include<algorithm>

using namespace std;

typedef long long ll;

inline int read() {
	int i = 0, f = 1; char c;
	for(c = getchar(); (c != '-') && (c < '0' || c > '9'); c = getchar());
	if(c == '-')f = -1, c = getchar();
	for(; c >= '0' && c <= '9'; c = getchar())i = (i << 3) + (i << 1) + c - '0';
	return i * f;
}

const int N = 100000;

struct point {
	double x, y;
	point() {}
	point(double _x, double _y): 
		x(_x), y(_y) {}
	
	inline friend point operator + (const point &a, const point &b) {
		return point(a.x + b.x, a.y + b.y);
	}
	
	inline friend point operator - (const point &a, const point &b) {
		return point(a.x - b.x, a.y - b.y);
	}
	
	inline friend point operator * (const point &a, const double &b) {
		return point(a.x * b, a.y * b);
	}
	
	inline friend double operator * (const point &a, const point &b) {
		return a.x * b.y - a.y * b.x;
	}

	inline double dis() {
		return x * x + y * y;
	}
} O, p[4];

inline bool operator < (const point &a, const point &b) {
	point a1 = a - O, b1 = b - O;
	if(a1.y * b1.y < 0)return a1.y < b1.y;
	if(a1.y == 0 && b1.y == 0 && a1.x * b1.x < 0)return a1.x > b1.x;
	if(a1.x == 0 && b1.x == 0 && a1.y * b1.y < 0)return a1.y < b1.y;
	double det = a1 * b1;
	if(det)return det > 0;
	return a1.dis() > b1.dis();
}

int n = 3, m;
ll ans;

set<point>S;

point getpre(point t) {
	point res;
	set<point>::iterator it = S.find(t);
	if(it == S.begin())
		it = S.end();
	res = *--it;
	return res;
}

point getsuf(point t) {
	point res;
	set<point>::iterator it = S.find(t);
	if(++it == S.end())it = S.begin();
	res = *it;
	return res;
}

void Delete(point t) {
	set<point>::iterator it = S.find(t);
	S.erase(it);
}

ll area(point a, point b) {
	a = a - p[1], b = b - p[1];
	return abs((ll)a.x * (ll)b.y - (ll)a.y * (ll)b.x);
}

int main() {
	//freopen("lx.in","r",stdin);
	//freopen("lx.out","w",stdout);
	for(int i = 1; i <= 3; i++) p[i].x = read(), p[i].y = read();
	O = (p[1] + (p[2] + p[3]) * 0.5) * 0.5;
	ans = area(p[2], p[3]);
	for(int i = 1; i <= 3; i++) S.insert(p[i]);
	m = read();
	while(m--) {
		point t, pre, suf, a;
		t.x = read(), t.y = read();
		if(S.count(t)) {
			cout << ans << '\n';
			continue;
		}
		S.insert(t);
		/*if(m==0)
		{
		    set<point>::iterator it;
		    for(it=S.begin();it!=S.end();++it)
		        cout<<it->x<<" "<<it->y<<'\n';
		    return 0;
		}*/
		pre = getpre(t), suf = getsuf(t);
		if((t - pre) * (suf - t) < 0) {
			Delete(t);
			cout << ans << '\n';
			continue;
		}
		++n;
		ans -= area(pre, suf);
		a = getpre(pre);
		while(n > 3 && (pre - a) * (t - pre) <= 0) {
			ans -= area(pre, a);
			Delete(pre); n--;
			pre = a, a = getpre(a);
		}
		a = getsuf(suf);
		while(n > 3 && (suf - t) * (a - suf) <= 0) {
			ans -= area(suf, a);
			Delete(suf); n--;
			suf = a, a = getsuf(a);
		}
		ans += area(pre, t) + area(suf, t);
		cout << ans << '\n';
	}
	return 0;
}
/*
0 0 0 2 2 0
3
2 2
1 1
2 4
 */