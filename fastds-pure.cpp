#include "fastds-pure.h"
#include <sstream>

int main(int argc, char *argv[])
{
    uint seed;
    cutoff_time = atoi(argv[2]);
    string instanceName = "";
    instanceName += argv[1];
    seed = atoi(argv[3]);
    floor0 = atoi(argv[4]);
    floor1 = atoi(argv[5]);
    insTimes = atoi(argv[6]);
    ceilingTimes = atoi(argv[7]);
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
    cout << best_c_size << endl
         << best_comp_time << endl;
    //fclose(stdout);
    return 0;
}
