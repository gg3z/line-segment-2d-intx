# line-segment-2d-intx
Implements an efficient computation of all intersections of a set of 2D line segments

This repo can be built in VS code with most c++20 compilers (I use clang 19.1.2)
Steps:
1. git clone to a local directory
2. open folder in VS code and configure (need CMake and ninja or equivalent)
3. Build (successful if ./build/lineseg.exe is present)
4. execute ./run_case.sh random_segs_100_1.txt (or any other text file of same format)
5. the console output will show the runtime and the number of intersections (and a couple of intermediate numbers)
6. the segments can be visualized by running: python graph_segments_2d.py random_segs_100_1.txt

There are a few pre-defined cases, to see them: ls random*.txt
Custom cases can be created manually, the format is very simple: each line holds one 2d segment [x1 y1 x2 y2].

To run a custom case:
- save it in the top level directory ('my_custom_case.txt')
- replace random_segs_100_1.txt with my_custom_case.txt
- do steps 2-4 above with your custom case.

Notes about the implementation:
A simple but fairly complete c++ code that returns the number of intersections in the input set within a specified tolerance (default = 1.e-12)

The types of intersection currently reported are:
- 0: transversal
- 1: touch intersection at one of the segment endpoints
- 2: overlap of the input segments

The performance of this code should be on par with that of the Bentley-Ottman algorithm, though a head-to-head comparison hasn't been performed yet.

Both algorithms run in o(nlog(n)), and both are sensitive to the number of intersections, so the runtime will be closer to o(n*n) if most segments intersect another segment.

Notes about building:
- build folder created by CMake in VS Code
- CMake kit used: clang 19.1.2 (NOT clang-cl 19.1.2)

Miscellaneous:
- the folder initial_runtime_data was generated on a range of randomly generated cases
- run graph_univariate.py to visualize the effect of the number of intersections on the runtime

Please send any comments/requests to ggChoud@gmail.com
