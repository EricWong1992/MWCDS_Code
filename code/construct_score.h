
#include "construct_degree.h"
void init_increase_dominate(int v)
{
    if (dominated[v] == 0)
    {               //如果该点本身没有被支配
        --score[v]; //该点以及其周围的点的分数要减少，因为现在从没有被支配到了被支配了，原来的可以加的分不见了
        int pos = pos_in_my_heap[v];
        my_heap_remove(pos);
        my_heap_insert(v); //根据分数重新确定位置，只有在初始化阶段才需要调整heap中的内容？？
        // all make but one break
        for (int i = 0; i < v_degree[v]; ++i)
        {
            int u = v_adj[v][i];
            --score[u];
            int pos = pos_in_my_heap[u];
            my_heap_remove(pos);
            my_heap_insert(u);
        } //周围的点分数都减少
        Dom(v);
    }
    else if (dominated[v] == 1)
    {
        // only two vertex need to update break;
        if (v_in_c[v] == 1)
        {
            ++score[v];
        } //如果该点本身在C中，则如果该点再次被支配后，则该点被删除后本身从不支配变为被支配
        // TODO: speed up
        for (int i = 0; i < v_degree[v]; ++i)
        {
            int u = v_adj[v][i];
            if (v_in_c[u])
            {
                ++score[u]; //如果点v原本只被u支配，则u被删除后，本来v不支配变成了支配了，为什么这里不要再调整heap？因为此处的u已经在C中了
            }
        }
    }
    ++dominated[v];
} //与increase_dominate的区别是这是在初始化阶段使用的，因此需要不断变化heap，而且不需要对conf改变
void init_removeRedundant()
{
    int v;
    for (int i = 0; i < candidate_size; ++i)
    {
        v = candidate[i];
        if (score[v] == 0)
        {
            RemoveInit(v);
            i--;
        }
    }
} //在初始化阶段将在候选集中的分数为0的点删除
void init_decrease_dominate(int v)
{
    if (dominated[v] == 1)
    {
        // all score of neighbours are make
        ++score[v];
        for (int i = 0; i < v_degree[v]; ++i)
        {
            int u = v_adj[v][i];
            ++score[u];
        }
        Undom(v);
    } //如果开始时该点只有被支配了一次，并且当前其变为不被支配了，则其自身以及周围的点都会分数加一，因为会将没有支配的该点变为支配
    else if (dominated[v] == 2)
    {
        // only need to update one break
        if (v_in_c[v])
        {
            --score[v];
        }
        else
        {
            // TODO: speed up this procedure
            for (int i = 0; i < v_degree[v]; ++i)
            {
                int u = v_adj[v][i];
                if (v_in_c[u])
                {
                    --score[u]; //如果目前v只被u支配，则在删除了u之后v不被支配，分数减少
                }
            }
        }
    }
    --dominated[v];
} //和decreaseDominate相同
void AddInit(int v)
{
    int pos;

    v_in_c[v] = 1;
    currentWeight += weight[v];

    int new_score = -score[v];
    init_increase_dominate(v);
    for (int i = 0; i < v_degree[v]; ++i)
    {
        int u = v_adj[v][i];
        init_increase_dominate(u);
    }
    score[v] = new_score;
    pos = pos_in_my_heap[v];
    my_heap_remove(pos);
} //初始化阶段加入点????dominated[]没有改变？ans：通过increase dominate改变了，此处需要改变heap状态，用于在构造阶段，并且不需要总是调整candidate，因为可以等结束时根据v_in_c一起调整

void RemoveInit(int v)
{
    v_in_c[v] = 0;
    currentWeight -= weight[v];
    c_size--;
    int last_candidate_v = candidate[--candidate_size];
    int index = index_in_candidate[v];
    candidate[index] = last_candidate_v;
    index_in_candidate[last_candidate_v] = index;
    score[v] = 0; //？？此句话多余了
    init_decrease_dominate(v);
    for (int i = 0; i < v_degree[v]; ++i)
    {
        int u = v_adj[v][i];

        init_decrease_dominate(u);
    }
} //在初始化阶段删除点，用于在删除分数为0的阶段

void ConstructByScore()
{
    best_c_size = (int)(~0U >> 1);
    int v, i = 0;
    undom_stack_fill_pointer = 0;
    for (v = 1; v < v_num + 1; ++v)
    {
        score[v] = v_degree[v] + 1;
        v_threshold[v] = v_degree[v] / 20;
        if (v_threshold[v] == 0)
        {
            v_threshold[v] = 1;
        }

        conf_change[v] = v_threshold[v] + 1;
        time_stamp[v] = 0;
        Undom(v);
    } //设置分数与threshold，将每个点都设为未入选
    int *best_array = new int[v_num + 1];
    for (v = 1; v < v_num + 1; ++v)
    {
        my_heap_insert(v);
    }
    for (v = 1; v < v_num + 1; ++v)
    {
        // aovoid duplicate, lin jinkun
        if (dominated[v] > 0)
        {
            continue;
        }
        if (v_degree[v] == 0 && v_in_c[v] == 0)
        {
            i++;
            AddInit(v);
            time_stamp[v] = (llong)(~0ULL >> 1);
            v_fixed[v] = 1;
        }
        else if (v_degree[v] == 1)
        {
            int n = v_adj[v][0];
            if (!v_in_c[n])
            {
                AddInit(n);
                i++;
                time_stamp[n] = (llong)(~0ULL >> 1);
                v_fixed[n] = 1;
            }
        }
    } //预处理
    while (undom_stack_fill_pointer > 0)
    {
        int best_v = my_heap[0];
        if (score[best_v] > 0)
        {
            AddInit(best_v);
            i++;
        }
    } //选择分数最大的点加入
    delete[] best_array;
    c_size = i;
    for (int p = 1; p < v_num + 1; p++)
    {
        cout << v_in_c[p] << endl;
    }
    ResetCandidate();
    init_removeRedundant();

    // ResetCandidate();
    UpdateBestSolution();
}
