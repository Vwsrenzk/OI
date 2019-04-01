/*
求任意两点任意时刻的最近距离，实际上我们可以转化为枚举两个点，求这两个点的历史最近距离。
我们作出向量AB到达边界以后反弹得到的向量BA'。
首先考虑一直点A，和向量AB（即运动方向），如何得到点B的坐标，已知点A(px,py)和向量AB(vx,vy)，假设圆心为(0,0)（否则将点A平移），设时间为λ，那么列出方程：
(px+λvx)^2+(py+λcy)^2=r^2，解出λ，那么B(px+λvx,py+λvy)。
然后观察向量AB和向量BA’，发现它们相当于半径OB的法向量BC（即BC⊥OB）轴对称，然后我们考虑两个向量u，v，关于向量t轴对称，已知u，t，如何求出v
我们将向量AB平移得到BA''，可以发现BA'显然和BA''关于向量BC轴对称。
考虑两个向量的点积dot(p,q)=p.x*q.x+p.y*q.y，根据点积的定义为len(p)*len(q)*cos(p,q)，我们可以得到dot(BA',BD)=dot(BD,BA'')，而后者已知。
然后作A'D⊥BD，那么dot(BA',BC)=len(BA')*len(BC)*cos∠A'BC=len(BA')*len(BD)，不妨令len(BA')=len(BA'')（向量所以长度没有关系），那么可以得到len(BD)，那么可以用len(BD)和len(BC)的长度关系得到向量BD，倍长得到BB'。
将BA''平移得到A'B'，那么显然BA'+A'B'=BB'，因此BA'=BB'-A'B'，得到向量BA'，结束。
然后我们就可以用上述方法得到k个碰撞点，以及碰撞之后的向量了，即得到了一个粒子运动的折线。
考虑两个点x，y的最近距离，我们可以根据碰撞点把两条折线的距离转化为2*k对线段的距离。那么如何求一对线段的距离呢？
设线段为p->p+v，p为一个端点，p+v为另一个端点，那么v为线段的向量。那么线段上面任一点可以用p+λv表示，其中λ∈[0,1]。那么x和y的距离可以转化为二次函数(px.x+λvx.x-py.x-λvy.x)^2+(px.y+λvx.y-py.y-λvt.y)^2，然后就可以用二次函数求最小值了
*/
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;

const int N = 210;

struct point {
	double x, y;
} o;

struct line {
	point p, v;
} a[N][N];

int n, m;
double r;
double c[N][N];

point operator -(point u, point v) {
	point t; t.x = u.x - v.x; t.y = u.y - v.y;
	return t;
}

point operator +(point u, point v) {
	point t; t.x = u.x + v.x; t.y = u.y + v.y;
	return t;
}

point operator *(point u, double t) {
	point v; v.x = u.x * t; v.y = u.y * t;
	return v;
}

double dot(point u, point v) {
	return u.x * v.x + u.y * v.y;
}

double crs(point u, point v) {
	return u.x * v.y - u.y * v.x;
}

double solve(int x, int y, int k1, int k2, double t1, double t2) {
	point v1 = (a[x][k1].p - a[x][k1].v * c[x][k1]) - (a[y][k2].p - a[y][k2].v * c[y][k2]), v2 = a[x][k1].v - a[y][k2].v;
	double u = dot(v2, v2), v = 2 * dot(v1, v2), w = dot(v1, v1), t;
	if (!u) {
		if (v > 0) t = t1; else t = t2;
		return sqrt(v * t + w);
	} else {
		t = -v / (2 * u);
		if (t < t1) t = t1; if (t > t2) t = t2;
		return sqrt(u * t * t + v * t + w);
	}
}

int main() {
	scanf("%lf%lf%lf", &o.x, &o.y, &r);
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++) scanf("%lf%lf%lf%lf", &a[i][0].p.x, &a[i][0].p.y, &a[i][0].v.x, &a[i][0].v.y);
	double t, u, v, w, ans = 1e100; point nml, p, q;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++) {
			p = a[i][j - 1].p - o; q = a[i][j - 1].v;
			u = dot(q, q); v = 2 * dot(p, q); w = dot(p, p) - r * r;
			t = (sqrt(v * v - 4 * u * w) - v) / u / 2;
			c[i][j] = c[i][j - 1] + t;
			a[i][j].p = a[i][j - 1].p + a[i][j - 1].v * t;
			nml = a[i][j].p - o; swap(nml.x, nml.y); nml.x = -nml.x;
			a[i][j].v = nml * (dot(nml, a[i][j - 1].v) / dot(nml, nml) * 2) - a[i][j - 1].v;
		}
	for (int i = 1; i < n; i++)
		for (int j = i + 1; j <= n; j++) {
			int k1 = 0, k2 = 0;
			while (k1 < m && k2 < m) {
				ans = min(ans, solve(i, j, k1, k2, max(c[i][k1], c[j][k2]), min(c[i][k1 + 1], c[j][k2 + 1])));
				if (c[i][k1 + 1] < c[j][k2 + 1]) k1++; else k2++;
			}	
		}
	printf("%.3f\n", ans);
	return 0;
}
/*
0 0 10
2 10
0 -5 0 1
5 0 1 0
*/