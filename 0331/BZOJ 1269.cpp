/*
由于rope的底层实现，insert，erase，get都是logn的
就是翻转不行，不是自己手写的打不了标记啊！！
怎么办？
答：同时维护一正一反两个rope……反转即交换两个子串……Orz……
区间循环位移？简单，拆成多个子串连起来就好了……
区间a变b b变c c变d …… z变a？ 呃……维护26个rope？
区间和？那是线段树的活
区间kth？sorry，与数值有关的操作rope一概不支持……
 */
#include <cstdio>
#include <iostream>
#include <ext/rope>
#include <algorithm>

using namespace std;

using namespace __gnu_cxx;

inline int read() {
	int x = 0; char ch = getchar();
	while(ch > '9' || ch < '0')ch = getchar();
	while(ch >= '0' && ch <= '9') {
		x = x * 10 + ch - '0';
		ch = getchar();
	}
	return x;
}

int n, now, len;
rope <char> a, b, tmp;
char s[2000000], rs[2000000];

int main() {
	n = read();
	int x;
	while(n--) {
		scanf("%s", s);
		switch(s[0]) {
		case 'M': now = read(); break;
		case 'P': now--; break;
		case 'N': now++; break;
		case 'G': printf("%c\n", a[now]); break;
		case 'I': {
			x = read(); len = a.length();
			for(int i = 0; i < x; i++) {
				s[i] = getchar();
				while(s[i] == '\n')
					s[i] = getchar();
				rs[x - i - 1] = s[i];
			}
			rs[x] = s[x] = 0;
			a.insert(now, s);
			b.insert(len - now, rs);
			break;
		}
		case 'D': {
			x = read(); len = a.length();
			a.erase(now, x);
			b.erase(len - now - x, x);
			break;
		}
		case 'R': {
			x = read(); len = a.length();
			tmp = a.substr(now, x);
			a = a.substr(0, now) + b.substr(len - now - x, x) + a.substr(now + x, len - now - x);
			b = b.substr(0, len - now - x) + tmp + b.substr(len - now, now);
			break;
		}
		}
	}
	return 0;
}
/*
10
Insert 13
Balanced eert
Move 2
Delete 5
Next
Insert 7
editor
Move 0
Get
Move 11
Rotate 4
Get
*/