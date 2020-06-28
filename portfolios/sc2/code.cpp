#include <Rcpp.h>
using namespace Rcpp;

// #include <cmath>
// #include <algorithm>

// [[Rcpp::export]]
NumericMatrix MatrixLog(NumericMatrix A)
{
  int nrow = A.nrow();
  int ncol = A.ncol();
  NumericMatrix out(nrow, ncol);
  for(int ii = 0; ii < nrow; ii++)
  {
    for(int jj = 0; jj < ncol; jj++)
    {
      out(ii, jj) = log(A(ii, jj));
    }
  }
  return(out);
}
  
// [[Rcpp::export]]
NumericMatrix MatrixLogTransform(NumericMatrix A)
{
  int nrow = A.nrow();
  int ncol = A.ncol();
  NumericMatrix out(nrow, ncol);
  std::transform(A.begin(), A.end(), out.begin(), ::log);
  return(out);
}