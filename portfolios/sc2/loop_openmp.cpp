#include <iostream>

#ifdef _OPENMP
    #include <omp.h>
#else
    #define omp_get_thread_num() 0
#endif

int main(int argc, const char **argv)
{
    #pragma omp parallel
    {
        int nloops = 0;
        
        #pragma omp for
        for (int i=0; i<1000; ++i)
        {
            ++nloops;
        }

        int thread_id = omp_get_thread_num();
        #pragma omp critical
        {
        std::cout << "Thread " << thread_id << " performed "
                  << nloops << " iterations of the loop.\n";
        }
    }

    return 0;
}
