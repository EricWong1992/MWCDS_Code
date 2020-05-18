
#include "build_free.h"

bool CompareBySecond(const pair<int, int> &lhs, const pair<int, int> &rhs)
{
    return lhs.second > rhs.second;
}

void ConstructByDegree()
{
    fill_n(v_in_c, v_num + 1, 0);
    fill_n(score, v_num + 1, 0);
    fill_n(dominated, v_num + 1, 0);
    int v;
    // step = 0;
    int ds_size = 0;

    for (v = 1; v < v_num + 1; v++)
    {
        // aovoid duplicate, lin jinkun
        if (dominated[v] > 0)
        {
            continue;
        }

        if (v_degree[v] == 0)
        {
            v_in_c[v] = 1;
            dominated[v]++;
            ds_size++;
            time_stamp[v] = (llong)(~0ULL >> 1);
            v_fixed[v] = 1;
        } //固定0度点
        else if (v_degree[v] == 1)
        {
            int n = v_adj[v][0];
            v_in_c[n] = 1;
            dominated[n]++;
            ds_size++;
            time_stamp[n] = (llong)(~0ULL >> 1);
            v_fixed[n] = 1;
            for (int i = 0; i < v_degree[n]; i++)
            {
                dominated[v_adj[n][i]]++;
            }
        } //固定1度点的邻居
    }     //预处理，对于0度点g和1度点

    // TODO: speed up by using bin sort
    vector<pair<int, int>> v_d_vec(v_num + 1);
    for (v = 1; v < v_num + 1; v++)
    {
        v_d_vec[v] = pair<int, int>(v, v_degree[v]);
    }                                                          //初始化点与度的pair
    sort(v_d_vec.begin() + 1, v_d_vec.end(), CompareBySecond); //按照度排序

    for (v = 1; v < v_num + 1; v++)
    {
        int v0 = v_d_vec[v].first;
        if (dominated[v0] == 0)
        {
            dominated[v0]++;
            v_in_c[v0] = 1;
            for (int n = 0; n < v_degree[v0]; n++)
            {
                dominated[v_adj[v0][n]]++;
            }
            ds_size++;
        }
    } //按照度从大到小排列，如果没支配就将其放进去

    // score
    for (v = 1; v < v_num + 1; v++)
    {
        if (v_in_c[v])
        {
            // lin jinkun
            if (dominated[v] == 1)
            {
                --score[v];
            }
            for (int n = 0; n < v_degree[v]; ++n)
            {
                if (dominated[v_adj[v][n]] == 1)
                {
                    --score[v];
                }
            }
        }
    } // 删去点C中的v后的分数改变量，因为对于一个支配集加入点没有帮助
    c_size = ds_size;
    ResetCandidate();
    init_removeRedundant();
    if (c_size < best_c_size)
    {
        // std::cout<<"1"<<endl;
        UpdateBestSolution();
    }
    else // recover related values
    {
        c_size = best_c_size;
        for (v = 1; v < v_num + 1; v++)
        {
            v_in_c[v] = best_v_in_c[v];
            dominated[v] = best_dominated[v];
            // time_stamp[v] = 0;
        }
        ResetCandidate();
    }
    fill_n(score, v_num + 1, 0);
    // score
    for (v = 1; v < v_num + 1; v++)
    {
        if (v_in_c[v])
        {
            // lin jinkun
            if (dominated[v] == 1)
            {
                --score[v];
                --subscore[v];
            } //如果在C中的该点只被支配了一次，删除其会导致分数降低
            for (int n = 0; n < v_degree[v]; ++n)
            {
                if (dominated[v_adj[v][n]] == 1)
                {
                    onlydominate[v_adj[v][n]] = v;
                    --score[v]; //如果C中的点V的邻居u只被v支配，则删除会降低分数
                    --subscore[v];
                }
            }
        }
    }
    // sub_score hwy
    // fill_n(sub_score,v_num + 1,0);
    /*
     for (v = 1; v < v_num + 1; v++) {
     if (v_in_c[v]) {
     if (dominated[v] == 2) {
     --sub_score[v];
     }
     for (int n = 0; n < v_degree[v]; ++n) {
     if (dominated[v_adj[v][n]] == 2) {
     --sub_score[v];
     }
     }
     
     } else {
     if (dominated[v] == 1) {
     sub_score[v]++;
     }
     for (int n = 0; n < v_degree[v]; ++n) {
     if (dominated[v_adj[v][n]] == 1) {
     sub_score[v]++;
     }
     }
     }
     }
     */
    // UpdateBestSolution();
} //按照度由大到小排列加入点
