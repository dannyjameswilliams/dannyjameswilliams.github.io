#include <Rcpp.h>
#include <RcppParallel.h>
using namespace Rcpp;
// [[Rcpp::depends(RcppParallel)]]

// [[Rcpp::export]]
NumericMatrix MatrixLogPar(NumericMatrix A) {
  
  int n = A.nrow();
  int d = A.ncol();
  NumericMatrix output(n, d);
  
  Log log_(A, output);
  parallelFor(0, A.length(), log_);
  
  return output;
}