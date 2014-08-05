/*
 Copyright (C) 2011 John Burkardt
               2014 Alexandr Fonari


This code is distributed under the GNU LGPL license.

*/


/**
 * @file jb_spline.h 
 * @brief header file for the handling of spline from JB
 */


#ifndef JB_SPLINE
#define JB_SPLINE

#include "pspio_error.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
* Interpolation data structure
*/
typedef struct{
  // Objects to be used with jb_spline
  int np;       /**< JB spline structure */
  double* t;
  double* y;
  double* ypp;
} jb_spline_t;

/**********************************************************************
 * Routines                                                           *
 **********************************************************************/


jb_spline_t *jb_spline_alloc(const int np);

int jb_spline_init(jb_spline_t **spline, const double *f, const double *r, const int np);

void jb_spline_free(jb_spline_t **spline);

double jb_spline_eval(const jb_spline_t *spline, const double r);

double jb_spline_eval_deriv(const jb_spline_t *spline, const double r);

double jb_spline_eval_deriv2(const jb_spline_t *spline, const double r);
/**
 * Compute the second derivatives of a piecewise cubic spline using 'natural'
 * boundary conditions.
 * @param[in] filename: source filename (use NULL if none).
 * @param[in] line: line number in the source file (ignored if filename
 *            is NULL).
 */
double *jb_natural_spline_cubic_set(int n, const double* t, const double* y);


/**
 * Add an error to the chain.
 * @param[in] filename: source filename (use NULL if none).
 * @param[in] line: line number in the source file (ignored if filename
 *            is NULL).
 */
double *jb_spline_cubic_set (int n, const double *t, const double *y,
  int ibcbeg, double ybcbeg, int ibcend, double ybcend);


/**
 * Add an error to the chain.
 * @param[in] filename: source filename (use NULL if none).
 * @param[in] line: line number in the source file (ignored if filename
 *            is NULL).
 */
double jb_spline_cubic_val (int n, const double *t, const double *y,
  const double *ypp, double tval, double *ypval, double *yppval);


/**
 * Solves a pentadiagonal system of linear equations.
 * @param[in] filename: source filename (use NULL if none).
 * @param[in] line: line number in the source file (ignored if filename
 *            is NULL).
 */
double *penta ( int n, double a1[], double a2[], double a3[], double a4[], 
  double a5[], double b[] );


#endif
