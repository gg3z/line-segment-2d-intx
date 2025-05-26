#pragma once
#include "interval.h"
#include "lseg.h"
#include <cstdint>
#include <vector>

using namespace std;

class LsegIntersector
{
  vector<Lineseg> segs_;
  double tol_;

  // this function can easily be generalized to any 2d vector
  bool overlaps_along_y(const pair<uint32_t, uint32_t> &op) const
  {
    auto &seg1 = segs_[op.first];
    intvl int1 = {min(seg1.S.y, seg1.E.y) - tol_,
                  max(seg1.S.y, seg1.E.y) + tol_};
    auto &seg2 = segs_[op.second];
    intvl int2 = {min(seg2.S.y, seg2.E.y) - tol_,
                  max(seg2.S.y, seg2.E.y) + tol_};
    return intvl::intvl_ovlp(int1, int2);
  }

  bool overlaps_along_diag_pos(const pair<uint32_t, uint32_t> &op) const
  {
    auto &seg1 = segs_[op.first];
    auto d1S = 0.5 * (seg1.S.x + seg1.S.y);
    auto d1E = 0.5 * (seg1.E.x + seg1.E.y);
    intvl int1 = {min(d1S, d1E) - tol_, max(d1S, d1E) + tol_};
    auto &seg2 = segs_[op.second];
    auto d2S = 0.5 * (seg2.S.x + seg2.S.y);
    auto d2E = 0.5 * (seg2.E.x + seg2.E.y);
    intvl int2 = {min(d2S, d2E) - tol_, max(d2S, d2E) + tol_};
    return intvl::intvl_ovlp(int1, int2);
  }

  bool overlaps_along_diag_neg(const pair<uint32_t, uint32_t> &op) const
  {
    auto &seg1 = segs_[op.first];
    auto d1S = 0.5 * (seg1.S.x - seg1.S.y);
    auto d1E = 0.5 * (seg1.E.x - seg1.E.y);
    intvl int1 = {min(d1S, d1E) - tol_, max(d1S, d1E) + tol_};
    auto &seg2 = segs_[op.second];
    auto d2S = 0.5 * (seg2.S.x - seg2.S.y);
    auto d2E = 0.5 * (seg2.E.x - seg2.E.y);
    intvl int2 = {min(d2S, d2E) - tol_, max(d2S, d2E) + tol_};
    return intvl::intvl_ovlp(int1, int2);
  }

public:
  LsegIntersector() : tol_(1.e-12) {}

  void setTol(double tol) { tol_ = tol; }

  int addSeg(const Lineseg &seg)
  {
    // leaving id tracking to the caller
    segs_.emplace_back(seg);
    return (int)segs_.size();
  }

  int numIntx_BF(); // brute force - intersect every pair

  // 2-stage pair filtration
  int numIntx(int filtered_pairs[2] = nullptr);
};

// initial set if tests - considerably more should be added
int test_intersector_1();
int test_intersector_2();
int test_intersector_3(ofstream &out, int nSegments, double maxSegLength,
                       bool BF = false);
