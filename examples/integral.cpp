#include <iostream>
#include <cymbolic/CymboliC.h>

int main() {
  std::cout << cymbolic::integrate("x^2") << '\n';
  return 0;
}
