
#include "construct_score.h"

void init_increase_dominate_simple(int v, int source_v)
{
    if (dominated[v] == 0)
    {            //如果该点本身没有被支配
        --WV[v]; //该点以及其周围的点的分数要减少，因为现在从没有被支配到了被支配了，原来的可以加的分不见了
        if (isInS[v] == 1)
        {
            initscore[v] -= 2;
            int pos = pos_in_my_heap[v];
            my_heap_remove(pos);
            my_heap_insert(v);
        }

        // all make but one break
        for (int i = 0; i < v_degree[v]; ++i)
        {
            int u = v_adj[v][i];
            --WV[u];
            if (isInS[u] == 1)
            {
                initscore[u] -= 2; //u，v两个点，所以-2
                int pos = pos_in_my_heap[u];
                my_heap_remove(pos);
                my_heap_insert(u);
            }

        } //周围的点分数都减少
        Dom(v);
        onlydominate[v] = source_v;
    }
    ++dominated[v];
} //不考虑黑色点的分数改变情况

int find(int i)
{
    if (pre[i] == i)
        return i;
    else
    {
        return pre[i] = find(pre[i]);
    }
}

void join(int a, int b)
{
    int r1 = find(a);
    int r2 = find(b);
    if (v_degree[r1] > v_degree[r2])
        pre[r2] = r1; //以度大的为优先选择
    else
        pre[r1] = r2;
}

/**
 计算点相邻的连通分支数量
 
 @param node 节点号
 */
int calCV(int node)
{
    map<int, int> m;
    int po = 0;
    for (int i = 0; i < v_degree[node]; i++)
    {
        int u = v_adj[node][i];
        if (v_in_c[u])
        {
            int root = find(u);
            if (m.find(root) == m.end())
                m[root] = po++;
        }
    }
    return CV[node] = po;
}

void lowerScore()
{
    // score
    for (int v = 1; v < v_num + 1; v++)
    {
        if (v_in_c[v])
        {
            // lin jinkun
            if (dominated[v] == 1)
            {
                --score[v];
                // --subscore[v];
            }
            for (int n = 0; n < v_degree[v]; ++n)
            {
                if (dominated[v_adj[v][n]] == 1)
                {
                    --score[v];
                    // --subscore[v];
                }
            }
        }
    } // 删去点C中的v后的分数改变量，因为对于一个支配集加入点没有帮助
}


//根据定义初始化subscore
void initSubScore()
{
    for (int v = 1; v < v_num + 1; v++)
    {
        int tempSubScore = 0;       //TODO：本来应该定义为double，但是在一个例子中int效果更好
        //v在D中，subscore为负
        if (v_in_c[v])
        {
            if (dominated[v] == 1)
            {
                tempSubScore -= weight_backup[v];
                for (size_t n = 0; n < v_degree[v]; n++)
                {
                    int neighbor = v_adj[v][n];
                    if (dominated[neighbor] == 1)
                    {
                        tempSubScore -= weight_backup[neighbor];
                    }
                }
            }
        }
        else
        {
            //v不在D中, subscore为正
            tempSubScore += weight_backup[v];
            for (size_t n = 0; n < v_degree[v]; n++)
            {
                int neighbor = v_adj[v][n];
                if (dominated[neighbor] == 0)
                {
                    tempSubScore += weight_backup[neighbor];
                }
            }
        }
        subscore[v] = tempSubScore;
    }
}


/**
 加入某个点后将点周围的连通分量连接起来
 
 @param node 加入C的点
 */
void joinV(int node)
{
    for (int i = 0; i < v_degree[node]; i++)
    {
        int u = v_adj[node][i];
        if (v_in_c[u])
            join(node, u);
    }
}
int Sinitindex = 0;
void addToS(int i)
{ //将自身和邻居都加入候选集中
    if (isInS[i] == false)
    {
        S[Snum++] = i;
        isInS[i] = true;
        initscore[i] = 2 * WV[i] + CV[i] - 1;
        my_heap_insert(i);
    }
    for (int j = 0; j < v_degree[i]; j++)
    {
        long u = v_adj[i][j];
        if (!v_in_c[u] && isInS[u] == false)
        {
            S[Snum++] = u;
            isInS[u] = true;
            initscore[u] = 2 * WV[u] + CV[u] - 1;
            my_heap_insert(u);
        }
    }
}
void updateS(int i)
{ //加入i后更新候选集S
    for (int j = 0; j < v_degree[i]; j++)
    {
        if (!v_in_c[v_adj[i][j]])
            //if(isInS[v_adj[i][j]]==false)
            addToS(v_adj[i][j]);
    }
}

//选出分数最高的点
int chooseMax()
{
    int max = 0;
    int index = -1;
    for (int i = 0; i < Snum; i++)
    {
        int p = S[i];
        if (dominated[p] == 0)
        {
            initscore[p] = 2 * WV[p] - 1;
        } //白色点的CV=0
        else
        {
            initscore[p] = 2 * WV[p] + calCV(p) - 1;
        }
        if (initscore[p] > max)
        {
            max = initscore[p];
            index = i;
        }
    }
    if (max > 0)
    {
        int tmp = S[Snum - 1];
        S[Snum - 1] = S[index];
        S[index] = tmp;
        return S[--Snum];
    }
    else
        return -1;
}

//返回最大score值顶点(直接取堆顶)
int chooseMax1()
{
    if (initscore[my_heap[0]] > 0)
        return my_heap[0];
    else
        return -1;
}

void addNodeInit(int i)
{
    updateS(i); //更新候选集
    v_in_c[i] = 1;
    currentWeight += weight_backup[i];
    isInS[i] = false;
    //被支配了多次
    if (dominated[i] != 0)
    {
        //joinV(i);//影响CV,加入的是灰点
        //TODO:
        connectedNum -= (calCV(i) - 1);
    }
    else
        connectedNum++;
    if (dominated[i] != 0)
        joinV(i);
    for (int j = 0; j < v_degree[i]; j++)
    {
        int u = v_adj[i][j];
        CV[u] = calCV(u); //time=>一阶邻居
        if (isInS[u] == true && (2 * WV[u] + CV[u] - 1) != initscore[u])
        {
            //score值修改，重新计算heap中位置
            initscore[u] = 2 * WV[u] + CV[u] - 1;
            int pos = pos_in_my_heap[u];
            my_heap_remove(pos);
            my_heap_insert(u);
        }
    }
    init_increase_dominate_simple(i, i);
    for (int j = 0; j < v_degree[i]; j++)
        init_increase_dominate_simple(v_adj[i][j], i); //改变WV time=>二阶邻居

} //加入到candidate中
void ConstructByInitScore()
{
    best_c_size = (int)(~0U >> 1);
    for (int v = 1; v < v_num + 1; ++v)
    {
        //initscore[v]=2*WV[v]-1;
        Undom(v);
    }
    fill_n(isInS, v_num + 1, 0);
    fill_n(S, v_num + 1, 0);
    Snum = 0;
    int ds_size = 0;

    addNodeInit(maxDegreeNode);
    int pos = pos_in_my_heap[maxDegreeNode];
    my_heap_remove(pos);
    ds_size++;
    int maxTmpIndex = chooseMax1();
    while (maxTmpIndex != -1 && (undom_stack_fill_pointer != 0 || connectedNum != 1))
    { //不是CDS且返回值不是-1
        int pos = pos_in_my_heap[maxTmpIndex];
        my_heap_remove(pos);
        addNodeInit(maxTmpIndex);
        ds_size++;
        maxTmpIndex = chooseMax1();
    }
    c_size = ds_size;
    fill_n(score, v_num + 1, 0);
    lowerScore();
    initSubScore();
    UpdateBestSolution();

    //初始化所有candidate用于寻找割点
    for (int i = 1; i <= v_num; ++i) {
        candidate[candidate_size++] = i;
    }

    //使用割点fix
    MarkCut();
    for (int i = 0; i < cutIndex; ++i) {
        if (isCut[cutPointSet[i]] != 0)
        {
            int cutPoint = cutPointSet[i];
            v_fixed[cutPoint] = 1;
            fixedSet[fixedNum++] = cutPoint;
        }
    }
    //还原candidate
    candidate_size = 0;

    ResetCandidate();
    //for(int i=1;i<=v_num;i++)
    //   cout<<score[i]<<' '<<find(i)<<' '<<dominated[i]<<endl;
}
