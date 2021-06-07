#pragma GCC optimize(3)
#include<bits/stdc++.h>
using namespace std;

void test()
{
    cerr<<"\n";
}
template<typename T,typename... Args>void test(T x,Args... args)
{
    cerr<<x<<" ";
    test(args...);
}

const int maxn=1e5+10;
int n,m,ans;
vector<int> E[maxn];
set<int> ansSet,A,B,CDS,PS;
clock_t finaltime=0;
clock_t c1;
//点权
double weight[maxn];
//邻接表中在CDS点集中的个数
int L[maxn];

//读入图的数据
char s[1005];
void Read()
{
    ifstream fin;
    fin.open(s,ios::in);
    fin>>n>>m;
    for(register int i=1; i<=m; i++)
    {
        int u,v;
        fin>>u>>v;
        E[u].push_back(v);
        E[v].push_back(u);
        //L[u]++,L[v]++;
        PS.insert(u);
        PS.insert(v);
        // CDS.insert(u);
        // CDS.insert(v);
    }

}

//检测CDS是否正确
int vis[maxn];
void Find(int rt)
{
    vis[rt]=1;
    for(int i:E[rt])
    {
        if(ansSet.find(i)!=ansSet.end()&&!vis[i])
            Find(i);
    }
}

void check()
{
    for(int i=0; i<=100000; i++)
        vis[i]=0;
    set<int> tempSet;
    int temp=0;
    for(int i:ansSet)
    {
        if(!vis[i])
            Find(i),temp++;
        for(int j:E[i])
        {
            tempSet.insert(j);
        }
    }
    if(temp==1&&tempSet.size()==n) puts("Yes");
    else
    {
        system("pause");
    }
    //puts(temp==1&&tempSet.size()==n?"Yes":"No"); //连通
    /*if()  //支配
        puts("Yes");
    else
        puts("No");*/
    //system("pause");
}


//Tarjan求割点
int tot;
vector<int> mp[maxn];
int low[maxn],dfn[maxn];
set<int> cutSet;
void dfs(int x,int pre)
{
    int k,son;
    low[x]=dfn[x]=++tot;
    son=k=0;
    for(int to:mp[x])
    {
        //判断重边
        if(to==pre&&!k)
        {
            k++;
            continue;
        }
        if(!dfn[to])
        {
            son++;
            dfs(to,x);
            low[x]=min(low[x],low[to]);
            if(x!=pre&&low[to]>=dfn[x])
            {
                cutSet.insert(x);
            }
        }
        else
            low[x]=min(low[x],dfn[to]);
    }
    if(x==pre&&son>1)
    {
        cutSet.insert(x);
    }
}
//O(n)

void Tarjan()
{
    for(register int i:CDS)
    {
        mp[i].clear();
        low[i]=dfn[i]=0;
    }
    cutSet.clear();
    for(register int i:CDS)
    {
        for(register int j:E[i])
        {
            if(CDS.find(j)!=CDS.end())
            {
                mp[i].push_back(j);
            }
        }
    }
    tot=0;
    for(register int i:CDS)
    {
        if(!dfn[i])
            dfs(i,i);
    }
}
//1或者1.1
#define SAT(K) pow(K,1)
//O(nlogn)
//从CDS中删除一个节点
int sz,optcnt,optsum;
int remove(double wf,int k)
{
    Tarjan();
    if(cutSet.size()==CDS.size())
        return -1;
    register int cnt=0;
    register int option=-1;
    register double loss=1e18;
    for(register int i:CDS)
    {
        if(cutSet.find(i)==cutSet.end())
        {
            double Apoint=0;
            for(int j:E[i])
            {
                if(L[j]==1&&A.find(j)!=A.end())
                {
                    Apoint+=1;//weight[j];
                }
            }
            if(loss>Apoint)
            {
                loss=Apoint;
                cnt=1;
                option=i;
            }
            else if(loss==Apoint)
            {
                cnt++;
                if(((int)rand())%cnt==0)
                {
                    option=i;
                }
            }
            else if(Apoint-1<loss)
            {
                int r=rand()%1000;
                if(r<=wf/10)
                {
                    loss=Apoint;
                    cnt=1;
                    option=i;
                }
            }
            else if(k%10000==5000)
            {
                int r=rand()%1000;
                if(r<=wf)
                {
                    loss=Apoint;
                    cnt=1;
                    option=i;
                }
            }
        }
    }
    CDS.erase(option);
    sz--;
    for(register int i:E[option])
    {
        L[i]--;
        if(L[i]==0&&A.find(i)!=A.end())
        {
            A.erase(i);
            B.insert(i);
            //weight[i]=SAT(k);
        }
    }
    A.insert(option);
    return option;
}


//交换 调整CDS
bool adjustment(int iter)
{
    optcnt=0;
    int minB=B.size();
    register double wf=1000;       //可调整参数
    for(register int k=1; k<=iter/*500000*/; k++)
    {
        optcnt++;
        //wf*=0.99994;            //可调整参数
        wf*=0.999935;
        int option=remove(wf,k);
        if(option==-1)
            return false;
        //weight[option]=k;
        register double addw=1e18;
        register int addpoint=-1;
        register int cnt=0,losscntB=0;
        for(int i:A)
        {
            if(i==option)
                continue;
            register double Bpointw=0;
            for(int j:E[i])
            {
                if(L[j]==0)
                {
                    Bpointw+=weight[j]-(rand()%(k))*(rand()%40*0.025+0.5);
                    //Bpointw+=weight[j]-(rand()%((int)SAT(k)))*(rand()%40*0.015+0.6);
                    losscntB++;
                }
            }
            if(addw>Bpointw)
            {
                addw=Bpointw;
                cnt=1;
                addpoint=i;
            }
            else if(addw==Bpointw)
            {
                cnt++;
                if(((int)rand())%cnt==0)
                {
                    addpoint=i;
                }
            }
            // -1或-2
            else if(addw>Bpointw-1&&k%10000==0)
            {
                //test(wf);
                int r=rand()%1000;
                if(r<=wf)
                {
                    addw=Bpointw;
                    cnt=1;
                    addpoint=i;
                }
            }
            else if(k%10000==5000)
            {
                int r=rand()%1000;
                if(r<=wf/2)
                {
                    addw=Bpointw;
                    cnt=1;
                    addpoint=i;
                }
            }
            /*else
            {
                int r=rand()%1000;
                if(r<=wf/50)
                {
                    addw=Bpointw;
                    cnt=1;
                    addpoint=i;
                }
            }*/
        }
        CDS.insert(addpoint);
        sz++;
        for(int i:E[addpoint])
        {
            L[i]++;
            if(L[i]==1&&B.find(i)!=B.end())
            {
                B.erase(i);
                A.insert(i);
                weight[i]=SAT(k);
            }
        }
        A.erase(addpoint);
        if(B.size()==0)
            break;
        minB=min(minB,(int)B.size());
        //test(option,addpoint);
       // if(k%10000==0) cout<<k<<endl;
    }
    return B.size()==0;
}

bool judge()
{
    int flag=0;
    for(auto i:PS) weight[i]=0;
    int iter=10;
    for(int i=1;i<=iter&&(clock()-c1)/(double)CLOCKS_PER_SEC<=300;i++)
    {
        if(adjustment(200000)&&!flag)
        {
            ansSet.clear();
            for(int i:CDS)
                ansSet.insert(i);
            //iter=rand()%10+1;
            flag=1;
            if(i==1) break;
            iter=i+rand()%5+3;
            finaltime=clock();
        }
        for(int i:PS)
        weight[i]=rand()%10000;
        test(i);
    }
    return flag;
}

void solve()
{
    sz=CDS.size();
    int temp=sz-1;
    ans=sz;
    ansSet=CDS;
    finaltime=clock();
   // check();
    //system("pause");
    while(temp>0)
    {
        remove(1000,0);
        if(judge())
        {
            ans=temp;
            ansSet.clear();
            for(int i:CDS)
                ansSet.insert(i);
            temp--;
            //finaltime=clock();
            optsum+=optcnt;
        }
        else
        {
            break;
        }
        cout<<ans<<endl;
        for(int i:CDS)
            cout<<i<<'?';
        cout<<endl;
        for(int i:A)
            cout<<i<<'A';
        cout<<endl;
        for(int i:B)
            cout<<i<<'B';
        cout<<endl;
    }
    cout<<ans<<"??"<<endl;
     for(int i:ansSet)
         cout<<i<<'?';
     cout<<endl;
}


ofstream fout;
void init()
{
    int temp=-1,cnt=INT_MIN,cntt=0;
    for(int i:PS)
    {
        int sum=0;
        for(int j:E[i])
        {
            if(L[j]==0) sum++;
        }
        if(sum>cnt)
        {
            temp=i;
            cnt=sum;
            cntt=1;
        }
        else if(sum==cnt)
        {
            cntt++;
            int r=rand()%cntt;
            if(r==0)
            {
                temp=i;
                cnt=sum;
            }
        }
    }
    CDS.insert(temp);
    B.erase(temp);
    for(int i:E[temp])
    {
        if(B.find(i)!=B.end())
        {
            A.insert(i);
            B.erase(i);
        }
        L[i]++;
    }
    while(B.size()>0)
    {
        int temp=-1,cnt=INT_MIN,cntt=0;
        for(int i:A)
        {
            int sum=0;
            for(int j:E[i])
            {
                if(L[j]==0) sum++;
            }
            if(sum>cnt)
            {
                temp=i;
                cnt=sum;
                cntt=1;
            }
            else if(sum==cnt)
            {
                cntt++;
                int r=rand()%cntt;
                if(r==0)
                {
                    temp=i;
                    cnt=sum;
                }
            }
        }
        CDS.insert(temp);
        A.erase(temp);
        for(int i:E[temp])
        {
            if(B.find(i)!=B.end())
            {
                A.insert(i);
                B.erase(i);
            }
            L[i]++;
        }
        cout<<B.size()<<' '<<CDS.size()<<endl;
    }
    /*int more=rand()%5+2;
    while(more--)
    {
        int temp=-1,cnt=0;
        for(int i:A)
        {
            cnt++;
            if(rand()%cnt==0)
            {
                temp=i;
            }
        }
        CDS.insert(temp);
        A.erase(temp);
        for(int i:E[temp])
        {
            if(B.find(i)!=B.end())
            {
                A.insert(i);
                B.erase(i);
            }
            L[i]++;
        }
    }*/
}
void Main(int cs)
{
    c1 = clock();
    srand((unsigned)time(NULL));
    for(int i=0; i<=1e5; i++)
        L[i]=0,E[i].clear();
    optsum=0;
    CDS.clear();
    PS.clear();
    A.clear();
    B.clear();
    Read();
    //
    B=PS;
    init();
    solve();
    check();
    fout<<ans<<","<<optsum<<","<<fixed<<setprecision(4)<<(finaltime - c1)/(double)CLOCKS_PER_SEC<<endl;
    std::cerr<<"Case #"<<cs<<":\n"<<ans<<'\t'<< optsum << "\tTime:" <<fixed<<setprecision(4)<<(finaltime - c1)/(double)CLOCKS_PER_SEC << std::endl;
}

void gao()
{
    for(int i=1; i<=10; i++)
        Main(i);
    //system("pause");
    system("cls");
}
signed main()
{
    ios::sync_with_stdio(false);
    fout.open("mydata.csv",ios::out);
    while(1)
    {
        scanf("%s",s);
        gao();
    }
    return 0;
}
