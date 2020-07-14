#include <iostream>
#include <cstdio>
#include <stack>
#include <cstring>
#include <queue>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

long **adj;
long *visit;
long *degree;
vector<long> nowCnt;
vector<long> bigCnt;
vector<string> weight;
long v, e;
long edge = 0;
long ind = 0;

void freeMemory()
{
    for (int i = 0; i < v + 1; ++i)
    {
        delete[] adj[i];
    }
    delete[] adj;
    delete[] visit;
    delete[] degree;
}

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

int main(int argc, char *argv[])
{
    string filename = argv[1];
    freopen(argv[1], "r", stdin);
    long a, b;
    string tmp1, tmp2;
    cin >> tmp1 >> tmp2 >> v >> e;

    visit = new long[v + 1];
    degree = new long[v + 1];
    adj = new long *[v + 1];
    for (int i = 0; i < v + 1; ++i)
    {
        adj[i] = new long[v + 1];
    }
    memset(degree, 0, sizeof(long) * (v + 1));
    memset(visit, 0, sizeof(long) * (v + 1));

    string w;
    weight.emplace_back(""); //index0为空字符串，为了跟顶点同步索引
    for (size_t i = 0; i < v; i++)
    {
        cin >> tmp1 >> tmp2 >> w;
        weight.emplace_back(w);
    }

    for (long i = 0; i < e; i++)
    {
        cin >> tmp1 >> a >> b;
        adj[a][degree[a]++] = b;
        adj[b][degree[b]++] = a;
    }
    fclose(stdin);
    // find max cnt
    for (int i = 1; i < v + 1; ++i)
    {
        if (degree[i] > 0 && visit[i] == 0)
        {
            bfs(i);
            if (nowCnt.size() > bigCnt.size())
            {
                bigCnt.assign(nowCnt.begin(), nowCnt.end());
            }
        }
    }
    edge = 0;
    map<long, long> m;
    sort(bigCnt.begin(), bigCnt.end());
    for (size_t i = 0; i < bigCnt.size(); i++)
    {
        long key = bigCnt[i];
        long value = i + 1;
        m.insert(std::pair<long, long>(key, value));
        for (size_t j = 0; j < degree[bigCnt[i]]; j++)
        {
            if (adj[bigCnt[i]][j] > bigCnt[i])
            {
                edge++;
            }
        }
    }

    cout << filename << " find maxCnt, v: " << bigCnt.size() << " e: " << edge << endl;
    freopen(argv[2], "w", stdout);
    cout << "p edge " << bigCnt.size() << ' ' << edge << endl;
    for (long i : bigCnt)
    {
        cout << "v " << m[i] << " " << weight[i] << endl;
    }

    for (long &i : bigCnt)
    {
        for (int j = 0; j < degree[i]; j++)
            if (adj[i][j] > i)
                cout << "e " << m[i] << ' ' << m[adj[i][j]] << endl;
    }

    fclose(stdout);
    freeMemory();
    return 0;
}
