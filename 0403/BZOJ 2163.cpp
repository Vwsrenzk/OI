/*
这个题乍一看好像没有什么思路。
最多使用w次？感觉像网络流...
我们可以先把模型转换为计算最大使用传送门的次数。
这不就是最大流嘛...
拆点没的说...
对于每个i，拆成i和i+n，然后从S到i连一条流量为要求达到访问次数的边，i+n到T也连一条容量为要求达到访问次数的边。
然后对于每个传送门，由u到v+n连容量为传送门使用次数w的边。
跑出最大流就是最大使用传送门的次数。
然后用所有的点要求达到的访问次数的总和减去最大流就好了
 */
#include <queue>
#include <cstdio>
#include <iostream>
#include <algorithm>

using namespace std;

inline int read() {
	int f = 0, x = 1; 
	char c = 0;
	while(c < '0' || c > '9') if(c == '-') x = -1, c = getchar();
	while(c >= '0' && c <= '9') f = f * 10 + c - '0', c = getchar();
	return f * x;
}

const int maxn = 100010;
const int INF = 0x3f3f3f3f;

int n, m, s, t, c = 2, sum = 0;
int head[maxn], deep[maxn];

struct Graph {
	int next, to, w;
} a[maxn << 1];


inline void add(int u, int v, int w) {
	a[c].to = v; a[c].w = w; a[c].next = head[u]; head[u] = c++;
	a[c].to = u; a[c].w = 0; a[c].next = head[v]; head[v] = c++;
}

bool bfs() {
	int u, v;
	queue <int> q;
	for(int i = s; i <= t; i++) deep[i] = 0;
	deep[s] = 1;
	q.push(s);
	while(!q.empty()) {
		u = q.front();
		q.pop();
		for(int i = head[u]; i; i = a[i].next) {
			v = a[i].to;
			if(a[i].w && !deep[v]) {
				deep[v] = deep[u] + 1;
				if(v == t) return true;
				q.push(v);
			}
		}
	}
	return false;
}

int dfs(int x, int limit) {
	if(x == t) return limit;
	int v, sum, cost = 0;
	for(int i = head[x]; i; i = a[i].next) {
		v = a[i].to;
		if(a[i].w && deep[v] == deep[x] + 1) {
			sum = dfs(v, min(a[i].w, limit - cost));
			if(sum > 0) {
				a[i].w -= sum;
				a[i ^ 1].w += sum;
				cost += sum;
				if(cost == limit) break;
			} else deep[v] = -1;
		}
	}
	return cost;
}

int dinic() {
	int ans = 0;
	while(bfs()) ans += dfs(s, INF);
	return ans;
}

void work() {
	int ans;
	ans = sum - dinic();
	printf("%d\n", ans);
}

void init() {
	int u, v, w;
	n = read(), m = read();
	s = 0; t = (n << 1) + 1;
	for(int i = 1; i <= n; i++) {
		w = read();
		sum += w;
		add(s, i, w); add(i + n, t, w);
	}
	for(int i = 1; i <= m; i++) {
		u = read(), v = read(), w = read();
		add(u, v + n, w);
	}
}

int main() {
	init();
	work();
	return 0;
}
/*
4 3
5 5 5 5
1 2 1
3 2 1
3 4 1
 */	