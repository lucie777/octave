/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "subtraction.h"

#define SIZE 10000

/* #define LOCAL_DEBUG */

static void zdiffaTest(void) {
  doubleComplex M1[SIZE];
  doubleComplex M2[SIZE];
  doubleComplex M1_but_M2[SIZE];

  int i = 0;

  srand(1);
  for (i = 0; i < SIZE; ++i) {
    M1[i] = DoubleComplex((double) rand(), (double) rand());
    M2[i] = DoubleComplex((double) rand(), (double) rand());
  }
  zdiffa(M1, SIZE, M2, SIZE, M1_but_M2);
  for (i = 0; i < SIZE; ++i) {
#ifdef LOCAL_DEBUG
    printf("M1_but_M2 = %e + %e i\n", zreals(M1_but_M2[i]), zimags(M1_but_M2[i]));
#endif
    assert(zreals(M1_but_M2[i]) -  (zreals(M1[i]) - zreals(M2[i])) == 0);
    assert(zimags(M1_but_M2[i]) -  (zimags(M1[i]) - zimags(M2[i])) == 0);
  }
}

static void cdiffaTest(void) {
  floatComplex M1[SIZE];
  floatComplex M2[SIZE];
  floatComplex M1_but_M2[SIZE];

  int i = 0;

  srand(1);
  for (i = 0; i < SIZE; ++i) {
    M1[i] = FloatComplex((float) rand(), (float) rand());
    M2[i] = FloatComplex((float) rand(), (float) rand());
  }
  cdiffa(M1, SIZE, M2, SIZE, M1_but_M2);
  for (i = 0; i < SIZE; ++i) {
#ifdef LOCAL_DEBUG
    printf("M1_but_M2 = %e + %e i\n", creals(M1_but_M2[i]), cimags(M1_but_M2[i]));
#endif
    assert(fabsf(creals(M1_but_M2[i]) -  (creals(M1[i]) - creals(M2[i]))) / creals(M1_but_M2[i]) < 1e-07);
    assert(fabsf(cimags(M1_but_M2[i]) -  (cimags(M1[i]) - cimags(M2[i]))) / cimags(M1_but_M2[i]) < 1e-07);
  }
}

static void sdiffaTest(void) {
  float M1[SIZE];
  float M2[SIZE];
  float M1_but_M2[SIZE];

  int i = 0;

  srand(1);
  for (i = 0; i < SIZE; ++i) {
    M1[i] = (float) rand();
    M2[i] = (float) rand();
  }
  sdiffa(M1, SIZE, M2, SIZE, M1_but_M2);
  for (i = 0; i < SIZE; ++i) {
#ifdef LOCAL_DEBUG
    printf("M1_but_M2 = %e\n", M1_but_M2[i]);
    printf("M1[i] - M2[i] = %e\n", (M1[i] - M2[i]));
    printf("Error = %e\n",(float) fabsf(M1_but_M2[i] -  (M1[i] - M2[i])) / M1_but_M2[i]);
#endif
    assert(fabsf(M1_but_M2[i] -  (M1[i] - M2[i])) / M1_but_M2[i] < 1e-07);
  }
}

static void ddiffaTest(void) {
  double M1[SIZE];
  double M2[SIZE];
  double M1_but_M2[SIZE];

  int i = 0;

  srand(1);
  for (i = 0; i < SIZE; ++i) {
    M1[i] = (double) rand();
    M2[i] = (double) rand();
  }
  ddiffa(M1, SIZE, M2, SIZE, M1_but_M2);
  for (i = 0; i < SIZE; ++i) {
#ifdef LOCAL_DEBUG
   printf("M1_but_M2 = %e\n", M1_but_M2[i]);
   printf("M1[i] = %e\n", M1[i]);
   printf("M2[i] = %e\n", M2[i]);
   printf("M1[i] - M2[i] = %e\n", (M1[i] - M2[i]));
   printf("Error = %e\n", fabs(M1_but_M2[i] -  (M1[i] - M2[i])));
#endif
    assert(fabs(M1_but_M2[i] -  (M1[i] - M2[i])) == 0);
  }

}

static int testSubtraction(void) {

  printf("\n>>>> Subtraction Tests\n");
  sdiffaTest();
  ddiffaTest();
  cdiffaTest();
  zdiffaTest();

  return 0;
}



int main(void) {
  assert(testSubtraction() == 0);
  return 0;
}
