# line-segment-2d-intx
Implements an efficient computation of all intersections of a set of 2D line segments

A simple but fairly complete code that for an input set of line segments in the xy plane, returns the number of intersections in the set within a specified tolerance.
The types of intersection currently reported are:
- 0: transversal
- 1: touch intersection at one of the segment endpoints
- 2: overlap of the input segments
- another type is planned (touch end-to-end)

The code is all C++ 17/20, except for a python script to visualize results.
The performance of this code should be on par with that of the Bentley-Ottman algorithm, though a head-to-head comparison hasn't been performed yet.
Both algorithms run in o(nlog(n)), and both are sensitive to the number of intersections, so the runtime will be closer to o(n*n) if most segments intersect another segment.

NOTE: the enclosed CMakeLists.txt references the clang compiler and assumes that LLVM has been installed. Using another compiler should not cause an issue, or maybe very minor ones.
