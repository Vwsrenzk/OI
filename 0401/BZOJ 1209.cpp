/*
这其实是一个三维凸包的板子..唯一需要调的就是精度..
话说网上的三维凸包的教程中判三点共线,四点共面的方法确实有些复杂..
经过idy博客的启发..其实在做凸包前对每个点加一遍噪音就行了。
如果一个点可以看见当前一个面，那么这个面就会被删除，其表现就是有向体积为负数
删除一个面的同时，删除这个面的三条边（也是有向的）最有扫一遍观察哪些边的反向边不存在
然后将他们的反向边与当前加的点一同构成新的凸包。
*/
#include <cmath>
#include <ctime>
#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <algorithm>

using namespace std;

const int N = 100 + 5;
const double Eps = 1e-8;

int Dcmp(double x) {
	if (fabs(x) < Eps) return 0;
	return x < 0 ? -1 : 1;
}

struct Point {
	double x, y, z;
	Point () {}
	Point (double x_, double y_, double z_) {
		x = x_, y = y_, z = z_;
	}
} P[N], p[N];

Point operator - (Point a, Point b) {
	return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

Point operator + (Point a, Point b) {
	return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator * (Point a, double x) {
	return Point(a.x * x, a.y * x, a.z * x);
}

Point operator / (Point a, double x) {
	return Point(a.x / x, a.y / x, a.z / x);
}

Point cross(Point a, Point b) {
	return Point(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Point cross(Point a, Point b, Point c) {
	return cross(b - a, c - a);
}

double Dot(Point a, Point b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

double Length(Point P) {
	return sqrt(Dot(P, P));
}

double rand01() {
	return (double)rand() / (double)RAND_MAX;
}

double randEps() {
	return (rand01() - 0.5) * Eps;
}

Point add_noise(Point P) {
	return Point(P.x + randEps(), P.y + randEps(), P.z + randEps());
}

struct Face {
	int v[3];
	Face() {}
	Face(int a, int b, int c) {
		v[0] = a, v[1] = b, v[2] = c;
	}
	Point Normal(Point *P) {
		return cross(P[v[0]], P[v[1]], P[v[2]]);
	}
	int CanSee(Point *P, int i) {
		return Dot(P[i] - P[v[0]], Normal(P)) > 0 ? 1 : 0;
	}
};

struct ConvexHull {
	vector <Face> cur;
	bool vis[N][N];
	void Increment(Point *P, int n) {
		vector <Face> next;
		memset(vis, 0, sizeof vis);
		for (int i = 1; i <= n; ++i) P[i] = add_noise(P[i]);
		cur.clear();
		cur.push_back(Face(1, 2, 3));
		cur.push_back(Face(3, 2, 1));
		for (int i = 4; i <= n; ++i) {
			next.clear();
			for (int j = 0; j < cur.size(); ++j) {
				Face F = cur[j];
				int res = F.CanSee(P, i);
				if (!res) next.push_back(F);
				for (int k = 0; k < 3; k++) vis[F.v[k]][F.v[(k + 1) % 3]] = res;
			}
			for (int j = 0; j < cur.size(); ++j)
				for (int k = 0; k < 3; k++) {
					int a = cur[j].v[k], b = cur[j].v[(k + 1) % 3];
					if (vis[a][b] && !vis[b][a]) next.push_back(Face(a, b, i));
				}
			cur = next;
		}
	}

	double Area(Point a, Point b, Point c) {
		return 0.5 * Length(cross(a, b, c));
	}

	double Area(Face F, Point *P) {
		return Area(P[F.v[0]], P[F.v[1]], P[F.v[2]]);
	}

	double Area(Point *P) {
		double Answer = 0;
		for (int i = 0; i < cur.size(); i++)
			Answer += Area(cur[i], P);
		return Answer;
	}
} Hull;

int n;

int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%lf%lf%lf", &P[i].x, &P[i].y, &P[i].z);
		p[i] = P[i];
	}
	Hull.Increment(p, n);
	printf("%.6lf", Hull.Area(P));
	return 0;
}
/*
4
0 0 0
1 0 0
0 1 0
0 0 1
*/