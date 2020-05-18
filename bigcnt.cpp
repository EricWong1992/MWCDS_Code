#include <iostream>
#include <stdio.h>
#include <stack>
#include <string.h>
#include <queue>
#include <fstream>
#include <vector>
#include <map>
using namespace std;
#define MAXN 53000000
vector<long> *adj;
//long adj[MAXN][1000];
//long visit[MAXN];
long *visit;
long *degree;
vector<long> nowCnt;
vector<long> bigCnt;
long v, e;
long ind = 0;
void bfs(long root)
{
    nowCnt = {};
    queue<long> q;
    nowCnt.push_back(root);
    q.push(root);
    visit[root] = 1;
    ind++;
    long cur;
    while (!q.empty())
    {
        cur = q.front();
        q.pop();
        for (int i = 0; i < degree[cur]; i++)
        {
            long tt = adj[cur][i];
            if (visit[tt] == 0)
            {
                visit[tt] = 1;
                q.push(tt);
                nowCnt.push_back(tt);
                ind++;
            }
        }
    }
}
long find()
{
    long i = 1;
    for (; i <= v; i++)
    {
        if (visit[i] == 0)
            break;
    }
    return i;
}
int main(int argc, char *argv[])
{
    visit = new long[10 * MAXN];
    degree = new long[10 * MAXN];
    adj = new vector<long>[10 * MAXN];
    //string filename="/Users/douglaslee/Desktop/CDScase/snap/Cit-HepTh.txt";
    string filename = argv[1];
    //ifstream infile(filename);
    freopen(argv[1], "r", stdin);
    long a, b;
    string tmp1, tmp2;
    memset(degree, 0, sizeof(degree));
    memset(visit, 0, sizeof(visit));
    cin >> tmp1 >> tmp2 >> v >> e;
    for (long i = 0; i < e; i++)
    {
        cin >> tmp1 >> a >> b;
        degree[a]++;
        degree[b]++;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    fclose(stdin);
    //infile.close();
    //while(bigCnt.size()<(v-ind)){
    bfs(find());
    if (nowCnt.size() > bigCnt.size())
    {
        bigCnt = nowCnt;
    }
    //}
    cout << bigCnt.size();
    long edge = 0;
    map<long, long> m;
    for (long i = 0; i < bigCnt.size(); i++)
    {
        m[bigCnt[i]] = i + 1;
        for (int j = 0; j < degree[bigCnt[i]]; j++)
            if (adj[bigCnt[i]][j] > bigCnt[i])
                edge++;
    }
    freopen(argv[2], "w", stdout);
    // ofstream outfile(filename+".txt");
    cout << "p edge " << bigCnt.size() << ' ' << edge << endl;
    for (long i = 0; i < bigCnt.size(); i++)
    {
        for (int j = 0; j < degree[bigCnt[i]]; j++)
            if (adj[bigCnt[i]][j] > bigCnt[i])
                cout << "p " << m[bigCnt[i]] << ' ' << m[adj[bigCnt[i]][j]] << endl;
    }
    fclose(stdout);
    delete[] visit;
    delete[] degree;
}
