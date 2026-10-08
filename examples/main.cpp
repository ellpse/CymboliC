#include <iostream>
#include <cymbolic/CymboliC.h>

int main() {
  auto roots = cymbolic::roots("15*x^2-10*x+3", true);
  for (const auto& root : roots) {
    std::cout << root << ", ";
  }
  std::cout << "\n";

  return 0;
}
