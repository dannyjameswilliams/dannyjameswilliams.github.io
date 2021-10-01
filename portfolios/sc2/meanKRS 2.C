#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>

SEXP meanKRS(SEXP y, SEXP x, SEXP x0, SEXP lambda)
{
  
  int n0;
  double lambda0, *x00;
  double *outy;
  
  SEXP out = PROTECT(allocVector(REALSXP, n0));
  
  outy = REAL(out);
  lambda0 = REAL(lambda)[0];
  x00 = REAL(x0);
  
  n0 = length(x0);
  
  for(int i = 1; i < n0; i++)
  {
    outy[i] = sum(dnorm(x, x00[i], lambda0)*y)/sum(dnorm(x, x00[i], lambda0));
  }
}