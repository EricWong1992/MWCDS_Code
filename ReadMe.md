# Introduction

## 1. Build && Run

```bash
build : g++ -O3 -std=c++11 fastds-pure.cpp -o fastcds

run: ./fastcds --i ./file.txt --t 30 --s 1 --f0 100 --f1 100000 --it 2 --ct 10
```

```txt
"--i": input file
"--s": rand seed
"--t": cutofftime
# arg[4]-arg[7]:HDC parameters. detailed information of the parameters
"--f0:" floor0
"--f1": floor1
"--it": insTimes
"--ct": ceilingTimes
```

bigcnt.cpp is used to make the graph connect
usage:
g++ bigcnt.cpp -o bigcnt
./bigcnt inputfile outpitfile
inputfile: the file path of graph to be modified
outputfile: the file path of graph modified

## 2. DIMACS graph format

c this is the graph with vertices {1,2,3,4,5} and edges {(1,2),(2,3),(2,4),(3,4),(4,5)}

```txt
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
