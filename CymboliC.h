/*
 syntax!!
 CONSTANTS
 pi
 e

 VARIABLE
 x

 ARITHMETIC
 x+y
 x-y
 x*y
 x/y
 x^y
 (x+y)
 -(x)
 abs(x)

 FUNCTIONS
 sin(x)
 cos(x)
 tan(x)
 cot(x)
 sec(x)
 csc(x)

 INVERSE TRIG FUNCTIONS
 asin(x)
 acos(x)
 atan(x)
 acot(x)
 asec(x)
 acsc(x)

 HYPERBOLIC FUNCTIONS
 sinh(x)
 cosh(x)
 tanh(x)
 coth(x)
 sech(x)
 csch(x)

 OTHER FUNCTIONS
 exp(x)
 ln(x)
 log(x)
 sqrt(x)
 abs(x)

 DIFFERENTIATION
 differentiate("x")
 differentiate("x^2")
 differentiate("2*x^3")
 differentiate("sin(x)")
 differentiate("cos(x)")
 differentiate("tan(x)")
 differentiate("exp(x)")
 differentiate("ln(x)")
 differentiate("sqrt(x)")
 differentiate("sin(2*x)")
 differentiate("x*sin(x)")
 differentiate("x^2+sin(x)")

 INTEGRATION
 integrate("x")
 integrate("x^2")
 integrate("2*x^3")
 integrate("sin(x)")
 integrate("cos(x)")
 integrate("tan(x)")
 integrate("sec(x)")
 integrate("csc(x)")
 integrate("sinh(x)")
 integrate("cosh(x)")
 integrate("tanh(x)")
 integrate("coth(x)")
 integrate("sech(x)")
 integrate("csch(x)")
 integrate("exp(x)")
 integrate("ln(x)")
 integrate("log(x)")
 integrate("sqrt(x)")
 integrate("sin(2*x)")
 integrate("exp(3*x)")
 integrate("sec^2(x)")
 integrate("csc^2(x)")
 integrate("sech^2(x)")
 integrate("csch^2(x)")

 EVALUATION
 evaluate("x", 5)
 evaluate("x^2", 5)
 evaluate("2*x^2+3*x", 5)
 evaluate("sin(x)", 5)
 evaluate("sqrt(x)", 5)

 DEFINITE INTEGRATION
 defintegral("x", 0, 5)
 defintegral("x^2", 0, 3)
 defintegral("2*x^2", 4.5, 3.14)
 defintegral("sin(x)", 0, pi)
 defintegral("exp(x)", 0, 1)

 INTEGRATION RETURNS AN ANTIDERIVATIVE
 integrate("x^2")
 x^3/3+C

 DEFINITE INTEGRATION RETURNS A NUMBER
 defintegral("x^2", 0, 3)
 9

 FUNCTION NAMES
 differentiate
 integrate
 evaluate
 defintegral

 ALL FUNCTION ARGUMENTS ARE STRINGS
 ALL NUMERIC EVALUATION VALUES ARE FLOATS

 IMPLICIT MULTIPLICATION IS NOT SUPPORTED
 USE:
 2*x
 x^2
 3*sin(x)

 INSTEAD OF:
 2x
 3sin(x)
 */
#pragma once

#include <cmath>
#include <complex>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <functional>
#include <algorithm>

namespace cymbolic {

  constexpr double pi = 3.14159265358979323846;
  constexpr double e = 2.71828182845904523536;

  using Complex = std::complex<double>;
  using Roots = std::vector<Complex>;

  inline const std::unordered_map<std::string, std::string> derivativeRules = {
    {"sin", "cos(x)"},
    {"cos", "-sin(x)"},
    {"tan", "sec(x)^2"},
    {"cot", "-csc(x)^2"},
    {"sec", "sec(x)*tan(x)"},
    {"csc", "-csc(x)*cot(x)"},
    {"asin", "1/sqrt(1-x^2)"},
    {"acos", "-1/sqrt(1-x^2)"},
    {"atan", "1/(1+x^2)"},
    {"acot", "-1/(1+x^2)"},
    {"asec", "1/(abs(x)*sqrt(x^2-1))"},
    {"acsc", "-1/(abs(x)*sqrt(x^2-1))"},
    {"sinh", "cosh(x)"},
    {"cosh", "sinh(x)"},
    {"tanh", "sech(x)^2"},
    {"coth", "-csch(x)^2"},
    {"sech", "-sech(x)*tanh(x)"},
    {"csch", "-csch(x)*coth(x)"},
    {"exp", "exp(x)"},
    {"ln", "1/x"},
    {"log", "1/(x*ln(10))"},
    {"sqrt", "1/(2*sqrt(x))"}
  };

  inline const std::unordered_map<std::string, std::string> integralRules = {
    {"sin", "-cos($)"},
    {"cos", "sin($)"},
    {"tan", "-ln(abs(cos($)))"},
    {"cot", "ln(abs(sin($)))"},
    {"sec", "ln(abs(sec($)+tan($)))"},
    {"csc", "-ln(abs(csc($)+cot($)))"},
    {"sinh", "cosh($)"},
    {"cosh", "sinh($)"},
    {"tanh", "ln(cosh($))"},
    {"coth", "ln(abs(sinh($)))"},
    {"sech", "atan(sinh($))"},
    {"csch", "ln(abs(tanh($/2)))"},
    {"exp", "exp($)"},
    {"sqrt", "(2/3)*($)^(3/2)"}
  };

  inline std::string formatNumber(double value) {
    if (std::abs(value) < 1e-12)
      value = 0.0;

    std::ostringstream output;
    output << std::fixed << std::setprecision(10) << value;

    std::string result = output.str();

    while (!result.empty() && result.back() == '0')
      result.pop_back();

    if (!result.empty() && result.back() == '.')
      result.pop_back();

    if (result == "-0")
      result = "0";

    return result;
  }

  inline std::string removeSpaces(const std::string& expression) {
    std::string result;

    for (char c : expression) {
      if (!std::isspace(static_cast<unsigned char>(c)))
        result += c;
    }

    return result;
  }

  inline bool isNumber(const std::string& value) {
    if (value.empty())
      return false;

    try {
      size_t position = 0;
      std::stod(value, &position);
      return position == value.size();
    }
    catch (...) {
      return false;
    }
  }

  inline bool isConstant(const std::string& expression) {
    if (isNumber(expression))
      return true;

    return expression == "pi" || expression == "e";
  }

  inline std::string replaceArgument(
    const std::string& expression,
    const std::string& argument
  ) {
    std::string result = expression;
    size_t position = 0;

    while ((position = result.find("$", position)) != std::string::npos) {
      result.replace(position, 1, argument);
      position += argument.size();
    }

    return result;
  }

  inline size_t findOperator(
    const std::string& expression,
    char target
  ) {
    int depth = 0;

    for (size_t i = 0; i < expression.size(); ++i) {
      char c = expression[i];

      if (c == '(')
        ++depth;
      else if (c == ')')
        --depth;
      else if (c == target && depth == 0)
        return i;
    }

    return std::string::npos;
  }

  inline std::vector<std::string> splitTerms(
    const std::string& expression
  ) {
    std::vector<std::string> terms;

    if (expression.empty())
      return terms;

    size_t start = 0;
    int depth = 0;

    for (size_t i = 0; i < expression.size(); ++i) {
      char c = expression[i];

      if (c == '(') {
        ++depth;
      }
      else if (c == ')') {
        --depth;
      }
      else if (
        depth == 0 &&
        (c == '+' || c == '-') &&
        i != 0 &&
        expression[i - 1] != '^'
      ) {
        terms.push_back(expression.substr(start, i - start));
        start = i;
      }
    }

    terms.push_back(expression.substr(start));

    return terms;
  }

  inline std::string differentiate(const std::string& expression);
  inline std::string integrate(const std::string& expression);

  inline std::string differentiateFunction(
    const std::string& functionName,
    const std::string& argument
  ) {
    auto rule = derivativeRules.find(functionName);

    if (rule == derivativeRules.end())
      return "r not found";

    std::string inside = argument;
    std::string derivative;

    if (functionName == "sqrt") {
      derivative = "1/(2*sqrt(" + inside + "))";
    }
    else {
      derivative = rule->second;

      size_t position = 0;

      while ((position = derivative.find("x", position)) != std::string::npos) {
        derivative.replace(position, 1, inside);
        position += inside.size();
      }
    }

    std::string insideDerivative = differentiate(inside);

    if (insideDerivative == "1")
      return derivative;

    if (isNumber(insideDerivative)) {
      double value = std::stod(insideDerivative);

      if (std::abs(value - 1.0) < 1e-12)
        return derivative;

      return "(" + formatNumber(value) + ")*(" + derivative + ")";
    }

    return "(" + derivative + ")*(" + insideDerivative + ")";
  }

  inline std::string differentiateTerm(const std::string& term) {
    std::string expression = removeSpaces(term);

    if (expression.empty())
      return "0";

    if (expression == "x")
      return "1";

    if (isConstant(expression))
      return "0";

    if (expression.front() == '+')
      return differentiateTerm(expression.substr(1));

    if (expression.front() == '-') {
      return "-(" + differentiateTerm(expression.substr(1)) + ")";
    }

    size_t plusPosition = findOperator(expression, '+');

    if (plusPosition != std::string::npos)
      return differentiate(expression);

    for (size_t i = 1; i < expression.size(); ++i) {
      if (
        expression[i] == '-' &&
        expression[i - 1] != '^' &&
        expression[i - 1] != '('
      ) {
        return differentiate(expression);
      }
    }

    size_t divisionPosition = findOperator(expression, '/');

    if (divisionPosition != std::string::npos) {
      std::string top = expression.substr(0, divisionPosition);
      std::string bottom = expression.substr(divisionPosition + 1);

      std::string topDerivative = differentiate(top);
      std::string bottomDerivative = differentiate(bottom);

      return "((" + topDerivative + ")*(" + bottom + ")-(" +
      top + ")*(" + bottomDerivative + "))/(" +
      bottom + ")^2";
    }

    size_t multiplicationPosition = findOperator(expression, '*');

    if (multiplicationPosition != std::string::npos) {
      std::string left = expression.substr(0, multiplicationPosition);
      std::string right = expression.substr(multiplicationPosition + 1);

      std::string leftDerivative = differentiate(left);
      std::string rightDerivative = differentiate(right);

      if (leftDerivative == "0")
        return "(" + left + ")*(" + rightDerivative + ")";

      if (rightDerivative == "0")
        return "(" + leftDerivative + ")*(" + right + ")";

      return "(" + leftDerivative + ")*(" + right + ")+(" +
      left + ")*(" + rightDerivative + ")";
    }

    size_t powerPosition = findOperator(expression, '^');

    if (powerPosition != std::string::npos) {
      std::string base = expression.substr(0, powerPosition);
      std::string exponent = expression.substr(powerPosition + 1);

      if (isNumber(exponent)) {
        double power = std::stod(exponent);

        if (std::abs(power) < 1e-12)
          return "0";

        std::string baseDerivative = differentiate(base);

        if (baseDerivative == "0")
          return "0";

        return "(" + formatNumber(power) + ")*(" +
        base + ")^(" + formatNumber(power - 1.0) +
        ")*(" + baseDerivative + ")";
      }
    }

    if (
      expression.size() >= 3 &&
      expression.back() == ')'
    ) {
      size_t openPosition = expression.find('(');

      if (openPosition != std::string::npos) {
        std::string functionName =
        expression.substr(0, openPosition);

        if (derivativeRules.find(functionName) != derivativeRules.end()) {
          std::string argument =
          expression.substr(
            openPosition + 1,
            expression.size() - openPosition - 2
          );

          return differentiateFunction(functionName, argument);
        }
      }
    }

    size_t xPosition = expression.find('x');

    if (xPosition != std::string::npos) {
      std::string coefficient = expression.substr(0, xPosition);
      std::string exponent = "1";

      if (
        xPosition + 1 < expression.size() &&
        expression[xPosition + 1] == '^'
      ) {
        exponent = expression.substr(xPosition + 2);
      }

      if (coefficient.empty())
        coefficient = "1";

      if (coefficient == "-")
        coefficient = "-1";

      if (isNumber(coefficient) && isNumber(exponent)) {
        double coefficientValue = std::stod(coefficient);
        double exponentValue = std::stod(exponent);

        if (std::abs(exponentValue) < 1e-12)
          return "0";

        return formatNumber(
          coefficientValue * exponentValue
        ) + "*x^" +
        formatNumber(exponentValue - 1.0);
      }
    }

    return "r not found";
  }

  inline std::string differentiate(const std::string& expression) {
    std::string expressionText = removeSpaces(expression);

    if (expressionText.empty())
      return "0";

    std::vector<std::string> terms =
    splitTerms(expressionText);

    if (terms.size() > 1) {
      std::string result;

      for (const std::string& term : terms) {
        std::string value = differentiateTerm(term);

        if (value == "r not found")
          return value;

        if (value == "0")
          continue;

        if (!value.empty() && value.front() == '-') {
          result += value;
        }
        else if (result.empty()) {
          result = value;
        }
        else {
          result += "+" + value;
        }
      }

      return result.empty() ? "0" : result;
    }

    return differentiateTerm(expressionText);
  }

  inline std::string integrateTerm(const std::string& term) {
    std::string expressionText = removeSpaces(term);

    if (expressionText.empty())
      return "0";

    if (expressionText.front() == '+')
      return integrateTerm(expressionText.substr(1));

    if (expressionText.front() == '-') {
      std::string result =
      integrateTerm(expressionText.substr(1));

      if (result == "r not found")
        return result;

      if (result == "0")
        return "0";

      return "-(" + result + ")";
    }

    if (isConstant(expressionText))
      return expressionText == "0" ? "0" : expressionText + "*x";

    size_t multiplicationPosition =
    findOperator(expressionText, '*');

    if (multiplicationPosition != std::string::npos) {
      std::string left =
      expressionText.substr(0, multiplicationPosition);

      std::string right =
      expressionText.substr(multiplicationPosition + 1);

      if (isNumber(left)) {
        std::string result = integrateTerm(right);

        if (result == "r not found")
          return result;

        return "(" + left + ")*(" + result + ")";
      }

      if (isNumber(right)) {
        std::string result = integrateTerm(left);

        if (result == "r not found")
          return result;

        return "(" + right + ")*(" + result + ")";
      }
    }

    if (
      expressionText.size() > 4 &&
      expressionText.back() == ')' &&
      expressionText.find("^2(") != std::string::npos
    ) {
      size_t squarePosition =
      expressionText.find("^2(");

      if (squarePosition != std::string::npos) {
        std::string functionName =
        expressionText.substr(0, squarePosition);

        std::string functionArgument =
        expressionText.substr(
          squarePosition + 3,
          expressionText.size() - squarePosition - 4
        );

        std::string innerDerivative =
        differentiate(functionArgument);

        if (innerDerivative == "1") {
          if (functionName == "sec")
            return "tan(" + functionArgument + ")";

          if (functionName == "csc")
            return "-cot(" + functionArgument + ")";

          if (functionName == "sech")
            return "tanh(" + functionArgument + ")";

          if (functionName == "csch")
            return "-coth(" + functionArgument + ")";

          if (functionName == "tan")
            return "tan(" + functionArgument + ")-" +
            functionArgument;
        }

        if (isNumber(innerDerivative)) {
          double value =
          std::stod(innerDerivative);

          if (std::abs(value) > 1e-12) {
            if (functionName == "sec")
              return "(" +
              formatNumber(1.0 / value) +
                ")*tan(" + functionArgument + ")";

              if (functionName == "csc")
                return "-" +
                formatNumber(1.0 / value) +
                  "*cot(" + functionArgument + ")";

                if (functionName == "sech")
                  return formatNumber(1.0 / value) +
                  "*tanh(" + functionArgument + ")";

                if (functionName == "csch")
                  return "-" +
                  formatNumber(1.0 / value) +
                    "*coth(" + functionArgument + ")";

                  if (functionName == "tan")
                    return "(" +
                    formatNumber(1.0 / value) +
                      ")*(tan(" + functionArgument + ")-" +
                      functionArgument + ")";
          }
        }
      }
    }

    size_t powerPosition =
    findOperator(expressionText, '^');

    if (powerPosition != std::string::npos) {
      std::string base =
      expressionText.substr(0, powerPosition);

      std::string exponent =
      expressionText.substr(powerPosition + 1);

      if (base == "x" && isNumber(exponent)) {
        double power = std::stod(exponent);

        if (std::abs(power + 1.0) < 1e-12)
          return "ln(abs(x))";

        return "x^" +
        formatNumber(power + 1.0) +
          "/" +
          formatNumber(power + 1.0);
      }

      if (isNumber(base) && isNumber(exponent)) {
        double value =
        std::pow(std::stod(base), std::stod(exponent));

        return formatNumber(value) + "*x";
      }
    }

    if (
      expressionText.size() >= 3 &&
      expressionText.back() == ')'
    ) {
      size_t openPosition =
      expressionText.find('(');

      if (openPosition != std::string::npos) {
        std::string functionName =
        expressionText.substr(0, openPosition);

        std::string argument =
        expressionText.substr(
          openPosition + 1,
          expressionText.size() - openPosition - 2
        );

        if (functionName == "ln") {
          std::string derivative =
          differentiate(argument);

          if (derivative == "1")
            return "x*ln(x)-x";

          if (isNumber(derivative)) {
            double value =
            std::stod(derivative);

            return "(" +
            formatNumber(1.0 / value) +
              ")*(" +
              argument +
              "*ln(" +
              argument +
              ")-(" +
              argument +
              "))";
          }
        }

        if (functionName == "log") {
          std::string derivative =
          differentiate(argument);

          if (derivative == "1")
            return "x*log(x)-x/ln(10)";

          if (isNumber(derivative)) {
            double value =
            std::stod(derivative);

            return "(" +
            formatNumber(1.0 / value) +
              ")*(" +
              argument +
              "*log(" +
              argument +
              ")-(" +
              argument +
              ")/ln(10))";
          }
        }

        if (functionName == "asin") {
          std::string derivative =
          differentiate(argument);

          if (derivative == "1")
            return argument +
            "*asin(" + argument +
            ")+sqrt(1-(" +
            argument + ")^2)";
        }

        if (functionName == "acos") {
          std::string derivative =
          differentiate(argument);

          if (derivative == "1")
            return argument +
            "*acos(" + argument +
            ")-sqrt(1-(" +
            argument + ")^2)";
        }

        if (functionName == "atan") {
          std::string derivative =
          differentiate(argument);

          if (derivative == "1")
            return argument +
            "*atan(" + argument +
            ")-0.5*ln(1+(" +
            argument + ")^2)";
        }

        if (functionName == "acot") {
          std::string derivative =
          differentiate(argument);

          if (derivative == "1")
            return argument +
            "*acot(" + argument +
            ")+0.5*ln(1+(" +
            argument + ")^2)";
        }

        if (functionName == "asec") {
          std::string derivative =
          differentiate(argument);

          if (derivative == "1")
            return argument +
            "*asec(" + argument +
            ")-ln(abs(" +
            argument +
            "+sqrt((" +
            argument +
            ")^2-1)))";
        }

        if (functionName == "acsc") {
          std::string derivative =
          differentiate(argument);

          if (derivative == "1")
            return argument +
            "*acsc(" + argument +
            ")+ln(abs(" +
            argument +
            "+sqrt((" +
            argument +
            ")^2-1)))";
        }

        auto rule =
        integralRules.find(functionName);

        if (rule != integralRules.end()) {
          std::string derivative =
          differentiate(argument);

          std::string result =
          replaceArgument(
            rule->second,
            argument
          );

          if (derivative == "1")
            return result;

          if (isNumber(derivative)) {
            double value =
            std::stod(derivative);

            if (std::abs(value) > 1e-12) {
              return "(" +
              formatNumber(1.0 / value) +
                ")*(" +
                result +
                ")";
            }
          }
        }
      }
    }

    size_t xPosition =
    expressionText.find('x');

    if (
      xPosition != std::string::npos &&
      xPosition == 0
    ) {
      if (expressionText == "x")
        return "x^2/2";

      if (
        xPosition + 1 < expressionText.size() &&
        expressionText[xPosition + 1] == '^'
      ) {
        std::string exponent =
        expressionText.substr(xPosition + 2);

        if (isNumber(exponent)) {
          double power =
          std::stod(exponent);

          if (std::abs(power + 1.0) < 1e-12)
            return "ln(abs(x))";

          return "x^" +
          formatNumber(power + 1.0) +
            "/" +
            formatNumber(power + 1.0);
        }
      }
    }

    if (isNumber(expressionText))
      return expressionText + "*x";

    return "r not found";
  }

  inline std::string integrate(const std::string& expression) {
    std::string expressionText =
    removeSpaces(expression);

    if (expressionText.empty())
      return "C";

    std::vector<std::string> terms =
    splitTerms(expressionText);

    std::string result;

    for (const std::string& term : terms) {
      std::string integrated =
      integrateTerm(term);

      if (integrated == "r not found")
        return integrated;

      if (integrated == "0")
        continue;

      if (result.empty()) {
        result = integrated;
      }
      else if (
        !integrated.empty() &&
        integrated.front() == '-'
      ) {
        result += integrated;
      }
      else {
        result += "+" + integrated;
      }
    }

    if (result.empty())
      return "C";

    return result + "+C";
  }

  inline float evaluate(
    const std::string& input,
    float xvalue
  ) {
    std::string expression =
    removeSpaces(input);

    size_t position = 0;

    std::function<double()> parseExpression;
    std::function<double()> parseTerm;
    std::function<double()> parsePower;
    std::function<double()> parseUnary;
    std::function<double()> parsePrimary;

    parseExpression = [&]() -> double {
      double value = parseTerm();

      while (position < expression.size()) {
        if (expression[position] == '+') {
          ++position;
          value += parseTerm();
        }
        else if (expression[position] == '-') {
          ++position;
          value -= parseTerm();
        }
        else {
          break;
        }
      }

      return value;
    };

    parseTerm = [&]() -> double {
      double value = parsePower();

      while (position < expression.size()) {
        if (expression[position] == '*') {
          ++position;
          value *= parsePower();
        }
        else if (expression[position] == '/') {
          ++position;
          value /= parsePower();
        }
        else {
          break;
        }
      }

      return value;
    };

    parsePower = [&]() -> double {
      double value = parseUnary();

      if (
        position < expression.size() &&
        expression[position] == '^'
      ) {
        ++position;
        double exponent = parsePower();
        value = std::pow(value, exponent);
      }

      return value;
    };

    parseUnary = [&]() -> double {
      if (
        position < expression.size() &&
        expression[position] == '+'
      ) {
        ++position;
        return parseUnary();
      }

      if (
        position < expression.size() &&
        expression[position] == '-'
      ) {
        ++position;
        return -parseUnary();
      }

      return parsePrimary();
    };

    parsePrimary = [&]() -> double {
      if (position >= expression.size())
        throw std::runtime_error("invalid expression");

      if (expression[position] == '(') {
        ++position;

        double value = parseExpression();

        if (
          position >= expression.size() ||
          expression[position] != ')'
        ) {
          throw std::runtime_error("missing )");
        }

        ++position;
        return value;
      }

      if (
        std::isdigit(
          static_cast<unsigned char>(expression[position])
        ) ||
        expression[position] == '.'
      ) {
        size_t start = position;

        while (
          position < expression.size() &&
          (
            std::isdigit(
              static_cast<unsigned char>(
                expression[position]
              )
            ) ||
            expression[position] == '.'
          )
        ) {
          ++position;
        }

        return std::stod(
          expression.substr(
            start,
            position - start
          )
        );
      }

      if (
        std::isalpha(
          static_cast<unsigned char>(
            expression[position]
          )
        )
      ) {
        size_t start = position;

        while (
          position < expression.size() &&
          std::isalpha(
            static_cast<unsigned char>(
              expression[position]
            )
          )
        ) {
          ++position;
        }

        std::string name =
        expression.substr(
          start,
          position - start
        );

        if (name == "x")
          return xvalue;

        if (name == "pi")
          return pi;

        if (name == "e")
          return e;

        if (
          position >= expression.size() ||
          expression[position] != '('
        ) {
          throw std::runtime_error(
            "unknown identifier"
          );
        }

        ++position;

        double argument =
        parseExpression();

        if (
          position >= expression.size() ||
          expression[position] != ')'
        ) {
          throw std::runtime_error(
            "missing )"
          );
        }

        ++position;

        if (name == "sin")
          return std::sin(argument);

        if (name == "cos")
          return std::cos(argument);

        if (name == "tan")
          return std::tan(argument);

        if (name == "cot")
          return 1.0 / std::tan(argument);

        if (name == "sec")
          return 1.0 / std::cos(argument);

        if (name == "csc")
          return 1.0 / std::sin(argument);

        if (name == "asin")
          return std::asin(argument);

        if (name == "acos")
          return std::acos(argument);

        if (name == "atan")
          return std::atan(argument);

        if (name == "acot")
          return pi / 2.0 - std::atan(argument);

        if (name == "asec")
          return std::acos(1.0 / argument);

        if (name == "acsc")
          return std::asin(1.0 / argument);

        if (name == "sinh")
          return std::sinh(argument);

        if (name == "cosh")
          return std::cosh(argument);

        if (name == "tanh")
          return std::tanh(argument);

        if (name == "coth")
          return 1.0 / std::tanh(argument);

        if (name == "sech")
          return 1.0 / std::cosh(argument);

        if (name == "csch")
          return 1.0 / std::sinh(argument);

        if (name == "exp")
          return std::exp(argument);

        if (name == "ln")
          return std::log(argument);

        if (name == "log")
          return std::log10(argument);

        if (name == "sqrt")
          return std::sqrt(argument);

        if (name == "abs")
          return std::abs(argument);

        throw std::runtime_error(
          "unknown function"
        );
      }

      throw std::runtime_error(
        "invalid expression"
      );
    };

    double result = parseExpression();

    if (position != expression.size())
      throw std::runtime_error(
        "invalid expression"
      );

    return static_cast<float>(result);
  }

  inline float defintegral(
    std::string input,
    float lowbound,
    float upbound
  ) {
    std::string expression =
    integrate(input);

    if (expression == "r not found")
      return 0.0f;

    size_t constantpos =
    expression.find("+C");

    if (constantpos != std::string::npos)
      expression =
      expression.substr(0, constantpos);

    float upperresult =
    evaluate(expression, upbound);

    float lowerresult =
    evaluate(expression, lowbound);

    return upperresult - lowerresult;
  }

  struct Polynomial {
    std::vector<Complex> coefficients;

    Polynomial() : coefficients(1, Complex(0.0, 0.0)) {}

    explicit Polynomial(Complex value)
    : coefficients(1, value) {}

    explicit Polynomial(std::vector<Complex> values)
    : coefficients(std::move(values)) {
      trim();
    }

    void trim() {
      while (
        coefficients.size() > 1 &&
        std::abs(coefficients.back()) < 1e-14
      ) {
        coefficients.pop_back();
      }
    }

    int degree() const {
      return static_cast<int>(coefficients.size()) - 1;
    }

    bool isConstant() const {
      return degree() == 0;
    }

    Complex evaluate(Complex x) const {
      Complex result(0.0, 0.0);

      for (auto it = coefficients.rbegin();
           it != coefficients.rend();
      ++it) {
        result = result * x + *it;
      }

      return result;
    }
  };

  inline Polynomial addPolynomial(
    const Polynomial& a,
    const Polynomial& b
  ) {
    size_t size =
    std::max(a.coefficients.size(), b.coefficients.size());

    std::vector<Complex> result(size, Complex(0.0, 0.0));

    for (size_t i = 0; i < a.coefficients.size(); ++i)
      result[i] += a.coefficients[i];

    for (size_t i = 0; i < b.coefficients.size(); ++i)
      result[i] += b.coefficients[i];

    return Polynomial(result);
  }

  inline Polynomial subtractPolynomial(
    const Polynomial& a,
    const Polynomial& b
  ) {
    size_t size =
    std::max(a.coefficients.size(), b.coefficients.size());

    std::vector<Complex> result(size, Complex(0.0, 0.0));

    for (size_t i = 0; i < a.coefficients.size(); ++i)
      result[i] += a.coefficients[i];

    for (size_t i = 0; i < b.coefficients.size(); ++i)
      result[i] -= b.coefficients[i];

    return Polynomial(result);
  }

  inline Polynomial multiplyPolynomial(
    const Polynomial& a,
    const Polynomial& b
  ) {
    std::vector<Complex> result(
      a.degree() + b.degree() + 1,
                                Complex(0.0, 0.0)
    );

    for (size_t i = 0; i < a.coefficients.size(); ++i) {
      for (size_t j = 0; j < b.coefficients.size(); ++j) {
        result[i + j] +=
        a.coefficients[i] * b.coefficients[j];
      }
    }

    return Polynomial(result);
  }

  inline Polynomial powerPolynomial(
    Polynomial base,
    int exponent
  ) {
    Polynomial result(Complex(1.0, 0.0));

    while (exponent > 0) {
      if (exponent & 1)
        result = multiplyPolynomial(result, base);

      base = multiplyPolynomial(base, base);
      exponent >>= 1;
    }

    return result;
  }

  inline bool parsePolynomial(
    const std::string& input,
    Polynomial& result
  ) {
    std::string expression = removeSpaces(input);

    if (expression.empty())
      return false;

    size_t position = 0;

    std::function<bool(Polynomial&)> parseExpression;
    std::function<bool(Polynomial&)> parseTerm;
    std::function<bool(Polynomial&)> parsePower;
    std::function<bool(Polynomial&)> parseUnary;
    std::function<bool(Polynomial&)> parsePrimary;

    parseExpression = [&](Polynomial& output) -> bool {
      if (!parseTerm(output))
        return false;

      while (position < expression.size()) {
        char op = expression[position];

        if (op != '+' && op != '-')
          break;

        ++position;

        Polynomial right;

        if (!parseTerm(right))
          return false;

        if (op == '+')
          output = addPolynomial(output, right);
        else
          output = subtractPolynomial(output, right);
      }

      return true;
    };

    parseTerm = [&](Polynomial& output) -> bool {
      if (!parsePower(output))
        return false;

      while (position < expression.size()) {
        char op = expression[position];

        if (op != '*' && op != '/')
          break;

        ++position;

        Polynomial right;

        if (!parsePower(right))
          return false;

        if (op == '*') {
          output = multiplyPolynomial(output, right);
        }
        else {
          if (!right.isConstant())
            return false;

          Complex divisor =
          right.coefficients[0];

          if (std::abs(divisor) < 1e-14)
            return false;

          for (Complex& value : output.coefficients)
            value /= divisor;
        }
      }

      return true;
    };

    parsePower = [&](Polynomial& output) -> bool {
      if (!parseUnary(output))
        return false;

      if (
        position < expression.size() &&
        expression[position] == '^'
      ) {
        ++position;

        size_t start = position;

        if (
          position < expression.size() &&
          (expression[position] == '+' ||
          expression[position] == '-')
        ) {
          ++position;
        }

        while (
          position < expression.size() &&
          std::isdigit(
            static_cast<unsigned char>(expression[position])
          )
        ) {
          ++position;
        }

        if (start == position)
          return false;

        int exponent = 0;

        try {
          exponent =
          std::stoi(
            expression.substr(
              start,
              position - start
            )
          );
        }
        catch (...) {
          return false;
        }

        if (exponent < 0)
          return false;

        output = powerPolynomial(output, exponent);
      }

      return true;
    };

    parseUnary = [&](Polynomial& output) -> bool {
      if (position < expression.size()) {
        if (expression[position] == '+') {
          ++position;
          return parseUnary(output);
        }

        if (expression[position] == '-') {
          ++position;

          if (!parseUnary(output))
            return false;

          for (Complex& value : output.coefficients)
            value = -value;

          return true;
        }
      }

      return parsePrimary(output);
    };

    parsePrimary = [&](Polynomial& output) -> bool {
      if (position >= expression.size())
        return false;

      if (expression[position] == '(') {
        ++position;

        if (!parseExpression(output))
          return false;

        if (
          position >= expression.size() ||
          expression[position] != ')'
        ) {
          return false;
        }

        ++position;
        return true;
      }

      if (
        expression[position] == 'x' &&
        (
          position + 1 >= expression.size() ||
          !std::isalpha(
            static_cast<unsigned char>(
              expression[position + 1]
            )
          )
        )
      ) {
        ++position;

        output = Polynomial(
          std::vector<Complex>{
            Complex(0.0, 0.0),
                            Complex(1.0, 0.0)
          }
        );

        return true;
      }

      if (
        std::isdigit(
          static_cast<unsigned char>(
            expression[position]
          )
        ) ||
        expression[position] == '.'
      ) {
        size_t start = position;

        while (
          position < expression.size() &&
          (
            std::isdigit(
              static_cast<unsigned char>(
                expression[position]
              )
            ) ||
            expression[position] == '.'
          )
        ) {
          ++position;
        }

        try {
          double value =
          std::stod(
            expression.substr(
              start,
              position - start
            )
          );

          output = Polynomial(
            Complex(value, 0.0)
          );

          return true;
        }
        catch (...) {
          return false;
        }
      }

      if (
        std::isalpha(
          static_cast<unsigned char>(
            expression[position]
          )
        )
      ) {
        size_t start = position;

        while (
          position < expression.size() &&
          std::isalpha(
            static_cast<unsigned char>(
              expression[position]
            )
          )
        ) {
          ++position;
        }

        std::string name =
        expression.substr(
          start,
          position - start
        );

        if (name == "pi") {
          output = Polynomial(
            Complex(pi, 0.0)
          );

          return true;
        }

        if (name == "e") {
          output = Polynomial(
            Complex(e, 0.0)
          );

          return true;
        }

        return false;
      }

      return false;
    };

    if (!parseExpression(result))
      return false;

    return position == expression.size();
  }

  inline Roots polynomialRoots(
    const Polynomial& polynomial
  ) {
    Polynomial p = polynomial;

    p.trim();

    int degree = p.degree();

    if (degree <= 0)
      return {};

    if (degree == 1) {
      Complex a = p.coefficients[1];
      Complex b = p.coefficients[0];

      if (std::abs(a) < 1e-14)
        return {};

      return {
        -b / a
      };
    }

    Complex leading =
    p.coefficients.back();

    for (Complex& value : p.coefficients)
      value /= leading;

    Roots roots(degree);

    const double twoPi =
    2.0 * pi;

    double radius = 1.0;

    for (int i = 0; i < degree; ++i) {
      double magnitude =
      std::abs(p.coefficients[i]);

      radius =
      std::max(
        radius,
        1.0 + magnitude
      );
    }

    for (int i = 0; i < degree; ++i) {
      double angle =
      twoPi * static_cast<double>(i) /
      static_cast<double>(degree);

      roots[i] =
      std::polar(
        radius,
        angle
      );
    }

    for (int iteration = 0;
         iteration < 2000;
    ++iteration) {
      double maximumChange = 0.0;

      for (int i = 0; i < degree; ++i) {
        Complex denominator(1.0, 0.0);

        for (int j = 0; j < degree; ++j) {
          if (i != j)
            denominator *= roots[i] - roots[j];
        }

        if (std::abs(denominator) < 1e-30)
          denominator = Complex(1e-30, 0.0);

        Complex correction =
        p.evaluate(roots[i]) /
        denominator;

        roots[i] -= correction;

        maximumChange =
        std::max(
          maximumChange,
          std::abs(correction)
        );
      }

      if (maximumChange < 1e-12)
        break;
    }

    for (Complex& root : roots) {
      if (std::abs(root.real()) < 1e-10)
        root.real(0.0);

      if (std::abs(root.imag()) < 1e-10)
        root.imag(0.0);
    }

    std::sort(
      roots.begin(),
              roots.end(),
              [](const Complex& a, const Complex& b) {
                if (std::abs(a.real() - b.real()) > 1e-10)
                  return a.real() < b.real();

                return a.imag() < b.imag();
              }
    );

    return roots;
  }

  inline Roots roots(
    const std::string& expression
  ) {
    Polynomial polynomial;

    if (!parsePolynomial(expression, polynomial))
      throw std::runtime_error(
        "bad input"
      );

    return polynomialRoots(polynomial);
  }

}
