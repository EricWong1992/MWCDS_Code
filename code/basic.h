#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <string>

using namespace std;

#define Pop(stack) stack[--stack##_fill_pointer]
#define Push(item, stack) stack[stack##_fill_pointer++] = item

//#define NDEBUG

typedef long long llong;
typedef unsigned int uint;

struct Edge
{
  int v1;
  int v2;
};
typedef struct Edge2
{           //边集数组
  int u, v; //弧尾和弧头
  int next; //指向同一个弧尾的下一条边
            //    EdgeType weight; //权值，对于非网图能够不须要
} EdgeLib;

int *first; //指向顶点的第一条边

EdgeLib *edge; //存储边信息

chrono::steady_clock::time_point start;

//llong max_steps;
llong step = 1;
llong stepaction;
llong steppre;
int try_step;
uint seed;
int cutoff_time;
//int mode;
int choosedremove_v, choosedadd_v;
int v_num;
int e_num;

//int iter;

//parameter
int floor0 = 100;
int floor1 = 100000;
int ceilingTimes = 10;
int insTimes = 2;

//Edge *edge;
//int *edge_weight;

double *weight;
double weight_para_aphle = 0.3; //权重调整系数
int *subscore;                  //带权的分数，可以变支配的点的权重和减去变不支配的点的权重和
int *toberemoved;
int toberemovednum;
int *outofcut;
int outofcutnum;

int *score;
int *initscore; //初始化阶段的分数，即2Wv+Cv-1
// int *sub_score;
llong *time_stamp;
//double s_alpha;
//double s_beta;
//double s_gamma;

int *v_weight;
int **v_edges;
long **v_adj;
int *v_degree;
int *v_fixed;
int *fixedSet;
int fixedNum = 0;
int *v_threshold;
int *isInS; //该点是否在候选集中
int *S;     //初始化过程中的候选集
int Snum = 0;
int *Stack;         //用来存储当前被处理顶点的栈，大小为v_num+1
int *SF;            //指向顶点的第一条未搜索边，大小为v_num+1
int *child;         //存储顶点的儿子数量，大小为v_num+1
long *onlydominate; //onlydominate[a]=b表示a只被b支配

int *dominated; //论文中的dd:dominated degree
int *greypointset;
int *indexingreypoint;
int *isgrey;
int greypointnum;
//int cur_c_size;
int c_size;
int *v_in_c;
int *candidate;
int *index_in_candidate;
int *outof_candidate;
int candidate_size;
int outof_candidate_size;
int *index_in_outofcandidate;
int maxDegreeNode = 1;
llong now_weight;

int best_c_size;
int *best_v_in_c;
int *best_dominated;
double best_comp_time;
llong best_step;

int *undom_stack;
int undom_stack_fill_pointer;
int *index_in_undom_stack;
long *taburemove; //删除节点的禁忌长度
long *tabuadd;
int tabutenue = 5;

int *conf_change;
int *pre; //前驱。用来确定该点属于哪个连通分量
int *CV;  //周围的C中的连通分量数量
int *WV;  //周围的C中的白色点数
//Tarjan算法使用
int *dnf;
int *low;
int *isCut;
//int *recordisCut;
int *recordCutPointSet;
//int recordcutIndex;
int *RemovedPoint; //删掉的点的集合
int *AddedPoint;   //加入的点的集合
int *cutPointSet;  //割点集合
int *initIndex;    //初始化时的序号

int cutIndex;     //割点集合大小
//int AddedIndex;   //加入点的数量
//int RemovedIndex; //删掉的点的数量
//int ave_weight;
long para_gama = 3000; //控制权重之和
//double p_scale;
int connectedNum = 0; //连通分量的数量
int minUndom;
int undomafteradd;
llong averagedegree = 0;

//double para1; // sub_param*subscore+age
//int para2;    // threshold = v_degree/count_param
//int para3;
//int para4;
//int CVpara = 1;
//int WVpara = 1;
//int Cmax; //无提升的最大周期
//double Temperature;
double totalweight;
long weightthreshold; //权重阈值
//llong check_size = 0;

int *father;
int *childnum;
int *inToberemoved;
int *indextoberemoved;
int toberemovedNum;

double TimeElapsed();
int BuildInstance(string);
void FreeMemory();
int find(int);
void join(int, int);
int calCV(int);
inline void Undom(int);
inline void Dom(int);
bool Add(int);
bool Remove(int);
void ConstructDS();
void ConstructByDegree();
void ConstructByScore();
void ResetCandidate();
void UpdateBestSolution();
bool CheckSolution();
int UpdateTargetSize(int);
int ChooseRemoveV(int);
int ChooseAddV(int);
void AddInit();
void RemoveInit(int);
void init_increase_dominate(int);
void init_decrease_dominate(int);
void init_removeRedundant();
void removeRedundant();
void lowerScore();
bool test_score();
void LocalSearch();
void MarkCut();
void minusWeight(int, int);
void removeUpdate(int);
int ChooseRemoveVTopofBMS(int count, int choice);
