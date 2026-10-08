#include <iostream>
#include <cymbolic/CymboliC.h>

int main() {
  std::cout << cymbolic::integrate("x^2", 0, 4, true) << '\n';
  return 0;
}
