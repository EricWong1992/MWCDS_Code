
#include "basic.h"
#include <queue>

//根据v_in_c更新candidate
void ResetCandidate()
{
    int j = 0;
    for (int v = 1; v < v_num + 1; v++)
    {
        if (v_in_c[v] == 1 && v_fixed[v] == 0)
        {
            candidate[j] = v;
            index_in_candidate[v] = j;
            j++;
        }
        else
        {
            index_in_candidate[v] = 0;
            if (dominated[v] > 0)
            {
                greyPointArray->insert_element(v);
            }
        }
    }
    candidate_size = j;
} //根据v_in_c来重置在候选解C中的点

void UpdateBestSolution()
{
    //    int v;
    if (currentWeight < bestWeight)
    {
        cout << "weight: " << currentWeight << endl;
        best_c_size = c_size;
        best_comp_time = TimeElapsed();
        best_step = step;
        bestWeight = currentWeight;
        memcpy(best_v_in_c, v_in_c, sizeof(int) * (v_num + 1));
    }
} //更新最优解

void cutPoint(int cur, int fa)
{
    int child = 0;
    dnf[cur] = low[cur] = ++ind;
    for (int i = 0; i < v_degree[cur]; i++)
    {
        int u = v_adj[cur][i];
        if (v_in_c[u] == 1)
        {
            if (dnf[u] == 0)
            {
                child++;
                cutPoint(u, cur);
                low[cur] = min(low[cur], low[u]);
                if ((cur != root && low[u] >= dnf[cur]) || (cur == root && child >= 2))
                {
                    isCut[cur] = 1;
                    cutPointSet[cutIndex++] = cur;
                }
            } //没有访问过
            else if (u != fa)
            {
                low[cur] = min(low[cur], dnf[u]);
            }
        }
    }
    if (score[cur] > maxScore)
        maxPoint = cur;
} //寻找黑色点图中的割点

//Tarjan算法寻找割点，无递归
void cutPointNoRecur(int root)
{
    int k, u, v, inde, top = 0;
    inde = 0;
    for (u = 0; u < candidate_size; u++) //初始化SF
        SF[candidate[u]] = first[candidate[u]];
    for (u = 0; u < fixedNum; u++)
        SF[fixedSet[u]] = first[fixedSet[u]];
    Stack[top] = root;
    dnf[root] = low[root] = ++inde;
    while (top >= 0)
    {
        k = SF[Stack[top]];

        if (k != -1)
        {
            if (v_in_c[edge[k].v] == 0)
            {
                SF[Stack[top]] = edge[k].next;
                continue;
            }
            SF[Stack[top]] = edge[k].next; //指向下一条边
            //顶点未被访问过
            if (dnf[edge[k].v] == 0)
            {
                child[Stack[top]]++;
                //未被访问过顶点入栈
                Stack[++top] = edge[k].v;
                //更新low和dfn
                low[edge[k].v] = dnf[edge[k].v] = ++inde;
            }
            else
            {
                //被访问过，更新low值
                low[Stack[top]] = (low[Stack[top]] < dnf[edge[k].v]) ? low[Stack[top]] : dnf[edge[k].v]; //取最小值
            }
        }
        else
        {
            if (top > 0)
            {
                u = Stack[top - 1];
                v = Stack[top];
                low[u] = (low[u] < low[v]) ? low[u] : low[v];
                if ((u != root && low[v] >= dnf[u]) || (u == root && child[u] >= 2))
                {
                    //加入割点集
                    isCut[u] = 1;
                    cutPointSet[cutIndex++] = u;
                }
            }
            top--;
        }
    }
}

/*从root顶点开始广度优先遍历，处理所有相关联的v_in_c==1的顶点
    把未被固定或者子节点为0的顶点添加到toberemoved数组
*/
void cutPoint1(long root)
{
    queue<int> qu;
    qu.push(root);
    dnf[root] = ++ind;
    while (!qu.empty())
    {
        int cur = qu.front();
        childnum[cur] = 0;
        qu.pop();
        for (int i = 0; i < v_degree[cur]; i++)
        {
            int u = v_adj[cur][i];
            if (v_in_c[u] == 1 && dnf[u] == 0)
            {
                dnf[u] = ++ind;
                childnum[cur]++;
                qu.push(u);
                father[u] = cur;
            }
        }
        if ((v_fixed[cur] == 0) && (childnum[cur] == 0 || (childnum[cur] == 1 && cur == root)))
        {
            indextoberemoved[cur] = toberemovedNum;
            toberemoved[toberemovedNum++] = cur;
            inToberemoved[cur] = 1;
        }
    }
}

bool judgeCut(int node)
{
    root = node;
    cutPoint(node, node);
    return (isCut[node] == 1);
} //判断点node是否是割点TODO：恢复low，dnf，iscut

//使用参数对权重进行更新
//TODO:权重设置阈值
void updateWeight()
{
    double avg_weight = totalweight / v_num;
    for (int i = 1; i <= v_num; i++)
    {
        //平滑权重
        int newweight = weight_para_aphle * frequency[i] + avg_weight * (1 - weight_para_aphle);
        // int newweight = weight_para_aphle * frequency[i];
        if (newweight < 1)
            newweight = 1;
        int tobeminus = (frequency[i] - newweight);
        if (tobeminus > 0)
            minusWeight(i, tobeminus);
        totalweight -= tobeminus;
    }
}

void updateSubscore()
{
    fill_n(frequency, v_num + 1, 1);
    memcpy(subscore, score, sizeof(int) * (v_num + 1));
    totalweight = v_num;
}
