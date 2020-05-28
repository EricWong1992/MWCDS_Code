
#include "my_heap.h"
double TimeElapsed()
{
    chrono::steady_clock::time_point finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    return duration.count();
}

int BuildInstance(string filename)
{
    ifstream infile;
    infile.open(filename);
    if (infile.fail())
    {
        cout << "# Read file error: there is no such file existed!" << endl;
        getchar();
        exit(0);
    }
    char tempStr[100];
    infile >> tempStr;
    if (infile.eof())
    {
        cout << "# Read file error: file is empty!" << endl;
        getchar();
        exit(0);
    }

    int edgeCount = 0;
    while (!infile.eof())
    {
        if (strcmp(tempStr, "p") == 0)
        {
            infile >> tempStr;
            infile >> v_num >> e_num;
            /*****************new******************/
            weight = new double[v_num + 1];
            weightthreshold = (v_num + 1) * para_gama;
            v_threshold = new int[v_num + 1];
            score = new int[v_num + 1];
            subscore = new int[v_num + 1];
            time_stamp = new llong[v_num + 1];
            isgrey = new int[v_num + 1];
            greypointset = new int[v_num + 1];
            first = new int[v_num + 1];
            indexingreypoint = new int[v_num + 1];
            v_edges = new int *[v_num + 1];
            v_adj = new long *[v_num + 1];
            v_degree = new int[v_num + 1];
            fixedSet = new int[v_num + 1];
            initIndex = new int[v_num + 1];
            v_in_c = new int[v_num + 1];
            candidate = new int[v_num + 1];
            index_in_candidate = new int[v_num + 1];
            outof_candidate = new int[v_num + 1];
            index_in_outofcandidate = new int[v_num + 1];
            best_v_in_c = new int[v_num + 1];
            conf_change = new int[v_num + 1];
            dominated = new int[v_num + 1];
            best_dominated = new int[v_num + 1];
            undom_stack = new int[v_num + 1];
            initscore = new int[v_num + 1];
            isInS = new int[v_num + 1];
            edge = new EdgeLib[e_num + e_num];
            S = new int[v_num + 1];
            CV = new int[v_num + 1];
            WV = new int[v_num + 1];
            pre = new int[v_num + 1];
            dnf = new int[v_num + 1];
            low = new int[v_num + 1];
            isCut = new int[v_num + 1];
            recordCutPointSet = new int[v_num + 1];
            index_in_undom_stack = new int[v_num + 1];
            v_fixed = new int[v_num + 1];
            RemovedPoint = new int[v_num + 1];
            AddedPoint = new int[v_num + 1];
            cutPointSet = new int[v_num + 1];
            my_heap = new int[v_num + 1];
            pos_in_my_heap = new int[v_num + 1];
            toberemoved = new int[v_num + 1];
            outofcut = new int[v_num + 1];
            taburemove = new long[v_num + 1];
            tabuadd = new long[v_num + 1];
            child = new int[v_num + 1];
            SF = new int[v_num + 1];
            Stack = new int[v_num + 1];
            onlydominate = new long[v_num + 1];

            father = new int[v_num + 1];
            childnum = new int[v_num + 1];
            inToberemoved = new int[v_num + 1];
            indextoberemoved = new int[v_num + 1];
            /***********************************/

            my_heap_count = 0;

            /*****************fill_n******************/
            fill_n(v_threshold, v_num + 1, 0);
            fill_n(v_degree, v_num + 1, 0);
            fill_n(child, v_num + 1, 0);
            fill_n(taburemove, v_num + 1, 0);
            fill_n(tabuadd, v_num + 1, 0);
            fill_n(initscore, v_num + 1, 0);
            fill_n(isgrey, v_num + 1, 0);
            fill_n(v_in_c, v_num + 1, 0);
            fill_n(score, v_num + 1, 0);
            fill_n(subscore, v_num + 1, 0);
            fill_n(conf_change, v_num + 1, 1);
            fill_n(time_stamp, v_num + 1, 0);
            fill_n(dominated, v_num + 1, 0);
            fill_n(best_dominated, v_num + 1, 0);
            fill_n(v_fixed, v_num + 1, 0);
            fill_n(CV, v_num + 1, 0);
            fill_n(dnf, v_num + 1, 0);
            fill_n(low, v_num + 1, 0);
            fill_n(isCut, v_num + 1, 0);
            fill_n(cutPointSet, v_num + 1, 0);
            fill_n(recordCutPointSet, v_num + 1, 0);
            fill_n(first, v_num + 1, -1);
            fill_n(inToberemoved, v_num + 1, 0);
            fill_n(indextoberemoved, v_num + 1, 1);
            /***********************************/
            for (int v = 1; v <= v_num; v++)
                pre[v] = v;
        }
        if (strcmp(tempStr, "v") == 0)
        {
            int v;
            double vertex_weight;
            infile >> v >> vertex_weight;
            weight[v] = vertex_weight;
            totalweight += vertex_weight;
        }
        if (strcmp(tempStr, "e") == 0)
        {
            int u, v;
            infile >> u >> v;
            edge[edgeCount].u = u;
            edge[edgeCount].v = v;
            edge[edgeCount].next = first[edge[edgeCount].u];
            first[edge[edgeCount].u] = edgeCount;
            
            edge[edgeCount + 1].u = edge[edgeCount].v;
            edge[edgeCount + 1].v = edge[edgeCount].u;
            edge[edgeCount + 1].next = first[edge[edgeCount + 1].u];
            first[edge[edgeCount + 1].u] = edgeCount + 1;
            
            ++v_degree[u];
            ++v_degree[v];
            edgeCount += 2;
        }
        infile >> tempStr;
    }

    infile.close();

    v_adj[0] = 0;
    v_edges[0] = 0;
    for (int v = 1; v < v_num + 1; v++)
    {
        v_adj[v] = new long[v_degree[v]];
        v_edges[v] = new int[v_degree[v]];
    }

    int *v_degree_tmp = new int[v_num + 1];
    fill_n(v_degree_tmp, v_num + 1, 0);

    for (int e = 0; e < e_num; e++)
    {
        int v1 = edge[2 * e].u;
        int v2 = edge[2 * e].v;

        v_edges[v1][v_degree_tmp[v1]] = e;
        v_edges[v2][v_degree_tmp[v2]] = e;

        v_adj[v1][v_degree_tmp[v1]] = v2;
        v_adj[v2][v_degree_tmp[v2]] = v1;

        v_degree_tmp[v1]++;
        v_degree_tmp[v2]++;
    }
    for (int v = 1; v <= v_num; v++)
    {
        WV[v] = v_degree[v] + 1;
        if (v_degree[v] > v_degree[maxDegreeNode])
            maxDegreeNode = v;
    }
    averagedegree = e_num / v_num * 2;

    delete[] v_degree_tmp;

    return 0;
}

void FreeMemory()
{
    int v;
    for (v = 0; v < v_num + 1; v++)
    {
        delete[] v_adj[v];
        delete[] v_edges[v];
    }
    delete[] dominated;
    delete[] undom_stack;
    delete[] index_in_undom_stack;
    delete[] conf_change;
    delete[] best_v_in_c;
    delete[] index_in_candidate;
    delete[] candidate;
    delete[] v_in_c;
    delete[] v_degree;
    delete[] v_adj;
    delete[] v_edges;
    delete[] time_stamp;
    // delete[] sub_score;
    delete[] score;
    delete[] my_heap;
    delete[] pos_in_my_heap;
    delete[] pre;
    delete[] CV;
    delete[] WV;
    delete[] S;
    delete[] isInS;
    delete[] RemovedPoint;
    delete[] AddedPoint;
    delete[] cutPointSet;
    delete[] outof_candidate;
    delete[] index_in_outofcandidate;
    delete[] indexingreypoint;
    delete[] greypointset;
    delete[] isgrey;
    delete[] edge;
    delete[] weight;
    delete[] subscore;
    delete[] toberemoved;
    delete[] outofcut;
    delete[] taburemove;
    delete[] tabuadd;
    delete[] first;
    delete[] child;
    delete[] SF;
    delete[] Stack;
    delete[] v_threshold;
    delete[] onlydominate;
    delete[] fixedSet;
    delete[] inToberemoved;
    delete[] indextoberemoved;
    delete[] father;
    delete[] childnum;
    delete[] isCut;
    delete[] initIndex;
}
