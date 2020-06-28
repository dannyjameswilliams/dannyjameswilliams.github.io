#include <Rcpp.h>
#include <RcppParallel.h>
using namespace Rcpp;
// [[Rcpp::depends(RcppParallel)]]

struct Log : public RcppParallel::Worker
{
  const RcppParallel::RMatrix<double> input;
  RcppParallel::RMatrix<double> output;
  
  Log(const NumericMatrix input, NumericMatrix output) 
    : input(input), output(output) {}
  
  void operator()(std::size_t begin, std::size_t end) {
    std::transform(input.begin() + begin, 
                   input.begin() + end, 
                   output.begin() + begin, 
                   ::log);
  }
};

// [[Rcpp::export]]
NumericMatrix MatrixLogPar(NumericMatrix A) {
  
  int n = A.nrow();
  int d = A.ncol();
  NumericMatrix output(n, d);
  
  Log log_(A, output);
  RcppParallel::parallelFor(0, A.length(), log_);
  
  return output;
}