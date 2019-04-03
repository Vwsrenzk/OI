/*
首先答案具有单调性，可以二分。
然后贪心一波，我们肯定要捡着最短路去走，然后我们增广出了一条路，因为时间时固定的，所以我们能通过的人数已经确定了。
就是(T-dis+1)*min(l)前面的是第一波人到终点到结束的时间， minl表示路径权值的最小值。
然后我们就一直重复这个过程，发现这个过程和EK费用流完全一样。	
 */
#include <queue>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

typedef long long ll;

const int N = 1010;
const int M = 20010;
const ll inf = 1e18;

ll num;
ll fl[N];
int n, m, k, tot;
int dis[N], head[N], pre[N];
bool vis[N];

const int maxn = 100010;
const int maxm = 200010;

queue <int> q;

inline int read() {
	int x = 0, f = 0;
	char c = getchar(); 
	while(!isdigit(c)) if(c == '-') f = 1, c = getchar();
	while(isdigit(c)) x = (x << 1) + (x << 3) + (c ^ 48), c = getchar();
	return f * x;
}

struct edge {
	int n, to, l, f;
} e[M];

inline void add(int u, int v, int l, int f) {
	e[++tot].n = head[u]; e[tot].to = v; 
	head[u] = tot; 
	e[tot].l = l; 
	e[tot].f = f;
	e[++tot].n = head[v]; 
	e[tot].to = u; 
	head[v] = tot; 
	e[tot].l = 0; 
	e[tot].f = -f;
}

inline bool spfa(int s, int t) {
	memset(dis, 0x3f, sizeof(dis));
	dis[s] = 0; q.push(s); fl[s] = inf;
	while(!q.empty()) {
		int u = q.front(); 
		q.pop(); 
		vis[u] = 0;
		for(int i = head[u]; i; i = e[i].n) {
			int v = e[i].to;
			if(e[i].l && dis[v] > dis[u] + e[i].f) {
				dis[v] = dis[u] + e[i].f; pre[v] = i; 
				fl[v] = min(fl[u], 1ll * e[i].l);
				if(!vis[v]) {
					vis[v] = 1;
					q.push(v);
				}
			}
		}
	}
	return dis[t] != 0x3f3f3f3f;
}

struct node {
	int u, v, w;
} a[M];

inline void calc(int s, int t, ll mid) {
	int x = t;
	while(x != s) {
		int i = pre[x];
		e[i].l -= fl[t]; 
		e[i ^ 1].l += fl[t]; 
		x = e[i ^ 1].to;
	}
	num += max(0ll, 1ll * (mid - dis[t] + 1) * fl[t]);
}

inline bool check(ll mid) {
	memset(head, 0, sizeof(head)); 
	tot = 1, num = 0;
	for (int i = 1; i <= m; ++i) add(a[i].u, a[i].v, a[i].w, 1);
	while (spfa(0, n - 1)) calc(0, n - 1, mid);
	return num >= k;
}

inline add(int add, p)

int main() {
	while (scanf("%d%d%d", &n, &m, &k) != EOF) {
		for (int i = 1; i <= m; ++i) a[i].u = read(), a[i].v = read(), a[i].w = read();
		ll l = 0, r = 1e12, ans = -1;
		while (l <= r) {
			ll mid = (l + r) >> 1;
			if (check(mid)) {
				ans = mid; 
				r = mid - 1;
			} else l = mid + 1;
		}
		if (~ans) printf("%d\n", ans); 
		else printf("No solution\n");
	}
	return 0;
}
/*
5 6 4 
0 1 2
0 3 1
1 2 1
2 3 1
1 4 1
3 4 2
 */