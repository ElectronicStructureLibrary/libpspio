/* Copyright (C) 2014-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
 */

/** 
 * @file jb_spline.c
 * @brief implementation to read and write in FHI files 
 */
#include <assert.h>
# include <stdlib.h>
# include <string.h>

#include "pspio_jb_spline.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * Interpolation data structure
 */
struct jb_spline_t {
    /* Objects to be used with jb_spline */
    int np;       /**< JB spline structure */
    double* t;
    double* y;
    double* ypp;
};


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int jb_spline_alloc(jb_spline_t **spline, int np)
{
  *spline = (jb_spline_t *) malloc (sizeof(jb_spline_t));
  FULFILL_OR_EXIT(*spline != NULL, PSPIO_ENOMEM);

  (*spline)->np = np;

  (*spline)->t = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT((*spline)->t != NULL, PSPIO_ENOMEM);

  (*spline)->y = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT((*spline)->y != NULL, PSPIO_ENOMEM);

  (*spline)->ypp = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT((*spline)->ypp != NULL, PSPIO_ENOMEM);

  return PSPIO_SUCCESS;
}

int jb_spline_init(jb_spline_t **spline, const double *r, const double *f, int np)
{
  memcpy((*spline)->t, r, np * sizeof(double));
  memcpy((*spline)->y, f, np * sizeof(double));
  (*spline)->ypp = jb_natural_spline_cubic_init(np, r, f);

  return PSPIO_SUCCESS;
}

double *jb_spline_cubic_init(int n, const double *t, const double *y, int ibcbeg,
			     double ybcbeg, int ibcend, double ybcend)
/******************************************************************************/
/*
  Purpose:

    SPLINE_CUBIC_INIT computes the second derivatives of a piecewise cubic spline.

  Discussion:

    For data interpolation, the user must call SPLINE_INIT to determine
    the second derivative data, passing in the data to be interpolated,
    and the desired boundary conditions.

    The data to be interpolated, plus the SPLINE_INIT output, defines
    the spline.  The user may then call SPLINE_VAL to evaluate the
    spline at any point.

    The cubic spline is a piecewise cubic polynomial.  The intervals
    are determined by the "knots" or abscissas of the data to be
    interpolated.  The cubic spline has continous first and second
    derivatives over the entire interval of interpolation.

    For any point T in the interval T(IVAL), T(IVAL+1), the form of
    the spline is

      SPL(T) = A(IVAL)
             + B(IVAL) * ( T - T(IVAL) )
             + C(IVAL) * ( T - T(IVAL) )^2
             + D(IVAL) * ( T - T(IVAL) )^3

    If we assume that we know the values Y(*) and YPP(*), which represent
    the values and second derivatives of the spline at each knot, then
    the coefficients can be computed as:

      A(IVAL) = Y(IVAL)
      B(IVAL) = ( Y(IVAL+1) - Y(IVAL) ) / ( T(IVAL+1) - T(IVAL) )
        - ( YPP(IVAL+1) + 2 * YPP(IVAL) ) * ( T(IVAL+1) - T(IVAL) ) / 6
      C(IVAL) = YPP(IVAL) / 2
      D(IVAL) = ( YPP(IVAL+1) - YPP(IVAL) ) / ( 6 * ( T(IVAL+1) - T(IVAL) ) )

    Since the first derivative of the spline is

      SPL'(T) =     B(IVAL)
              + 2 * C(IVAL) * ( T - T(IVAL) )
              + 3 * D(IVAL) * ( T - T(IVAL) )^2,

    the requirement that the first derivative be continuous at interior
    knot I results in a total of N-2 equations, of the form:

      B(IVAL-1) + 2 C(IVAL-1) * (T(IVAL)-T(IVAL-1))
      + 3 * D(IVAL-1) * (T(IVAL) - T(IVAL-1))^2 = B(IVAL)

    or, setting H(IVAL) = T(IVAL+1) - T(IVAL)

      ( Y(IVAL) - Y(IVAL-1) ) / H(IVAL-1)
      - ( YPP(IVAL) + 2 * YPP(IVAL-1) ) * H(IVAL-1) / 6
      + YPP(IVAL-1) * H(IVAL-1)
      + ( YPP(IVAL) - YPP(IVAL-1) ) * H(IVAL-1) / 2
      =
      ( Y(IVAL+1) - Y(IVAL) ) / H(IVAL)
      - ( YPP(IVAL+1) + 2 * YPP(IVAL) ) * H(IVAL) / 6

    or

      YPP(IVAL-1) * H(IVAL-1) + 2 * YPP(IVAL) * ( H(IVAL-1) + H(IVAL) )
      + YPP(IVAL) * H(IVAL)
      =
      6 * ( Y(IVAL+1) - Y(IVAL) ) / H(IVAL)
      - 6 * ( Y(IVAL) - Y(IVAL-1) ) / H(IVAL-1)

    Boundary conditions must be applied at the first and last knots.  
    The resulting tridiagonal system can be solved for the YPP values.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    07 June 2013

  Author:

    John Burkardt

  Reference:

    Carl deBoor,
    A Practical Guide to Splines,
    Springer, 2001,
    ISBN: 0387953663.

  Parameters:

    Input, int N, the number of data points.  N must be at least 2.
    In the special case where N = 2 and IBCBEG = IBCEND = 0, the
    spline will actually be linear.

    Input, double T[N], the knot values, that is, the points were data is
    specified.  The knot values should be distinct, and increasing.

    Input, double Y[N], the data values to be interpolated.

    Input, int IBCBEG, left boundary condition flag:
    0: the cubic spline should be a quadratic over the first interval;
    1: the first derivative at the left endpoint should be YBCBEG;
    2: the second derivative at the left endpoint should be YBCBEG;
    3: Not-a-knot: the third derivative is continuous at T(2).

    Input, double YBCBEG, the values to be used in the boundary
    conditions if IBCBEG is equal to 1 or 2.

    Input, int IBCEND, right boundary condition flag:
    0: the cubic spline should be a quadratic over the last interval;
    1: the first derivative at the right endpoint should be YBCEND;
    2: the second derivative at the right endpoint should be YBCEND;
    3: Not-a-knot: the third derivative is continuous at T(N-1).

    Input, double YBCEND, the values to be used in the boundary
    conditions if IBCEND is equal to 1 or 2.

    Output, double SPLINE_CUBIC_INIT[N], the second derivatives 
    of the cubic spline.
*/
{
  double *a1;
  double *a2;
  double *a3;
  double *a4;
  double *a5;
  double *b;
  int i;
  double *ypp;
/*
  Check.
*/
  if ( n <= 1 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "SPLINE_CUBIC_INIT - Fatal error!\n" );
    fprintf ( stderr, "  The number of data points N must be at least 2.\n" );
    fprintf ( stderr, "  The input value is %d.\n", n );
    exit ( 1 );
  }

  for ( i = 0; i < n - 1; i++ )
  {
    if ( t[i+1] <= t[i] )
    {
      fprintf ( stderr, "\n" );
      fprintf ( stderr, "SPLINE_CUBIC_INIT - Fatal error!\n" );
      fprintf ( stderr, "  The knots must be strictly increasing, but\n" );
      fprintf ( stderr, "  T(%d) = %g\n", i, t[i] );
      fprintf ( stderr, "  T(%d) = %g\n", i+1, t[i+1] );
      exit ( 1 );
    }
  }
  a1 = ( double * ) malloc ( n * sizeof ( double ) );
  a2 = ( double * ) malloc ( n * sizeof ( double ) );
  a3 = ( double * ) malloc ( n * sizeof ( double ) );
  a4 = ( double * ) malloc ( n * sizeof ( double ) );
  a5 = ( double * ) malloc ( n * sizeof ( double ) );
  b = ( double * ) malloc ( n * sizeof ( double ) );

  for ( i = 0; i < n; i++ )
  {
    a1[i] = 0.0;
    a2[i] = 0.0;
    a3[i] = 0.0;
    a4[i] = 0.0;
    a5[i] = 0.0;
  }
/*
  Set up the first equation.
*/
  if ( ibcbeg == 0 )
  {
    b[0] = 0.0;
    a3[0] = 1.0;
    a4[0] = -1.0;
  }
  else if ( ibcbeg == 1 )
  {
    b[0] = ( y[1] - y[0] ) / ( t[1] - t[0] ) - ybcbeg;
    a3[0] = ( t[1] - t[0] ) / 3.0;
    a4[0] = ( t[1] - t[0] ) / 6.0;
  }
  else if ( ibcbeg == 2 )
  {
    b[0] = ybcbeg;
    a3[0] = 1.0;
    a4[0] = 0.0;
  }
  else if ( ibcbeg == 3 )
  {
    b[0] = 0.0;
    a3[0] = - ( t[2] - t[1] );
    a4[0] =   ( t[2]        - t[0] );
    a5[0] = - (        t[1] - t[0] );
  }
  else
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "SPLINE_CUBIC_INIT - Fatal error!\n" );
    fprintf ( stderr, "  IBCBEG must be 0, 1 or 2.\n" );
    fprintf ( stderr, "  The input value is %d.\n", ibcbeg );
    exit ( 1 );
  }
/*
  Set up the intermediate equations.
*/
  for ( i = 1; i < n - 1; i++ )
  {
    b[i] = ( y[i+1] - y[i] ) / ( t[i+1] - t[i] )
      - ( y[i] - y[i-1] ) / ( t[i] - t[i-1] );
    a2[i] = ( t[i+1] - t[i]   ) / 6.0;
    a3[i] = ( t[i+1] - t[i-1] ) / 3.0;
    a4[i] = ( t[i]   - t[i-1] ) / 6.0;
  }
/*
  Set up the last equation.
*/
  if ( ibcend == 0 )
  {
    b[n-1] = 0.0;
    a2[n-1] = -1.0;
    a3[n-1] = 1.0;
  }
  else if ( ibcend == 1 )
  {
    b[n-1] = ybcend - ( y[n-1] - y[n-2] ) / ( t[n-1] - t[n-2] );
    a2[n-1] = ( t[n-1] - t[n-2] ) / 6.0;
    a3[n-1] = ( t[n-1] - t[n-2] ) / 3.0;
  }
  else if ( ibcend == 2 )
  {
    b[n-1] = ybcend;
    a2[n-1] = 0.0;
    a3[n-1] = 1.0;
  }
  else if ( ibcbeg == 3 )
  {
    b[n-1] = 0.0;
    a1[n-1] = - ( t[n-1] - t[n-2] );
    a2[n-1] =   ( t[n-1]          - t[n-3] );
    a3[n-1] = - (          t[n-2] - t[n-3] );
  }
  else
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "SPLINE_CUBIC_INIT - Fatal error!\n" );
    fprintf ( stderr, "  IBCEND must be 0, 1 or 2.\n" );
    fprintf ( stderr, "  The input value is %d.\n", ibcend );
    exit ( 1 );
  }
/*
  Solve the linear system.
*/
  if ( n == 2 && ibcbeg == 0 && ibcend == 0 )
  {
    ypp = ( double * ) malloc ( 2 * sizeof ( double ) );

    ypp[0] = 0.0;
    ypp[1] = 0.0;
  }
  else
  {
    ypp = penta ( n, a1, a2, a3, a4, a5, b );
  }

  free ( a1 );
  free ( a2 );
  free ( a3 );
  free ( a4 );
  free ( a5 );
  free ( b );

  return ypp;
}

double *jb_natural_spline_cubic_init(int n, const double *t, const double *y)
{
  int ibcbeg, ibcend;
  double ybcbeg, ybcend;

  /* defining boundary conditions for the natural spline */
  ibcbeg = 2;
  ibcend = 2;
  ybcbeg = 0.0;
  ybcend = 0.0;

  return jb_spline_cubic_init(n, t, y, ibcbeg, ybcbeg, ibcend, ybcend);
}

int jb_spline_copy(jb_spline_t **dst, const jb_spline_t *src)
{
  assert(src != NULL);
  assert(src->t != NULL);
  assert(src->y != NULL);
  assert(src->ypp != NULL);

  if ( *dst != NULL ) {
    jb_spline_free(*dst);
  }
  SUCCEED_OR_RETURN( jb_spline_alloc(dst, src->np) );

  (*dst)->t = (double *) malloc (src->np*sizeof(double));
  FULFILL_OR_EXIT((*dst)->t != NULL, PSPIO_ENOMEM);
  memcpy((*dst)->y, src->y, src->np*sizeof(double));

  (*dst)->y = (double *) malloc (src->np*sizeof(double));
  FULFILL_OR_EXIT((*dst)->y != NULL, PSPIO_ENOMEM);
  memcpy((*dst)->t, src->t, src->np*sizeof(double));

  (*dst)->ypp = (double *) malloc (src->np*sizeof(double));
  FULFILL_OR_EXIT((*dst)->ypp != NULL, PSPIO_ENOMEM);
  memcpy((*dst)->ypp, src->ypp, src->np*sizeof(double));

  return PSPIO_SUCCESS;
}

void jb_spline_free(jb_spline_t *spline)
{
  if (spline != NULL) {
    free(spline->t);
    free(spline->y);
    free(spline->ypp);

    free(spline);
  }
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

double jb_spline_eval(const jb_spline_t *spline, double r)
{
  double ret;

  jb_spline_cubic_val(spline->np, spline->t, spline->y, spline->ypp, r, &ret, NULL, NULL);

  return ret;
}

double jb_spline_eval_deriv(const jb_spline_t *spline, double r)
{
  double ret;

  jb_spline_cubic_val(spline->np, spline->t, spline->y, spline->ypp, r, NULL, &ret, NULL );

  return ret;
}

double jb_spline_eval_deriv2(const jb_spline_t *spline, double r)
{
  double ret;

  jb_spline_cubic_val(spline->np, spline->t, spline->y, spline->ypp, r, NULL, NULL, &ret);

  return ret;
}

void jb_spline_cubic_val(int n, const double *t, const double *y, const double *ypp, 
			 double tval, double *yval, double *ypval, double *yppval)
/******************************************************************************/
/*
  Purpose:

    SPLINE_CUBIC_VAL evaluates a piecewise cubic spline at a point.

  Discussion:

    SPLINE_CUBIC_INIT must have already been called to define the values of YPP.

    For any point T in the interval T(IVAL), T(IVAL+1), the form of
    the spline is

      SPL(T) = A
             + B * ( T - T(IVAL) )
             + C * ( T - T(IVAL) )^2
             + D * ( T - T(IVAL) )^3

    Here:
      A = Y(IVAL)
      B = ( Y(IVAL+1) - Y(IVAL) ) / ( T(IVAL+1) - T(IVAL) )
        - ( YPP(IVAL+1) + 2 * YPP(IVAL) ) * ( T(IVAL+1) - T(IVAL) ) / 6
      C = YPP(IVAL) / 2
      D = ( YPP(IVAL+1) - YPP(IVAL) ) / ( 6 * ( T(IVAL+1) - T(IVAL) ) )

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    25 August 2011

  Author:

    John Burkardt

  Parameters:

    Input, int N, the number of knots.

    Input, double T[N], the knot values.

    Input, double Y[N], the data values at the knots.

    Input, double YPP[N], the second derivatives of the spline at
    the knots.

    Input, double TVAL, a point, typically between T[0] and T[N-1], at
    which the spline is to be evalulated.  If TVAL lies outside
    this range, extrapolation is used.

    Output, double *YVAL, the value of the spline at TVAL. If YVAL is
    NULL, the value of the spline is not computed.

    Output, double *YPVAL, the derivative of the spline at TVAL. If
    YPVAL is NULL, the derivative is not computed.

    Output, double *YPPVAL, the second derivative of the spline at
    TVAL. If YPPVAL is NULL, the second derivative is not computed.
*/
{
  double dt;
  double h;
  int i;
  int ival;
/*
  Determine the interval [ T(I), T(I+1) ] that contains TVAL.
  Values below T[0] or above T[N-1] use extrapolation.
*/
  ival = n - 2;

  for ( i = 0; i < n-1; i++ )
  {
    if ( tval < t[i+1] )
    {
      ival = i;
      break;
    }
  }
/*
  In the interval I, the polynomial is in terms of a normalized
  coordinate between 0 and 1.
*/
  dt = tval - t[ival];
  h = t[ival+1] - t[ival];

  if (yval != NULL) {
    *yval = y[ival]
    + dt * ( ( y[ival+1] - y[ival] ) / h
           - ( ypp[ival+1] / 6.0 + ypp[ival] / 3.0 ) * h
    + dt * ( 0.5 * ypp[ival]
    + dt * ( ( ypp[ival+1] - ypp[ival] ) / ( 6.0 * h ) ) ) );
  }

  if (ypval != NULL) {
    *ypval = ( y[ival+1] - y[ival] ) / h
    - ( ypp[ival+1] / 6.0 + ypp[ival] / 3.0 ) * h
    + dt * ( ypp[ival]
    + dt * ( 0.5 * ( ypp[ival+1] - ypp[ival] ) / h ) );
  }

  if (yppval != NULL) {
    *yppval = ypp[ival] + dt * ( ypp[ival+1] - ypp[ival] ) / h;
  }

}

double *penta(int n, double a1[], double a2[], double a3[], double a4[], 
	      double a5[], double b[])
/******************************************************************************/
/*
  Purpose:

    PENTA solves a pentadiagonal system of linear equations.

  Discussion:

    The matrix A is pentadiagonal.  It is entirely zero, except for
    the main diagaonal, and the two immediate sub- and super-diagonals.

    The entries of Row I are stored as:

      A(I,I-2) -> A1(I)
      A(I,I-1) -> A2(I)
      A(I,I)   -> A3(I)
      A(I,I+1) -> A4(I)
      A(I,I-2) -> A5(I)

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    07 June 2013

  Author:

    John Burkardt

  Reference:

    Cheney, Kincaid,
    Numerical Mathematics and Computing,
    1985, pages 233-236.

  Parameters:

    Input, int N, the order of the matrix.

    Input, double A1[N], A2[N], A3[N], A4[N], A5[N], the nonzero
    elements of the matrix.  Note that the data in A2, A3 and A4
    is overwritten by this routine during the solution process.

    Input, double B[N], the right hand side of the linear system.

    Output, double PENTA[N], the solution of the linear system.
*/
{
  int i;
  double *x;
  double xmult;

  x = ( double * ) malloc ( n * sizeof ( double ) );

  for ( i = 1; i < n - 1; i++ )
  {
    xmult = a2[i] / a3[i-1];
    a3[i] = a3[i] - xmult * a4[i-1];
    a4[i] = a4[i] - xmult * a5[i-1];
    b[i] = b[i] - xmult * b[i-1];
    xmult = a1[i+1] / a3[i-1];
    a2[i+1] = a2[i+1] - xmult * a4[i-1];
    a3[i+1] = a3[i+1] - xmult * a5[i-1];
    b[i+1] = b[i+1] - xmult * b[i-1];
  }

  xmult = a2[n-1] / a3[n-2];
  a3[n-1] = a3[n-1] - xmult * a4[n-2];
  x[n-1] = ( b[n-1] - xmult * b[n-2] ) / a3[n-1];
  x[n-2] = ( b[n-2] - a4[n-2] * x[n-1] ) / a3[n-2];
  for ( i = n - 3; 0 <= i; i-- )
  {
    x[i] = ( b[i] - a4[i] * x[i+1] - a5[i] * x[i+2] ) / a3[i];
  }

  return x;
}
