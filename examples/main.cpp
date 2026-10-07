#include <iostream>
#include "CymboliC.h"

int main() {
  std::cout << cymbolic::integrate("2*sqrt(1-x^2)", -1, 1, true);
  return 0;
}
