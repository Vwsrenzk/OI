/*
旋(wo)转(bu)卡(hui)壳(de)裸题。显然四个点一定在凸包上。然后枚举四边形的对角线O(N^2)，然后可以发现可以在凸包上O(N)找到在对角线两侧的离对角线最远的点，用这四个点更新答案。
考虑优化时间，在固定对角线的一个定点i，枚举另一个定点j时，考虑两侧最优解x，y，但j沿着凸包逆时针旋转的时候，x，y也沿着凸包逆时针旋转。因此我们只需要找到一开始的x，y，然后就可以O(N)扫过去了。时间复杂度O(N^2)。
*/
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;

const int N = 2010;

struct point {
	double x, y;
} a[N], q[N]; 

int n, tp;

point operator -(point u, point v) {
	point t; t.x = u.x - v.x; t.y = u.y - v.y; return t;
}

double crs(point u, point v) {
	return u.x * v.y - u.y * v.x;
}

double dist(point u, point v) {
	return sqrt((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
}

bool cmp(point u, point v) {
	double t = crs(u - a[1], v - a[1]);
	return t > 0 || t == 0 && dist(u, a[1]) < dist(v, a[1]);
}

int main() {
	scanf("%d", &n); int i, j;
	for (i = 1; i <= n; i++) scanf("%lf%lf", &a[i].x, &a[i].y);
	for (i = 2; i <= n; i++)
		if (a[i].y < a[1].y || a[i].y == a[1].y && a[i].x < a[1].x) swap(a[1], a[i]);
	sort(a + 2, a + n + 1, cmp);
	q[1] = a[1]; q[2] = a[2]; 
	int tp = 2;
	for (i = 3; i <= n; i++) {
		while (tp > 1 && crs(a[i] - q[tp - 1], q[tp] - q[tp - 1]) >= 0) tp--;
		q[++tp] = a[i];
	}
	q[tp + 1] = q[1]; 
	int x, y; 
	double ans = 0;
	for (i = 1; i <= tp; i++) {
		x = i % tp + 1; y = (i + 2) % tp + 1;
		for (j = i + 2; j <= tp; j++) {
			while (x % tp + 1 != j && crs(q[x + 1] - q[i], q[j] - q[i]) > crs(q[x] - q[i], q[j] - q[i]))
				x = x % tp + 1;
			while (y % tp + 1 != i && crs(q[j] - q[i], q[y + 1] - q[i]) > crs(q[j] - q[i], q[y] - q[i]))
				y = y % tp + 1;
			ans = max(ans, crs(q[x] - q[i], q[j] - q[i]) + crs(q[j] - q[i], q[y] - q[i]));
		}
	}
	printf("%.3f\n", ans / 2);
	return 0;
}
/*
5
0 0
1 0
1 1
0 1
0.5 0.5
*/