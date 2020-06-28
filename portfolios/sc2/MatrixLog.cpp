#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericMatrix MatrixLog(NumericMatrix A)
{
  int nrow = A.nrow();
  int ncol = A.ncol();
  NumericMatrix out(nrow, ncol);
  std::transform(A.begin(), A.end(), out.begin(), ::log);
  return(out);
}