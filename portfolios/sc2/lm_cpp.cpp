// [[Rcpp::depends(RcppArmadillo)]]
#include <RcppArmadillo.h>
#include <Rcpp.h>
using namespace arma;

// [[Rcpp::export(name="new_mvn")]]
vec dmvnInt(mat & X, const rowvec & mu, mat & L)
{
  unsigned int d = X.n_cols;
  unsigned int m = X.n_rows;
  vec D = L.diag();
  vec out(m);
  vec z(d);
  double acc;
  unsigned int icol, irow, ii;
  for(icol = 0; icol < m; icol++)
  {
    for(irow = 0; irow < d; irow++)
    {
      acc = 0.0;
      for(ii = 0; ii < irow; ii++) acc += z.at(ii) * L.at(irow, ii);
      z.at(irow) = ( X.at(icol, irow) - mu.at(irow) - acc ) / D.at(irow);
    }
    out.at(icol) = sum(square(z));
  }
  out = exp( - 0.5 * out - ( (d / 2.0) * log(2.0 * M_PI) + sum(log(D)) ) );
  return out;
}

// [[Rcpp::export(name="lm_cpp_w")]]
vec lm_cpp_w_I(vec y, mat X)
{
  mat Q, R;
  qr_econ(Q, R, X);
  vec beta = solve(R, (trans(Q) * y));  return beta;
}

// [[Rcpp::export(name="local_lm")]]
vec local_lm_I(vec& y, rowvec x0, rowvec X0, mat& x, mat& X, mat& H)
{
  mat Hstar = chol(H, "lower"); 
  vec w = dmvnInt(x, x0, Hstar);
  vec beta = lm_cpp_w_I(y % sqrt(w), X.each_col() % sqrt(w));
  return X0 * beta;
}

// [[Rcpp::export(name="local_lm_fit")]]
vec local_lm_fit_I(vec& y, mat x0, mat X0, mat& x, mat& X, mat& H)
{
  int n0 = x0.n_rows;
  vec out(n0);
  for(int ii=0; ii < n0; ii++)
  {
    rowvec x00 = x0.row(ii);
    rowvec X00 = X0.row(ii);
    out(ii) = as_scalar(local_lm_I(y, x00, X00, x, X, H));
    if(ii % 50 == 0)
    {
      R_CheckUserInterrupt();
    }
  }
  
  return out;
}


















