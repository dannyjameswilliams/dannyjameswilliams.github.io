#include <Rcpp.h>
using namespace Rcpp;

NumericVector fitKRS(const NumericVector x, const NumericVector x0, 
                     const double lambda, const NumericVector y, const NumericVector lambda_vec)
{
  NumericVector copy_y = clone(y);
  int n = x.size();
  int n0 = x0.size();
  NumericVector out(n0);
  for(int ii=0; ii<n0; ii++)
  {
    NumericVector dval = dnorm(x, x0[ii], lambda*lambda_vec[ii], 1);
    double max_dval = max(dval);
    double num=0, den=0;
    for(int jj=0; jj<n; jj++)
    {
      num = num + exp(dval[jj] - max_dval)*copy_y[jj];
      den = den + exp(dval[jj] - max_dval);
    }
    out[ii] = num/den;
  }
  return out;
}

// [[Rcpp::export(name = "mean_var_KRS")]]
NumericVector mean_var_KRS_I(const NumericVector y, const NumericVector x, 
                             const NumericVector x0, const double lambda)
{
  
  int n = x.size();
  int n0 = x0.size();
  NumericVector res(n);
  NumericVector lambda_1sn(n, 1.0);
  NumericVector lambda_1sn0(n0, 1.0);
  
  NumericVector mu = fitKRS(x, x, lambda, y, lambda_1sn);
  NumericVector resAbs = abs(y - mu);
  NumericVector madHat = fitKRS(x, x0, lambda, resAbs, lambda_1sn0);
    
  NumericVector w = 1 / madHat;
  w = w / mean(w);
  
  NumericVector out = fitKRS(x, x0, lambda, y, w);

  return out;
}










