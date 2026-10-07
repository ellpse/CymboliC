#include <iostream>
#include <iomanip>
#include "CymboliC.h"

int main() {
  std::cout << std::setprecision(17) << cymbolic::defintegral("sqrt(x)", 0.0, 3.0, true) << '\n';
  return 0;
}
