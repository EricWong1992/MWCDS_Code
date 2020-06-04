rm fastwcds
g++ -O3 -std=c++11 fastds-pure.cpp -o fastwcds
./fastwcds --i ../Instances/n1000_m10000_s1.dimw --t 30 --s 1 --f0 100 --f1 100000 --it 2 --ct 10
