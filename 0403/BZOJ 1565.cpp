/*
这题折腾了三天，差点没吐出血来。
先拓扑排序求环，把环删掉。
之后重建图求最大权闭合子图，把一个植物向保护它的植物连边，跑一遍网络流即可。
第一次只过了4个点。不明所以。
换方法重写了拓扑，在网络流中加限制乱搞。
 */
#include <bits/stdc++.h>

#define F(i,j,k) for(int i=j;i<=k;++i)
#define M(a,b) memset(a,b,sizeof(a))
#define FF(i,j,k) for(int i=j;i>=k;i--)
#define inf 0x3f3f3f3f
#define maxm 1001
#define mod 998244353
//#define LOCAL
using namespace std;
int read() {
	int x = 0, f = 1; char ch = getchar();
	while(ch < '0' || ch > '9') {
		if(ch == '-')f = -1;
		ch = getchar();
	}
	while(ch >= '0' && ch <= '9') {
		x = x * 10 + ch - '0';
		ch = getchar();
	}
	return x * f;
}
int n, m;
int mp[2000];
struct EDGE {
	int from;
	int to;
	int value;
	int next;
} e[800005];
int head[4001];
int v[4001], in[4001], out[4001];
int tot = 1;
inline void addedge(int u, int v, int w) {
	e[++tot].from = u;
	e[tot].to = v;
	e[tot].value = w;
	e[tot].next = head[u];
	head[u] = tot;
	//    tot++;
}
//bool kan[2000][2000];
int ans = 0;
//inline int at(int x,int y)
//{
//    return m*x+y+1;
//}
//int stk[4001],top=0;
//inline void topsort()
//{
//    int temp[4001];M(temp,0);
//    F(i,1,m*n) temp[i]=in[i];
//    F(i,1,m*n){
//        if(in[i]==0){
//            stk[top]=i;
////            cout<<i<<"********"<<endl;
//            top++;
//        }
//    }
////    cout<<top<<": "<<endl;
////    F(i,0,top-1) cout<<stk[i]<<" ";cout<<endl;
//    while(top!=0){
//        top--;int u=stk[top];
//        for(int i=head[u];i!=-1;i=e[i].next)
//        {
//            in[e[i].to]--;
//            if(in[e[i].to]==0){
//                stk[top]=e[i].to;
//                top++;
//            }
//        }
//    }
//    F(i,1,m*n){
//        if(in[i]!=0){
//            stk[top]=i;
//            top++;
//        }
//    }
//    while(top!=0){
//        top--;int u=stk[top];
//        temp[u]=-1;out[u]=-1;
//        v[u]=0;
//    }
//    F(i,1,m*n) in[i]=temp[i];
//    return;
//}
inline void add(int u, int v, int w) {
	addedge(u, v, w);
	addedge(v, u, 0);
	++in[u];
	return;
}
int mmp[4001];
int S, T;
bool ok() {
	F(i, S, T) if(mmp[i] != -2) mmp[i] = -1;
	int que[4001];
	int hd = 0, tl = 0;
	que[tl++] = S;
	mmp[S] = 1;
	while (hd < tl) {
		int u = que[hd++];
		for(int i = head[u]; i; i = e[i].next) {
			if(mmp[e[i].to] == -1 && e[i].value) {
				mmp[e[i].to] = mmp[u] + 1;
				que[tl++] = e[i].to;
			}
		}
	}
	//    if (mmp[T]!=-1) return true;
	//    else return false;
	return mmp[T] > 0;
}
int cur[4000];
int zeng(int k, int now) {
	if (k == T) return now;
	int r = 0;
	for (int i = cur[k]; i && now > r; i = e[i].next) {
		if (mmp[k] + 1 == mmp[e[i].to] && e[i].value) {
			int t = zeng(e[i].to, min(now - r, e[i].value));
			e[i].value -= t; e[i ^ 1].value += t; r += t;
			if(e[i].value) cur[k] = i; //***
			//            cout<<e[i].from<<" ---> "<<e[i].to<<" $ "<<t<<endl;
		}
	}
	if (!r) mmp[k] = -1;
	return r;
}
int dinic() {
	int r = 0, t;
	M(cur, 0);
	while (ok()) {
		for(int i = S; i <= T; ++i) cur[i] = head[i];
		r += zeng(S, inf);
		//        while(t=zeng(S,inf)) r+=t;
	}
	return r;
}
//pair<int,int> kk[4001];
//int cur;
int main() {
	std::ios::sync_with_stdio(false);//cout<<setiosflags(ios::fixed)<<setprecision(1)<<y;
#ifdef LOCAL
	freopen("111.in", "r", stdin);
	freopen("data.out", "w", stdout);
#endif
	cin >> n >> m;
	S = 0; T = m * n + 1;
	M(mp, 0); M(head, 0);
	//    F(i,0,n-1)F(j,0,m-1){
	//        int x,y;
	//        cin>>x>>y;
	//        mp[i][j]=x;
	//        v[at(i,j)]=x;
	//        in[at(i,j)]++;
	//        if(j==m-1) in[at(i,j)]--;
	//        if(j!=0) add(at(i,j),at(i,j-1),v[at(i-1,j)]);
	//        if(y==0) continue;
	////        **********************************************
	//        F(k,1,y){
	//            int a,b;
	//            cin>>a>>b;
	//            kk[cur].first=at(i,j);
	//            kk[cur].second=at(a,b);
	//            cur++;
	//            in[at(a,b)]++;
	//            addedge(at(i,j),at(a,b),0);
	////            cout<<at(i,j)<<" "<<at(a,b)<<" &&&&&"<<endl;
	//        }
	//    }
	//    F(i,1,m*n) cout<<in[i]<<" ";cout<<endl;
	//    cout<<"/*****************************"<<endl;
	//    F(i,1,tot) cout<<e[i].from<<" ---> "<<e[i].to<<" $ "<<e[i].value<<endl;
	//    cout<<"*****************************/"<<endl;
	F(i, 1, n * m) {
		int temp;
		cin >> mp[i];
		if(mp[i] > 0) add(S, i, mp[i]);
		else add(i, T, -mp[i]);
		cin >> temp;
		while(temp--) {
			int x, y;
			cin >> x >> y;
			add(x * m + y + 1, i, inf);
		}
		if(i % m) add(i, i + 1, inf);
	}
	//    topsort();
	//    cout<<"))))))"<<endl;
	//    F(i,1,m*n) if(in[i]==-1) cout<<i<<" ";cout<<"((((("<<endl;
	//    M(e,0);M(head,-1);tot=0;
	int q[4000]; M(q, 0);
	int cur1 = 0, cur2 = 0;
	for(int i = S; i <= T; ++i) {
		if(!in[i]) q[cur2++] = i;
		mmp[i] = -2;
	}
	int sum = 0;
	while(cur1 < cur2) {
		int cnt = q[cur1++]; mmp[cnt] = 0;
		if(mp[cnt] > 0) sum += mp[cnt];
		for(int i = head[cnt]; i; i = e[i].next) {
			if(i & 1) if(!--in[e[i].to]) q[cur2++] = e[i].to;
		}
	}
	cout << sum - dinic() << endl;
	return 0;
}
/*
3 2
10 0
20 0
-10 0
-5 1 0 0
100 1 2 1
100 0
*/