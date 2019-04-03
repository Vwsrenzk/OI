/*
和那个八皇后很像的,但是八皇后是一年前做的题了
这个我们搜索就算了,虽然用一些技巧也不是说过不去,我们dp吧
但是这个图像我们显然是不可以dp的； 
所以我们将图全部拉到右边,靠右对齐...
因为是横竖关系,所以平移一点影响都没有
但是你平移成这样,就可以dp了:
f[i,j]:=f[i,j]+f[k,j-1]*(Len[i]-(j-1)) [j-1<=k<=i-1] 
1.f[i,j]表示前i列放置j个的方案,且第j个放在第i列上,
2.前面f[k,j-1]个都需要累加上来,举一个说明为什么需要累加：
对于前4排放置2个的情况(平移后的),2个即可以放在第一列和第三列
也可以放在第一列和第四列,所以需要把这些分布在不同列的情况累加上来。 
3.乘(Len[i]-(j-1))是因为前面k列放了j-1个棋子了
然后每行只能放一个棋子,所以第j个棋子在第i列可以放的情况就是Len[i]-(j-1),
len[i]是第i列有多少行,程序中是L[i],
 */
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

typedef long long ll;

inline int read() {
	int x = 0, f = 0;
	char c = getchar();
	while (!isdigit(c)) if (c == '-') f = 1, c = getchar();
	while (isdigit(c)) x = (x << 1) + (x << 3) + (c ^ 48), c = getchar();
	return f * x;
}

const int maxn = 210;
const int MOD = 504;

int n, m;
int len[maxn];
int dp[maxn][maxn];

int main() {
	n = read(), m = read();
	for (int i = 1; i < n; ++i) len[(i << 1) - 1] = len[i << 1] = (i << 1) - 1;
	len[(n << 1) - 1] = (n << 1) - 1;
	if(m == 0) puts("1");
	else if(m > (n << 1) - 1) puts("0");
	else {
		for(int i = 0; i <= ((n << 1) - 1); ++i) dp[i][0] = 1;
		for(int i = 1; i <= ((n << 1) - 1); ++i)
			for(int j = 1; j <= min(m, len[i]); ++j)
				dp[i][j] = (dp[i - 1][j] + dp[i - 1][j - 1] * (len[i] - j + 1)) % MOD;
		printf("%d\n", dp[(n << 1) - 1][m]);
	}
	return 0;
}
