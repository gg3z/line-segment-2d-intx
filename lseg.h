#pragma once
#include <cstdint>
#include <iostream>

using namespace std;

static bool dbg = false;

// this file sets up the geometry objects and utilities that support the line
// segment operations, mainly min distance and intersection

const double eps = 1.e-12; // lower bound for non-zero values; TBD: get this
                           // value from the intersector class

struct Pnt2 {
  double x;
  double y;

  Pnt2() : x(0.), y(0.) {}
  Pnt2(double x, double y) : x(x), y(y) {}
  Pnt2(const Pnt2 &P) : x(P.x), y(P.y) {}

  double distSq(const Pnt2 &P) const {
    return (x - P.x) * (x - P.x) + (y - P.y) * (y - P.y);
  }
  double dist(const Pnt2 &P) const { return sqrt(distSq(P)); }
};

struct Vec2 {
  double x;
  double y;

  Vec2() : x(0.), y(0.) {}
  Vec2(const Vec2 &V) : x(V.x), y(V.y) {}
  Vec2(const Pnt2 &P1, const Pnt2 &P2) : x(P2.x - P1.x), y(P2.y - P1.y) {}

  double lenSq() const { return x * x + y * y; }
  double len() const { return sqrt(lenSq()); }
  double dot(const Vec2 &V) const { return x * V.x + y * V.y; }

  static double CrossZ(const Vec2 &V1, const Vec2 &V2) {
    return V1.x * V2.y - V1.y * V2.x; // z-coordinate of cross-product; x- and
                                      // y-coordinates are 0.
  }
};

struct Lineseg {
  Pnt2 S;
  Pnt2 E;
  uint32_t id;

  Lineseg() : S(), E(), id(0) {}
  Lineseg(const Lineseg &Seg) : S(Seg.S), E(Seg.E), id(Seg.id) {}
  Lineseg(const Pnt2 &P, const Pnt2 &Q, uint32_t id = 0) : S(P), E(Q), id(id) {}

  double lenSq() const { return S.distSq(E); }
  double len() const { return sqrt(lenSq()); }

  // distance to a point
  // TBD: return closest parameter
  double dist(const Pnt2 &P, bool bounded = true) const {
    const double eps = 1.e-12; // could/should be exposed

    double distSsq = S.distSq(P);
    double distEsq = E.distSq(P);
    double minEnds = sqrt(min(distEsq, distSsq));
    if (len() < eps) {
      return minEnds; // regardless of bounded option
    }
    return min(minEnds, abs(Vec2::CrossZ(Vec2(S, P), Vec2(S, E))) / len());
  }

  // all cases are handled:
  // - transverse (not parallel) if applicable
  // - all remaining cases are covered by min distance between point and line
  // segment
  static int intx(const Lineseg &l1, const Lineseg &l2, double params[2],
                  double tol = eps) {
    // handle general transverse case
    bool inBounds = false;
    double D = Vec2::CrossZ(Vec2(l1.S, l1.E), Vec2(l2.S, l2.E));
    if (dbg)
      cout << "D = " << D << endl;
    if (fabs(D) > eps * l1.len() * l2.len()) // not parallel
    {
      Vec2 PP(l1.S, l2.S);
      double alfa = Vec2::CrossZ(PP, Vec2(l2.S, l2.E)) / D;
      double beta = Vec2::CrossZ(PP, Vec2(l1.S, l1.E)) / D;
      if (dbg)
        cout << "alfa = " << alfa << ", beta = " << beta << endl;
      if (params != nullptr) {
        params[0] = alfa;
        params[1] = beta;
      }
      // using strict inequality because boundary cases are handled
      // by min distance in next block
      inBounds |= (alfa > 0. && alfa < 1. && beta > 0. && beta < 1.);
    } else {
      if (dbg) {
        cout << "Segments are parallel or degenerate\n";
      }
    }
    // all other cases are covered by min distance between one segment endpoint
    // and the other segment
    if (!inBounds) {
      double l1l2S = l1.dist(l2.S), l1l2E = l1.dist(l2.E);
      double l2l1S = l2.dist(l1.S), l2l1E = l2.dist(l1.E);
      double minEnds = min(min(l1l2S, l1l2E), min(l2l1S, l2l1E));
      if (dbg) {
        cout << "l1l2S = " << l1l2S << ", l1l2E = " << l1l2E << endl;
        cout << "l2l1S = " << l2l1S << ", l2l1E = " << l2l1E << endl;
        cout << "minEnds = " << minEnds << endl;
      }
      return minEnds < tol ? 1 : 0;
    }
    if (dbg) {
      cout << "transverse intersection\n";
    }
    return 2;
  }
};

void test_lineseg_intx();
