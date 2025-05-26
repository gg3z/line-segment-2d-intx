#include "lseg.h"
#include "lseg_intersector.h"
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>

static double getRandomDouble()
{
  // Seed the random number generator
  std::random_device rd;
  std::mt19937 gen(rd());

  // Define the distribution range (0.0 to 1.0)
  std::uniform_real_distribution<> dis(0.0, 1.0);

  // Generate and return the random double
  return dis(gen);
}

static Pnt2 getRandomPoint(double scale = 1.)
{
  return Pnt2{getRandomDouble() * scale, getRandomDouble() * scale};
}

static bool inUnitSquare(const Pnt2 &P)
{
  return P.x >= 0. && P.x <= 1. && P.y >= 0. && P.y <= 1.;
}

// one point of the output segment will lie in the unit square
// lower bound for length is to avoid creating many very short segments
// somewhat arbitrary but makes sense to me :)
static Lineseg getRandomSeg(double maxLen)
{
  auto P = getRandomPoint();
  Lineseg L(P, P);
  while (L.len() < 0.5 * maxLen || L.len() > maxLen)
  {
    L.E.x = P.x + getRandomDouble() * maxLen;
    L.E.y = P.y + getRandomDouble() * maxLen;
  }
  return L;
}

int test_intersector_1() // simple case with just 2 intersections
{
  LsegIntersector SI;
  uint32_t id = 0;
  SI.addSeg(Lineseg(Pnt2{3., 4.}, Pnt2{1., 2.}, id++));
  SI.addSeg(Lineseg(Pnt2{2., 1.}, Pnt2{5., 4.}, id++));
  SI.addSeg(Lineseg(Pnt2{6., 2.}, Pnt2{7., 3.}, id++));
  SI.addSeg(Lineseg(Pnt2{4., 4.}, Pnt2{5., 3.}, id++));
  SI.addSeg(Lineseg(Pnt2{6., 5.}, Pnt2{6., 6.}, id++));
  SI.addSeg(Lineseg(Pnt2{6., 2.}, Pnt2{8., 4.}, id++));
  int nExpected = 2, nIntx = SI.numIntx();
  // 1 transverse and 1 overlap
  cout << "num intersections = " << nIntx;
  if (nIntx == nExpected)
  {
    cout << " as expected\n";
  }
  else
  {
    cout << " but expected " << nExpected << endl;
  }
  return 0;
}

int test_intersector_2() // larger case with 56 intersections
{
  LsegIntersector SI;
  uint32_t id = 0;
  // each consecutive group of 4 segments forms a quadrilateral
  Pnt2 P0 = {0., 0.}, P1 = {3., 0.}, P2 = {3., 3.}, P3 = {0., 3.};
  SI.addSeg(Lineseg(P0, P1, id++));
  SI.addSeg(Lineseg(P1, P2, id++));
  SI.addSeg(Lineseg(P2, P3, id++));
  SI.addSeg(Lineseg(P3, P0, id++));

#if 1
  // same as last square, translated by 6.
  P0.x += 6.;
  P1.x += 6.;
  P2.x += 6.;
  P3.x += 6.;
  SI.addSeg(Lineseg(P0, P1, id++));
  SI.addSeg(Lineseg(P1, P2, id++));
  SI.addSeg(Lineseg(P2, P3, id++));
  SI.addSeg(Lineseg(P3, P0, id++));

  // same as last square, translated by 6.
  P0.x += 6.;
  P1.x += 6.;
  P2.x += 6.;
  P3.x += 6.;
  SI.addSeg(Lineseg(P0, P1, id++));
  SI.addSeg(Lineseg(P1, P2, id++));
  SI.addSeg(Lineseg(P2, P3, id++));
  SI.addSeg(Lineseg(P3, P0, id++));

  // now 3 diagonal rectangles
  Pnt2 P4 = {2., 1.}, P5 = {5., 4.}, P6 = {4., 5.}, P7 = {1., 2.};
  SI.addSeg(Lineseg(P4, P5, id++));
  SI.addSeg(Lineseg(P5, P6, id++));
  SI.addSeg(Lineseg(P6, P7, id++));
  SI.addSeg(Lineseg(P7, P4, id++));

  P4.x += 6.;
  P5.x += 6.;
  P6.x += 6.;
  P7.x += 6.;
  SI.addSeg(Lineseg(P4, P5, id++));
  SI.addSeg(Lineseg(P5, P6, id++));
  SI.addSeg(Lineseg(P6, P7, id++));
  SI.addSeg(Lineseg(P7, P4, id++));

  P4.x += 6.;
  P5.x += 6.;
  P6.x += 6.;
  P7.x += 6.;
  SI.addSeg(Lineseg(P4, P5, id++));
  SI.addSeg(Lineseg(P5, P6, id++));
  SI.addSeg(Lineseg(P6, P7, id++));
  SI.addSeg(Lineseg(P7, P4, id++));

  // now 3 opposite diagonal rectangles
  Pnt2 P8 = {4., 4.}, P9 = {7., 1.}, P10 = {8., 2.}, P11 = {5., 5.};
  SI.addSeg(Lineseg(P8, P9, id++));
  SI.addSeg(Lineseg(P9, P10, id++));
  SI.addSeg(Lineseg(P10, P11, id++));
  SI.addSeg(Lineseg(P11, P8, id++));

  P8.x += 6.;
  P9.x += 6.;
  P10.x += 6.;
  P11.x += 6.;
  SI.addSeg(Lineseg(P8, P9, id++));
  SI.addSeg(Lineseg(P9, P10, id++));
  SI.addSeg(Lineseg(P10, P11, id++));
  SI.addSeg(Lineseg(P11, P8, id++));

  P8.x += 6.;
  P9.x += 6.;
  P10.x += 6.;
  P11.x += 6.;
  SI.addSeg(Lineseg(P8, P9, id++));
  SI.addSeg(Lineseg(P9, P10, id++));
  SI.addSeg(Lineseg(P10, P11, id++));
  SI.addSeg(Lineseg(P11, P8, id++));
#endif
  int nFiltered[2] = {-1, -1};
  auto start_time = std::chrono::high_resolution_clock::now();
  int nExpected = 56, nIntx = SI.numIntx(nFiltered);
  auto end_time = std::chrono::high_resolution_clock::now();
  cout << "Runtime in milliseconds = "
       << std::chrono::duration<double, std::milli>(end_time - start_time).count()
       << endl;

  // total pairs: 630
  // filtered pairs: 56
  // 36 ennd-to-end intersections + 20 transversal
  cout << "num filtered pairs = " << nFiltered[0] << ", " << nFiltered[1]
       << endl;
  cout << "num intersections = " << nIntx;
  if (nIntx == nExpected)
  {
    cout << " as expected\n";
  }
  else
  {
    cout << " but expected " << nExpected << endl;
  }
  return 0;
}

// large case with many segments; all are close to or inside the unit square
int test_intersector_3(ofstream &out, int nSegments, double maxSegLength,
                       bool BF)
{
  LsegIntersector SI;
  uint32_t id = 0;
  for (int k = 0; k < nSegments; ++k)
  {
    Lineseg L = getRandomSeg(maxSegLength);
    if (L.len() > maxSegLength)
      cout << "$$$ segment is too long $$$\n";
    else if (L.len() < 0.5 * maxSegLength)
      cout << "$$$ segment is too short $$$\n";

    L.id = id++;
    SI.addSeg(L);
  }
  cout << "number of input segments = " << nSegments << " ----------" << endl;
  cout << "max segment length in unit square = " << maxSegLength << endl;

  if (BF)
  {
    auto start_time = std::chrono::high_resolution_clock::now();
    int nIntx = SI.numIntx_BF();
    auto end_time = std::chrono::high_resolution_clock::now();

    auto run_time =
        std::chrono::duration<double, std::milli>(end_time - start_time)
            .count();
    cout << "Brute force - Runtime in milliseconds = " << run_time << endl;
    cout << "num intersections = " << nIntx << endl;
    return 0;
  }

  int nFiltered[2] = {-1, -1};
  auto start_time = std::chrono::high_resolution_clock::now();
  int nIntx = SI.numIntx(nFiltered);
  auto end_time = std::chrono::high_resolution_clock::now();

  auto run_time =
      std::chrono::duration<double, std::milli>(end_time - start_time).count();
  out << nFiltered[1] << ' ' << run_time << endl;
  cout << "Efficient - Runtime in milliseconds = " << run_time << endl;
  cout << "num filtered pairs = " << nFiltered[0] << ", " << nFiltered[1]
       << endl;
  cout << "num intersections = " << nIntx << endl;
  return 0;
}
