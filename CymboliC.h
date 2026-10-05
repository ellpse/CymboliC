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

inline const std::unordered_map<std::string, std::string> derivativeRules = {
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

inline const std::unordered_map<std::string, std::string> integralRules = {
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

inline std::string formatNumber(double value) {
  if (std::abs(value) < 1e-12) value = 0.0;

  std::ostringstream numberBuffer;
  numberBuffer << std::setprecision(15) << value;

  std::string resultText = numberBuffer.str();

  if (resultText.find('.') != std::string::npos) {
    while (!resultText.empty() && resultText.back() == '0')
      resultText.pop_back();

    if (!resultText.empty() && resultText.back() == '.')
      resultText.pop_back();
  }

  return resultText.empty() || resultText == "-0" ? "0" : resultText;
}

inline std::string removeSpaces(const std::string& expression) {
  std::string resultText;

  for (char character : expression) {
    if (!std::isspace(static_cast<unsigned char>(character)))
      resultText += character;
  }

  return resultText;
}

inline bool isNumber(const std::string& value) {
  if (value.empty()) return false;

  size_t index = 0;

  if (value[index] == '+' || value[index] == '-')
    ++index;

  bool hasDigit = false;
  bool hasDecimal = false;

  while (index < value.size()) {
    char character = value[index];

    if (std::isdigit(static_cast<unsigned char>(character))) {
      hasDigit = true;
    } else if (character == '.' && !hasDecimal) {
      hasDecimal = true;
    } else {
      return false;
    }

    ++index;
  }

  return hasDigit;
}

inline bool isConstant(const std::string& expression) {
  return isNumber(expression) ||
  expression == "pi" ||
  expression == "e";
}

inline std::string replaceArgument(
  const std::string& expression,
  const std::string& functionArgument) {

  std::string resultText;

  for (char character : expression) {
    if (character == '$')
      resultText += "(" + functionArgument + ")";
    else
      resultText += character;
  }

  return resultText;
  }

  inline int findOperator(
    const std::string& expression,
    char operatorSymbol) {

    int parenthesisLevel = 0;

    for (int index = static_cast<int>(expression.size()) - 1;
         index >= 0;
    --index) {

      char character = expression[index];

      if (character == ')')
        ++parenthesisLevel;
      else if (character == '(')
        --parenthesisLevel;
      else if (parenthesisLevel == 0 && character == operatorSymbol)
        return index;
    }

    return -1;
    }

    inline std::vector<std::string> splitTerms(
      const std::string& expression) {

      std::vector<std::string> terms;
      std::string currentTerm;
      int parenthesisLevel = 0;

      for (size_t index = 0; index < expression.size(); ++index) {
        char character = expression[index];

        if (character == '(')
          ++parenthesisLevel;
        else if (character == ')')
          --parenthesisLevel;

        bool isSeparator =
        (character == '+' || character == '-') &&
        parenthesisLevel == 0 &&
        index > 0 &&
        expression[index - 1] != '^';

        if (isSeparator) {
          if (!currentTerm.empty())
            terms.push_back(currentTerm);

          currentTerm.clear();
        }

        currentTerm += character;
      }

      if (!currentTerm.empty())
        terms.push_back(currentTerm);

      return terms;
      }

      inline std::string differentiate(const std::string& expression);
      inline std::string integrate(const std::string& expression);

      inline std::string differentiateFunction(
        const std::string& expression) {

        std::string expressionText = removeSpaces(expression);

        if (expressionText.empty() || isConstant(expressionText))
          return "0";

        if (expressionText == "x")
          return "1";

        if (expressionText.front() == '(' &&
          expressionText.back() == ')') {

          return differentiate(
            expressionText.substr(
              1,
              expressionText.size() - 2
            )
          );
          }

          size_t openParen = expressionText.find('(');

          if (openParen != std::string::npos &&
            expressionText.back() == ')') {

            std::string functionName =
            expressionText.substr(0, openParen);

          std::string functionArgument =
          expressionText.substr(
            openParen + 1,
            expressionText.size() - openParen - 2
          );

          auto ruleEntry = derivativeRules.find(functionName);

          if (ruleEntry == derivativeRules.end())
            return "r not found";

            std::string innerDerivative =
            differentiate(functionArgument);

          if (innerDerivative == "r not found")
            return innerDerivative;

            if (innerDerivative == "0")
              return "0";

            if (functionName == "tan")
              return innerDerivative == "1"
              ? "sec^2(" + functionArgument + ")"
              : "sec^2(" + functionArgument + ")*" + innerDerivative;

            if (functionName == "cot")
              return innerDerivative == "1"
              ? "-csc^2(" + functionArgument + ")"
              : "-csc^2(" + functionArgument + ")*" + innerDerivative;

            if (functionName == "sec")
              return innerDerivative == "1"
              ? "sec(" + functionArgument + ")*tan(" + functionArgument + ")"
              : "sec(" + functionArgument + ")*tan(" + functionArgument + ")*" + innerDerivative;

            if (functionName == "csc")
              return innerDerivative == "1"
              ? "-csc(" + functionArgument + ")*cot(" + functionArgument + ")"
              : "-csc(" + functionArgument + ")*cot(" + functionArgument + ")*" + innerDerivative;

            if (functionName == "sinh")
              return innerDerivative == "1"
              ? "cosh(" + functionArgument + ")"
              : "cosh(" + functionArgument + ")*" + innerDerivative;

            if (functionName == "cosh")
              return innerDerivative == "1"
              ? "sinh(" + functionArgument + ")"
              : "sinh(" + functionArgument + ")*" + innerDerivative;

            if (functionName == "tanh")
              return innerDerivative == "1"
              ? "sech^2(" + functionArgument + ")"
              : "sech^2(" + functionArgument + ")*" + innerDerivative;

            if (functionName == "coth")
              return innerDerivative == "1"
              ? "-csch^2(" + functionArgument + ")"
              : "-csch^2(" + functionArgument + ")*" + innerDerivative;

            if (functionName == "sech")
              return "-sech(" + functionArgument + ")*tanh(" +
              functionArgument + ")*" + innerDerivative;

            if (functionName == "csch")
              return "-csch(" + functionArgument + ")*coth(" +
              functionArgument + ")*" + innerDerivative;

            if (functionName == "exp")
              return "exp(" + functionArgument + ")*" + innerDerivative;

            if (functionName == "ln")
              return "(" + innerDerivative + ")/" + functionArgument;

            if (functionName == "log")
              return "(" + innerDerivative + ")/(" +
              functionArgument + "*ln(10))";

            if (functionName == "sqrt")
              return "(" + innerDerivative + ")/(2*sqrt(" +
              functionArgument + "))";

            if (functionName == "asin")
              return "(" + innerDerivative + ")/sqrt(1-(" +
              functionArgument + ")^2)";

            if (functionName == "acos")
              return "-(" + innerDerivative + ")/sqrt(1-(" +
              functionArgument + ")^2)";

            if (functionName == "atan")
              return "(" + innerDerivative + ")/(1+(" +
              functionArgument + ")^2)";

            if (functionName == "acot")
              return "-(" + innerDerivative + ")/(1+(" +
              functionArgument + ")^2)";

            if (functionName == "asec")
              return "(" + innerDerivative + ")/(abs(" +
              functionArgument + ")*sqrt((" +
              functionArgument + ")^2-1))";

            if (functionName == "acsc")
              return "-(" + innerDerivative + ")/(abs(" +
              functionArgument + ")*sqrt((" +
              functionArgument + ")^2-1))";

            if (innerDerivative == "1")
              return ruleEntry->second + "(" + functionArgument + ")";

            return ruleEntry->second + "(" +
            functionArgument + ")*" +
            innerDerivative;
            }

            size_t variablePosition = expressionText.find('x');

            if (variablePosition == std::string::npos)
              return "0";

        size_t exponentPosition = expressionText.find('^');

        std::string coefficientText =
        expressionText.substr(0, variablePosition);

        if (coefficientText.empty() ||
          coefficientText == "+")
          coefficientText = "1";

        if (coefficientText == "-")
          coefficientText = "-1";

        if (!isNumber(coefficientText))
          return "r not found";

        double coefficientValue =
        std::stod(coefficientText);

        double powerValue = 1.0;

        if (exponentPosition != std::string::npos) {
          std::string powerText =
          expressionText.substr(exponentPosition + 1);

          if (!isNumber(powerText))
            return "r not found";

          powerValue = std::stod(powerText);
        }

        double newCoefficient =
        coefficientValue * powerValue;

        double newPower =
        powerValue - 1.0;

        if (newCoefficient == 0.0)
          return "0";

        std::string coefficientOutput =
        formatNumber(newCoefficient);

        if (newPower == 0.0)
          return coefficientOutput;

        if (coefficientOutput == "1")
          coefficientOutput.clear();
        else if (coefficientOutput == "-1")
          coefficientOutput = "-";

        if (newPower == 1.0)
          return coefficientOutput + "x";

        return coefficientOutput +
        "x^" +
        formatNumber(newPower);
        }

        inline std::string differentiateTerm(
          const std::string& expression) {

          std::string expressionText =
          removeSpaces(expression);

          if (expressionText.empty())
            return "0";

          if (expressionText.front() == '+')
            return differentiateTerm(
              expressionText.substr(1)
            );

          if (expressionText.front() == '-')
            return "-" + differentiateTerm(
              expressionText.substr(1)
            );

          int plusPosition =
          findOperator(expressionText, '+');

          if (plusPosition > 0) {
            return differentiate(
              expressionText.substr(0, plusPosition)
            ) + "+" +
            differentiate(
              expressionText.substr(plusPosition + 1)
            );
          }

          int minusPosition =
          findOperator(expressionText, '-');

          if (minusPosition > 0) {
            return differentiate(
              expressionText.substr(0, minusPosition)
            ) + "-" +
            differentiate(
              expressionText.substr(minusPosition + 1)
            );
          }

          int multiplyPosition =
          findOperator(expressionText, '*');

          if (multiplyPosition > 0) {
            std::string leftSide =
            expressionText.substr(0, multiplyPosition);

            std::string rightSide =
            expressionText.substr(multiplyPosition + 1);

            std::string leftDerivative =
            differentiate(leftSide);

            std::string rightDerivative =
            differentiate(rightSide);

            if (leftDerivative == "0")
              return "(" + leftSide + ")*" + rightDerivative;

            if (rightDerivative == "0")
              return "(" + leftDerivative + ")*" + rightSide;

            return "(" + leftDerivative + ")*" +
            rightSide + "+(" +
            leftSide + ")*(" +
            rightDerivative + ")";
          }

          int dividePosition =
          findOperator(expressionText, '/');

          if (dividePosition > 0) {
            std::string numerator =
            expressionText.substr(0, dividePosition);

            std::string denominator =
            expressionText.substr(dividePosition + 1);

            std::string numeratorDerivative =
            differentiate(numerator);

            std::string denominatorDerivative =
            differentiate(denominator);

            return "((" + numeratorDerivative + ")*" +
            denominator + "-(" +
            numerator + ")*(" +
            denominatorDerivative + "))/(" +
            denominator + ")^2";
          }

          return differentiateFunction(expressionText);
          }

          inline std::string differentiate(
            const std::string& expression) {

            std::string expressionText =
            removeSpaces(expression);

            if (expressionText.empty())
              return "0";

            if (expressionText.front() == '+' ||
              expressionText.front() == '-') {

              if (expressionText.front() == '-')
                return "-" + differentiate(
                  expressionText.substr(1)
                );

              return differentiate(
                expressionText.substr(1)
              );
              }

              std::vector<std::string> terms =
              splitTerms(expressionText);

              if (terms.size() > 1) {
                std::string resultText;

                for (const std::string& term : terms) {
                  std::string derivative =
                  differentiateTerm(term);

                  if (derivative == "r not found")
                    return derivative;

                  if (derivative == "0")
                    continue;

                  if (resultText.empty())
                    resultText = derivative;
                  else if (!derivative.empty() &&
                    derivative.front() == '-')
                    resultText += derivative;
                  else
                    resultText += "+" + derivative;
                }

                return resultText.empty()
                ? "0"
                : resultText;
              }

              return differentiateTerm(expressionText);
            }

            inline std::string integrateTerm(
              const std::string& expression) {

              std::string expressionText =
              removeSpaces(expression);

              if (expressionText.empty())
                return "0";

              if (expressionText.front() == '-')
                return "-" + integrateTerm(
                  expressionText.substr(1)
                );

              if (expressionText.front() == '+')
                return integrateTerm(
                  expressionText.substr(1)
                );

              int multiplyPosition =
              findOperator(expressionText, '*');

              if (multiplyPosition > 0) {
                std::string leftSide =
                expressionText.substr(0, multiplyPosition);

                std::string rightSide =
                expressionText.substr(multiplyPosition + 1);

                if (isNumber(leftSide)) {
                  std::string resultText =
                  integrateTerm(rightSide);

                  if (resultText == "r not found")
                    return resultText;

                  if (resultText == "0")
                    return "0";

                  return leftSide + "*" + resultText;
                }
              }

              int dividePosition =
              findOperator(expressionText, '/');

              if (dividePosition > 0)
                return "r not found";

              size_t openParen =
              expressionText.find('(');

              if (openParen != std::string::npos &&
                expressionText.back() == ')') {

                std::string functionName =
                expressionText.substr(0, openParen);

              std::string functionArgument =
              expressionText.substr(
                openParen + 1,
                expressionText.size() - openParen - 2
              );

              auto ruleEntry =
              integralRules.find(functionName);

              if (ruleEntry == integralRules.end())
                return "r not found";

                std::string innerDerivative =
                differentiate(functionArgument);

              if (innerDerivative == "r not found" ||
                innerDerivative == "0")
                return "r not found";

              if (innerDerivative == "1")
                return replaceArgument(
                  ruleEntry->second,
                  functionArgument
                );

              if (isNumber(innerDerivative)) {
                double derivativeValue =
                std::stod(innerDerivative);

                if (derivativeValue == 0.0)
                  return "0";

                return formatNumber(1.0 / derivativeValue) +
                "*" +
                ruleEntry->second;
              }

              return "r not found";
                }

                size_t variablePosition =
                expressionText.find('x');

                if (variablePosition == std::string::npos) {
                  if (!isNumber(expressionText))
                    return "r not found";

                  double constantValue =
                  std::stod(expressionText);

                  return formatNumber(constantValue) + "*x";
                }

                size_t exponentPosition =
                expressionText.find('^');

                std::string coefficientText =
                expressionText.substr(0, variablePosition);

                if (coefficientText.empty() ||
                  coefficientText == "+")
                  coefficientText = "1";

                if (coefficientText == "-")
                  coefficientText = "-1";

              if (!isNumber(coefficientText))
                return "r not found";

              double coefficientValue =
              std::stod(coefficientText);

              double powerValue = 1.0;

              if (exponentPosition != std::string::npos) {
                std::string powerText =
                expressionText.substr(exponentPosition + 1);

                if (!isNumber(powerText))
                  return "r not found";

                powerValue = std::stod(powerText);
              }

              if (powerValue == -1.0) {
                std::string coefficientOutput =
                formatNumber(coefficientValue);

                if (coefficientOutput == "1")
                  return "ln(abs(x))";

                if (coefficientOutput == "-1")
                  return "-ln(abs(x))";

                return coefficientOutput + "*ln(abs(x))";
              }

              double newPower =
              powerValue + 1.0;

              if (newPower == 0.0)
                return "r not found";

              double newCoefficient =
              coefficientValue / newPower;

              std::string coefficientOutput =
              formatNumber(newCoefficient);

              if (newPower == 1.0)
                return coefficientOutput + "*x";

              if (coefficientOutput == "1")
                coefficientOutput.clear();
              else if (coefficientOutput == "-1")
                coefficientOutput = "-";

              return coefficientOutput +
              "x^" +
              formatNumber(newPower);
              }

              inline std::string integrate(
                const std::string& expression) {

                std::string expressionText =
                removeSpaces(expression);

                if (expressionText.empty())
                  return "C";

                std::vector<std::string> terms =
                splitTerms(expressionText);

                std::string resultText;

                for (const std::string& term : terms) {
                  std::string integratedTerm =
                  integrateTerm(term);

                  if (integratedTerm == "r not found")
                    return integratedTerm;

                  if (integratedTerm == "0")
                    continue;

                  if (resultText.empty())
                    resultText = integratedTerm;
                  else if (!integratedTerm.empty() &&
                    integratedTerm.front() == '-')
                    resultText += integratedTerm;
                  else
                    resultText += "+" + integratedTerm;
                }

                return resultText.empty()
                ? "C"
                : resultText + "+C";
                }
