#include "lseg_intersector.h"
#include "interval.h"
#include "lseg.h"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

#define TIMING 1

struct customComp
{
  bool operator()(const intvl_end &i1, const intvl_end &i2) const
  {
    return (i1.pI->ends[i1.iend] < i2.pI->ends[i2.iend]) ||
           ((i1.pI->ends[i1.iend] == i2.pI->ends[i2.iend] &&
             i1.iend > i2.iend));
  }
};

static void print_intervals(vector<intvl> &intervals)
{
  cout << "\nnumber of intervals = " << intervals.size() << endl;
  for (auto intv : intervals)
  {
    cout << "(" << intv.ends[0] << "," << intv.ends[1] << ", " << intv.id
         << ") " << endl;
  }
}

static void print_ovlp_pairs(vector<pair<uint32_t, uint32_t>> &pairs)
{
  cout << "\nnumber of pairs = " << pairs.size() << endl;
  for (auto p : pairs)
  {
    cout << "(" << p.first << "," << p.second << ") " << endl;
  }
}

static shared_ptr<vector<intvl>> get_x_intervals(vector<Lineseg> &segs,
                                                 double expand = 0.)
{
  shared_ptr<vector<intvl>> x_intervals = make_shared<vector<intvl>>();
  for (const auto &seg : segs)
  {
    x_intervals->emplace_back(intvl{min(seg.S.x, seg.E.x) - expand,
                                    max(seg.S.x, seg.E.x) + expand, seg.id});
  }
  return x_intervals;
}

static shared_ptr<vector<intvl>> get_y_intervals(vector<Lineseg> &segs,
                                                 double expand = 0.)
{
  shared_ptr<vector<intvl>> y_intervals = make_shared<vector<intvl>>();
  for (const auto &seg : segs)
  {
    y_intervals->emplace_back(intvl{min(seg.S.y, seg.E.y) - expand,
                                    max(seg.S.y, seg.E.y) + expand, seg.id});
  }
  return y_intervals;
}

static unique_ptr<vector<intvl_end>>
sorted_endpoints(vector<intvl> &intervals)
{
  vector<intvl_end> sides;
  for (auto seg1 : intervals)
  {
    auto endlambda = [seg1](int end)
    {
      intvl_end Iside = {make_shared<intvl>(seg1), end};
      return Iside;
    };
    sides.push_back(endlambda(0));
    sides.push_back(endlambda(1));
  }
  // sort all interval endpoints
  sort(sides.begin(), sides.end(), customComp());

  return make_unique<vector<intvl_end>>(sides);
}

// compute_overlaps() implements the initial filtering
// input intervals are the projections of each line segment on the x-axis
// those intervals are then broken down into
// endpoints which are sorted in increasing order
// overlaps are then collected by traversing the sorted endpoints

static shared_ptr<vector<pair<uint32_t, uint32_t>>>
compute_overlaps(vector<intvl> &intervals)
{
#if TIMING
  auto start_sort = std::chrono::high_resolution_clock::now();
#endif

  unique_ptr<vector<intvl_end>> seq = sorted_endpoints(intervals);

#if TIMING
  auto end_sort = std::chrono::high_resolution_clock::now();

  cout << "Sort x-interval ends = "
       << std::chrono::duration<double, std::milli>(end_sort - start_sort).count()
       << endl;
#endif

  unordered_set<uint32_t> curr_ovlps;

#if TIMING
  auto start_insert = std::chrono::high_resolution_clock::now();
#endif

  vector<pair<uint32_t, uint32_t>> ovlp_pairs;

  for (auto end : *seq)
  {
    if (end.iend == 0)
    {
      for (auto id : curr_ovlps)
      {
        ovlp_pairs.emplace_back(pair<uint32_t, uint32_t>{id, end.pI->id});
      }
      auto ins = curr_ovlps.insert(end.pI->id);
    }
    else
    {
      curr_ovlps.extract(end.pI->id);
    }
  }
#if TIMING
  auto end_insert = std::chrono::high_resolution_clock::now();
  cout << "Time to construct " << ovlp_pairs.size() << " x-overlap pairs = "
       << std::chrono::duration<double, std::milli>(end_insert - start_insert).count()
       << endl;
#endif

  return make_shared<vector<pair<uint32_t, uint32_t>>>(ovlp_pairs);
}

int LsegIntersector::numIntx(int *filtered_pairs)
{
  // collect all x end coordinates
#if TIMING
  auto start_getx = std::chrono::high_resolution_clock::now();
#endif

  shared_ptr<vector<intvl>> x_intervals = get_x_intervals(segs_, tol_);

#if TIMING
  auto end_getx = std::chrono::high_resolution_clock::now();

  cout << "Get x-intervals = "
       << std::chrono::duration<double, std::milli>(end_getx - start_getx).count()
       << endl;
#endif

  if (dbg) { print_intervals(*x_intervals); }

  // aggregate the overlapping segment pairs as projected on the x-axis
  shared_ptr<vector<pair<uint32_t, uint32_t>>> xovlp_pairs =
      compute_overlaps(*x_intervals);

  if (dbg) { print_ovlp_pairs(*xovlp_pairs); }

  // filter the x-overlap pairs against the y-axis
  // and along both diagonals (can be any direction)
  // this should provide a performance boost though it will be variable

  vector<pair<uint32_t, uint32_t>> yovlp_pairs;

#if TIMING
  auto start_time = std::chrono::high_resolution_clock::now();
#endif

  for (const auto &op : *xovlp_pairs)
  {
    if (overlaps_along_y(op) && overlaps_along_diag_pos(op) &&
        overlaps_along_diag_neg(op))
    {
      yovlp_pairs.emplace_back(op);
    }
  }

#if TIMING
  auto end_time = std::chrono::high_resolution_clock::now();

  cout << "Direction filter = "
       << std::chrono::duration<double, std::milli>(end_time - start_time).count()
       << endl;
#endif

  if (dbg) { print_ovlp_pairs(yovlp_pairs); }

  int nIntx = 0, nTouch = 0, nTrans = 0;
#if TIMING
  auto start_intx = std::chrono::high_resolution_clock::now();
#endif

  for (const auto &op : yovlp_pairs)
  {
    Lineseg &l1 = segs_.at(op.first);
    Lineseg &l2 = segs_.at(op.second);
    double params[2];
    int pair_res = Lineseg::intx(l1, l2, params);
    if (dbg)
    {
      if (pair_res == 1)
        ++nTouch;
      else if (pair_res == 2)
        ++nTrans;
      cout << "Result for pair (" << op.first << "," << op.second
           << ") = " << pair_res << endl;
    }
    if (pair_res > 0)
      ++nIntx;
  }

#if TIMING
  auto end_intx = std::chrono::high_resolution_clock::now();

  cout << "Compute Intx = "
       << std::chrono::duration<double, std::milli>(end_intx - start_intx).count()
       << endl;
#endif

  if (filtered_pairs != nullptr)
  {
    filtered_pairs[0] = (int)xovlp_pairs->size();
    filtered_pairs[1] = (int)yovlp_pairs.size();
  }
  if (dbg)
  {
    cout << "Transverse intx = " << nTrans << endl;
    cout << "Touch intx = " << nTouch << endl;
  }
  return nIntx;
}

int LsegIntersector::numIntx_BF()
{
  int nIntx = 0;
  double params[2];
  for (size_t is = 0; is < segs_.size(); ++is)
  {
    for (size_t js = is + 1; js < segs_.size(); ++js)
    {
      if (Lineseg::intx(segs_.at(is), segs_.at(js), params) > 0)
        ++nIntx;
    }
  }
  return nIntx;
}
