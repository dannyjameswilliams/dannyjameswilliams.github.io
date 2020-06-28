#include <iostream>

#ifdef _OPENMP
    #include <omp.h>
#else
    #define omp_get_num_threads() 0
    #define omp_get_thread_num() 0
#endif

int main(int argc, const char **argv)
{
    std::cout << "Hello I am here safe and sound home in the main thread.\n";

    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        std::cout << "Help I am trapped in thread number " << thread_id
                  << " out of a total " << nthreads 
                  << std::endl;
    }

    std::cout << "Thank god I'm safe back home now.\n";

    return 0;
}
