/*
    * Description: parse cmd for irace
    * function: turning parameters
    * Author: Wang Kai
*/
#include <string.h>
#include <map>
#include <iostream>

using namespace std;

/// modify here: total parameters count
static const int total_para_num = 7;
/// modify here: parameter type
static const char* const parastr[] =
        {
                "--i",          // instance
                "--s",         // seed
                "--t",          // cutoff time
                "--f0",
                "--f1",
                "--it",
                "--ct",
        };

class Parameters
{
private:
    //set<parameter string, argu value>
    map<string, string> paras;
    //parsed parameter pairs count
    int parsed_parameter_count;
public:
    Parameters(const int argc, char** const argv)
    {
        if (argc > total_para_num * 2 + 1)
        {
            cout << "Warning: Too many params input, please modify parse arguments model!!" << endl;
        }
        int i = 0;
        while(++i < argc)
        {
            const char* p = argv[i];
            for (size_t j = 0; j < total_para_num; j++)
            {
                if (strcmp(p, parastr[j]) == 0)
                {
                    string key = argv[i];
                    string value = argv[++i];
                    paras.emplace(map<string, string>::value_type(key, value));
                    break;
                }
            }
        }
    }
    int getParsedParameterCount(){return parsed_parameter_count;}

    string getParameterValue(const string key)
    {
        if (paras.count(key))
            return paras.at(key);
        return "";
    }

    void printParamaterPairs()
    {
        cout << "Arguments list:" << endl;
        for(auto it = paras.begin(); it != paras.end(); ++it)
        {
            cout << it->first << "\t" << it->second << endl;
        }
    }
};
