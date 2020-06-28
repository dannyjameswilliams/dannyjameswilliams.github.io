#include <cmath>
#include <cstdlib>
#include <iostream>

double rand_one()
{
    return std::rand() / (RAND_MAX + 1.0);
}

int main()
{
	
	// declare variables
	int circle_points = 0;
	int square_points = 0;
	
	int circle_points_loop = 0;
	int square_points_loop = 0;

	// set up parallel OpenMP
	#pragma omp parallel
	{	

		// run for loop in parallel
		#pragma omp for
		for(int ii=0; ii < 200000; ii++)
		{

			// get random x and y coordinates
			double x_coord = (2*rand_one() - 1);
			double y_coord = (2*rand_one() - 1);

			// calculate radius
			double r = std::sqrt(pow(x_coord,2) + pow(y_coord,2));

			// if r is less than or equal to 1 then it is within the circle
			if(r < 1.0)
			{
				++circle_points_loop;
			} else 
			{
				++square_points_loop;
			}
		
		}

		// use critical when counting the final number of counts for each thread
		#pragma omp critical
		{
		    circle_points += circle_points_loop;
		    square_points += square_points_loop;
		}
	}

	// calculate final value of pi using ratios
	double pi = (4.0*circle_points)/(square_points+circle_points);
	
	// print pi
	std::cout << pi << std::endl;
	return 0;
}
