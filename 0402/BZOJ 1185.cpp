/*
最小的矩形一定存在一条边在凸包上，那么枚举这条边，我们还差三个点，即距离当前边的最远点，以及做这条边的垂线的最靠左和最靠右的两个点。
通过这个方法，似乎可以求解已知所有夹角的n边形覆盖，只需要旋转卡壳的时候依次考虑每个点，而每个点是否能否移动到下个点的条件与夹角相关，而夹角的信息可以通过点积得到。
然后这题就是卡精度卡精度卡精度之类的
 */
#include <cmath>
#include <cstdio>
#include <iostream>
#include <algorithm>

using namespace std;

#define double long double

const int MAX = 50050;
const double eps = 1e-10;
const double Pi = acos(-1);

struct Point {
	double x, y, ang;
};

bool operator < (Point a, Point b) {
	return (a.ang != b.ang) ? a.ang < b.ang : a.x < b.x;
}

Point operator + (Point a, Point b) {
	return (Point) {
		a.x + b.x, a.y + b.y
	};
}

Point operator - (Point a, Point b) {
	return (Point) {
		a.x - b.x, a.y - b.y
	};
}
Point operator * (Point a, double b) {
	return (Point) {
		a.x *b, a.y *b
	};
}

Point operator/(Point a, double b) {
	return (Point) {
		a.x / b, a.y / b
	};
}

double operator*(Point a, Point b) {
	return a.x * b.x + a.y * b.y;
}

double Cross(Point a, Point b) {
	return a.x * b.y - a.y * b.x;
}

double Len(Point a) {
	return sqrt(a.x * a.x + a.y * a.y);
}

double Dis(Point a, Point b) {
	return Len(a - b);
}

Point Rotate(Point p, double a) {
	double c = cos(a), s = sin(a);
	return (Point) {
		p.x *c - p.y *s, p.x *s + p.y *c
	};
}

int top;
Point S[MAX], Ans[10];

void Graham(Point *p, int n) {
	int pos = 1;
	for(int i = 2; i <= n; ++i)
		if(p[i].x < p[pos].x || (p[i].x == p[pos].x && p[i].y < p[pos].y)) pos = i;
	swap(p[1], p[pos]);
	for(int i = 2; i <= n; ++i)p[i].ang = atan2(p[i].y - p[1].y, p[i].x - p[1].x);
	sort(&p[2], &p[n + 1]);
	S[++top] = p[1]; S[++top] = p[2];
	for(int i = 3; i <= n; ++i) {
		while(top > 2 && Cross(p[i] - S[top], p[i] - S[top - 1]) >= 0) --top;
		S[++top] = p[i];
	}
}

struct Line {
	Point a, v;
};

Point Intersection(Line a, Line b) {
	Point c = b.a - a.a;
	double t = Cross(b.v, c) / Cross(b.v, a.v);
	return a.a + a.v * t;
}

int n;
double ans = 1e18;
Point p[MAX], tmp[5];

void ScanLine(int n) {
	S[n + 1] = S[1]; S[0] = S[n];
	for(int i = 1, j1 = 3, j2 = 3, j3 = n; i <= n; ++i) {
		if(i == 1)while((S[i] - S[i + 1]) * (S[j3 - 1] - S[j3]) > 0)j3 = (j3 == 1) ? n : j3 - 1;
		while(Cross(S[j1] - S[i], S[j1] - S[i + 1]) <= Cross(S[j1 + 1] - S[i], S[j1 + 1] - S[i + 1]))j1 = (j1 == n) ? 1 : j1 + 1;
		while((S[i + 1] - S[i]) * (S[j2 + 1] - S[j2]) > 0)j2 = (j2 == n) ? 1 : j2 + 1;
		while((S[i + 1] - S[i]) * (S[j3 + 1] - S[j3]) < 0)j3 = (j3 == n) ? 1 : j3 + 1;
		Line l0 = (Line) {
			S[i], S[i + 1] - S[i]
		};

		Line l1 = (Line) {
			S[j1], S[i] - S[i + 1]
		};

		Line l2 = (Line) {
			S[j2], Rotate(S[i + 1] - S[i], Pi / 2)
		};

		Line l3 = (Line) {
			S[j3], Rotate(S[i] - S[i + 1], Pi / 2)
		};

		tmp[1] = Intersection(l0, l2);
		tmp[2] = Intersection(l2, l1);
		tmp[3] = Intersection(l1, l3);
		tmp[4] = Intersection(l3, l0);
		double area = Dis(tmp[1], tmp[2]) * Dis(tmp[2], tmp[3]);
		if(area < ans) ans = area, Ans[1] = tmp[1], Ans[2] = tmp[2], Ans[3] = tmp[3], Ans[4] = tmp[4];
	}
}

int main() {
	scanf("%d", &n);
	for(int i = 1; i <= n; ++i) scanf("%Lf%Lf", &p[i].x, &p[i].y);
	for(int i = 1; i <= n; ++i) p[i].x += eps, p[i].y -= eps;
	Graham(p, n);
	ScanLine(top);
	printf("%.5Lf\n", ans);
	Ans[5] = Ans[1], Ans[6] = Ans[2], Ans[7] = Ans[3], Ans[8] = Ans[4];
	int pos = 1;
	for(int i = 2; i <= 4; ++i)
		if(Ans[i].y < Ans[pos].y || (Ans[i].y == Ans[pos].y && Ans[i].x <= Ans[pos].x)) pos = i;
	for(int i = pos; i <= pos + 3; ++i)printf("%.5Lf %.5Lf\n", Ans[i].x + 100 * eps, Ans[i].y + 100 * eps);
	return 0;
}
/*
6 1.0 3.00000
1 4.00000
2.0000 1
3 0.0000
3.00000 6
6.0 3.0
*/