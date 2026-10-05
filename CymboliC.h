#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cctype>

inline constexpr double pi = 3.141592653589793238462643383279502884;
inline constexpr double e = 2.718281828459045235360287471352662498;

inline const std::unordered_map<std::string, std::string> diffrules = {
  {"sin", "cos"},
  {"cos", "-sin"},
  {"tan", "sec^2"},
  {"cot", "-csc^2"},
  {"sec", "sec*tan"},
  {"csc", "-csc*cot"},
  {"asin", "1/sqrt(1-x^2)"},
  {"acos", "-1/sqrt(1-x^2)"},
  {"atan", "1/(1+x^2)"},
  {"acot", "-1/(1+x^2)"},
  {"asec", "1/(x^2*sqrt(1-1/x^2))"},
  {"acsc", "-1/(x^2*sqrt(1-1/x^2))"},
  {"exp", "exp"},
  {"ln", "1/x"},
  {"log", "1/(x*ln(10))"},
  {"sqrt", "1/(2*sqrt(x))"},
  {"sinh", "cosh"},
  {"cosh", "sinh"},
  {"tanh", "sech^2"},
  {"coth", "-csch^2"},
  {"sech", "-sech*tanh"},
  {"csch", "-csch*coth"}
};

inline const std::unordered_map<std::string, std::string> intrules = {
  {"sin", "-cos($)"},
  {"cos", "sin($)"},
  {"tan", "-ln(abs(cos($)))"},
  {"cot", "ln(abs(sin($)))"},
  {"sec", "ln(abs(sec($)+tan($)))"},
  {"csc", "-ln(abs(csc($)+cot($)))"},
  {"exp", "exp($)"},
  {"sinh", "cosh($)"},
  {"cosh", "sinh($)"},
  {"tanh", "ln(cosh($))"},
  {"coth", "ln(abs(sinh($)))"},
  {"sech", "atan(sinh($))"},
  {"csch", "ln(abs(tanh(x/2)))"},
  {"ln", "$*ln($)-$"}
};

inline std::string formatdouble(double value) {
  if (std::abs(value) < 1e-12) value = 0.0;
  std::ostringstream stream;
  stream << std::setprecision(15) << value;
  std::string result = stream.str();
  if (result.find('.') != std::string::npos) {
    while (!result.empty() && result.back() == '0') result.pop_back();
    if (!result.empty() && result.back() == '.') result.pop_back();
  }
  return result.empty() || result == "-0" ? "0" : result;
}

inline std::string removedspaces(const std::string& expression) {
  std::string result;
  for (char character : expression) {
    if (!std::isspace(static_cast<unsigned char>(character))) result += character;
  }
  return result;
}

inline bool isnumber(const std::string& value) {
  if (value.empty()) return false;
  size_t position = 0;
  if (value[position] == '+' || value[position] == '-') ++position;
  bool digit = false;
  bool decimal = false;
  while (position < value.size()) {
    char character = value[position];
    if (std::isdigit(static_cast<unsigned char>(character))) {
      digit = true;
    } else if (character == '.' && !decimal) {
      decimal = true;
    } else {
      return false;
    }
    ++position;
  }
  return digit;
}

inline bool isconstant(const std::string& expression) {
  return isnumber(expression) || expression == "pi" || expression == "e";
}

inline std::string replacevariable(const std::string& expression, const std::string& argument) {
  std::string result;
  for (char character : expression) {
    if (character == '$') result += "(" + argument + ")";
    else result += character;
  }
  return result;
}

inline int findtopoperator(const std::string& expression, char target) {
  int level = 0;
  for (int position = static_cast<int>(expression.size()) - 1; position >= 0; --position) {
    char character = expression[position];
    if (character == ')') ++level;
    else if (character == '(') --level;
    else if (level == 0 && character == target) return position;
  }
  return -1;
}

inline std::vector<std::string> splitterms(const std::string& expression) {
  std::vector<std::string> terms;
  std::string current;
  int level = 0;
  for (size_t position = 0; position < expression.size(); ++position) {
    char character = expression[position];
    if (character == '(') ++level;
    else if (character == ')') --level;

    bool separator = (character == '+' || character == '-') &&
    level == 0 && position > 0 && expression[position - 1] != '^';
    if (separator) {
      if (!current.empty()) terms.push_back(current);
      current.clear();
    }
    current += character;
  }
  if (!current.empty()) terms.push_back(current);
  return terms;
}

inline std::string differentiate(const std::string& expression);
inline std::string integrate(const std::string& expression);

inline std::string differentiatefactor(const std::string& expression) {
  std::string cleanexpr = removedspaces(expression);
  if (cleanexpr.empty() || isconstant(cleanexpr)) return "0";
  if (cleanexpr == "x") return "1";

  if (cleanexpr.front() == '(' && cleanexpr.back() == ')') {
    return differentiate(cleanexpr.substr(1, cleanexpr.size() - 2));
  }

  size_t openparen = cleanexpr.find('(');
  if (openparen != std::string::npos && cleanexpr.back() == ')') {
    std::string functionname = cleanexpr.substr(0, openparen);
    std::string argument = cleanexpr.substr(openparen + 1, cleanexpr.size() - openparen - 2);
    auto rule = diffrules.find(functionname);
    if (rule == diffrules.end()) return "rule not found";

    std::string inner = differentiate(argument);
    if (inner == "rule not found") return inner;
    if (inner == "0") return "0";

    if (functionname == "tan") return inner == "1" ? "sec^2(" + argument + ")" : "sec^2(" + argument + ")*" + inner;
    if (functionname == "cot") return inner == "1" ? "-csc^2(" + argument + ")" : "-csc^2(" + argument + ")*" + inner;
    if (functionname == "sec") return inner == "1" ? "sec(" + argument + ")*tan(" + argument + ")" : "sec(" + argument + ")*tan(" + argument + ")*" + inner;
    if (functionname == "csc") return inner == "1" ? "-csc(" + argument + ")*cot(" + argument + ")" : "-csc(" + argument + ")*cot(" + argument + ")*" + inner;
    if (functionname == "sinh") return inner == "1" ? "cosh(" + argument + ")" : "cosh(" + argument + ")*" + inner;
    if (functionname == "cosh") return inner == "1" ? "sinh(" + argument + ")" : "sinh(" + argument + ")*" + inner;
    if (functionname == "tanh") return inner == "1" ? "sech^2(" + argument + ")" : "sech^2(" + argument + ")*" + inner;
    if (functionname == "coth") return inner == "1" ? "-csch^2(" + argument + ")" : "-csch^2(" + argument + ")*" + inner;
    if (functionname == "sech") return "-sech(" + argument + ")*tanh(" + argument + ")*" + inner;
    if (functionname == "csch") return "-csch(" + argument + ")*coth(" + argument + ")*" + inner;
    if (functionname == "exp") return "exp(" + argument + ")*" + inner;
    if (functionname == "ln") return "(" + inner + ")/" + argument;
    if (functionname == "log") return "(" + inner + ")/(" + argument + "*ln(10))";
    if (functionname == "sqrt") return "(" + inner + ")/(2*sqrt(" + argument + "))";
    if (functionname == "asin") return "(" + inner + ")/sqrt(1-(" + argument + ")^2)";
    if (functionname == "acos") return "-(" + inner + ")/sqrt(1-(" + argument + ")^2)";
    if (functionname == "atan") return "(" + inner + ")/(1+(" + argument + ")^2)";
    if (functionname == "acot") return "-(" + inner + ")/(1+(" + argument + ")^2)";
    if (functionname == "asec") return "(" + inner + ")/(abs(" + argument + ")*sqrt((" + argument + ")^2-1))";
    if (functionname == "acsc") return "-(" + inner + ")/(abs(" + argument + ")*sqrt((" + argument + ")^2-1))";
    if (inner == "1") return rule->second + "(" + argument + ")";
    return rule->second + "(" + argument + ")*" + inner;
  }

  size_t xpos = cleanexpr.find('x');
  if (xpos == std::string::npos) return "0";

  size_t caret = cleanexpr.find('^');
  std::string coefficient = cleanexpr.substr(0, xpos);
  if (coefficient.empty() || coefficient == "+") coefficient = "1";
  if (coefficient == "-") coefficient = "-1";
  if (!isnumber(coefficient)) return "rule not found";

  double coefficientvalue = std::stod(coefficient);
  double exponent = 1.0;
  if (caret != std::string::npos) {
    std::string exponenttext = cleanexpr.substr(caret + 1);
    if (!isnumber(exponenttext)) return "rule not found";
    exponent = std::stod(exponenttext);
  }

  double newcoefficient = coefficientvalue * exponent;
  double newexponent = exponent - 1.0;
  if (newcoefficient == 0.0) return "0";

  std::string coefficienttext = formatdouble(newcoefficient);
  if (newexponent == 0.0) return coefficienttext;
  if (coefficienttext == "1") coefficienttext.clear();
  else if (coefficienttext == "-1") coefficienttext = "-";

  if (newexponent == 1.0) return coefficienttext + "x";
  return coefficienttext + "x^" + formatdouble(newexponent);
}

inline std::string differentiatesingleterm(const std::string& expression) {
  std::string cleanexpr = removedspaces(expression);
  if (cleanexpr.empty()) return "0";
  if (cleanexpr.front() == '+') return differentiatesingleterm(cleanexpr.substr(1));
  if (cleanexpr.front() == '-') return "-" + differentiatesingleterm(cleanexpr.substr(1));

  int plus = findtopoperator(cleanexpr, '+');
  if (plus > 0) {
    return differentiate(cleanexpr.substr(0, plus)) + "+" + differentiate(cleanexpr.substr(plus + 1));
  }
  int minus = findtopoperator(cleanexpr, '-');
  if (minus > 0) {
    return differentiate(cleanexpr.substr(0, minus)) + "-" + differentiate(cleanexpr.substr(minus + 1));
  }

  int multiply = findtopoperator(cleanexpr, '*');
  if (multiply > 0) {
    std::string left = cleanexpr.substr(0, multiply);
    std::string right = cleanexpr.substr(multiply + 1);
    std::string leftdiff = differentiate(left);
    std::string rightdiff = differentiate(right);
    if (leftdiff == "0") return "(" + left + ")*" + rightdiff;
    if (rightdiff == "0") return "(" + leftdiff + ")*" + right;
    return "(" + leftdiff + ")*" + right + "+(" + left + ")*(" + rightdiff + ")";
  }

  int divide = findtopoperator(cleanexpr, '/');
  if (divide > 0) {
    std::string numerator = cleanexpr.substr(0, divide);
    std::string denominator = cleanexpr.substr(divide + 1);
    std::string numeratordiff = differentiate(numerator);
    std::string denominatordiff = differentiate(denominator);
    return "((" + numeratordiff + ")*" + denominator + "-(" + numerator + ")*(" + denominatordiff + "))/(" + denominator + ")^2";
  }

  return differentiatefactor(cleanexpr);
}

inline std::string differentiate(const std::string& expression) {
  std::string cleanexpr = removedspaces(expression);
  if (cleanexpr.empty()) return "0";
  if (cleanexpr.front() == '+' || cleanexpr.front() == '-') {
    if (cleanexpr.front() == '-') return "-" + differentiate(cleanexpr.substr(1));
    return differentiate(cleanexpr.substr(1));
  }

  std::vector<std::string> terms = splitterms(cleanexpr);
  if (terms.size() > 1) {
    std::string result;
    for (const std::string& term : terms) {
      std::string derivative = differentiatesingleterm(term);
      if (derivative == "rule not found") return derivative;
      if (derivative == "0") continue;
      if (result.empty()) result = derivative;
      else if (!derivative.empty() && derivative.front() == '-') result += derivative;
      else result += "+" + derivative;
    }
    return result.empty() ? "0" : result;
  }
  return differentiatesingleterm(cleanexpr);
}

inline std::string integratesingleterm(const std::string& expression) {
  std::string cleanexpr = removedspaces(expression);
  if (cleanexpr.empty()) return "0";

  if (cleanexpr.front() == '-') return "-" + integratesingleterm(cleanexpr.substr(1));
  if (cleanexpr.front() == '+') return integratesingleterm(cleanexpr.substr(1));

  int multiply = findtopoperator(cleanexpr, '*');
  if (multiply > 0) {
    std::string left = cleanexpr.substr(0, multiply);
    std::string right = cleanexpr.substr(multiply + 1);
    if (isnumber(left)) {
      std::string result = integratesingleterm(right);
      if (result == "rule not found") return result;
      if (result == "0") return "0";
      return left + "*" + result;
    }
  }

  int divide = findtopoperator(cleanexpr, '/');
  if (divide > 0) return "rule not found";

  size_t openparen = cleanexpr.find('(');
  if (openparen != std::string::npos && cleanexpr.back() == ')') {
    std::string functionname = cleanexpr.substr(0, openparen);
    std::string argument = cleanexpr.substr(openparen + 1, cleanexpr.size() - openparen - 2);
    auto rule = intrules.find(functionname);
    if (rule == intrules.end()) return "rule not found";

    std::string inner = differentiate(argument);
    if (inner == "rule not found" || inner == "0") return "rule not found";
    if (inner == "1") return replacevariable(rule->second, argument);

    if (isnumber(inner)) {
      double value = std::stod(inner);
      if (value == 0.0) return "0";
      return formatdouble(1.0 / value) + "*" + rule->second;
    }
    return "rule not found";
  }

  size_t xpos = cleanexpr.find('x');
  if (xpos == std::string::npos) {
    if (!isnumber(cleanexpr)) return "rule not found";
    double value = std::stod(cleanexpr);
    return formatdouble(value) + "*x";
  }

  size_t caret = cleanexpr.find('^');
  std::string coefficient = cleanexpr.substr(0, xpos);
  if (coefficient.empty() || coefficient == "+") coefficient = "1";
  if (coefficient == "-") coefficient = "-1";
  if (!isnumber(coefficient)) return "rule not found";

  double coefficientvalue = std::stod(coefficient);
  double exponent = 1.0;
  if (caret != std::string::npos) {
    std::string exponenttext = cleanexpr.substr(caret + 1);
    if (!isnumber(exponenttext)) return "rule not found";
    exponent = std::stod(exponenttext);
  }

  if (exponent == -1.0) {
    std::string coefficienttext = formatdouble(coefficientvalue);
    if (coefficienttext == "1") return "ln(abs(x))";
    if (coefficienttext == "-1") return "-ln(abs(x))";
    return coefficienttext + "*ln(abs(x))";
  }

  double newexponent = exponent + 1.0;
  double newcoefficient = coefficientvalue / newexponent;
  std::string coefficienttext = formatdouble(newcoefficient);
  if (newexponent == 0.0) return "rule not found";
  if (newexponent == 1.0) return coefficienttext + "*x";
  if (coefficienttext == "1") coefficienttext.clear();
  else if (coefficienttext == "-1") coefficienttext = "-";
  return coefficienttext + "x^" + formatdouble(newexponent);
}

inline std::string integrate(const std::string& expression) {
  std::string cleanexpr = removedspaces(expression);
  if (cleanexpr.empty()) return "C";

  std::vector<std::string> terms = splitterms(cleanexpr);
  std::string result;
  for (const std::string& term : terms) {
    std::string integral = integratesingleterm(term);
    if (integral == "rule not found") return integral;
    if (integral == "0") continue;
    if (result.empty()) result = integral;
    else if (!integral.empty() && integral.front() == '-') result += integral;
    else result += "+" + integral;
  }
  return result.empty() ? "C" : result + "+C";
}
