#pragma once

#include "checker.h"

#define NDEBUG
#define N_DEBUG_OUTPUT

void printDebugMsg(string msg)
{
#ifdef DEBUG_OUTPUT
    cout << msg << endl;
#endif
}

void printDebugRemove(int v, int step, int time)
{
#ifdef DEBUG_OUTPUT
    cout << "Remove: " << v << " at " << step << " since last time: " << time << endl;
#endif
}
void printDebugAdd(int v, int step, int time)
{
#ifdef DEBUG_OUTPUT
    cout << "Add: " << v << " at " << step << " since last time: " << time << endl;
#endif
}

//不支配v
inline void Undom(int v)
{
#ifndef NDEBUG
    std::cout << "Undom\t" << v << std::endl;
#endif
    index_in_undom_stack[v] = undom_stack_fill_pointer;
    Push(v, undom_stack);
} //向undom中push v

//支配v
inline void Dom(int v)
{
#ifndef NDEBUG
    std::cout << "Dom\t" << v << std::endl;
#endif
    int index, last_undom_vertex;

    last_undom_vertex = Pop(undom_stack);
    index = index_in_undom_stack[v];
    undom_stack[index] = last_undom_vertex;
    index_in_undom_stack[last_undom_vertex] = index;
} //将v从undomstack中移除

void increase_dominate(long v, long source_v)
{
    if (dominated[v] == 0)
    { //该点被支配了，因此分数要降低
        --score[v];
        subscore[v] -= frequency[v];
        // all make but one break
        for (int i = 0; i < v_degree[v]; ++i)
        {
            long u = v_adj[v][i];
            --score[u];
            subscore[u] -= frequency[v];
        }
        Dom(v);
        onlydominate[v] = source_v; //libohan_10_6
        isgrey[v] = 1;
        indexingreypoint[v] = greypointnum;
        greypointset[greypointnum++] = v; //新被支配的白点变成灰点

    } //一个原本不被支配的点变得被支配了，该点以及周围的点都要分数-1
    else if (dominated[v] == 1)
    {
        if (v_in_c[v] == 1)
        {
            ++score[v];
            subscore[v] += frequency[v];
        } //本来该点如果被删除的话则该点就不被支配，但是，现在不会了，因为被其他点先支配了
        else
        {
            // TODO: speed up
            //      for (int i = 0; i < v_degree[v]; ++i) {
            //        int u = v_adj[v][i];
            //        if (v_in_c[u]) {
            //          ++score[u];
            //            subscore[u]+=frequency[v];
            //          break;
            //        }
            //      }
            long u = onlydominate[v];
            ++score[u];
            subscore[u] += frequency[v];
        } //如果该点a原先只被b支配了，但前被另外的点支配了，那么原来b删除后，a就不被支配，但是现在不会不被支配，所以分数+1
    }
    ++dominated[v];
    //  if (dominated[v] == 1) {
    //    conf_change[source_v] = 0;
    //  }//由于source_v的加入，使得v变得从不支配变成被支配？？如果不存在让v从不支配变成支配的情况，则不需要将source_v变为confC=0，在remove中要看其是否为0，如果是0表示该点刚刚被加入
    //  if (dominated[v] == 2) {//？？这句话应该改成else
    //    if (v != source_v) {
    //      conf_change[v] = 1;
    //    }//如果该点v从被支配一次变成被支配2次，则该点的confC变为1，因为相当于该点的周围邻居的conf加入了C
    //    for (int i = 0; i < v_degree[v]; ++i) {
    //      int u = v_adj[v][i];
    //      if (u != source_v) {//？为什么不是U？？
    //        conf_change[u] = 1;//此处做了2阶CC？？？为啥此处改变了confC但是decreaseDominate没有改变？
    //      }
    //    }
    //  }
}

bool test_score();

void addUpdate(int v)
{
    int u;
    int uinc;
    childnum[v] = 0;
    for (int i = 0; i < v_degree[v]; i++)
    {
        u = v_adj[v][i];
        if (v_in_c[u] == 1)
        {
            uinc = u;
            //找一个枝干结点
            if (childnum[u] != 0 && u != root)
            {
                childnum[u]++;
                father[v] = u;
                indextoberemoved[v] = toberemovedNum;
                toberemoved[toberemovedNum++] = v; //v加在非叶节点上
                inToberemoved[v] = 1;
                return;
            }
        }
    }
    if (uinc == root)
    {
        if (childnum[root] == 1 && v_fixed[root] == 0)
        {
            int indRemove = indextoberemoved[uinc];
            toberemoved[indRemove] = v;
            indextoberemoved[v] = indRemove;
            inToberemoved[v] = 1;
            inToberemoved[uinc] = 0;
        }
        else
        {
            indextoberemoved[v] = toberemovedNum;
            toberemoved[toberemovedNum++] = v; //v加在非叶节点上
            inToberemoved[v] = 1;
        }
        father[uinc] = v;
        childnum[v] = 1;
        father[v] = 0;
        root = v;
    }
    else
    {
        childnum[uinc]++;
        father[v] = uinc;
        if (v_fixed[uinc] == 0)
        {
            int indRemove = indextoberemoved[uinc];
            toberemoved[indRemove] = v;
            indextoberemoved[v] = indRemove;
            inToberemoved[v] = 1;
            inToberemoved[uinc] = 0; //如果没有找到不是叶子节点的C中邻居，那就将是叶子的C中邻居换成v
        }
        else
        {
            indextoberemoved[v] = toberemovedNum;
            toberemoved[toberemovedNum++] = v; //v加在非叶节点上
            inToberemoved[v] = 1;
        }
    }
}
bool Add(int v, int choice)
{
    if (v_in_c[v] == 1 || v < 0)
        return false; //不会重复加入
    isgrey[v] = 0;
    int indextmp = indexingreypoint[v];
    int last = greypointset[--greypointnum];
    indexingreypoint[last] = indextmp;
    greypointset[indextmp] = last; //新加入的点原先是灰点，要删除

    int new_score = -score[v];
    int new_subscore = -subscore[v];

    increase_dominate(v, v);
    for (int i = 0; i < v_degree[v]; ++i)
    {
        long u = v_adj[v][i];

        increase_dominate(u, v);
    }
    score[v] = new_score;
    subscore[v] = new_subscore;
    v_in_c[v] = 1;
    currentWeight += weight_backup[v];
    c_size++;
    candidate[candidate_size] = v; //新加入的点总是最后加入
    index_in_candidate[v] = candidate_size++;
    int last_out_candidate_v = outof_candidate[--outof_candidate_size];
    int index = index_in_outofcandidate[v];
    outof_candidate[index] = last_out_candidate_v;
    index_in_outofcandidate[last_out_candidate_v] = index; //将index in candidate中去掉v

    //    if(dominated[v]!=0){
    //        connectedNum-=(calCV(v)-1);
    //        joinV(v);//加入一个灰色点之后需要对其周围点几起自身的Pre进行调整
    //    }
    //    else
    //        connectedNum++;

    if (choice == 0)
    {
        int u;
        int tmp = conf_change[v];
        for (int i = 0; i < v_degree[v]; i++)
        {
            u = v_adj[v][i];
            for (int j = 0; j < v_degree[u]; j++)
                conf_change[v_adj[u][j]] = 1; //二阶CC
            conf_change[u] = 1;               //一阶CC
        }
        conf_change[v] = tmp;
    }                                                 //与CC相关
                                                      //    taburemove[v] =step +maxNeighborSize *(tabutenue + rand() % 10); //加入点后设置禁忌,与tabu相关
    taburemove[v] = (step + tabutenue + rand() % 10); //加入点后设置禁忌
    //    if(v==choosedremove_v)
    //        taburemove[v]+=100;
    return true;

} //添加点v

void decrease_dominate(int v)
{
    if (dominated[v] == 1)
    {
        // all score of neighbours are make
        ++score[v];
        subscore[v] += frequency[v];
        for (int i = 0; i < v_degree[v]; ++i)
        {
            int u = v_adj[v][i];
            ++score[u];
            subscore[u] += frequency[v];
        }
        Undom(v);
        isgrey[v] = 0;
        int indextmp = indexingreypoint[v];
        int last = greypointset[--greypointnum];
        greypointset[indextmp] = last;
        indexingreypoint[last] = indextmp;
    } //如果只被支配了一次的点的被支配数要下降，则该点不被支配了，则该点及其周围的点的分数+1，并且undom这个点&&对于连通的黑点的情况不可能出现这样的情况，因此分数只会下降
    else if (dominated[v] == 2)
    {
        // only need to update one break
        if (v_in_c[v])
        {
            --score[v];
            subscore[v] -= frequency[v];
        } //如果在C中并且被支配了两次，则这两次一次被自己支配，一次被删掉的点支配，则分数为删掉自己后的分数，会多减少1，因为删除自己后，自己被无支配了
        else
        {
            // TODO: speed up this procedure
            for (int i = 0; i < v_degree[v]; ++i)
            {
                long u = v_adj[v][i];
                if (v_in_c[u])
                {
                    --score[u]; //如果该点d被两个点支配了，那么一个是被删的点，另一个是另一个在C中的点p，删除p后，d会不被支配
                    subscore[u] -= frequency[v];
                    onlydominate[v] = u;
                    break;
                }
            }
        }
    }
    --dominated[v];
} //对分数的操作前提是该点不是被删除的点

void removeUpdate(int v)
{
    if (v != root)
    {
        int fa = father[v];
        childnum[fa]--;
        int indRemove = indextoberemoved[v];
        if (fa != root)
        {
            //fa成为新的叶子结点
            if (childnum[fa] == 0 && v_fixed[fa] == 0)
            {
                toberemoved[indRemove] = fa;
                indextoberemoved[fa] = indRemove;
                inToberemoved[v] = 0;
                inToberemoved[fa] = 1;
            }
            else
            {
                int last = toberemoved[--toberemovedNum];
                toberemoved[indRemove] = last;
                indextoberemoved[last] = indRemove;
                inToberemoved[v] = 0;
            }
        }
        else
        {
            //fa == root && fa未被fix
            if (childnum[fa] == 1 && v_fixed[fa] == 0)
            {
                toberemoved[indRemove] = fa;
                indextoberemoved[fa] = indRemove;
                inToberemoved[v] = 0;
                inToberemoved[fa] = 1;
            }
            else
            {
                //fa == root && fa被fix
                int last = toberemoved[--toberemovedNum];
                toberemoved[indRemove] = last;
                indextoberemoved[last] = indRemove;
                inToberemoved[v] = 0;
            }
        } //父亲是根结点
    }     //v不是根结点
    else
    {
        int chi;
        int indRemove = indextoberemoved[v];
        for (int i = 0; i < v_degree[v]; i++)
        {
            int u = v_adj[v][i];
            if (v_in_c[u] == 1 && father[u] == v)
            {
                chi = u;
                break;
            }
        }
        //重设根节点
        root = chi;
        if (childnum[chi] == 1 && v_fixed[chi] == 0)
        {
            toberemoved[indRemove] = chi;
            indextoberemoved[chi] = indRemove;
            inToberemoved[v] = 0;
            inToberemoved[chi] = 1;
        }
        else
        {
            int last = toberemoved[--toberemovedNum];
            toberemoved[indRemove] = last;
            indextoberemoved[last] = indRemove;
            inToberemoved[v] = 0;
        }
    } //v是根结点
    father[v] = 0;
    childnum[v] = 0;
} //在删除点后对待删除点集进行更新

//从C中删除点，相应地将该点从candidate中删除，并改变周围点的支配次数和分数，最后将自身的分数取相反数
bool Remove(int v, int choice)
{
    isgrey[v] = 1;
    indexingreypoint[v] = greypointnum;
    greypointset[greypointnum++] = v; //删掉一个非割点后，这个点肯定是灰点
    v_in_c[v] = 0;
    currentWeight -= weight_backup[v];
    c_size--;
    outof_candidate[outof_candidate_size] = v; //新加入的点总是最后加入
    index_in_outofcandidate[v] = outof_candidate_size++;
    int last_candidate_v = candidate[--candidate_size];
    int index = index_in_candidate[v];
    candidate[index] = last_candidate_v;
    index_in_candidate[last_candidate_v] = index; //将index in candidate中去掉v
    // from break to 0, then from 0 to break;
    int new_score = -score[v];
    int new_subscore = -subscore[v];
    decrease_dominate(v);
    int neighbours = v_degree[v];
    for (int i = 0; i < neighbours; ++i)
    {
        int u = v_adj[v][i];
        decrease_dominate(u);
    }                     //对v及其周围邻居u进行decreaseDominate
    score[v] = new_score; //？？一个在C中的点的分数为负数，如果将其删除后其分数为正数，且正好是相反数
    subscore[v] = new_subscore;
    //pre[v]=v;//删除了一个非割点之后TODO:如果删除的点是标号最小的点情况
    if (choice == 0)
    {
        int u;
        for (int i = 0; i < v_degree[v]; i++)
        {
            u = v_adj[v][i];
            for (int j = 0; j < v_degree[u]; j++)
                conf_change[v_adj[u][j]] = 1; //二阶CC
            conf_change[u] = 1;               //一阶CC
        }
        conf_change[v] = 0;
    } //用CC

    tabuadd[v] = step + maxNeighborSize * (tabutenue + rand() % 10);
    //    tabuadd[v]=step+(tabutenue+rand()%10);
    return true;
}

//增加权重，只对白点及其周围点
//TODO:权重设置阈值
void addWeight(int node)
{
    frequency[node]++;
    for (int i = 0; i < v_degree[node]; i++)
        subscore[v_adj[node][i]]++;
    subscore[node]++;
} //增加权重的总是白色点，周围及其自身的分数都要增加

void minusWeight(int node, int tobeminus)
{
    frequency[node] -= tobeminus;
    if (dominated[node] == 0)
    {
        for (int i = 0; i < v_degree[node]; i++)
            subscore[v_adj[node][i]] -= tobeminus;
        subscore[node] -= tobeminus;
    } //白点
    else if (v_in_c[node] == 0)
    {
        if (dominated[node] == 1)
        {
            long u = onlydominate[node];
            subscore[u] += tobeminus;
            //            for(int i=0;i<v_degree[node];i++){
            //                if(v_in_c[v_adj[node][i]]==1)
            //                    subscore[v_adj[node][i]]++;
            //            }
        }
    } //不是白点且不是黑点，则为灰点，减权重之后要将仅仅支配的点分数+1
}

bool cmp(int a, int b)
{
    return (subscore[a] / weight_backup[a] > subscore[b] / weight_backup[b]);
}

/**
 if there is no non-tabu operation then choose top 40%

    @return choosen vertex
    */
int ChooseRemoveVTopof()
{
    int v;
    int best_score = -weightthreshold;
    int best_remove_v = -1;
    const int toberemovedNum1 = (int)(candidate_size);
    int toberemoved1[toberemovedNum1];
    int topIndex = 0;
    int curscore;
    for (int i = 0; i < candidate_size; i++)
    {
        v = candidate[i];
        if (isCut[v] == 1 || v_fixed[v] == 1 || v == choosedadd_v)
            continue;
        curscore = score[v];
        curscore = subscore[v];
        toberemoved1[topIndex++] = v;
        if (step > taburemove[v])
        {
            //if(score[v]>best_score)
            if (curscore > best_score)
            {
                best_remove_v = v;
                best_score = curscore;
            }
            //            else if(curscore==best_score&&time_stamp[v]<time_stamp[best_remove_v])
            //                best_remove_v=v;
            else if (curscore == best_score)
                if ((dominated[v] < dominated[best_remove_v]) || (dominated[v] == dominated[best_remove_v] && time_stamp[v] < time_stamp[best_remove_v]))
                    best_remove_v = v;
        }
    }
    if (best_remove_v != -1)
    {
        return best_remove_v;
    }
    else
    {
        sort(toberemoved1, toberemoved1 + topIndex, cmp);
        int topof = (topIndex * 0.4) + 1;
        return toberemoved1[rand() % topof];
    }
} //挑选删除点的阶段使用最优选择，选择分数最大的中最老的点

/**
     if there is no non-tabu operation then choose top 40%
    @param count: select vertex count
    @param chioce: 0 select from candidate; 1 select from toberemoved set;
    @return choosen vertex
    */
int ChooseRemoveVTopofBMS(int count, int choice)
{
    int v, i;
    double best_score = -weightthreshold;
    int best_remove_v = -1;
    const int toberemovedNum1 = (int)(count);
    int toberemoved1[toberemovedNum1];
    int topIndex = 0;
    double cscore; // subscore/weight，取最大，因为此时subscore是负的
    for (i = 0; i < count; i++)
    {
        if (choice == 1)
        {
            v = toberemoved[rand() % toberemovedNum]; //1为toberemoved
            if (v_fixed[v] == 1)
                continue;
        }
        else
        {
            v = candidate[rand() % candidate_size]; //0为candidate
            if (v_fixed[v] == 1 || isCut[v] == 1)
                continue;
        }
        cscore = subscore[v] / weight_backup[v];
        toberemoved1[topIndex++] = v;
        if (step > taburemove[v])
        {
            //if(score[v]>best_score)
            if (cscore > best_score)
            {
                best_remove_v = v;
                best_score = cscore;
            }
            else if (cscore == best_score)
                if ((dominated[v] < dominated[best_remove_v]) || (dominated[v] == dominated[best_remove_v] && time_stamp[v] < time_stamp[best_remove_v]))
                    best_remove_v = v; //关乎safety！！！
        }
    }
    if (best_remove_v != -1)
    {
        return best_remove_v;
    }
    else
    {
        //从score值最大的前40%中随机选择顶点
        sort(toberemoved1, toberemoved1 + topIndex, cmp);
        int topof = (topIndex * 0.4) + 1;
        return toberemoved1[rand() % topof];
    }
}

int ChooseRemoveVFromArray(Array *removedNodeNeighbor)
{
    int best_remove_v = -1;
    double cscore;
    double best_cscore = -weightthreshold;
    for (size_t i = 0; i < removedNodeNeighbor->size(); i++)
    {
        int v = removedNodeNeighbor->element_at(i);
        if (v_in_c[v] == 0 || inToberemoved[v] == 0 || v_fixed[v] == 1 || isCut[v] == 1)
            continue;
        cscore = subscore[v] / weight_backup[v];
        if (step > taburemove[v])
        {
            if (cscore > best_cscore)
            {
                best_remove_v = v;
                best_cscore = cscore;
            }
            else if (cscore == best_cscore)
            {
                if ((dominated[v] < dominated[best_remove_v]) || (dominated[v] == dominated[best_remove_v] && time_stamp[v] < time_stamp[best_remove_v]))
                {
                    best_remove_v = v;
                }
            }
        }
    }
    return best_remove_v;
}

int ChooseAddVsubscore(int count = 100)
{
    // TODO: proof there is at least one avaible add_v
    int base_v, add_v;
    int curScore;
    long best_score = -weightthreshold;
    int best_add_v = -1;
    long best_score_outCC = -weightthreshold;
    int best_add_v_outCC;
    vector<int> promisinggreypoint;
    std::vector<int> best_vec;
    for (int i = 0; i < undom_stack_fill_pointer; ++i)
    {
        base_v = undom_stack[i];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (isgrey[add_v])
            {
                if (add_v != choosedremove_v)
                    promisinggreypoint.push_back(add_v);
                curScore = score[add_v]; //灰点
                curScore = subscore[add_v];
                if (conf_change[add_v] == 1)
                {
                    if (curScore > best_score)
                    {
                        best_add_v = add_v;
                        best_score = curScore;
                    }
                    else if (curScore == best_score)
                    {
                        if ((dominated[add_v] > dominated[best_add_v]) || (dominated[add_v] == dominated[best_add_v] && time_stamp[add_v] < time_stamp[best_add_v]))
                            best_add_v = add_v;
                    }
                }
            }
        }
    }
    if (best_add_v != -1)
        return best_add_v; //存在：：此处总是会存在的可以删除的点，所以永远不会用解禁策略
    else
    {
        int greysize = promisinggreypoint.size();
        for (int i = 0; i < count; i++)
        {
            add_v = promisinggreypoint[rand() % greysize];
            curScore = subscore[add_v];
            if (curScore > best_score_outCC)
            {
                best_score_outCC = curScore;
                best_add_v_outCC = add_v;
            }
            else if (curScore == best_score)
            {
                if ((dominated[add_v] > dominated[best_add_v_outCC]) || (dominated[add_v] == dominated[best_add_v_outCC] && time_stamp[add_v] < time_stamp[best_add_v_outCC]))
                    best_add_v_outCC = add_v;
            }
        }
        if (best_add_v_outCC != -1)
            return best_add_v_outCC;
        else
            return choosedremove_v;
    }
}

/*
最优选择法选一个顶点
*/
int ChooseAddVsubscorefast(int count = 40)
{
    // TODO: proof there is at least one avaible add_v
    int base_v, add_v;
    double cscore; //  subscore/weight，取最大
    double best_score = -weightthreshold;
    int best_add_v = -1;
    std::vector<int> best_vec;
    for (int i = 0; i < undom_stack_fill_pointer; ++i)
    {
        base_v = undom_stack[i];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (isgrey[add_v])
            {
                if (weight_backup[add_v] + currentWeight > bestWeight)
                    continue;
                cscore = subscore[add_v] / weight_backup[add_v];
                if (conf_change[add_v] == 1)
                {
                    if (cscore > best_score)
                    {
                        best_add_v = add_v;
                        best_score = cscore;
                    }
                    else if (cscore == best_score)
                    {
                        if ((dominated[add_v] > dominated[best_add_v]) || (dominated[add_v] == dominated[best_add_v] && time_stamp[add_v] < time_stamp[best_add_v]))
                            best_add_v = add_v;
                    } //关乎safety
                }
            }
        }
    }
    if (best_add_v != -1)
        return best_add_v; //存在：：此处总是会存在的可以添加的点，所以永远不会用解禁策略
    else
    {
        best_add_v = -1;
        best_score = -weightthreshold;
        map<int, int> m;
        int mindex = 0;
        for (int i = 0; i < count; i++)
        {
            base_v = undom_stack[rand() % undom_stack_fill_pointer];
            if (m.find(base_v) == m.end())
                m[mindex++] = base_v;
            else
            {
                continue;
            }
            for (int j = 0; j < v_degree[base_v]; ++j)
            {
                add_v = v_adj[base_v][j];
                if (isgrey[add_v])
                {
                    if (weight_backup[add_v] + currentWeight > bestWeight)
                        continue;
                    cscore = subscore[add_v] / weight_backup[add_v];
                    if (cscore > best_score)
                    {
                        best_add_v = add_v;
                        best_score = cscore;
                    }
                    else if (cscore == best_score)
                    {
                        if ((dominated[add_v] > dominated[best_add_v]) || (dominated[add_v] == dominated[best_add_v] && time_stamp[add_v] < time_stamp[best_add_v]))
                            best_add_v = add_v;
                    }
                }
            }
        }
        if (best_add_v != -1)
            return best_add_v;
        else
            return greypointset[rand() % greypointnum];
    }
}

long ChooseAddtabu(int count = 10)
{
    long base_v, add_v;
    int curScore;
    int best_score = -1;
    long best_add_v = -1;
    int best_score_outtabu = -1;
    long best_add_v_outtabu = -1;
    map<int, long> m;
    int Mindex = 0;
    for (int i = 0; i < count; ++i)
    {
        base_v = undom_stack[rand() % undom_stack_fill_pointer];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (isgrey[add_v] && add_v != choosedremove_v)
            {
                if (m.find(add_v) != m.end())
                    continue;
                m[Mindex++] = add_v;
                curScore = score[add_v]; //灰点
                curScore = subscore[add_v];
                if (curScore > best_score_outtabu)
                {
                    best_add_v_outtabu = add_v;
                    best_score_outtabu = curScore;
                }
                else if (curScore == best_score_outtabu)
                {
                    //if((dominated[add_v]>dominated[best_add_v])||(dominated[add_v]==dominated[best_add_v]&&time_stamp[add_v]<time_stamp[best_add_v]))
                    if (time_stamp[add_v] < time_stamp[best_add_v])
                        best_add_v_outtabu = add_v;
                }
                if (step >= tabuadd[add_v])
                {
                    if (curScore > best_score)
                    {
                        best_add_v = add_v;
                        best_score = curScore;
                    }
                    else if (curScore == best_score)
                    {
                        if ((dominated[add_v] > dominated[best_add_v]) || (dominated[add_v] == dominated[best_add_v] && time_stamp[add_v] < time_stamp[best_add_v]))
                            best_add_v = add_v;
                    }
                }
            }
        }
    }
    if (undom_stack_fill_pointer - best_score_outtabu < minUndom)
        return best_add_v_outtabu; //解禁
    else if (best_add_v != -1)
        return best_add_v; //限制下的最优解
    else if (Mindex > 0)
        return m[rand() % Mindex];
    else
    {
        //v_fixed[choosedremove_v]=1;
        return choosedremove_v; //对于删除点后一块稠密区域的情况libohan
    }
}

/// 返回节点i是否为刚刚被删除的点
/// @param node 节点i
bool checkLastRemoved(int node)
{
    bool flag = false;
    for (int i = 0; i < LastRemovedIndex; i++)
    {
        if (node == LastRemoved[i])
        {
            flag = true;
            break;
        }
    }
    return flag;
}
int ChooseAddVtabufast(int count = 40)
{
    // TODO: proof there is at least one avaible add_v
    int base_v, add_v;
    double cscore; //  subscore/weight，取最大
    double best_score = -weightthreshold;
    int best_add_v = -1;
    vector<int> lastRemovedTobeAdd; //刚刚被删除，并且满足：是灰点，在白点周围，加上后权重不超（由于刚刚被删，肯定是被禁忌的）
    map<int, int> m;
    int best_outtabu_addv = -1;
    double best_outtabu_score = -weightthreshold;
    //    std::vector<int> best_vec;
    const int tobeaddNum1 = (int)(undom_stack_fill_pointer * v_degree[maxDegreeNode]);
    int tobeadd1[tobeaddNum1];
    int topIndex = 0;
    for (int i = 0; i < undom_stack_fill_pointer; ++i)
    {
        base_v = undom_stack[i];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (m.find(add_v) == m.end() && isgrey[add_v] && weight_backup[add_v] + currentWeight < bestWeight)
            //仅仅考虑白点周围的，加进来不让总权重超出的灰点,并且不重复考虑点
            {
                m[topIndex] = add_v;
                if (!checkLastRemoved(add_v))
                {
                    tobeadd1[topIndex++] = add_v;
                    cscore = subscore[add_v] / weight_backup[add_v];
                    if (cscore > best_outtabu_score)
                    {
                        best_add_v = add_v;
                        best_outtabu_score = cscore; //纪录下最好的符合条件的点(无禁忌)
                    }
                } //如果add_v刚刚不是被删除，则放进tobeadd1中
                else
                    lastRemovedTobeAdd.push_back(add_v); //如果add_v刚刚被删除，则放进lastRemovedtobeAdd中

                if (step >= tabuadd[add_v]) //刚被删除的点必然被禁忌，因此进不来这里
                {
                    if (cscore > best_score)
                    {
                        best_add_v = add_v;
                        best_score = cscore;
                    }
                    else if (cscore == best_score)
                    {
                        if ((dominated[add_v] > dominated[best_add_v]) || (dominated[add_v] == dominated[best_add_v] && time_stamp[add_v] < time_stamp[best_add_v]))
                            best_add_v = add_v;
                    } //关乎safety
                }
            }
        }
    }
    //TODO::解禁的处理
    //    if(undom_stack_fill_pointer-score[best_outtabu_addv]<minUndom&&rightAfternewlow==false)
    //        return best_outtabu_addv;//如果不是刚刚更新过最好权重，并且刷新了最小未支配点数，就解禁
    if (best_add_v != -1)
        return best_add_v; //此处找到未被禁的最大分数的点
    else
    {
        if (topIndex != 0)
        {
            sort(tobeadd1, tobeadd1 + topIndex, cmp);
            int topof = (topIndex * 0.4) + 1;
            return tobeadd1[rand() % topof]; //如果白点周围的，并且不是刚刚被删除的，加上后权重不超的灰点存在的话，就随机选前40%
        }
        else
        {
            if (lastRemovedTobeAdd.size() != 0)
            {
                return lastRemovedTobeAdd[rand() % lastRemovedTobeAdd.size()];
            }
            else
                return -1;
        }
    }
} //如果可以找到一个能让如果能找到不被tabu，并且在白点周围，并且加上后权重不超出的灰点，则选它，
//否则随机从白点周围的，并且不是刚刚被删除的，加上后权重不超的，灰点集合中选一个前40%的任一个，(不管tabu)
//如果还没有，就找从刚刚被删的中找一个，在白点周围，加上后权重不超的灰点中随机找一个
//如果还没有，说明找不到在白点周围，加上后权重不超的灰点，返回-1
//解禁策略：找到所有的里面cscore最大的，如果这个点加入可以减少最小未支配数就选它（尚未实现）

int ChooseAddVtabufastNolastRemove(int count = 40)
{
    // TODO: proof there is at least one avaible add_v
    int base_v, add_v;
    double cscore; //  subscore/weight，取最大
    double best_score = -weightthreshold;
    int best_add_v = -1;
    vector<int> lastRemovedTobeAdd; //刚刚被删除，并且满足：是灰点，在白点周围，加上后权重不超（由于刚刚被删，肯定是被禁忌的）
    map<int, int> m;
    int best_outtabu_addv = -1;
    double best_outtabu_score = -weightthreshold;
    //    std::vector<int> best_vec;
    const int tobeaddNum1 = (int)(undom_stack_fill_pointer * v_degree[maxDegreeNode]);
    int tobeadd1[tobeaddNum1];
    int topIndex = 0;
    for (int i = 0; i < undom_stack_fill_pointer; ++i)
    {
        base_v = undom_stack[i];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (m.find(add_v) == m.end() && isgrey[add_v] && weight_backup[add_v] + currentWeight < bestWeight)
            //仅仅考虑白点周围的，加进来不让总权重超出的灰点,并且不重复考虑点
            {
                m[topIndex] = add_v;
                //                if(!checkLastRemoved(add_v)){//////////
                tobeadd1[topIndex++] = add_v;
                cscore = subscore[add_v] / weight_backup[add_v];
                if (cscore > best_outtabu_score)
                {
                    best_add_v = add_v;
                    best_outtabu_score = cscore; //纪录下最好的符合条件的点(无禁忌)
                }
                //                }//如果add_v刚刚不是被删除，则放进tobeadd1中
                //                else
                //                    lastRemovedTobeAdd.push_back(add_v);//如果add_v刚刚被删除，则放进lastRemovedtobeAdd中

                if (step >= tabuadd[add_v]) //刚被删除的点必然被禁忌，因此进不来这里
                {
                    if (cscore > best_score)
                    {
                        best_add_v = add_v;
                        best_score = cscore;
                    }
                    else if (cscore == best_score)
                    {
                        if ((dominated[add_v] > dominated[best_add_v]) || (dominated[add_v] == dominated[best_add_v] && time_stamp[add_v] < time_stamp[best_add_v]))
                            best_add_v = add_v;
                    } //关乎safety
                }
            }
        }
    }
    //TODO::解禁的处理
    //    if(undom_stack_fill_pointer-score[best_outtabu_addv]<minUndom&&rightAfternewlow==false)
    //        return best_outtabu_addv;//如果不是刚刚更新过最好权重，并且刷新了最小未支配点数，就解禁
    if (best_add_v != -1)
        return best_add_v; //此处找到未被禁的最大分数的点
    else
    {
        if (topIndex != 0)
        {
            sort(tobeadd1, tobeadd1 + topIndex, cmp);
            int topof = (topIndex * 0.4) + 1;
            return tobeadd1[rand() % topof]; //如果白点周围的，并且不是刚刚被删除的，加上后权重不超的灰点存在的话，就随机选前40%
        }
        else
        {
            //            if(lastRemovedTobeAdd.size()!=0){
            //                return lastRemovedTobeAdd[rand()%lastRemovedTobeAdd.size()];
            //            }
            //            else
            return -1;
        }
    }
}

//标记候选解中割点
void MarkCut()
{
    ind = 0;
    for (int i = 0; i < cutIndex; i++)
        isCut[cutPointSet[i]] = 0; //将之前割点给还
    //toberemoved恢复初始状态
    toberemovedNum = 0;
    cutIndex = 0;
    root = candidate[rand() % candidate_size];

    //cutPoint(root,root);
    cutPointNoRecur(root);

    for (int i = 0; i < candidate_size; i++)
        child[candidate[i]] = low[candidate[i]] = dnf[candidate[i]] = 0;
    for (int i = 0; i < fixedNum; i++)
        child[fixedSet[i]] = low[fixedSet[i]] = dnf[fixedSet[i]] = 0;
}

/*
TODO:
构造树应该有3个目标，
与之前的树不同
叶结点尽量多
叶结点的权重和尽量小

可以加一些启发式信息
利用当前的权重信息

有两个方案，一个是利用原来的权重，一个是利用搜索中改变过的权重
*/
void MarkCuttree()
{
    ind = 0;
    for (int i = 0; i < toberemovedNum; i++)
        inToberemoved[toberemoved[i]] = 0;
    toberemovedNum = 0;
    //    root = candidate[rand() % candidate_size];
    //TODO:生成树的根节点选择会对结果造成影响
    root = candidate[0];
    cutPoint1(root);
    for (int i = 0; i < candidate_size; i++)
        dnf[candidate[i]] = 0;
    for (int i = 0; i < fixedNum; i++)
        dnf[fixedSet[i]] = 0;
}

void localSearchFramework1()
{
    int control = 0;
    while (TimeElapsed() < cutoff_time)
    {
        if (control % 2 == 0)
        {
            Framework1CutTree();
        }
        else
        {
            Framework1Tarjan();
        }
    }
}

void localSearchFramework2()
{
    Framework2Tarjan();
    //    int control = 0;
    //    while (TimeElapsed() < cutoff_time)
    //    {
    //        if (control % 2 == 0)
    //        {
    //            Framework2CutTree();
    //        }
    //        else
    //        {
    //            Framework2Tarjan();
    //        }
    //    }
}

int instance1 = floor1 * insTimes;
int gap1 = floor1 * ceilingTimes;
void Framework1CutTree()
{
    if (candidate_size < 100)
    {
        return;
    }
    try_step = 10000;
    int stepAction = 1; //实际走过的步数
    long NoImpro_trystep = 50000;
    long NOimprovementstep = 0;
    long improvementCount = 0;

    int neighborSize = 1;
    MarkCuttree();
    while (true)
    {
        if (undom_stack_fill_pointer == 0 && toberemovedNum == 0)
        {
            return;
        }
        if (stepAction % try_step == 0)
        {
            int timenow = TimeElapsed();
            if (timenow > cutoff_time)
            {
                return;
            }
            //时间片结束退出
            if (stepAction > gap1)
            {
                if (improvementCount > 10)
                {
                    instance1 -= floor1;
                    if (instance1 < floor1)
                    {
                        instance1 = floor1;
                    }
                }
                return;
            }
            //局部搜索中
            if (NOimprovementstep > instance1)
            {
                instance1 += floor1;
                if (instance1 > gap1)
                {
                    instance1 = gap1;
                }
                return;
            }
        }
        //选点删除
        removedNodeNeighbor->clear();
        for (size_t i = 0; i < neighborSize; i++)
        {
            int best_removed_v = -1;
            if (i == 0)
            {
                //BMS从可移除列表选一个点删除
                best_removed_v = ChooseRemoveVTopofBMS(100, 1);
            }
            else
            {
                //从removedNodeNeighbors中随机选一个点
                if (removedNodeNeighbor->size() != 0 && toberemovedNum != 0)
                {
                    best_removed_v = ChooseRemoveVFromArray(removedNodeNeighbor);
                }
            }
            if (best_removed_v != -1)
            {
                Remove(best_removed_v, 1);
                removeUpdate(best_removed_v);
                // removedNodeNeighbor->delete_element(best_removed_v);
                for (int n = 0; n < v_degree[best_removed_v]; ++n)
                {
                    int neighbor = v_adj[best_removed_v][n];
                    if (v_in_c[neighbor] == 1 && inToberemoved[neighbor] == 1 && !removedNodeNeighbor->is_in_array(neighbor))
                    {
                        removedNodeNeighbor->insert_element(neighbor);
                    }
                }
                time_stamp[best_removed_v] = step;
                step++;
            }
        }
        //选点添加
        while (undom_stack_fill_pointer != 0)
        {
            int best_add_v = ChooseAddVsubscorefast();
            // int best_add_v = ChooseAddVtabufast();
            Add(best_add_v, 1);
            addUpdate(best_add_v);
            time_stamp[best_add_v] = step;
            //增加未支配顶点权重
            for (size_t i = 0; i < undom_stack_fill_pointer; i++)
            {
                addWeight(undom_stack[i]);
            }
            totalweight += undom_stack_fill_pointer;
            if (totalweight > weightthreshold)
            {
                updateWeight();
            }
            step++;
        }
        //已全部支配，判断当前解是否为更优解
        if (currentWeight < bestWeight)
        {
            UpdateBestSolution();
            neighborSize = 1;
            minUndom = undom_stack_fill_pointer;
            improvementCount++;
            NOimprovementstep = 0;
        }
        else
        {
            NOimprovementstep++;
            if (neighborSize > maxNeighborSize)
            {
                neighborSize = 1;
            }
            else
            {
                neighborSize++;
            }
        }
        stepAction++;
        if (NOimprovementstep % NoImpro_trystep == 0)
        {
            MarkCuttree();
        }
    }
}

void Framework2CutTree()
{
    if (candidate_size < 100)
    {
        return;
    }
    try_step = 10000;
    int stepAction = 1; //实际走过的步数
    long NoImpro_trystep = 50000;
    long NOimprovementstep = 0;
    long improvementCount = 0;

    int neighborSize = 1;
    MarkCuttree();
    while (true)
    {
        if (undom_stack_fill_pointer == 0 && toberemovedNum == 0)
        {
            return;
        }
        if (stepAction % try_step == 0)
        {
            int timenow = TimeElapsed();
            if (timenow > cutoff_time)
            {
                return;
            }
            //时间片结束退出
            if (stepAction > gap1)
            {
                if (improvementCount > 10)
                {
                    instance1 -= floor1;
                    if (instance1 < floor1)
                    {
                        instance1 = floor1;
                    }
                }
                return;
            }
            //局部搜索中
            if (NOimprovementstep > instance1)
            {
                instance1 += floor1;
                if (instance1 > gap1)
                {
                    instance1 = gap1;
                }
                return;
            }
        }
        //选点删除
        removedNodeNeighbor->clear();
        for (size_t i = 0; i < neighborSize; i++)
        {
            int best_removed_v = -1;
            if (i == 0)
            {
                //BMS从可移除列表选一个点删除
                best_removed_v = ChooseRemoveVTopofBMS(100, 1);
            }
            else
            {
                //从removedNodeNeighbors中随机选一个点
                if (removedNodeNeighbor->size() != 0 && toberemovedNum != 0)
                {
                    best_removed_v = ChooseRemoveVFromArray(removedNodeNeighbor);
                }
            }
            if (best_removed_v != -1)
            {
                Remove(best_removed_v, 1);
                removeUpdate(best_removed_v);
                // removedNodeNeighbor->delete_element(best_removed_v);
                for (int n = 0; n < v_degree[best_removed_v]; ++n)
                {
                    int neighbor = v_adj[best_removed_v][n];
                    if (v_in_c[neighbor] == 1 && inToberemoved[neighbor] == 1 && !removedNodeNeighbor->is_in_array(neighbor))
                    {
                        removedNodeNeighbor->insert_element(neighbor);
                    }
                }
                time_stamp[best_removed_v] = step;
                step++;
            }
        }
        //选点添加
        while (undom_stack_fill_pointer != 0 && currentWeight < bestWeight)
        {
            int best_add_v = ChooseAddVsubscorefast();
            // int best_add_v = ChooseAddVtabufast();
            Add(best_add_v, 1); //集中选点
            addUpdate(best_add_v);

            time_stamp[best_add_v] = step;
            step++;
        }
        //已全部支配，判断当前解是否为更优解
        if (undom_stack_fill_pointer == 0 && currentWeight < bestWeight)
        {
            UpdateBestSolution();
            neighborSize = 1;
            improvementCount++;
            NOimprovementstep = 0;
        }
        else
        {
            for (size_t i = 0; i < undom_stack_fill_pointer; i++)
            {
                addWeight(undom_stack[i]);
            }
            totalweight += undom_stack_fill_pointer;
            //TODO:加入一些平滑策略
            if (totalweight > weightthreshold)
            {
                updateWeight();
            }
            NOimprovementstep++;
            if (neighborSize > maxNeighborSize)
            {
                neighborSize = 1;
            }
            else
            {
                neighborSize++;
            }
        }
        stepAction++;
        if (NOimprovementstep % NoImpro_trystep == 0)
        {
            MarkCuttree();
        }
    }
}

int instance0 = floor0 * insTimes;
int gap0 = floor0 * ceilingTimes;
int flag = 0;
void Framework1Tarjan()
{
    try_step = 10;
    int improvementCount = 0;
    int stepAction = 1;
    long NOimprovementstep = 0;
    fill_n(inToberemoved, v_num + 1, 1);
    fill_n(conf_change, v_num + 1, 1);
    int neighborSize = 1;
    MarkCut();
    while (true)
    {
        if (c_size < 5000 && flag == 0)
        {
            floor0 *= 10;
            gap0 = floor0 * ceilingTimes;
            instance0 = floor0 * insTimes;
            flag = 1;
        }
        if (stepAction % try_step == 0)
        {
            int timenow = TimeElapsed();
            if (timenow > cutoff_time)
                return;
            if (stepAction > gap0)
            { //还在爬坡中
                if (improvementCount > 10)
                {
                    instance0 -= floor0;
                    if (instance0 < floor0)
                        instance0 = floor0;
                }
                return;
            }
            if (NOimprovementstep > instance0)
            { //局部搜索中
                instance0 += floor0;
                if (instance0 > gap0)
                    instance0 = gap0;
                return;
            }
        }
        if (candidate_size == 1)
        {
            return;
        }
        //选点删除
        removedNodeNeighbor->clear();
        for (size_t i = 0; i < neighborSize; i++)
        {
            int best_removed_v = -1;
            if (i == 0)
            {
                //BMS从可移除列表选一个点删除
                best_removed_v = ChooseRemoveVTopofBMS(100, 0);
            }
            else
            {
                //从removedNodeNeighbors中随机选一个点
                if (removedNodeNeighbor->size() != 0 && candidate_size != 0)
                {
                    best_removed_v = ChooseRemoveVFromArray(removedNodeNeighbor);
                }
            }
            if (best_removed_v != -1)
            {
                Remove(best_removed_v, 1);
                //                removedNodeNeighbor->delete_element(best_removed_v);
                if (candidate_size != 0)
                {
                    MarkCut();
                }
                for (int n = 0; n < v_degree[best_removed_v]; ++n)
                {
                    int neighbor = v_adj[best_removed_v][n];
                    if (v_in_c[neighbor] == 1 && isCut[neighbor] == 0 && !removedNodeNeighbor->is_in_array(neighbor))
                    {
                        removedNodeNeighbor->insert_element(neighbor);
                    }
                }
                time_stamp[best_removed_v] = step;
                step++;
            }
        }
        //选点添加
        while (undom_stack_fill_pointer != 0)
        {
            int timenow = TimeElapsed();
            if (timenow > cutoff_time)
            {
                running_is_interrupted = true;
                return;
            }
            int best_add_v = ChooseAddVsubscorefast();
            // int best_add_v = ChooseAddVtabufast();
            Add(best_add_v, 1);
            time_stamp[best_add_v] = step;
            //增加未支配顶点权重
            for (size_t i = 0; i < undom_stack_fill_pointer; i++)
            {
                addWeight(undom_stack[i]);
            }
            totalweight += undom_stack_fill_pointer;
            if (totalweight > weightthreshold)
            {
                updateWeight();
            }
            step++;
        }
        //已全部支配，判断当前解是否为更优解
        if (currentWeight < bestWeight)
        {
            UpdateBestSolution();
            neighborSize = 1;
            minUndom = undom_stack_fill_pointer;
            improvementCount++;
            NOimprovementstep = 0;
        }
        else
        {
            NOimprovementstep++;
            if (neighborSize > maxNeighborSize)
            {
                neighborSize = 1;
            }
            else
            {
                neighborSize++;
            }
        }
        stepAction++;
        MarkCut();
    }
}

void Framework2Tarjan()
{
    try_step = 1000;
    int improvementCount = 0;
    int stepAction = 1;
    long NOimprovementstep = 0;
    fill_n(inToberemoved, v_num + 1, 1);
    fill_n(conf_change, v_num + 1, 1);
    int neighborSize = 1;
    MarkCut();
    while (true)
    {
        if (c_size < 5000 && flag == 0)
        {
            floor0 *= 10;
            gap0 = floor0 * ceilingTimes;
            instance0 = floor0 * insTimes;
            flag = 1;
        }
        if (stepAction % try_step == 0)
        {
            int timenow = TimeElapsed();
            if (timenow > cutoff_time)
                return;
            if (stepAction > gap0)
            { //还在爬坡中
                if (improvementCount > 10)
                {
                    instance0 -= floor0;
                    if (instance0 < floor0)
                        instance0 = floor0;
                }
                //return;
            }
            if (NOimprovementstep > instance0)
            { //局部搜索中
                instance0 += floor0;
                if (instance0 > gap0)
                    instance0 = gap0;
                //return;
            }
        }
        if (candidate_size == 1)
        {
            return;
        }
        //选点删除
        removedNodeNeighbor->clear();
        printDebugMsg("NewTurn");
        LastRemovedIndex = 0;
        for (size_t i = 0; i < neighborSize; i++)
        {
            int best_removed_v = -1;
            if (i == 0)
            {
                //BMS从可移除列表选一个点删除
                best_removed_v = ChooseRemoveVTopofBMS(100, 0);
            }
            else
            {
                //从removedNodeNeighbors中随机选一个点
                if (removedNodeNeighbor->size() != 0 && candidate_size != 0)
                {
                    best_removed_v = ChooseRemoveVFromArray(removedNodeNeighbor);
                }
            }
            if (best_removed_v != -1)
            {
                Remove(best_removed_v, 1);
                printDebugRemove(best_removed_v, step, step - time_stamp[best_removed_v]);
                LastRemoved[LastRemovedIndex++] = best_removed_v;
                //                removedNodeNeighbor->delete_element(best_removed_v);
                if (candidate_size != 0)
                {
                    MarkCut();
                }
                for (int n = 0; n < v_degree[best_removed_v]; ++n)
                {
                    int neighbor = v_adj[best_removed_v][n];
                    if (v_in_c[neighbor] == 1 && isCut[neighbor] == 0 && !removedNodeNeighbor->is_in_array(neighbor))
                    {
                        removedNodeNeighbor->insert_element(neighbor);
                    }
                    for (int m = 0; m < v_degree[neighbor]; m++)
                    {
                        int neinei = v_adj[neighbor][m];
                        if (v_in_c[neinei] == 1 && isCut[neinei] == 0 && !removedNodeNeighbor->is_in_array(neinei))
                        {
                            removedNodeNeighbor->insert_element(neinei);
                        }
                    } //二阶邻居
                }
                time_stamp[best_removed_v] = step;
                step++;
            }
            else
                cout << "failed to remove" << endl;
        }
        //选点添加
        while (undom_stack_fill_pointer != 0 && currentWeight < bestWeight)
        {
            //           int best_add_v = ChooseAddVsubscorefast();
            int best_add_v = ChooseAddVtabufast();
            //            int best_add_v=ChooseAddVtabufastNolastRemove();
            if (best_add_v == -1)
                break; //如果没有找到白点周围的，能让权重和不超的灰点，则退出循环
            printDebugAdd(best_add_v, step, step - time_stamp[best_add_v]);
            Add(best_add_v, 1);
            time_stamp[best_add_v] = step;
            step++;
        }
        //已全部支配，判断当前解是否为更优解
        if (undom_stack_fill_pointer == 0 && currentWeight < bestWeight)
        {
            //TODO::用于加点处的解禁
            //            rightAfternewlow=true;//刚刚刷新了一次最优解
            //            minUndom=v_num;//如果现在已经支配了所有点，则要将minUndom重制
            UpdateBestSolution();
            neighborSize = 1;
            improvementCount++;
            NOimprovementstep = 0;
        }
        else
        {
            //            rightAfternewlow=false;//通过一轮删除和一轮添加，未能刷新最优解
            //            if(undom_stack_fill_pointer<minUndom)
            //                minUndom=undom_stack_fill_pointer;
            for (size_t i = 0; i < undom_stack_fill_pointer; i++)
            {
                addWeight(undom_stack[i]);
            }
            totalweight += undom_stack_fill_pointer;
            if (totalweight > weightthreshold)
            {
                updateWeight();
            }
            NOimprovementstep++;
            if (neighborSize > maxNeighborSize)
            {
                neighborSize = 1;
            }
            else
            {
                neighborSize++;
            }
        }
        stepAction++;
        MarkCut();
    }
}
