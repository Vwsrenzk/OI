/*
这道题需要我们维护一个字符串，每次快速查询两个后缀的LCP，并且动态修改、往这个字符串插入元素。开始我一直在想用后缀数据结构，因为修改和插入操作不知道该怎么办，没有思路。后来发现插入和修改事实上可以用splay维护，而我的splay上的结点维护的就是这个结点控制的区间的，这一段的字符串的hash值。然后每次旋转的时候重新算一下就可以了。
根据上述所说，插入操作，我可以旋转插入位置到根，下一位到根的右子树，然后直接往右子树的左子树上插就可以了。这样做的好处就是不用一路update，直接在根结点上修改。
修改操作就更简单了，把修改的点旋转到根结点，然后修改、update即可。
我们接下来考虑查询怎么做，因为需要查询两个后缀的LCP，我们并不能直接求出值，但是判断一下还是可以的，于是想到二分答案，然后splay旋转区间，比较hash值就可以了。
这道题细节肯定没有维护数列多，不过我貌似常数写大了，虽然没TLE，但是跑的很慢我很不爽啊...于是就是一顿优化常数，比如把find改成非递归式，还有修改操作改成不旋转，找到之后一路往上update。
大概就是这样了。
ps:结果BZOJ太慢了，后来改成了自然溢才过。
 */
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <vector>
#include <queue>
#include <map>
#include <set>

using namespace std;

typedef unsigned long long LL;

#define RG register

const int inf = (1 << 30);
const int MAXN = 100011;
//const int MOD = 1000007;

int n, m, ans, tot, rt, ql, qr;
char s[MAXN], ch[12];
LL mi[MAXN], hash[MAXN];
int tr[MAXN][2], size[MAXN], father[MAXN];

inline int getint() {
	RG int w = 0, q = 0; RG char c = getchar();
	while((c < '0' || c > '9') && c != '-') c = getchar(); if(c == '-') q = 1, c = getchar();
	while (c >= '0' && c <= '9') w = w * 10 + c - '0', c = getchar(); return q ? -w : w;
}

inline void update(RG int x) {
	RG int l = tr[x][0], r = tr[x][1]; size[x] = size[l] + size[r] + 1;
	hash[x] = hash[r] + (s[x] - 'a' + 1) * mi[size[r]]; //hash[x]%=MOD;
	hash[x] += hash[l] * mi[size[r] + 1]; // hash[x]%=MOD;
}

inline void rotate(RG int x, RG int &rt) {
	RG int y = father[x], z = father[y];
	RG int l = (tr[y][1] == x), r = l ^ 1;
	if(y == rt) rt = x; else tr[z][(tr[z][1] == y)] = x;
	father[x] = z; tr[y][l] = tr[x][r]; father[tr[x][r]] = y;
	father[y] = x; tr[x][r] = y;
	update(y); update(x);
}

inline void splay(RG int x, RG int &rt) {
	RG int y, z;
	while(x != rt) {
		y = father[x]; z = father[y];
		if(y != rt) {
			if((tr[z][0] == y) ^ (tr[y][0] == x)) rotate(x, rt);
			else rotate(y, rt);
		}
		rotate(x, rt);
	}
}

inline void build(RG int l, RG int r, RG int fa) {
	if(l > r) return ; RG int mid = (l + r) >> 1;
	if(l == r) {
		hash[l] = s[l] - 'a' + 1;
		size[l] = 1;
	} else build(l, mid - 1, mid), build(mid + 1, r, mid);
	tr[fa][mid > fa] = mid; father[mid] = fa; update(mid);
}

inline int rank(RG int root, RG int k) {
	RG int l, r;
	while(1) {
		l = tr[root][0], r = tr[root][1];
		if(size[l] + 1 == k) return root;
		if(size[l] >= k) root = l; else root = r, k = k - size[l] - 1;
	}
}

inline bool check(RG int len) {
	RG int hash1; RG int x = rank(rt, ql); splay(x, rt);
	x = rank(rt, ql + len + 1); splay(x, tr[rt][1]); x = tr[tr[rt][1]][0];
	hash1 = hash[x];
	RG int hash2; x = rank(rt, qr); splay(x, rt);
	x = rank(rt, qr + len + 1); splay(x, tr[rt][1]); x = tr[tr[rt][1]][0];
	hash2 = hash[x];
	if(hash1 == hash2) return true;
	return false;
}

inline void query() {
	ql = getint(), qr = getint(); ans = 0;
	RG int l = 1, r = tot - 2 - max(ql, qr) + 1, mid;
	while(l <= r) {
		mid = (l + r) >> 1;
		if(check(mid)) l = mid + 1, ans = mid;
		else r = mid - 1;
	}
	printf("%d\n", ans);
}

inline void insert() {
	RG int pos = getint(); scanf("%s", ch); s[++tot] = ch[0]; hash[tot] = ch[0] - 'a' + 1;
	RG int x = rank(rt, pos + 1);
	splay(x, rt); x = rank(rt, pos + 2); splay(x, tr[rt][1]);
	x = tr[rt][1]; father[tot] = x; size[tot] = 1;
	tr[x][0] = tot; update(x); update(rt);
}

inline void change() { //只需要修改一条链，不用rotate
	RG int pos = getint(); scanf("%s", ch);
	RG int x = rank(rt, pos + 1); splay(x, rt);
	s[x] = ch[0]; update(x);
}

inline void work() {
	scanf("%s", s + 2); n = strlen(s + 2);  m = getint();
	mi[0] = 1; for(RG int i = 1; i <= 100010; i++) mi[i] = mi[i - 1] * 27; //,mi[i]%=MOD;
	s[1] = s[n + 2] = 'a'; build(1, n + 2, 0); tot = n + 2; rt = (1 + n + 2) / 2;
	while(m--) {
		scanf("%s", ch);
		if(ch[0] == 'Q') query();
		else if(ch[0] == 'I') insert();
		else change();
	}
}

int main() {
	work();
	return 0;
}
/*
madamimadam
7
Q 1 7
Q 4 8
Q 10 11
R 3 a
Q 1 7
I 10 a
Q 2 11
 */