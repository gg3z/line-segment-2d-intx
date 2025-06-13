#pragma once
#include "interval.h"
#include "lseg.h"
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class LsegIntersector {
  vector<Lineseg> segs_;
  double tol_;

  // this function can easily be generalized to any 2d vector
  bool overlaps_along_y(const pair<uint32_t, uint32_t> &op) const {
    auto &seg1 = segs_[op.first];
    intvl int1 = {min(seg1.S.y, seg1.E.y) - tol_,
                  max(seg1.S.y, seg1.E.y) + tol_};
    auto &seg2 = segs_[op.second];
    intvl int2 = {min(seg2.S.y, seg2.E.y) - tol_,
                  max(seg2.S.y, seg2.E.y) + tol_};
    return intvl::intvl_ovlp(int1, int2);
  }

  bool overlaps_along_diag_pos(const pair<uint32_t, uint32_t> &op) const {
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

  bool overlaps_along_diag_neg(const pair<uint32_t, uint32_t> &op) const {
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

  bool overlaps_along_y_and_diags(const pair<uint32_t, uint32_t> &op) const {
    auto &S1 = segs_[op.first].S;
    auto &S2 = segs_[op.second].S;
    auto &E1 = segs_[op.first].E;
    auto &E2 = segs_[op.second].E;
    return (max(S1.y, E1.y) + 2. * tol_ > min(S2.y, E2.y)) &&
           (min(S1.y, E1.y) < max(S2.y, E2.y) + 2. * tol_) &&
           (max(S1.x + S1.y, E1.x + E1.y) + 4. * tol_ >
            min(S2.x + S2.y, E2.x + E2.y)) &&
           (min(S1.x + S1.y, E1.x + E1.y) <
            max(S2.x + S2.y, E2.x + E2.y) + 4. * tol_) &&
           (max(S1.x - S1.y, E1.x - E1.y) + 4. * tol_ >
            min(S2.x - S2.y, E2.x - E2.y)) &&
           (min(S1.x - S1.y, E1.x - E1.y) <
            max(S2.x - S2.y, E2.x - E2.y) + 4. * tol_);
  }

public:
  LsegIntersector() : tol_(1.e-12) {}

  void setTol(double tol) { tol_ = tol; }

  int addSeg(const Lineseg &seg) {
    // leaving id tracking to the caller
    segs_.emplace_back(seg);
    return (int)segs_.size();
  }

  int numIntx_BF(); // brute force - intersect every pair

  // 2-stage pair filtration
  int numIntx(int *filtered_pairs = nullptr);
};

// initial set if tests - considerably more should be added
int test_intersector_1();
int test_intersector_2();
int test_intersector_3(ofstream &out, int nSegments, double maxSegLength,
                       bool BF = false);

shared_ptr<vector<Lineseg>> random_segment_generator(int nSeg, double maxLen);

shared_ptr<vector<Lineseg>> read_segments_from_file(string segfile);
void write_segments_to_file(vector<Lineseg> &segments, string segfile);

void generate_random_case(int nSeg, double maxsegLen, string caseName);

int test_intersector_from_file(string segfile);