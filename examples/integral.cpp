#include <iostream>
#include <cymbolic/CymboliC.h>

int main() {
  auto roots = cymbolic::roots("15*x^2-10*x+3", true);
  std::cout << cymbolic::latex(roots[1]);
  return 0;
}
