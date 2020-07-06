#include "fastds-pure.h"
#include "parse_cmd.h"
#include <sstream>

int main(int argc, char *argv[])
{
    Parameters* para = new Parameters(argc, argv);
    string instanceName = para->getParameterValue("--i");
    seed = atoi(para->getParameterValue("--s").c_str());
    cutoff_time = atoi(para->getParameterValue("--t").c_str());
    floor0 = atoi(para->getParameterValue("--f0").c_str());
    floor1 = atoi(para->getParameterValue("--f1").c_str());
    insTimes = atoi(para->getParameterValue("--it").c_str());
    ceilingTimes = atoi(para->getParameterValue("--ct").c_str());
    delete para;

    BuildInstance(instanceName);
    srand(seed);
    start = chrono::steady_clock::now();
    ConstructByInitScore();
    //cout<<"constructDS "<<best_c_size<<endl;
    //cout<<endl;
    int control = 0;
    while (TimeElapsed() < cutoff_time)
    {
        if (control % 2 == 0)
            newLocalSearch1();
        else
            newLocalSearch();
        control++;
    }
    /*Debug阶段使用
     * 被打断判断是否因为图为不连通图
     * (不会计算耗时)
     * */
    bool is_not_connected = false;
    if (running_is_interrupted)
    {
        for (int i = 1; i < v_num + 1; ++i) {
            if (v_degree[i] == 0)
            {
                is_not_connected = true;

                break;
            }
        }
    }
    if (is_not_connected)
    {
        cout << "Running is interrupted: Graph is not connected" << endl;
    } else
    {
        if (running_is_interrupted)
        {
            cout << "Running is interrupted: Time is not enough." << endl;
        }
        cout << "best_c_size:" << best_c_size << endl << "best_comp_time:" << best_comp_time << endl;
        cout << "best_weight:" << bestWeight << endl;
    }
    //    string outfile = argv[1];
    //    string seednum = argv[3];
    //outfile="/home/libh/CDStuning11_6/20w_1b_2_10/sample/"+outfile;
    //freopen(outfile.data(),"a+",stdout);
    //fclose(stdout);
    FreeMemory();
    return 0;
}
