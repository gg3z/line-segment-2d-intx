#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <utility>

using namespace std;

// basic 1-d interval struct
struct intvl
{
  array<double, 2> ends;
  // pair<double, double> ends;
  uint32_t id;

  // assumes interval bounds are in non-decreasing order
  static bool intvl_ovlp(const intvl &int1, const intvl &int2)
  {
    return !(int1.ends[0] > int2.ends[1] || int1.ends[1] < int2.ends[0]);
  }
};

// this struct helps to isolate each interval endpoint
// without losing track of its id
struct intvl_end
{
  shared_ptr<intvl> pI;
  int iend;
};

struct pair_hash
{
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2> &pair) const
  {
    auto hash1 = hash<T1>{}(pair.first);
    auto hash2 = hash<T2>{}(pair.second);
    return hash1 ^ (hash2 << 1); // Combine the two hashes
  }
};
