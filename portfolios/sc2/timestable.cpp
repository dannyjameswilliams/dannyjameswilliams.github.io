#include <iostream>

void timestable(int a, int b)
{
  for(int i = 1 ; i <= b; i++)
  {
    std::cout << a*i << " ";
  }
  std::cout << std::endl;
}