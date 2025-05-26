#include "interval.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

// used to sort interval endpoints in increasing order
struct customComp
{
  bool operator()(const intvl_end &i1, const intvl_end &i2) const
  {
    return (i1.pI->ends[i1.iend] < i2.pI->ends[i2.iend]) ||
           ((i1.pI->ends[i1.iend] == i2.pI->ends[i2.iend] &&
             i1.iend > i2.iend));
  }
};

// debug code below
static void print_ends(vector<intvl_end> &vends)
{
  for (auto end : vends)
  {
    cout << "id = " << end.pI->id << ", side = " << end.iend
         << " , value = " << end.pI->ends[end.iend] << endl;
  }
}

void print_ovlp_pairs(vector<pair<uint32_t, uint32_t>> &pairs)
{
  cout << "number of pairs = " << pairs.size() << endl;
  for (auto p : pairs)
  {
    cout << "(" << p.first << "," << p.second << ")" << endl;
  }
}
