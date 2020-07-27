#ifndef _MY_HEAP_H
#define _MY_HEAP_H
#include "basic.h"
//class MyHeap
//{
//public:
//    MyHeap(int capacity)
//    {
//        pos_in_array = new int[capacity];
//        array = new int[capacity];
//        score = new int[capacity];
//        std::fill_n(score, capacity, 0);
//        count = 0;
//    }
//    ~MyHeap()
//    {
//        delete[] pos_in_array;
//        pos_in_array = nullptr;
//        delete[] array;
//        array = nullptr;
//        delete[] score;
//        score = nullptr;
//    }
//    int getPos(int v)
//    {
//        return pos_in_array[v];
//    }
//    int getVAtPos(int pos)
//    {
//        return array[pos];
//    }
//    void swap(int a, int b)
//    {
//        int t = array[a];
//        array[a] = array[b];
//        pos_in_array[array[a]] = a;
//
//        array[b] = t;
//        pos_in_array[array[b]] = b;
//    }
//    bool is_leaf(int pos) const
//    {
//        return (pos >= count / 2) && (pos < count);
//    }
//    int left_child(int pos)
//    {
//        return 2 * pos + 1;
//    }
//    int right_child(int pos)
//    {
//        return 2 * pos + 2;
//    }
//    int parent(int pos)
//    {
//        return (pos - 1) / 2;
//    }
//    void shiftDown(int pos)
//    {
//        while (!is_leaf(pos))
//        {
//            int lc = left_child(pos);
//            int rc = right_child(pos);
//            int vl = array[lc];
//            int vr = array[rc];
//            if ((rc < count) && ((score[vr] > score[vl]) || ((score[vr] == score[vl]) && (vr < vl))))
//            {
//                lc = rc;
//            }
//            if (score[array[pos]] > score[vl])
//            {
//                return;
//            }
//            swap(pos, lc);
//            pos = lc;
//        }
//    }
//    void insert(int v, int _score)
//    {
//        int cur = count++;
//        array[cur] = v;
//        pos_in_array[v] = cur;
//        score[v] = _score;
//
//        int v_cur = array[cur];
//        while (cur != 0 && (score[v_cur] > score[array[parent(cur)]] || ((score[v_cur] == score[array[parent(cur)]]) && v_cur < array[parent(cur)])))
//        {
//            swap(cur, parent(cur));
//            cur = parent(cur);
//            v_cur = array[cur];
//        }
//    }
//    int remove_first()
//    {
//        swap(0, --count);
//        if (count != 0)
//        {
//            shiftDown(0);
//        }
//        return array[count];
//    }
//    int remove(int pos)
//    {
//        if (pos == count - 1)
//        {
//            count--;
//        }
//        else
//        {
//            swap(pos, --count);
//            int v_pos = array[pos];
//            while (pos != 0 && ((score[v_pos] > score[array[parent(pos)]]) || ((score[v_pos] == score[array[parent(pos)]]) && v_pos < array[parent(pos)])))
//            {
//                swap(pos, parent(pos));
//                pos = parent(pos);
//                v_pos = array[pos];
//            }
//            if (count != 0)
//            {
//                shiftDown(pos);
//            }
//        }
//        return array[count];
//    }
//
//private:
//    int *pos_in_array;
//    int *array;
//    int *score;
//    int count;
//};



 void my_heap_swap(int a, int b)
 {
     int t = my_heap[a];

     my_heap[a] = my_heap[b];
     pos_in_my_heap[my_heap[a]] = a;

     my_heap[b] = t;
     pos_in_my_heap[my_heap[b]] = b;
 }

 bool my_heap_is_leaf(int pos)
 {
     return (pos >= my_heap_count / 2) && (pos < my_heap_count);
 }

 int my_heap_left_child(int pos) { return (2 * pos + 1); }

 int my_heap_right_child(int pos) { return (2 * pos + 2); }

 int my_heap_parent(int pos) { return (pos - 1) / 2; }

 void my_heap_shiftdown(int pos1)
 {
     while (!my_heap_is_leaf(pos1))
     {
         int j = my_heap_left_child(pos1);
         int rc = my_heap_right_child(pos1);
         if ((rc < my_heap_count) && ((initscore[my_heap[rc]] > initscore[my_heap[j]]) || ((initscore[my_heap[rc]] == initscore[my_heap[j]]) && (my_heap[rc] < my_heap[j]))))
             j = rc;
         if (initscore[my_heap[pos1]] > initscore[my_heap[j]])
             return;
         my_heap_swap(pos1, j);
         pos1 = j;
     }
 }

 void my_heap_insert(int v)
 {
     int curr = my_heap_count++;
     my_heap[curr] = v;
     pos_in_my_heap[v] = curr;

     while (curr != 0 &&
            (initscore[my_heap[curr]] > initscore[my_heap[my_heap_parent(curr)]] || ((initscore[my_heap[curr]] == initscore[my_heap[my_heap_parent(curr)]]) && (my_heap[curr] < my_heap[my_heap_parent(curr)]))))
     {
         my_heap_swap(curr, my_heap_parent(curr));
         curr = my_heap_parent(curr);
     }
 }

 int my_heap_remove_first()
 {
     my_heap_swap(0, --my_heap_count);
     if (my_heap_count != 0)
         my_heap_shiftdown(0);
     return my_heap[my_heap_count];
 }

 int my_heap_remove(int pos)
 {
     if (pos == (my_heap_count - 1))
         my_heap_count--;
     else
     {
         my_heap_swap(pos, --my_heap_count);
         while ((pos != 0) &&
                ((initscore[my_heap[pos]] > initscore[my_heap[my_heap_parent(pos)]]) || ((initscore[my_heap[pos]] == initscore[my_heap[my_heap_parent(pos)]]) && my_heap[pos] < my_heap[my_heap_parent(pos)])))
         {
             my_heap_swap(pos, my_heap_parent(pos));
             pos = my_heap_parent(pos);
         }
         if (my_heap_count != 0)
             my_heap_shiftdown(pos);
     }
     return my_heap[my_heap_count];
 }
 
#endif