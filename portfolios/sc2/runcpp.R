library(Rcpp)
library(microbenchmark)
setwd("~/Documents/SC2/ParallelRcpp")
sourceCpp("code.cpp")
sourceCpp("MatrixLog.cpp")


d = 200
A = matrix(1:(d^2), d, d)
MatrixLog(A)
log(A)

microbenchmark(log(A), MatrixLog(A), MatrixLogTransform(A), unit="relative", times=1000)


sourceCpp("MatrixLog.cpp")
sourceCpp("MatrixLogPar.cpp")
all.equal(log(A), MatrixLog(A), MatrixLogPar(A))
microbenchmark(log(A), MatrixLog(A), MatrixLogPar(A), unit="relative", times=1000)
