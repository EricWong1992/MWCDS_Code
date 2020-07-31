#include "basic.h"

void constructIncreaseDominate(int v, int v_dominater)
{
    if (dominated[v] == 0)
    {
        --score[v];
        subscore[v] -= frequency[v];
        for (size_t i = 0; i < v_degree[v]; i++)
        {
            int u = v_adj[v][i];
            --score[u];
            subscore[u] -= frequency[v];
        }
        Dom(v);
        onlydominate[v] = v_dominater;
        //被自身支配时，此时是直接把白点变成黑点，不需要修改灰点集合
        if (v != v_dominater)
        {
            greyPointArray->insert_element(v);
        }
    }
    else if (dominated[v] == 1)
    {
        if (v_in_c[v] == 1)
        {
            ++score[v];
            subscore[v] += frequency[v];
        }
        else
        {
            int v_dominater = onlydominate[v];
            ++score[v_dominater];
            subscore[v_dominater] += frequency[v];
        }
    }
    ++dominated[v];
}

void constructAdd(int v)
{
    if (v_in_c[v] == 1)
        return;
    v_in_c[v] = 1;
    c_size++;
    currentWeight += weight[v];
    //初始点添加的时候是直接从白点变成黑点
    greyPointArray->delete_element(v);
    
    if (currentMode == ChooseMode::ModeC)
    {
        pre_deci_step[v] = add_step++;
    }

    int new_score = -score[v];
    int new_subscore = -subscore[v];
    constructIncreaseDominate(v, v);
    for (size_t i = 0; i < v_degree[v]; i++)
    {
        int u = v_adj[v][i];
        constructIncreaseDominate(u, v);
    }
    score[v] = new_score;
    subscore[v] = new_subscore;
}

void constructInitialSolution()
{
    for (int v = 1; v < v_num + 1; ++v)
    {
        undomPointArray->insert_element(v);
    }
    int root = -1;
    int best_weight = INT_MAX;
    for (size_t i = 1; i < v_num + 1; i++)
    {
        if (weight[i] < best_weight)
        {
            root = i;
            best_weight = weight[i];
        }
    }
    constructAdd(root);
    while (undomPointArray->size() != 0)
    {
        int best_add_v = -1;
        best_weight = INT_MAX;
        for (size_t i = 0; i < greyPointArray->size(); i++)
        {
            int greyPoint = greyPointArray->element_at(i);
            if (weight[greyPoint] < best_weight)
            {
                best_add_v = greyPoint;
                best_weight = weight[greyPoint];
            }
        }
        if (best_add_v != -1)
        {
            constructAdd(best_add_v);
        }
    }

    bestWeightInTurn = currentWeight;

    //初始化所有candidate用于寻找割点
    candidate_size = 0;
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
    ResetCandidate();

    RemoveRedundant(0);
    UpdateBestSolution();
}

int NewSolutionChooseVFromMethodA()
{
    int best_add_v = -1;
    int cscore;
    int best_cscore = -weightthreshold;
    if (c_size == 0)
    {
        for (size_t i = 1; i < v_num + 1; i++)
        {
            cscore = (int)(subscore[i] / weight[i]);
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
        for (size_t i = 0; i < greyPointArray->size(); i++)
        {
            int greyPoint = greyPointArray->element_at(i);
            cscore = (int)(subscore[greyPoint] / weight[greyPoint]);
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
    int cscore;
    int best_cscore = -weightthreshold;
    if (c_size == 0)
    {
        for (size_t i = 1; i < v_num + 1; i++)
        {
            cscore = (int)(subscore[i] / weight[i]);
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
        for (size_t i = 0; i < greyPointArray->size(); i++)
        {
            int greyPoint = greyPointArray->element_at(i);
            cscore = (int)(subscore[greyPoint] / weight[greyPoint]);
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

void ConstructRestartSolution()
{
    //开始构建解
    while (undomPointArray->size() != 0)
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
            constructAdd(addV);
            if (currentMode == ChooseMode::ModeC)
            {
                temp_pre_deci_step[addV] = add_step++;
            }
        }
    }
}