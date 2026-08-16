//
// File: rt_assert.h
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.84
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 16 16:27:15 2026
//
#ifndef rt_assert_h_
#define rt_assert_h_

//=========*
//  Asserts *
// =========
#ifndef utAssert
#if defined(DOASSERTS)
#if !defined(PRINT_ASSERTS)
#include <assert.h>
#define utAssert(exp)                  assert(exp)
#else
#include <stdio.h>

static void _assert(char_T *statement, char_T *file, int line)
{
  printf("%s in %s on line %d\n", statement, file, line);
}

#define utAssert(_EX)                  ((_EX) ? (void)0 : _assert(#_EX, __FILE__, __LINE__))
#endif

#else
#define utAssert(exp)                                            // do nothing
#endif
#endif
#endif                                 // rt_assert_h_

//
// File trailer for generated code.
//
// [EOF]
//
