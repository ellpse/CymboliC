#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "CymboliC.h"

static std::string joinRoots(const std::vector<std::string>& roots) {
  std::ostringstream out;
  for (size_t i = 0; i < roots.size(); ++i) {
    if (i)
      out << "\n";
    out << roots[i];
  }
  return out.str();
}

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "usage: cymbolic_backend <operation> <expression> [arguments...]\n";
    return 1;
  }

  try {
    std::string operation = argv[1];
    std::string expression = argv[2];

    if (operation == "differentiate") {
      bool closed = argc >= 4 ? std::string(argv[3]) == "1" : false;
      std::cout << cymbolic::differentiate(expression, closed) << '\n';
    }
    else if (operation == "integrate") {
      bool closed = argc >= 4 ? std::string(argv[3]) == "1" : false;
      std::cout << cymbolic::integrate(expression, closed) << '\n';
    }
    else if (operation == "evaluate") {
      if (argc < 4) {
        std::cerr << "evaluate requires an x value\n";
        return 1;
      }
      float x = std::stof(argv[3]);
      bool closed = argc >= 5 ? std::string(argv[4]) == "1" : false;
      std::cout << cymbolic::evaluate(expression, x, closed) << '\n';
    }
    else if (operation == "defintegral") {
      if (argc < 5) {
        std::cerr << "defintegral requires lower and upper bounds\n";
        return 1;
      }
      float low = std::stof(argv[3]);
      float high = std::stof(argv[4]);
      bool closed = argc >= 6 ? std::string(argv[5]) == "1" : false;
      std::cout << cymbolic::defintegral(expression, low, high, closed) << '\n';
    }
    else if (operation == "roots") {
      bool closed = argc >= 4 ? std::string(argv[3]) == "1" : false;
      std::cout << joinRoots(cymbolic::roots(expression, closed)) << '\n';
    }
    else {
      std::cerr << "unknown operation\n";
      return 1;
    }
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 2;
  }

  return 0;
}
