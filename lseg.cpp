#include "lseg.h"
#include <assert.h>
#include <iostream>

static bool test_intx1() // transverse intersection
{
  cout << "Testing transverse intx\n";
  Lineseg l1(Pnt2{-1., 1.}, Pnt2(2., 4.));
  Lineseg l2(Pnt2{1., 2.}, Pnt2(-1., 4.));
  double params[2];
  bool pass = Lineseg::intx(l1, l2, params) == 2;
  cout << "intx params = (" << params[0] << "," << params[1] << ")\n";
  return pass;
}

static bool test_intx2() // transverse intersection
{
  cout << "Testing transverse intx\n";
  Lineseg l1(Pnt2{5., 5.}, Pnt2(1., -1.));
  Lineseg l2(Pnt2{5., 2.}, Pnt2(-3., -2.));
  double params[2];
  bool pass = Lineseg::intx(l1, l2, params) == 2;
  cout << "intx params = (" << params[0] << "," << params[1] << ")\n";
  return pass;
}

static bool test_intx3() // touch intersection at end of l1
{
  cout << "Testing touch intx\n";
  Lineseg l1(Pnt2{5., 5.}, Pnt2(1., 0.));
  Lineseg l2(Pnt2{5., 2.}, Pnt2(-3., -2.));
  double params[2];
  bool pass = Lineseg::intx(l1, l2, params) == 1;
  cout << "intx params = (" << params[0] << "," << params[1] << ")\n";
  return pass;
}

static bool test_intx4() // no intersection
{
  cout << "Testing no intx (transverse)\n";
  Lineseg l1(Pnt2{5., 5.}, Pnt2(1., 2.5));
  Lineseg l2(Pnt2{5., 2.}, Pnt2(-3., -2.));
  double params[2];
  bool pass = Lineseg::intx(l1, l2, params) == 0;
  cout << "intx params = (" << params[0] << "," << params[1] << ")\n";
  return pass;
}

static bool test_intx5() // no intersection
{
  cout << "Testing no intx (parallel)\n";
  Lineseg l1(Pnt2{5., 5.}, Pnt2(-1., 2.));
  Lineseg l2(Pnt2{5., 2.}, Pnt2(-3., -2.));
  double params[2];
  bool pass = Lineseg::intx(l1, l2, params) == 0;
  cout << "intx params = (" << params[0] << "," << params[1] << ")\n";
  return pass;
}

void test_lineseg_intx()
{
  bool test1_result = test_intx1();
  cout << "test_intx1() ==> " << (test1_result ? "Pass" : "Fail") << endl;
  bool test2_result = test_intx2();
  cout << "test_intx2() ==> " << (test2_result ? "Pass" : "Fail") << endl;
  bool test3_result = test_intx3();
  cout << "test_intx3() ==> " << (test3_result ? "Pass" : "Fail") << endl;
  bool test4_result = test_intx4();
  cout << "test_intx4() ==> " << (test4_result ? "Pass" : "Fail") << endl;
  bool test5_result = test_intx5();
  cout << "test_intx5() ==> " << (test5_result ? "Pass" : "Fail") << endl;
}
