/*
终于搞掉这题了，耗时一下午。
应该是splay维护序列题型的巅峰了吧。
这道题不能单纯的开点，因为会MLE，所以要用回收节点的过程。
说一下搞法吧：
1、插入，提取区间，递归建树
2、删除，提取区间，把删去的点用一个栈储存下来
3、修改，提取区间，打个标记
4、翻转，提取区间，打个标记
5、求和、最大连续子段和，维护一下就可以了。
WA点：
这道题不用开long long，简直是福利
没说数据范围，需要回收内存
标记稍微处理一下就可以了没什么
初始化节点注意把权值赋成-inf，虽然好像没用什么用
 */
#include <queue>
#include <cstdio>
#include <iostream>
#include <algorithm>

#define MAXN 500010
#define MAX 999999999

using namespace std;

queue<int> point;

int n, m, size = 1, root, val[MAXN];

struct node {
	int f, s, flag, set, son[2];
	int v, w, sum, suml, sumr;
} a[MAXN];

inline int read() {
	int date = 0, w = 1; char c = 0;
	while(c < '0' || c > '9') {
		if(c == '-')w = -1;
		c = getchar();
	}
	while(c >= '0' && c <= '9') {
		date = date * 10 + c - '0';
		c = getchar();
	}
	return date * w;
}

inline void clean(int rt) {
	a[rt].son[0] = a[rt].son[1] = a[rt].f = a[rt].s = a[rt].flag = a[rt].v = a[rt].w = 0;
	a[rt].sum = a[rt].suml = a[rt].sumr = -MAX;
}

inline void pushup(int rt) {
	if(!rt)return;
	a[rt].s = a[a[rt].son[0]].s + a[a[rt].son[1]].s + 1;
	a[rt].w = a[a[rt].son[0]].w + a[a[rt].son[1]].w + a[rt].v;
	a[rt].suml = max(a[a[rt].son[0]].suml, a[a[rt].son[0]].w + a[rt].v + max(0, a[a[rt].son[1]].suml));
	a[rt].sumr = max(a[a[rt].son[1]].sumr, a[a[rt].son[1]].w + a[rt].v + max(0, a[a[rt].son[0]].sumr));
	a[rt].sum = max(a[rt].v + max(0, a[a[rt].son[0]].sumr) + max(0, a[a[rt].son[1]].suml), max(a[a[rt].son[0]].sum, a[a[rt].son[1]].sum));
}

inline void pushdown(int rt) {
	if(!rt)return;
	if(a[rt].set != -MAX) {
		if(a[rt].son[0]) {
			a[a[rt].son[0]].set = a[a[rt].son[0]].v = a[rt].set;
			a[a[rt].son[0]].w = a[rt].set * a[a[rt].son[0]].s;
			a[a[rt].son[0]].suml = a[a[rt].son[0]].sumr = a[a[rt].son[0]].sum = max(a[a[rt].son[0]].set, a[a[rt].son[0]].w);
		}
		if(a[rt].son[1]) {
			a[a[rt].son[1]].set = a[a[rt].son[1]].v = a[rt].set;
			a[a[rt].son[1]].w = a[rt].set * a[a[rt].son[1]].s;
			a[a[rt].son[1]].suml = a[a[rt].son[1]].sumr = a[a[rt].son[1]].sum = max(a[a[rt].son[1]].set, a[a[rt].son[1]].w);
		}
		a[rt].set = -MAX;
		a[rt].flag = 0;
	}
	if(a[rt].flag) {
		if(a[rt].son[0]) {
			a[a[rt].son[0]].flag ^= 1;
			swap(a[a[rt].son[0]].suml, a[a[rt].son[0]].sumr);
			swap(a[a[rt].son[0]].son[0], a[a[rt].son[0]].son[1]);
		}
		if(a[rt].son[1]) {
			a[a[rt].son[1]].flag ^= 1;
			swap(a[a[rt].son[1]].suml, a[a[rt].son[1]].sumr);
			swap(a[a[rt].son[1]].son[0], a[a[rt].son[1]].son[1]);
		}
		a[rt].flag ^= 1;
	}
}

inline int newnode(int x) {
	int rt;
	if(point.empty())rt = size++;
	else {
		rt = point.front();
		point.pop();
	}
	a[rt].v = x;
	a[rt].suml = a[rt].sumr = a[rt].sum = -MAX;
	a[rt].flag = 0; a[rt].set = -MAX;
	return rt;
}

inline void turn(int rt, int k) {
	int x = a[rt].f, y = a[x].f;
	a[x].son[k ^ 1] = a[rt].son[k];
	if(a[rt].son[k])a[a[rt].son[k]].f = x;
	a[rt].f = y;
	if(y)a[y].son[a[y].son[1] == x] = rt;
	a[x].f = rt;
	a[rt].son[k] = x;
	pushup(x); pushup(rt);
}

void splay(int rt, int ancestry) {
	while(a[rt].f != ancestry) {
		int x = a[rt].f, y = a[x].f;
		if(y == ancestry)turn(rt, a[x].son[0] == rt);
		else {
			int k = a[y].son[0] == x ? 1 : 0;
			if(a[x].son[k] == rt) {
				turn(rt, k ^ 1);
				turn(rt, k);
			} else {
				turn(x, k);
				turn(rt, k);
			}
		}
	}
	if(ancestry == 0)root = rt;
}

int kth(int rt, int k) {
	if(a[rt].s < k)return 0;
	while(1) {
		pushdown(rt);
		int y = a[rt].son[0];
		if(k > a[y].s + 1) {
			rt = a[rt].son[1];
			k -= a[y].s + 1;
		} else if(k <= a[y].s)rt = y;
		else return rt;
	}
}

int buildtree(int l, int r) {
	if(l > r)return 0;
	int rt, mid = l + r >> 1, lson = 0, rson = 0;
	lson = buildtree(l, mid - 1);
	rt = newnode(val[mid]);
	rson = buildtree(mid + 1, r);
	a[rt].son[0] = lson;
	a[rt].son[1] = rson;
	if(lson)a[lson].f = rt;
	if(rson)a[rson].f = rt;
	pushup(rt);
	return rt;
}

inline void split(int front, int next) {
	splay(front, 0); splay(next, front);
}

inline void insert(int rt, int x, int y) {
	int front = kth(rt, x + 1), next = kth(rt, x + 2);
	split(front, next);
	int k = a[next].son[0];
	rt = buildtree(1, y);
	a[next].son[0] = rt; a[rt].f = next;
	pushup(next); pushup(front);
}

void delete_x(int rt) {
	if(!rt)return;
	point.push(rt);
	if(a[rt].son[0])delete_x(a[rt].son[0]);
	if(a[rt].son[1])delete_x(a[rt].son[1]);
	clean(rt);
}

inline void remove(int rt, int l, int r) {
	int front = kth(rt, l), next = kth(rt, r + 2);
	split(front, next);
	int k = a[next].son[0];
	if(k)delete_x(k);
	a[next].son[0] = 0;
	pushup(next); pushup(front);
}

inline void reverge(int rt, int l, int r) {
	int front = kth(rt, l), next = kth(rt, r + 2);
	split(front, next);
	int k = a[next].son[0];
	if(!k)return;
	a[k].flag ^= 1;
	swap(a[k].son[0], a[k].son[1]);
	swap(a[k].suml, a[k].sumr);
	pushup(next); pushup(front);
}

inline void same(int rt, int l, int r, int x) {
	int front = kth(rt, l), next = kth(rt, r + 2);
	split(front, next);
	int k = a[next].son[0];
	if(!k)return;
	a[k].set = a[k].v = x;
	a[k].w = x * a[k].s;
	a[k].suml = a[k].sumr = a[k].sum = max(x, a[k].w);
	pushup(next); pushup(front);
}

void work() {
	int x, y, k;
	char ch[20];
	while(m--) {
		scanf("%s", ch);
		switch(ch[0]) {
		case 'I': {
			x = read(); y = read();
			for(int i = 1; i <= y; i++)val[i] = read();
			insert(root, x, y);
			n += y;
			break;
		}
		case 'D': x = read(); y = read(); n -= y; remove(root, x, x + y - 1); break;
		case 'R': x = read(); y = read(); reverge(root, x, x + y - 1); break;
		case 'G': {
			x = read(); y = read();
			int front = kth(root, x), next = kth(root, x + y + 1);
			split(front, next);
			int k = a[next].son[0];
			printf("%d\n", a[k].w);
			break;
		}
		case 'M': {
			if(ch[4] == '-') {
				x = read(); y = read(); k = read();
				same(root, x, x + y - 1, k);
			} else {
				x = 1; y = n;
				int front = kth(root, x), next = kth(root, x + y + 1);
				split(front, next);
				int k = a[next].son[0];
				printf("%d\n", a[k].sum);
			}
			break;
		}
		}
	}
}

void init() {
	n = read(); m = read();
	for(int i = 1; i <= n; i++)val[i] = read();
	val[0] = val[n + 1] = 0;
	clean(0); clean(n + 1);
	root = buildtree(0, n + 1);
}

int main() {
	init();
	work();
	return 0;
}
/*
9 8
2 -6 3 5 1 -5 -3 6 3
GET-SUM 5 4
MAX-SUM
INSERT 8 3 -5 7 2
DELETE 12 1
MAKE-SAME 3 3 2
REVERSE 3 6
GET-SUM 5 4
MAX-SUM
 */