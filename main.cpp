#include "lseg_intersector.h"
#include <fstream>
#include <iostream>

/*
 * The code in this folder finds all intersections for a given input list of
 * 2d line segments. It has only been tested on a CMake build in VS Code, but
 * should not present difficulties to build and run in other build environments
 *
 * Algorithmically, it is structured in 2 phases:
 * - filtering of non overlap/intersection pairs of segments
 *    - this is done by first sorting the x-coordinates of all segments
 *    - while tracking the owning segment,
 *    - then sweeping through the x-values and aggregating pairs of indices
 *    - the resulting pairs are further filtered along other directions
 *
 * - fine computation of the intersection result for each remaining pair
 *    - first compute the transverse intersection if determinant != 0
 *    - if needed, calculate all min distances from each endpoint to the other
 *      segment
 *
 * Computational complexity: for an input list of n segments, the cost of this
 * algorithm will be o(nlog(n)) which is comparable to the Bentley-Ottman
 * algorithm and is optimal.
 * The algorithm as implemented is similar to the Bentley-Ottman approach and
 * will run faster when there are fewer intersections relative to the total
 * number of pairs.
 *
 * Possible enhancements to the current code:
 * - performance: filtering can easily be improved by choosing additional
 *  directions
 * - performance: intersection code can be streamlined to run faster
 *
 * - functionality: intersection types can be distinguished between:
 *                  - transverse intersection
 *                  - touch intersection (with interior of one segment)
 *                  - touch intersection (end-to-end)
 *                  - overlap of length > tol
 *   functionality: the above intersection types would allow end-to-end
 *                  intersections in a polyline to be ignored
 * - functionality: reporting intersections between 2 or more groups of segments
 *                  would be highly desirable
 *
 * - testing: more test cases to be added for better case coverage
 *            - for both pair-wise intersection and intersection detection
 *              between groups of segments
 *
 * - architecture: abstract classes can be defined with interfaces that don't
 *  depend on the implementation
 */

int main() {
#if 0
  test_lineseg_intx();
  cout << "--- running test case 1 -----------\n";
  int n1 = test_intersector_1();
  cout << "--- running test case 2 -----------\n";
  int n2 = test_intersector_2();
  cout << "--- running test case 3 -----------\n";

  const int nSegments = 1000;
  std::string fname = "data_1000.txt";
  std::ofstream timing_out(fname);

  test_intersector_3(timing_out, nSegments, 0.05);

  // for testing and benchmarking purposes
  // for (double maxSegLen = 0.01; maxSegLen < 0.21; maxSegLen += 0.01) {
  //   [[maybe_unused]] int n3 =
  //       test_intersector_3(timing_out, nSegments, maxSegLen);
  // }
  timing_out.close();
#endif

  // generate_random_case(1000, 0.1, "random_segs_1000_1.txt");
  string case_file("current_case.txt");
  ifstream segfile(case_file);
  if (segfile.is_open()) {
    string line;
    std::getline(segfile, line);

    test_intersector_from_file(line);
  }

  return 0;
}
