build : g++ -O3 -std=c++11 fastds-pure.cpp -o fastcds

run: ./fastcds ./file.txt 30 1 100 100000 2 10

arg[1]: input file

arg[2]: cutofftime

arg[3]: rand seed

arg[4]-arg[7]:HDC parameters. detailed information of the parameters can be found in main.cpp

bigcnt.cpp is used to make the graph connect
usage:
g++ bigcnt.cpp -o bigcnt
./bigcnt inputfile outpitfile
inputfile: the file path of graph to be modified
outputfile: the file path of graph modified

