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
            LocalSearch1();
        else
            LocalSearch();
        control++;
    }
    FreeMemory();
    string outfile = argv[1];
    string seednum = argv[3];
    //outfile="/home/libh/CDStuning11_6/20w_1b_2_10/sample/"+outfile;
    //freopen(outfile.data(),"a+",stdout);
    cout << "best_c_size:" << best_c_size << endl << "best_comp_time:" << best_comp_time << endl;
    //fclose(stdout);
    return 0;
}
