#include <Rcpp.h>
#include <iostream>
using namespace Rcpp;

IntegerVector stl_order(NumericVector x) {
  NumericVector sorted = clone(x).sort();
  return match(sorted, x);
}

NumericVector stl_sort(NumericVector x) {
  NumericVector y = clone(x);
  std::sort(y.begin(), y.end());
  return y;
}

NumericVector sort_order(NumericVector x, NumericVector y) {
  IntegerVector idx = seq_along(x) - 1;
  std::sort(idx.begin(), idx.end(), [&](int i, int j){return y[i] < y[j];});
  return x[idx];
}

// [[Rcpp::export(name = "meanKRS")]]
NumericVector meanKRS_I(const NumericVector y, const NumericVector x, 
                        const NumericVector x0, const double lambda)
{
  int n, n0;
  n0 = x0.size();
  n = x.size();
  NumericVector out(n0);

  for(int i=0; i<n0; i++)
  {
    double num = 0, den = 0;
    NumericVector dval = dnorm(x, x0[i], lambda, 1);
    double max_dval = max(dval);
    
    for(int j=0; j<n; j++)
    {
      num = num + exp(dval[j]-max_dval)*y[j];
      den = den + exp(dval[j]-max_dval);
    }
    out[i] = num/den;
  } 

  return out;
 
}


// [[Rcpp::export(name = "cvKRS")]]
NumericVector cvKRS_I(const NumericVector y, const NumericVector x, 
                      const int k, const NumericVector lambdas)
{
  int n = y.size();
  NumericVector mse_lambda(lambdas.size());
  NumericVector out(1);
  NumericVector sorted_x(n);
  IntegerVector order_x = stl_order(x);

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
      NumericVector fit = meanKRS_I(y_cross, x_cross, sorted_x, lambda);
      
      NumericVector error = pow((fit[!kvals] - sorted_y[!kvals]), 2);
      mse[ii-1] = mean(error);
    }
    mse_lambda[jj] = mean(mse);
  }
  
  int best_pos = which_min(mse_lambda);
  out[0] = lambdas[best_pos];
  
  return out;
}





