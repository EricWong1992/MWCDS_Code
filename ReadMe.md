```
build : g++ -O3 -std=c++11 fastds-pure.cpp -o fastcds

run: ./fastcds ./file.txt 30 1 100 100000 2 10
```
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

## This graph is represented in the DIMACS graph format in the following way:

c this is the graph with vertices {1,2,3,4,5} and edges {(1,2),(2,3),(2,4),(3,4),(4,5)}
```
p edge 5 5
v 1 1
v 2 1
v 3 1
e 1 2
e 2 3
e 2 4
e 3 4
e 4 5
```