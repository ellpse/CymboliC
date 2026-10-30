#include <iostream>
#include <cymbolic>
#include <string>

int main() {
    std::string input = cymbolic::integrate("x^2-4", true);
    std::string output = cymbolic::latex(input);
    std::cout << output << '\n';
    return 0;
}
