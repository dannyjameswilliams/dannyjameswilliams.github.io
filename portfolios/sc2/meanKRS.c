#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>

SEXP meanKRS(SEXP y, SEXP x, SEXP x0, SEXP lambda)
{
  
  int n, n0;
  double lambda0;
  double *outy, *x00, *xy, *y0;
  
  n0 = length(x0);
  
  x = PROTECT(coerceVector(x, REALSXP));
  y = PROTECT(coerceVector(y, REALSXP));
  SEXP out = PROTECT(allocVector(REALSXP, n0));
  
  n = length(x);
  
  outy = REAL(out);
  lambda0 = REAL(lambda)[0];
  x00 = REAL(x0);
  xy = REAL(x);
  y0 = REAL(y);
  
  for(int i=0; i<n0; i++)
  {
    double num = 0, den = 0;
    for(int j=0; j<n; j++)
    {
      num += dnorm(xy[j], x00[i], lambda0, 0)*y0[j];
      den += dnorm(xy[j], x00[i], lambda0, 0);
    }
    outy[i] = num/den;
  }
  
  UNPROTECT(3);
  
  return out;
}
