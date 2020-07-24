#pragma once

#include "checker.h"

#define NDEBUG
#define N_DEBUG_OUTPUT //输出添加删除节点信息

//模块开关
bool moduleRemoveRedundant = true; //删除冗余结点

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

//更新冗余结点
void updateRedundantV(int v)
{
    if (!moduleRemoveRedundant)
        return;
    if (v_in_c[v] == 1 && score[v] == 0 && v_fixed[v] == 0) //只有未被固定的，在cs中的，分数为0的点才有可能会被删除
    {
        redundantNodes->insert_element(v);
    }
    else
    {
        redundantNodes->delete_element(v);
    }
}

//删除冗余
void RemoveRedundant(int choice)
{
    if (!moduleRemoveRedundant)
        return;
    MarkCut();
    for (int k = 0; k < redundantNodes->size(); k++)
    {
        int bestRemoveV = -1;
        double bestRemoveVWeight = 0.0;
        for (size_t i = 0; i < redundantNodes->size(); i++)
        {
            int redundantV = redundantNodes->element_at(k);
            if (weight[redundantV] > bestRemoveVWeight && isCut[redundantV] == 0 && v_in_c[redundantV] == 1 && v_fixed[redundantV] == 0)
            {
                bestRemoveV = redundantV;
                bestRemoveVWeight = weight[redundantV];
            }
        }
        if (bestRemoveV != -1)
        {
            Remove(bestRemoveV, choice);
            MarkCut();
            k = 0;
        }
        else
        {
            break;
        }
    }
}

int NewSolutionChooseVFromMethodA()
{
    int best_add_v = -1;
    double cscore;
    double best_cscore = -weightthreshold;
    if (c_size == 0)
    {
        for (size_t i = 1; i < v_num + 1; i++)
        {
            cscore = subscore[i] / weight[i];
            if (cscore > best_cscore)
            {
                best_add_v = i;
                best_cscore = cscore;
            }
            else if (cscore == best_cscore)
            {
                if (subWeight[i] > subWeight[best_add_v])
                {
                    best_add_v = i;
                }
            }
        }
    }
    else
    {
        //从灰点中选择
        // cout << "greyNum:" << greypointnum << endl;
        for (size_t i = 0; i < greypointnum; i++)
        {
            int greyPoint = greypointset[i];
            cscore = subscore[greyPoint] / weight[greyPoint];
            if (cscore > best_cscore)
            {
                best_add_v = greyPoint;
                best_cscore = cscore;
            }
            else if (cscore == best_cscore)
            {
                if (subWeight[greyPoint] > subWeight[best_add_v])
                {
                    best_add_v = greyPoint;
                }
            }
        }
    }
    return best_add_v;
}
int NewSolutionChooseVFromMethodB()
{
}

int NewSolutionChooseVFromMethodC()
{
    int best_add_v = -1;
    double cscore;
    double best_cscore = -weightthreshold;
    if (c_size == 0)
    {
        for (size_t i = 1; i < v_num + 1; i++)
        {
            cscore = subscore[i] / weight[i];
            if (cscore > best_cscore)
            {
                best_add_v = i;
                best_cscore = cscore;
            }
            else if (cscore == best_cscore)
            {
                if (pre_deci_step[i] > pre_deci_step[best_add_v])
                {
                    best_add_v = i;
                }
            }
        }
    }
    else
    {
        //从灰点中选择
        // cout << "greyNum:" << greypointnum << endl;
        for (size_t i = 0; i < greypointnum; i++)
        {
            int greyPoint = greypointset[i];
            cscore = subscore[greyPoint] / weight[greyPoint];
            if (cscore > best_cscore)
            {
                best_add_v = greyPoint;
                best_cscore = cscore;
            }
            else if (cscore == best_cscore)
            {
                if (pre_deci_step[greyPoint] > pre_deci_step[best_add_v])
                {
                    best_add_v = greyPoint;
                }
            }
        }
    }
    return best_add_v;
}

int NewSolutionChooseVFromMethodD()
{
}

void Restart()
{
    cout << "restart" << endl;
    //备份数据
    last_c_size = c_size;
    memcpy(last_v_in_c, v_in_c, sizeof(int) * (v_num + 1));
    //重置数据
    fill_n(v_in_c, v_num + 1, 0);
    fill_n(isgrey, v_num + 1, 0);
    fill_n(dominated, v_num + 1, 0);
    fill_n(taburemove, v_num + 1, 0);
    fill_n(tabuadd, v_num + 1, 0);
    fill_n(conf_change, v_num + 1, 1);
    fill_n(v_fixed, v_num + 1, 0);
    fill_n(isCut, v_num + 1, 0);
    fill_n(cutPointSet, v_num + 1, 0);
    fill_n(inToberemoved, v_num + 1, 0);
    fill_n(indextoberemoved, v_num + 1, 1);
    c_size = 0;
    cutIndex = 0;
    toberemovedNum = 0;
    undom_stack_fill_pointer = 0;
    currentWeight = 0;
    totalweight = totalweight_backup;
    for (size_t i = 1; i < v_num + 1; i++)
    {
        Undom(i);
    }
    greypointnum = 0;
    if (currentMode == ChooseMode::ModeC)
    {
        add_step = 0;
    }
    //构造新解
    ConstructNewSolution();
    //选点完毕subweight清空, 重新记录
    //score, subscore清除
    fill_n(score, v_num + 1, 0);
    lowerScore();
    initSubScore();
    if (currentMode == ChooseMode::ModeA || currentMode == ChooseMode::ModeB)
    {
        fill_n(subWeight, v_num + 1, 0);
    }
    else if (currentMode == ChooseMode::ModeC)
    {
        memcpy(pre_deci_step, temp_pre_deci_step, sizeof(int) * (v_num + 1));
        fill_n(temp_pre_deci_step, v_num + 1, v_num);
    }

    //删除新解中冗余
    RemoveRedundant(0);
    cout << "c_size:" << c_size << endl;
    cout << "newSolutionWeight:" << currentWeight << endl;

    //初始化所有candidate用于寻找割点
    for (int i = 1; i <= v_num; ++i)
    {
        candidate[candidate_size++] = i;
    }
    //使用割点fix
    MarkCut();
    for (int i = 0; i < cutIndex; ++i)
    {
        if (isCut[cutPointSet[i]] != 0)
        {
            int cutPoint = cutPointSet[i];
            v_fixed[cutPoint] = 1;
            fixedSet[fixedNum++] = cutPoint;
        }
    }
    //还原candidate
    ResetCandidate();
}

void ConstructNewSolution()
{
    //开始构建解
    while (undom_stack_fill_pointer != 0)
    {
        int addV = -1;
        switch (currentMode)
        {
        case ChooseMode::ModeA:
            addV = NewSolutionChooseVFromMethodA();
            break;
        case ChooseMode::ModeB:
            addV = NewSolutionChooseVFromMethodA();
            break;
        case ChooseMode::ModeC:
            addV = NewSolutionChooseVFromMethodC();
            break;
        case ChooseMode::ModeD:
            addV = NewSolutionChooseVFromMethodD();
            break;
        }
        if (addV != -1)
        {
            Add(addV, 1);
            if (currentMode == ChooseMode::ModeC)
            {
                temp_pre_deci_step[addV] = add_step++;
            }
        }
    }
}

void increase_dominate(long v, long source_v)
{
    if (dominated[v] == 0)
    { //该点被支配了，因此分数要降低
        --score[v];
        subscore[v] -= frequency[v];
        updateRedundantV(v);
        // all make but one break
        for (int i = 0; i < v_degree[v]; ++i)
        {
            long u = v_adj[v][i];
            --score[u];
            subscore[u] -= frequency[v];
            updateRedundantV(v);
        }
        Dom(v);
        onlydominate[v] = source_v; //libohan_10_6
        if (v != source_v)
        {
            isgrey[v] = 1;
            indexingreypoint[v] = greypointnum;
            greypointset[greypointnum++] = v; //新被支配的白点变成灰点
        }
    } //一个原本不被支配的点变得被支配了，该点以及周围的点都要分数-1
    else if (dominated[v] == 1)
    {
        if (v_in_c[v] == 1)
        {
            ++score[v];
            subscore[v] += frequency[v];
            updateRedundantV(v);
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
            updateRedundantV(u);
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

///
//添加顶点
//@param v 顶点
//@param choice 0修改cc
bool Add(int v, int choice = 1)
{
    if (v_in_c[v] == 1 || v < 0)
        return false; //不会重复加入
    //构建新解调用时，加入的点不一定是灰点
    if (isgrey[v] == 1)
    {
        isgrey[v] = 0;
        int indextmp = indexingreypoint[v];
        int last = greypointset[--greypointnum];
        indexingreypoint[last] = indextmp;
        greypointset[indextmp] = last; //新加入的点原先是灰点，要删除
    }
    if (currentMode == ChooseMode::ModeA)
    {
        subWeight[v]++;
    }

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
    updateRedundantV(v);
    currentWeight += weight[v];
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
    } //与CC相关
      //        taburemove[v] =step +maxNeighborSize *(tabutenue + rand() % 10); //加入点后设置禁忌,与tabu相关
    taburemove[v] = (step + 1 + rand() % 3);
    //    taburemove[v] = (step + tabutenue + rand() % 10); //加入点后设置禁忌
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
        updateRedundantV(v);
        for (int i = 0; i < v_degree[v]; ++i)
        {
            int u = v_adj[v][i];
            ++score[u];
            subscore[u] += frequency[v];
            updateRedundantV(u);
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
            updateRedundantV(v);
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
                    updateRedundantV(u);
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

///
//删除顶点
//从C中删除点，相应地将该点从candidate中删除，并改变周围点的支配次数和分数，最后将自身的分数取相反数
//@param v 顶点
//@param choice 0修改cc
bool Remove(int v, int choice = 1)
{
    isgrey[v] = 1;
    indexingreypoint[v] = greypointnum;
    greypointset[greypointnum++] = v; //删掉一个非割点后，这个点肯定是灰点
    v_in_c[v] = 0;
    currentWeight -= weight[v];
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
    updateRedundantV(v);
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

    //    tabuadd[v] = step +  (2 + rand() % 3);
    tabuadd[v] = step + 1;
    return true;
}

//增加权重，只对白点及其周围点
//TODO:权重改变，暂时设置为5可以更快收敛
void addWeight(int node)
{
    int increment = 5;
    frequency[node] += increment;
    for (int i = 0; i < v_degree[node]; i++)
        subscore[v_adj[node][i]] += increment;
    subscore[node] += increment;
    if (currentMode == ChooseMode::ModeB)
    {
        subWeight[node]++;
    }
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
    return (subscore[a] / weight[a] > subscore[b] / weight[b]);
}

/**
 if there is no non-tabu operation then choose top 40%

    @return choosen vertex
    */
int ChooseRemoveVTopof()
{
    int v;
    double best_score = -weightthreshold;
    int best_remove_v = -1;
    int topIndex = 0;
    double cscore;
    for (int i = 0; i < candidate_size; i++)
    {
        v = candidate[i];
        if (isCut[v] == 1 || v_fixed[v] == 1 || v == choosedadd_v)
            continue;
        cscore = subscore[v] / weight[v];
        if (step > taburemove[v])
        {
            //if(score[v]>best_score)
            if (cscore > best_score)
            {
                best_remove_v = v;
                best_score = cscore;
            }
            //            else if(curscore==best_score&&time_stamp[v]<time_stamp[best_remove_v])
            //                best_remove_v=v;
            else if (cscore == best_score)
            {
                //                if(score[v]>score[best_remove_v])
                //                    best_remove_v=v;
                //                else if (score[v]==score[best_remove_v])
                //                if(weight[v]>weight[best_remove_v])
                //                    best_remove_v=v;
                //                else if (weight[v]==weight[best_remove_v])
                if ((dominated[v] < dominated[best_remove_v]) || (dominated[v] == dominated[best_remove_v] && time_stamp[v] < time_stamp[best_remove_v]))
                    best_remove_v = v;
            }
        }
    }
    return best_remove_v;
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
        cscore = subscore[v] / weight[v];
        toberemoved1[topIndex++] = v;
        if (step > taburemove[v])
        {
            if (cscore > best_score)
            {
                best_remove_v = v;
                best_score = cscore;
            }
            else if (cscore == best_score)
            {
                //                if(weight[v]>weight[best_remove_v])
                //                    best_remove_v=v;
                //                else if (weight[v]==weight[best_remove_v])
                if ((dominated[v] < dominated[best_remove_v]) || (dominated[v] == dominated[best_remove_v] && time_stamp[v] < time_stamp[best_remove_v]))
                    best_remove_v = v; //关乎safety！！！
            }
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

int ChooseRemoveVFromArray(Array *removedNodeNeighbor, int choice) //choice==0,tarjan;choice==1,tree
{
    int best_remove_v = -1;
    double cscore;
    double best_cscore = -weightthreshold;
    for (size_t i = 0; i < removedNodeNeighbor->size(); i++)
    {
        int v = removedNodeNeighbor->element_at(i);
        if (choice == 0)
        {
            if (v_in_c[v] == 0 || v_fixed[v] == 1 || isCut[v] == 1)
                continue;
        }
        else
        {
            if (v_in_c[v] == 0 || inToberemoved[v] == 0 || v_fixed[v] == 1)
                continue;
        }
        cscore = subscore[v] / weight[v];
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

int ChooseAddVsubscorefast()
{
    // TODO: proof there is at least one avaible add_v
    int base_v, add_v;
    double cscore; //  subscore/weight，取最大
    double best_score = -weightthreshold;
    int best_add_v = -1;
    map<int, int> m;
    const int tobeaddNum1 = (int)(undom_stack_fill_pointer * v_degree[maxDegreeNode]);
    int tobeadd1[tobeaddNum1];
    int topIndex = 0;
    for (int i = 0; i < undom_stack_fill_pointer; ++i)
    {
        base_v = undom_stack[i];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (m.find(add_v) == m.end() && isgrey[add_v] && (weight[add_v] + currentWeight < bestWeight))
            {
                m[add_v] = topIndex; //不重复考虑
                tobeadd1[topIndex++] = add_v;
                cscore = subscore[add_v] / weight[add_v];
                if (conf_change[add_v] == 1)
                {
                    if (cscore > best_score)
                    {
                        best_add_v = add_v;
                        best_score = cscore;
                    }
                    else if (cscore == best_score)
                    {
                        if (score[add_v] > score[best_add_v])
                            best_add_v = add_v;
                        else if (score[add_v] == score[best_add_v])
                            if ((dominated[add_v] > dominated[best_add_v]) || (dominated[add_v] == dominated[best_add_v] && time_stamp[add_v] < time_stamp[best_add_v]))
                                best_add_v = add_v;
                    } //关乎safety找出best_add_v考虑CC
                }
            }
        }
    } //遍历所有白点周围的，加上后权重不超的灰点，best_add_v为不被CC限制的点，tobeadd1中不考虑CC
    if (best_add_v != -1)
        return best_add_v; //存在：：此处总是会存在的可以添加的点，所以永远不会用解禁策略
    else
    {
        if (topIndex != 0)
        {
            sort(tobeadd1, tobeadd1 + topIndex, cmp);
            int topof = (topIndex * 0.4) + 1;
            return tobeadd1[rand() % topof];
        }
        else
            return -1; //不能返回任意灰点，因为要保证权重不超，如果找不到权重不超的灰点就直接返回-1
    }
}

//带解禁的CC加点,将score作为第二标准
int ChooseAddVsubscorefastAspration()
{
    // TODO: proof there is at least one avaible add_v
    int base_v, add_v;
    double cscore; //  subscore/weight，取最大
    double best_score = -weightthreshold;
    double best_outCC_score = -weightthreshold;
    int best_outCC_add_v = -1;
    int best_add_v = -1;
    map<int, int> m;
    const int tobeaddNum1 = (int)(undom_stack_fill_pointer * v_degree[maxDegreeNode]);
    int tobeadd1[tobeaddNum1];
    int topIndex = 0;
    for (int i = 0; i < undom_stack_fill_pointer; ++i)
    {
        base_v = undom_stack[i];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (m.find(add_v) == m.end() && isgrey[add_v] && (weight[add_v] + currentWeight < bestWeight))
            {
                m[add_v] = topIndex; //不重复考虑
                tobeadd1[topIndex++] = add_v;
                cscore = subscore[add_v] / weight[add_v];
                if (cscore > best_outCC_score)
                {
                    best_outCC_score = cscore;
                    best_outCC_add_v = add_v;
                }
                else if (cscore == best_outCC_score)
                {
                    if (score[add_v] > score[best_outCC_add_v])
                        best_outCC_add_v = add_v;
                    else if (score[add_v] == score[best_outCC_add_v])
                        if ((dominated[add_v] > dominated[best_outCC_add_v]) || (dominated[add_v] == dominated[best_outCC_add_v] && time_stamp[add_v] < time_stamp[best_outCC_add_v]))
                            best_outCC_add_v = add_v;
                } //找出best_outcc_add_v
                if (conf_change[add_v] == 1)
                {
                    if (cscore > best_score)
                    {
                        best_add_v = add_v;
                        best_score = cscore;
                    }
                    else if (cscore == best_score)
                    {
                        if (score[add_v] > score[best_add_v])
                            best_add_v = add_v;
                        else if (score[add_v] == score[best_add_v])
                            if ((dominated[add_v] > dominated[best_add_v]) || (dominated[add_v] == dominated[best_add_v] && time_stamp[add_v] < time_stamp[best_add_v]))
                                best_add_v = add_v;
                    } //关乎safety找出best_add_v考虑CC
                }
            }
        }
    } //遍历所有白点周围的，加上后权重不超的灰点，best_add_v为不被CC限制的点，tobeadd1中不考虑CC
    if (undom_stack_fill_pointer - score[best_outCC_add_v] <= minUndom && rightAfternewlow == false && best_outCC_add_v != -1)
        return best_outCC_add_v; //解禁
    if (best_add_v != -1)
        return best_add_v; //存在：：此处总是会存在的可以添加的点，所以永远不会用解禁策略
    else
    {
        if (topIndex != 0)
        {
            sort(tobeadd1, tobeadd1 + topIndex, cmp);
            int topof = (topIndex * 0.4) + 1;
            return tobeadd1[rand() % topof];
        }
        else
            return -1; //不能返回任意灰点，因为要保证权重不超，如果找不到权重不超的灰点就直接返回-1
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
int ChooseAddVbest()
{
    int base_v, add_v;
    double cscore; //  subscore/weight，取最大
    double best_score = -weightthreshold;
    int best_add_v = -1;
    for (int i = 0; i < undom_stack_fill_pointer; ++i)
    {
        base_v = undom_stack[i];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (isgrey[add_v] && weight[add_v] + currentWeight < bestWeight)
            {
                cscore = subscore[add_v] / weight[add_v];
                if (cscore > best_score)
                {
                    best_add_v = add_v;
                    best_score = cscore;
                }
            }
        }
    }
    return best_add_v;
} //直接找最好的

int ChooseAddVtabufastbanlasttime() //仅仅禁掉刚刚上一轮刚刚删除的点
{
    int base_v, add_v;
    double cscore; //  subscore/weight，取最大
    double best_score = -weightthreshold;
    int best_add_v = -1;
    map<int, int> m;
    const int tobeaddNum1 = (int)(undom_stack_fill_pointer * v_degree[maxDegreeNode]);
    int tobeadd1[tobeaddNum1];
    int topIndex = 0;
    for (int i = 0; i < undom_stack_fill_pointer; ++i)
    {
        base_v = undom_stack[i];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (m.find(add_v) == m.end() && isgrey[add_v] && weight[add_v] + currentWeight < bestWeight)
            //仅仅考虑白点周围的，加进来不让总权重超出的灰点,并且不重复考虑点
            {
                m[add_v] = topIndex;
                tobeadd1[topIndex++] = add_v; //tobeadd1中记录了所有的白点周围满足权重不超的灰点
                cscore = subscore[add_v] / weight[add_v];
                if (step != time_stamp[add_v]) //step不等于timestamp，则不是刚删的也不是刚加的
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
            return -1;
    }
}

int ChooseAddVtabufast()
{
    int base_v, add_v;
    double cscore; //  subscore/weight，取最大
    double best_score = -weightthreshold;
    int best_add_v = -1;
    map<int, int> m;
    const int tobeaddNum1 = (int)(undom_stack_fill_pointer * v_degree[maxDegreeNode]);
    int tobeadd1[tobeaddNum1];
    int topIndex = 0;
    for (int i = 0; i < undom_stack_fill_pointer; ++i)
    {
        base_v = undom_stack[i];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (m.find(add_v) == m.end() && isgrey[add_v] && weight[add_v] + currentWeight < bestWeight)
            //仅仅考虑白点周围的，加进来不让总权重超出的灰点,并且不重复考虑点
            {
                m[add_v] = topIndex; //不重复考虑
                cscore = subscore[add_v] / weight[add_v];
                tobeadd1[topIndex++] = add_v; //所有满足条件的灰点放进tobeadd1中
                if (step >= tabuadd[add_v])   //刚被删除的点必然被禁忌，因此进不来这里
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
            return -1;
        }
    }
} //如果可以找到一个能让如果能找到不被tabu，并且在白点周围，并且加上后权重不超出的灰点，则选它，
//否则随机从白点周围的，并且加上后权重不超的，灰点集合中选一个前40%的任一个，(不管tabu)
//如果还没有，说明找不到在白点周围，加上后权重不超的灰点，返回-1
//解禁策略：找到所有的里面cscore最大的，如果这个点加入可以减少最小未支配数就选它（尚未实现）

int ChooseAddVtabufastAspration() //解禁的tabu选点
{
    int base_v, add_v;
    double cscore; //  subscore/weight，取最大
    double best_score = -weightthreshold;
    int best_add_v = -1;
    map<int, int> m;
    int best_outtabu_addv = -1;
    double best_outtabu_score = -weightthreshold;
    const int tobeaddNum1 = (int)(undom_stack_fill_pointer * v_degree[maxDegreeNode]);
    int tobeadd1[tobeaddNum1];
    int topIndex = 0;
    for (int i = 0; i < undom_stack_fill_pointer; ++i)
    {
        base_v = undom_stack[i];
        for (int j = 0; j < v_degree[base_v]; ++j)
        {
            add_v = v_adj[base_v][j];
            if (m.find(add_v) == m.end() && isgrey[add_v] && weight[add_v] + currentWeight < bestWeight)
            //仅仅考虑白点周围的，加进来不让总权重超出的灰点,并且不重复考虑点
            {
                m[add_v] = topIndex; //不重复考虑
                cscore = subscore[add_v] / weight[add_v];
                if (cscore > best_outtabu_score)
                {
                    best_outtabu_addv = add_v;
                    best_outtabu_score = cscore; //纪录下最好的符合条件的点(无禁忌，不考虑是否刚刚被删)
                }
                else if (cscore == best_outtabu_score)
                {
                    if ((dominated[add_v] > dominated[best_outtabu_addv]) || (dominated[add_v] == dominated[best_outtabu_addv] && time_stamp[add_v] < time_stamp[best_outtabu_addv]))
                        best_outtabu_addv = add_v;
                }
                tobeadd1[topIndex++] = add_v; //如果add_v刚刚不是被删除，则放进tobeadd1中
                if (step >= tabuadd[add_v])   //刚被删除的点必然被禁忌，因此进不来这里
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
    if (undom_stack_fill_pointer - score[best_outtabu_addv] <= minUndom && rightAfternewlow == false && best_outtabu_addv != -1)
        return best_outtabu_addv; //如果不是刚刚更新过最好权重，并且刷新了最小未支配点数，就解禁
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
    //     Framework2Tarjan();
    //    Framework2TarjanFocus();
    //    Framework2TarjanScatter();
    int noImproveRestart = 3; //解质量没提升重启
    int noImproveCount = 0;
    int control = 0;
    while (TimeElapsed() < cutoff_time)
    {
        //TODO:测试代码
        Framework2TarjanScatter();
        // Framework2TarjanFocus();
        Restart();


        //        if (noImproveCount < noImproveRestart)
        //        {
        //            if (control % 2 == 0)
        //            {
        //                Framework2TarjanScatter();
        //            }
        //            else
        //            {
        //                Framework2TarjanFocus();
        //            }
        //            control++;
        //        } else
        //        {
        //            Restart();
        //            noImproveCount = 0;
        //            control = 0;
        //        }
        //        noImproveCount++;
    }
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
                    best_removed_v = ChooseRemoveVFromArray(removedNodeNeighbor, 1);
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
                    best_removed_v = ChooseRemoveVFromArray(removedNodeNeighbor, 1);
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

int instance0 = floor0 * insTimes; //50*2
int gap0 = floor0 * ceilingTimes;  //50*10
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
                    best_removed_v = ChooseRemoveVFromArray(removedNodeNeighbor, 0);
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

void Framework2TarjanFocus()
{
    cout << "it's tarjanFocus now\n";
    if (c_size < 5000)
    {
        gap0 = floor0 * ceilingTimes * 10;
        instance0 = floor0 * insTimes * 10;
    }
    else
    {
        gap0 = floor0 * ceilingTimes;
        instance0 = floor0 * insTimes;
    }
    try_step = 1000;
    int improvementCount = 0;
    int stepAction = 1;
    long NOimprovementstep = 0;
    //    fill_n(inToberemoved, v_num + 1, 1);//不需要叶节点信息
    //    fill_n(conf_change, v_num + 1, 1);//tarjan不需要操作CC
    int neighborSize = 1;
    MarkCut();
    while (true)
    {
        if (stepAction % try_step == 0)
        {
            int timenow = TimeElapsed();
            if (timenow > cutoff_time)
                return;
            if (stepAction > gap0)
            { //还在爬坡中
                //                if (improvementCount > 10)
                //                {
                //                    instance0 -= floor0;
                //                    if (instance0 < floor0)
                //                        instance0 = floor0;
                //                }
                //                return;
            }
            cout << "step:" << step << " noimprovementstep:" << NOimprovementstep << endl;
            if (NOimprovementstep > instance0)
            { //局部搜索中
                instance0 += floor0;
                if (instance0 > gap0)
                    instance0 = gap0;
                //return;
            }
            //TODO:临时方案，待修改
            if (NOimprovementstep > 10000)
            {
                return;
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
        // int best_removed_v = -1;
        // best_removed_v = ChooseRemoveVTopof();
        // if (best_removed_v != -1)
        // {
        //     Remove(best_removed_v, 1);
        //     printDebugRemove(best_removed_v, step, step - time_stamp[best_removed_v]);
        //     LastRemoved[LastRemovedIndex++] = best_removed_v;
        //     time_stamp[best_removed_v] = step;
        //     MarkCut();
        // }
        // best_removed_v = -1;
        // best_removed_v = ChooseRemoveVTopofBMS(50, 0);
        // if (best_removed_v != -1)
        // {
        //     Remove(best_removed_v, 1);
        //     printDebugRemove(best_removed_v, step, step - time_stamp[best_removed_v]);
        //     LastRemoved[LastRemovedIndex++] = best_removed_v;
        //     time_stamp[best_removed_v] = step;
        // }

        for (size_t i = 0; i < neighborSize; i++)
        {
            int best_removed_v = -1;
            if (i == 0)
            {
                //BMS从可移除列表选一个点删除
                best_removed_v = ChooseRemoveVTopofBMS(100, 0);
                //                best_removed_v = ChooseRemoveVTopof();
            }
            else
            {
                //从removedNodeNeighbors中随机选一个点
                if (removedNodeNeighbor->size() != 0 && candidate_size != 0)
                {
                    best_removed_v = ChooseRemoveVFromArray(removedNodeNeighbor, 0);
                }
            }
            if (best_removed_v != -1)
            {
                Remove(best_removed_v, 1);
                printDebugRemove(best_removed_v, step, step - time_stamp[best_removed_v]);
                LastRemoved[LastRemovedIndex++] = best_removed_v;
                if (candidate_size != 0 && i < neighborSize - 1) //最后一次删点不用标割点
                {
                    MarkCut();
                }
                for (int n = 0; n < v_degree[best_removed_v]; ++n)
                {
                    int neighbor = v_adj[best_removed_v][n];
                    if (v_in_c[neighbor] == 1 && !removedNodeNeighbor->is_in_array(neighbor)) //以后可能不是割点了
                    {
                        removedNodeNeighbor->insert_element(neighbor);
                    }
                    for (int m = 0; m < v_degree[neighbor]; m++)
                    {
                        int neinei = v_adj[neighbor][m];
                        if (v_in_c[neinei] == 1 && !removedNodeNeighbor->is_in_array(neinei)) //以后可能不是割点
                        {
                            removedNodeNeighbor->insert_element(neinei);
                        }
                    } //二阶邻居
                }
                time_stamp[best_removed_v] = step;
                //                step++;
            }
            else
                printDebugMsg("Failed to remove");
        }
        //选点添加
        while (undom_stack_fill_pointer != 0 && currentWeight < bestWeight)
        {
            //           int best_add_v = ChooseAddVsubscorefast();
            int best_add_v = ChooseAddVtabufast();
            //            int best_add_v = ChooseAddVtabufastAspration();
            //            int best_add_v=ChooseAddVbest();
            //            int best_add_v=ChooseAddVtabufastbanlasttime();
            //防止加入的点为冗余顶点，下一步删除之后重新被选择陷入死循环
            if (best_add_v == -1 || time_stamp[best_add_v] == step)
                break; //如果没有找到白点周围的，能让权重和不超的灰点，则退出循环
            printDebugAdd(best_add_v, step, step - time_stamp[best_add_v]);
            Add(best_add_v, 1);
            RemoveRedundant(1);
            time_stamp[best_add_v] = step;
            //            step++;
        }
        //已全部支配，判断当前解是否为更优解
        if (undom_stack_fill_pointer == 0 && currentWeight < bestWeight)
        {
            //TODO::用于加点处的解禁
            rightAfternewlow = true; //刚刚刷新了一次最优解
            minUndom = v_num;        //如果现在已经支配了所有点，则要将minUndom重制
            UpdateBestSolution();
            neighborSize = 1;
            improvementCount++;
            NOimprovementstep = 0;
        }
        else
        {
            rightAfternewlow = false; //通过一轮删除和一轮添加，未能刷新最优解
            if ((undom_stack_fill_pointer < minUndom) || (NOimprovementstep % 1000 == 0 && NOimprovementstep > 0))
                //如果提升了最优情况，或者已经一千步没有提升了，则更新minUndom
                minUndom = undom_stack_fill_pointer;
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
        step++;
        MarkCut();
    }
}

void Framework2TarjanScatter()
{
    cout << "it's scatterTarjan now\n";
    if (c_size < 5000)
    {
        gap0 = floor0 * ceilingTimes * 10;
        instance0 = floor0 * insTimes * 10;
    }
    else
    {
        gap0 = floor0 * ceilingTimes;
        instance0 = floor0 * insTimes;
    }
    try_step = 1000;
    int improvementCount = 0;
    int stepAction = 1;
    long NOimprovementstep = 0;
    //    fill_n(inToberemoved, v_num + 1, 1);//不需要叶节点信息
    fill_n(conf_change, v_num + 1, 1);
    int neighborSize = 1;
    MarkCut();
    while (true)
    {
        if (stepAction % try_step == 0)
        {
            int timenow = TimeElapsed();
            if (timenow > cutoff_time)
                return;
            if (stepAction > gap0)
            { //还在爬坡中
                //                if (improvementCount > 10)
                //                {
                //                    instance0 -= floor0;
                //                    if (instance0 < floor0)
                //                        instance0 = floor0;
                //                }
                return;
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
        printDebugMsg("NewTurn");
        LastRemovedIndex = 0;
        //         int best_removed_v = -1;
        //         best_removed_v = ChooseRemoveVTopof();
        //         if (best_removed_v != -1)
        //         {
        //             Remove(best_removed_v, 0);
        //             printDebugRemove(best_removed_v, step, step - time_stamp[best_removed_v]);
        //             LastRemoved[LastRemovedIndex++] = best_removed_v;
        //             time_stamp[best_removed_v] = step;
        //             MarkCut();
        //         }
        //         best_removed_v = -1;
        //         best_removed_v = ChooseRemoveVTopofBMS(50, 0);
        //         if (best_removed_v != -1)
        //         {
        //             Remove(best_removed_v, 0);
        //             printDebugRemove(best_removed_v, step, step - time_stamp[best_removed_v]);
        //             LastRemoved[LastRemovedIndex++] = best_removed_v;
        //             time_stamp[best_removed_v] = step;
        //         }

        for (size_t i = 0; i < neighborSize; i++)
        {
            int best_removed_v = -1;
            //选择散点
            best_removed_v = ChooseRemoveVTopofBMS(100, 0);
            if (best_removed_v != -1)
            {
                Remove(best_removed_v, 0);
                printDebugRemove(best_removed_v, step, step - time_stamp[best_removed_v]);
                LastRemoved[LastRemovedIndex++] = best_removed_v;
                if (candidate_size != 0 && i < neighborSize - 1) //最后一次删点不用标割点
                {
                    MarkCut();
                }
                time_stamp[best_removed_v] = step;
                //                step++;
            }
            else
                printDebugMsg("Failed to remove");
        }
        //选点添加
        while (undom_stack_fill_pointer != 0 && currentWeight < bestWeight)
        {
            int best_add_v = ChooseAddVsubscorefast();
            //            int best_add_v = ChooseAddVsubscorefastAspration();
            //            int best_add_v = ChooseAddVtabufast();//在这里不用这个tabu的函数，因为是CC框架
            // int best_add_v=ChooseAddVtabufastNolastRemove();
            if (best_add_v == -1 || time_stamp[best_add_v] == step)
                break; //如果没有找到白点周围的，能让权重和不超的灰点，则退出循环
            printDebugAdd(best_add_v, step, step - time_stamp[best_add_v]);
            Add(best_add_v, 0); //CC的Add
            RemoveRedundant(0);
            time_stamp[best_add_v] = step;
            //            step++;
        }
        if (undom_stack_fill_pointer == 0)
        {
            cout << "currentWeight:" << currentWeight << endl;
        }

        //已全部支配，判断当前解是否为更优解
        if (undom_stack_fill_pointer == 0 && currentWeight < bestWeight)
        {
            //            TODO::用于加点处的解禁
            rightAfternewlow = true; //刚刚刷新了一次最优解
            minUndom = v_num;        //如果现在已经支配了所有点，则要将minUndom重制
            UpdateBestSolution();
            neighborSize = 1;
            improvementCount++;
            NOimprovementstep = 0;
        }
        else
        {
            rightAfternewlow = false; //通过一轮删除和一轮添加，未能刷新最优解
            if ((undom_stack_fill_pointer < minUndom) || (NOimprovementstep % 1000 == 0 && NOimprovementstep > 0))
                minUndom = undom_stack_fill_pointer;
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
        step++;
        MarkCut();
    }
}
