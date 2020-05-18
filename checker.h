#include "update.h"

bool test_score()
{
    vector<int> correct_score(v_num + 1, 0);
    vector<int> correct_dominate(v_num + 1, 0);
    for (int v = 1; v < v_num + 1; ++v)
    {
        if (v_in_c[v])
        {
            correct_dominate[v]++;
            for (int i = 0; i < v_degree[v]; ++i)
            {
                int u = v_adj[v][i];
                correct_dominate[u]++;
            }
        }
    }
    for (int v = 1; v < v_num + 1; ++v)
    {
        if (correct_dominate[v] == 0)
        {
            correct_score[v]++;
            for (int i = 0; i < v_degree[v]; ++i)
            {
                int u = v_adj[v][i];
                correct_score[u]++;
            }
        }
        else if (correct_dominate[v] == 1)
        {
            if (v_in_c[v])
            {
                correct_score[v]--;
            }
            else
            {
                for (int i = 0; i < v_degree[v]; ++i)
                {
                    int u = v_adj[v][i];
                    if (v_in_c[u])
                    {
                        correct_score[u]--;
                    }
                }
            }
        }
    }
    bool right = true;
    for (int v = 1; v < v_num + 1; ++v)
    {
        if (correct_dominate[v] != dominated[v])
        {
            std::cout << "dominated wrong: " << v << std::endl;
            std::cout << "correct: " << correct_dominate[v]
                      << ", wrong: " << dominated[v] << std::endl;
            right = false;
        }
        if (correct_score[v] != score[v])
        {
            std::cout << "score wrong: " << v << std::endl;
            std::cout << "correct: " << correct_score[v] << ", wrong: " << score[v]
                      << std::endl;
            std::cout << v_in_c[v] << std::endl;
            right = false;
        }
    }
    if (!right)
        abort();

    right = true;
    int undom_num = 0;
    for (int v = 1; v < v_num + 1; ++v)
    {
        if (dominated[v] == 0)
        {
            undom_num++;
            bool okay = false;
            for (int i = 0; i < undom_stack_fill_pointer; ++i)
            {
                if (undom_stack[i] == v)
                {
                    okay = true;
                    break;
                }
            }
            if (!okay)
            {
                std::cout << "not in undom_stack: " << v << std::endl;
                right = false;
            }
        }
    }
    if (undom_num != undom_stack_fill_pointer)
    {
        std::cout << "undom_num wrong!" << std::endl;
    }
    if (!right)
        abort();

    return true;
}
bool CheckSolution()
{
    int v;
    memset(dominated, 0, sizeof(int) * (v_num + 1));

    for (v = 1; v < v_num + 1; v++)
    {
        if (best_v_in_c[v])
            for (int j = 0; j < v_degree[v]; j++)
                dominated[v_adj[v][j]]++;
        dominated[v]++;
    }
    for (v = 1; v < v_num + 1; v++)
        if (dominated[v] == 0)
            return false;
    return true;
}
