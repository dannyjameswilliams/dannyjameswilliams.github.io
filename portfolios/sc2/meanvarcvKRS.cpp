#include <Rcpp.h>
using namespace Rcpp;

IntegerVector stl_order(NumericVector x) {
  NumericVector sorted = clone(x).sort();
  return match(sorted, x);
}

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

// [[Rcpp::export(name = "mean_var_cv_KRS")]]
NumericVector mean_var_cv_KRS_I(const NumericVector y, const NumericVector x, 
                      const int k, const NumericVector lambdas)
{
  int n = y.size();
  NumericVector mse_lambda(lambdas.size());
  NumericVector out(1);
  NumericVector sorted_x(n);
  IntegerVector order_x = stl_order(x);
  NumericVector test;
  NumericVector sorted_y(n);
  for(int kk=0; kk < n; kk++)
  {
    int ind = order_x[kk]-1;
    sorted_y[kk] = y[ind];  
    sorted_x[kk] = x[ind];
  }
  
  IntegerVector idxs = seq_len(k);
  IntegerVector all_idxs = rep_each(idxs, n/k);
  
  for(int jj = 0; jj < lambdas.size(); jj++)
  {
    double lambda = lambdas[jj];
    NumericVector mse(k);
    for(int ii=1; ii <= k; ii++)
    {
      const LogicalVector kvals = all_idxs != ii;
      
      NumericVector y_t = clone(sorted_y);
      NumericVector x_t = clone(sorted_x);
      NumericVector y_cross = y_t[kvals];
      NumericVector x_cross = x_t[kvals];
      NumericVector fit = mean_var_KRS_I(y_cross, x_cross, sorted_x, lambda);
      
      NumericVector error = pow((fit[!kvals] - sorted_y[!kvals]), 2);
      mse[ii-1] = mean(error);

    }
    mse_lambda[jj] = mean(mse);
    
  }
  
  int best_pos = which_min(mse_lambda);
  out[0] = lambdas[best_pos];
  
  return out;
}





