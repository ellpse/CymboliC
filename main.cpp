#include <iostream>
#include "CymboliC.h"

int main() {
  auto r = cymbolic::roots("3*x^3+2*x^2-3*x");

  for (const auto& root : r)
    std::cout << root << '\n';

  return 0;
}
