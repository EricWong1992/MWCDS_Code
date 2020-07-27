#ifndef _BASIC_H
#define _BASIC_H
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
#include "Array.h"
//#include "my_heap.h"

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
{             //边集数组
    int u, v; //弧尾和弧头
    int next; //指向同一个弧尾的下一条边
              //    EdgeType weight; //权值，对于非网图能够不须要
} EdgeLib;

int *first; //指向顶点的第一条边

EdgeLib *edge; //存储边信息

chrono::steady_clock::time_point start;

enum ChooseMode
{
    ModeA = 0,
    ModeB,
    ModeC,
    ModeD,
};

//重启时选点模式
ChooseMode currentMode = ChooseMode::ModeC;
/*
    重启时选点副评分
    ModeA时代表频繁进入Candidate的频度，add()时更新
    ModeB时代表经常不在Candidate的频度，addWeight()时更新
*/
int *subWeight;
int *pre_deci_step;      //上一轮构造解中顶点选择顺序
int *temp_pre_deci_step; //暂存本轮构造解时顶点选择顺序，值越大的点优先级越高
int add_step = 0;        //构造解时添加顶点的step

//setting
bool running_is_interrupted = false;

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
int maxNeighborSize = 3; //在一次迭代中最大同时删除顶点数量
int *frequency;

double *weight;
double weight_para_aphle = 0.3; //权重调整系数
int *subscore;                  //带权的分数，可以变支配的点的权重和减去变不支配的点的权重和
int *toberemoved;
int *outofcut;
int outofcutnum;
int LastRemoved[4];           //上一轮最多有4个被删除
int LastRemovedIndex = 0;     //上一轮被删除的个数
bool rightAfternewlow = true; //刚刚刷新了一次新纪录
Array *removedNodeNeighbor;   //用来暂存删除顶点的邻居
Array *redundantNodes;        //用来记录score为0的冗余结点

//MyHeap * myHeap; //用于初始化解

int *score;
int *initscore; //初始化阶段的分数，即2Wv+Cv-1
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
int greypointnum = 0;
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

//上次启动最后的解
int last_c_size;
int *last_v_in_c;

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
//int recordcutIndex;
int *RemovedPoint; //删掉的点的集合
int *AddedPoint;   //加入的点的集合
int *cutPointSet;  //割点集合
int *initIndex;    //初始化时的序号

int cutIndex; //割点集合大小
//int AddedIndex;   //加入点的数量
//int RemovedIndex; //删掉的点的数量
//int ave_weight;
long para_gama = 300; //控制权重之和
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
double weightthreshold;   //权重阈值
double currentWeight = 0; //当前解权重和
double bestWeight;        //当前最优解权重和
//llong check_size = 0;

int *father;
int *childnum;
int *inToberemoved;
int *indextoberemoved;
int toberemovedNum;

//my_heap.h
int *pos_in_my_heap;
int *my_heap;
int my_heap_count = 0;
bool my_heap_is_leaf(int);
int my_heap_left_child(int);
int my_heap_right_child(int);
int my_heap_parent(int);
void my_heap_swap(int, int);
void my_heap_shiftdown(int);
void my_heap_insert(int);
int my_heap_remove_first();
int my_heap_remove(int);

//build_free.h
double TimeElapsed();
int BuildInstance(string);
void FreeMemory();

//construct_initscore.h
void init_increase_dominate(int, int);
int find(int);
void join(int, int);
int calCV(int);
void lowerScore();
void initSubScore();
void joinV(int);
void addToS(int);
void updateS(int);
int chooseMax();
void addNodeInit(int);
void ConstructByInitScore();

//update.h
int ind = 0;
int root = 1;
int maxScore = 0; //删除点时的最大分数
int maxPoint = 0; //选中的最大点
int Toroot = -1;  //直接连接root的点，有则标号，无则-1
void ResetCandidate();
void UpdateBestSolution();
void cutPoint(int, int);
void cutPointNoRecur(int);
void cutPoint1(long);
bool judgeCut(int);
void updateWeight();
void updateSubscore();

//checker.h
bool test_score();
bool CheckSolution();
bool CheckSolutionIsConnected();
bool CheckGraphIsConnected();

//fastds-pure.h
inline void Undom(int);
inline void Dom(int);
void increase_dominate(long, long);
void decrease_dominate(int);
bool Add(int, int);
bool Remove(int, int);
void MarkCut();
void MarkCuttree();
void addWeight(int);
void minusWeight(int, int);
void addUpdate(int);
void removeUpdate(int);
bool checkLastRemoved(int);
int ChooseAddVsubscorefast();
int ChooseAddVsubscorefastAspration();
int ChooseAddVbest();
int ChooseAddVtabufastbanlasttime();
int ChooseAddVtabufast();
int ChooseAddVtabufastAspration();
int ChooseRemoveVTopof();
int ChooseRemoveVTopofBMS(int count, int choice);
int ChooseRemoveVFromArray(Array *, int);
void localSearchFramework1();
void localSearchFramework2();
void Framework1CutTree();
void Framework1Tarjan();
void Framework2CutTree();
void Framework2TarjanFocus();   //集中
void Framework2TarjanScatter(); //散点
void updateRedundantV(int);
void RemoveRedundant(int);
void Restart();
void ConstructNewSolution();
int NewSolutionChooseVFromMethodA(); //副分数选择频繁进出Candidate的点
int NewSolutionChooseVFromMethodB(); //副分数选择总不在Candidate的点
int NewSolutionChooseVFromMethodC(); //副分数尽量选择和上一轮构造不同的点
int NewSolutionChooseVFromMethodD(); //从solution pool中选解mix

#endif