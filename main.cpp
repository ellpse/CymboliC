#include <iostream>
#include "CymboliC.h"

int main() {
  std::cout << cymbolic::evaluate("sqrt(x)", 2, true);

  return 0;
}
