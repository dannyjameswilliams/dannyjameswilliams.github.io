#include <iostream>
int main()
{
  for (int i=1; i<=100; i++)
  {
    if (i>=95)
    {
      std::cout << i << " ";
    }
    else if (i<5)
    {
      for(float j=-1.5; j>=-3.5; j--)
      {
        std::cout << i*j << " ";
      }
    }
  }
}