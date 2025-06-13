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

#define TIMING 0

struct customComp {
  bool operator()(const intvl_end &i1, const intvl_end &i2) const {
    return (i1.pI->ends[i1.iend] < i2.pI->ends[i2.iend]) ||
           ((i1.pI->ends[i1.iend] == i2.pI->ends[i2.iend] &&
             i1.iend > i2.iend));
  }
};

static void print_intervals(vector<intvl> &intervals) {
  cout << "\nnumber of intervals = " << intervals.size() << endl;
  for (auto intv : intervals) {
    cout << "(" << intv.ends[0] << "," << intv.ends[1] << ", " << intv.id
         << ") " << endl;
  }
}

int LsegIntersector::numIntx(int *filtered_pairs) {
  // collect all x end coordinates

  vector<intvl_end> sides;
  for (const auto &seg : segs_) {
    shared_ptr<intvl> inSeg = make_shared<intvl>(intvl{
        min(seg.S.x, seg.E.x) - tol_, max(seg.S.x, seg.E.x) + tol_, seg.id});
    sides.emplace_back(intvl_end{inSeg, 0});
    sides.emplace_back(intvl_end{inSeg, 1});
  }
  // sort all interval endpoints
  sort(sides.begin(), sides.end(), customComp());

  unordered_set<uint32_t> curr_ovlps;
  int nFiltered = 0, nIntx = 0;

  for (const auto &end : sides) {
    if (end.iend == 0) {
      for (auto id : curr_ovlps) {
        if (overlaps_along_y_and_diags(
                pair<uint32_t, uint32_t>{id, end.pI->id})) {
          ++nFiltered;
          double params[2];
          int pair_res =
              Lineseg::intx(segs_.at(id), segs_.at(end.pI->id), params);
          if (pair_res > 0)
            ++nIntx;
        }
      }
      auto ins = curr_ovlps.insert(end.pI->id);
    } else {
      curr_ovlps.extract(end.pI->id);
    }
  }

  if (filtered_pairs != nullptr) {
    *filtered_pairs = nFiltered;
  }
  return nIntx;
}

int LsegIntersector::numIntx_BF() {
  int nIntx = 0;
  double params[2];
  for (size_t is = 0; is < segs_.size(); ++is) {
    for (size_t js = is + 1; js < segs_.size(); ++js) {
      if (Lineseg::intx(segs_.at(is), segs_.at(js), params) > 0)
        ++nIntx;
    }
  }
  return nIntx;
}