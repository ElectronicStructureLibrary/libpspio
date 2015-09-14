/*
 Copyright (C) 2011 John Burkardt
               2014 Alexandr Fonari
               2015 Micael Oliveira

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
typedef struct jb_spline jb_spline_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * 
 */
int jb_spline_alloc(jb_spline_t **spline, const int np);

/**
 * 
 */
int jb_spline_init(jb_spline_t **spline, const double *f, const double *r, 
		   const int np);

/**
 * 
 */
int jb_spline_copy(jb_spline_t **dst, const jb_spline_t *src);

/**
 * Compute the second derivatives of a piecewise cubic spline using 'natural'
 * boundary conditions.
 */
double *jb_natural_spline_cubic_init(int n, const double* t, const double* y);

/**
 * 
 */
double *jb_spline_cubic_init (int n, const double *t, const double *y, int ibcbeg,
			      double ybcbeg, int ibcend, double ybcend);

/**
 * 
 */
void jb_spline_free(jb_spline_t *spline);


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

/**
 * 
 */
double jb_spline_eval(const jb_spline_t *spline, const double r);

/**
 * 
 */
double jb_spline_eval_deriv(const jb_spline_t *spline, const double r);

/**
 * 
 */
double jb_spline_eval_deriv2(const jb_spline_t *spline, const double r);

/**
 * Evaluates a piecewise cubic spline at a point.
 */
void jb_spline_cubic_val(int n, const double *t, const double *y, const double *ypp, 
			 double tval, double *yval, double *ypval, double *yppval);

/**
 * Solves a pentadiagonal system of linear equations.
 */
double *penta(int n, double a1[], double a2[], double a3[], double a4[], 
	      double a5[], double b[]);

#endif
