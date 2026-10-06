/*
 * syntax!!
 *
 * CONSTANTS
 * pi
 * e
 *
 * VARIABLE
 * x
 *
 * ARITHMETIC
 * x+y
 * x-y
 * x*y
 * x/y
 * x^y
 * (x+y)
 * -(x)
 * abs(x)
 *
 * FUNCTIONS
 * sin(x)
 * cos(x)
 * tan(x)
 * cot(x)
 * sec(x)
 * csc(x)
 *
 * INVERSE TRIG FUNCTIONS
 * asin(x)
 * acos(x)
 * atan(x)
 * acot(x)
 * asec(x)
 * acsc(x)
 *
 * HYPERBOLIC FUNCTIONS
 * sinh(x)
 * cosh(x)
 * tanh(x)
 * coth(x)
 * sech(x)
 * csch(x)
 *
 * OTHER FUNCTIONS
 * exp(x)
 * ln(x)
 * log(x)
 * sqrt(x)
 * abs(x)
 *
 * DIFFERENTIATION
 * differentiate("x")
 * differentiate("x^2")
 * differentiate("2*x^3")
 * differentiate("sin(x)")
 * differentiate("cos(x)")
 * differentiate("tan(x)")
 * differentiate("exp(x)")
 * differentiate("ln(x)")
 * differentiate("sqrt(x)")
 * differentiate("sin(2*x)")
 * differentiate("x*sin(x)")
 * differentiate("x^2+sin(x)")
 * differentiate("x^2", true)
 *
 * INTEGRATION
 * integrate("x")
 * integrate("x^2")
 * integrate("2*x^3")
 * integrate("sin(x)")
 * integrate("cos(x)")
 * integrate("tan(x)")
 * integrate("sec(x)")
 * integrate("csc(x)")
 * integrate("sinh(x)")
 * integrate("cosh(x)")
 * integrate("tanh(x)")
 * integrate("coth(x)")
 * integrate("sech(x)")
 * integrate("csch(x)")
 * integrate("exp(x)")
 * integrate("ln(x)")
 * integrate("log(x)")
 * integrate("sqrt(x)")
 * integrate("sin(2*x)")
 * integrate("exp(3*x)")
 * integrate("sec^2(x)")
 * integrate("csc^2(x)")
 * integrate("sech^2(x)")
 * integrate("csch^2(x)")
 * integrate("x^2", true)
 *
 * EVALUATION
 * evaluate("x", 5)
 * evaluate("x", 5, true)
 * evaluate("x^2", 5)
 * evaluate("x^2", 5, true)
 * evaluate("2*x^2+3*x", 5)
 * evaluate("sin(x)", 5)
 * evaluate("sqrt(x)", 5)
 *
 * DEFINITE INTEGRATION
 * integrate("x", 0, 5)
 * integrate("x", 0, 5, true)
 * integrate("x^2", 0, 3)
 * integrate("x^2", 0, 3, true)
 * integrate("2*x^2", 4.5, 3.14)
 * integrate("sin(x)", 0, pi)
 * integrate("exp(x)", 0, 1)
 * defintegral("x", 0, 5)
 * defintegral("x", 0, 5, true)
 *
 * ROOTS
 * roots("x^2-4")
 * roots("x^2-4", true)
 *
 * INTEGRATION RETURNS AN ANTIDERIVATIVE
 * integrate("x^2")
 * x^3/3+C
 *
 * DEFINITE INTEGRATION RETURNS A NUMBER
 * integrate("x^2", 0, 3)
 * 9
 *
 * FUNCTION NAMES
 * differentiate
 * integrate
 * evaluate
 * defintegral
 * roots
 *
 * CLOSED FORM IS FALSE BY DEFAULT
 *
 * ALL FUNCTION ARGUMENTS ARE STRINGS
 * ALL NUMERIC EVALUATION VALUES ARE FLOATS
 *
 * IMPLICIT MULTIPLICATION IS NOT SUPPORTED
 * USE:
 * 2*x
 * x^2
 * 3*sin(x)
 *
 * INSTEAD OF:
 * 2x
 * 3sin(x)
 */

#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <limits>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

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

  inline std::string formatNumber(double v) {
    if (std::abs(v) < 1e-12)
      v = 0;

    std::ostringstream o;
    o << std::fixed << std::setprecision(10) << v;

    std::string s = o.str();

    while (!s.empty() && s.back() == '0')
      s.pop_back();

    if (!s.empty() && s.back() == '.')
      s.pop_back();

    if (s == "-0")
      s = "0";

    return s;
  }

  inline std::string removeSpaces(const std::string& s) {
    std::string r;

    for (char c : s)
      if (!std::isspace((unsigned char)c))
        r += c;

    return r;
  }

  inline bool isNumber(const std::string& s) {
    if (s.empty())
      return false;

    try {
      size_t p = 0;
      std::stod(s, &p);
      return p == s.size();
    } catch (...) {
      return false;
    }
  }

  inline bool isConstant(const std::string& s) {
    return isNumber(s) || s == "pi" || s == "e";
  }

  inline std::string replaceArgument(
    std::string s,
    const std::string& a
  ) {
    size_t p = 0;

    while ((p = s.find("$", p)) != std::string::npos) {
      s.replace(p, 1, a);
      p += a.size();
    }

    return s;
  }

  inline size_t findOperator(
    const std::string& s,
    char target
  ) {
    int d = 0;

    for (size_t i = 0; i < s.size(); ++i) {
      if (s[i] == '(')
        ++d;
      else if (s[i] == ')')
        --d;
      else if (s[i] == target && d == 0)
        return i;
    }

    return std::string::npos;
  }

  inline std::vector<std::string> splitTerms(
    const std::string& s
  ) {
    std::vector<std::string> r;

    if (s.empty())
      return r;

    size_t start = 0;
    int d = 0;

    for (size_t i = 0; i < s.size(); ++i) {
      if (s[i] == '(')
        ++d;
      else if (s[i] == ')')
        --d;
      else if (
        d == 0 &&
        (s[i] == '+' || s[i] == '-') &&
        i != 0 &&
        s[i - 1] != '^'
      ) {
        r.push_back(s.substr(start, i - start));
        start = i;
      }
    }

    r.push_back(s.substr(start));
    return r;
  }

  inline std::string differentiateRaw(const std::string&);
  inline std::string integrateRaw(const std::string&);
  inline float evaluateRaw(const std::string&, float);
  inline float defintegralRaw(
    const std::string&,
    float,
    float
  );

  inline std::string differentiateFunction(
    const std::string& fn,
    const std::string& arg
  ) {
    auto it = derivativeRules.find(fn);

    if (it == derivativeRules.end())
      return "r not found";

    std::string d = it->second;
    size_t p = 0;

    while ((p = d.find("x", p)) != std::string::npos) {
      d.replace(p, 1, arg);
      p += arg.size();
    }

    std::string id = differentiateRaw(arg);

    if (id == "1")
      return d;

    if (isNumber(id)) {
      double v = std::stod(id);

      if (std::abs(v - 1) < 1e-12)
        return d;

      return "(" + formatNumber(v) + ")*(" + d + ")";
    }

    return "(" + d + ")*(" + id + ")";
  }

  inline std::string differentiateTerm(
    const std::string& t
  ) {
    std::string s = removeSpaces(t);

    if (s.empty())
      return "0";

    if (s == "x")
      return "1";

    if (isConstant(s))
      return "0";

    if (s[0] == '+')
      return differentiateTerm(s.substr(1));

    if (s[0] == '-')
      return "-(" + differentiateTerm(s.substr(1)) + ")";

    if (findOperator(s, '+') != std::string::npos)
      return differentiateRaw(s);

    for (size_t i = 1; i < s.size(); ++i) {
      if (
        s[i] == '-' &&
        s[i - 1] != '^' &&
        s[i - 1] != '('
      )
        return differentiateRaw(s);
    }

    size_t p = findOperator(s, '/');

    if (p != std::string::npos) {
      std::string a = s.substr(0, p);
      std::string b = s.substr(p + 1);

      return "((" +
      differentiateRaw(a) +
      ")*(" +
      b +
      ")-(" +
      a +
      ")*(" +
      differentiateRaw(b) +
      "))/(" +
      b +
      ")^2";
    }

    p = findOperator(s, '*');

    if (p != std::string::npos) {
      std::string a = s.substr(0, p);
      std::string b = s.substr(p + 1);

      std::string da = differentiateRaw(a);
      std::string db = differentiateRaw(b);

      if (da == "0")
        return "(" + a + ")*(" + db + ")";

      if (db == "0")
        return "(" + da + ")*(" + b + ")";

      return "(" +
      da +
      ")*(" +
      b +
      ")+(" +
      a +
      ")*(" +
      db +
      ")";
    }

    p = findOperator(s, '^');

    if (p != std::string::npos) {
      std::string b = s.substr(0, p);
      std::string q = s.substr(p + 1);

      if (isNumber(q)) {
        double n = std::stod(q);

        if (std::abs(n) < 1e-12)
          return "0";

        std::string db = differentiateRaw(b);

        if (db == "0")
          return "0";

        if (db == "1") {
          return "(" +
          formatNumber(n) +
            ")*(" +
            b +
            ")^(" +
            formatNumber(n - 1) +
              ")";
        }

        return "(" +
        formatNumber(n) +
          ")*(" +
          b +
          ")^(" +
          formatNumber(n - 1) +
            ")*(" +
            db +
            ")";
      }
    }

    if (s.back() == ')') {
      size_t p0 = s.find('(');

      if (p0 != std::string::npos) {
        std::string fn = s.substr(0, p0);

        if (derivativeRules.count(fn)) {
          return differentiateFunction(
            fn,
            s.substr(
              p0 + 1,
              s.size() - p0 - 2
            )
          );
        }
      }
    }

    size_t xp = s.find('x');

    if (xp != std::string::npos) {
      std::string c = s.substr(0, xp);
      std::string q = "1";

      if (
        xp + 1 < s.size() &&
        s[xp + 1] == '^'
      )
        q = s.substr(xp + 2);

        if (c.empty())
          c = "1";

      if (c == "-")
        c = "-1";

      if (isNumber(c) && isNumber(q)) {
        double cv = std::stod(c);
        double n = std::stod(q);

        if (std::abs(n) < 1e-12)
          return "0";

        return formatNumber(cv * n) +
        "*x^" +
        formatNumber(n - 1);
      }
    }

    return "r not found";
  }

  inline std::string differentiateRaw(
    const std::string& expression
  ) {
    std::string s = removeSpaces(expression);

    if (s.empty())
      return "0";

    auto terms = splitTerms(s);

    if (terms.size() == 1)
      return differentiateTerm(s);

    std::string r;

    for (const auto& t : terms) {
      std::string v = differentiateTerm(t);

      if (v == "r not found")
        return v;

      if (v == "0")
        continue;

      if (r.empty())
        r = v;
      else if (v[0] == '-')
        r += v;
      else
        r += "+" + v;
    }

    return r.empty() ? "0" : r;
  }

  inline std::string integrateTerm(
    const std::string& term
  ) {
    std::string s = removeSpaces(term);

    if (s.empty())
      return "0";

    if (s[0] == '+')
      return integrateTerm(s.substr(1));

    if (s[0] == '-') {
      std::string r = integrateTerm(s.substr(1));

      if (r == "r not found")
        return r;

      if (r == "0")
        return "0";

      return "-(" + r + ")";
    }

    if (isConstant(s))
      return s == "0" ? "0" : s + "*x";

    size_t p = findOperator(s, '*');

    if (p != std::string::npos) {
      std::string a = s.substr(0, p);
      std::string b = s.substr(p + 1);

      if (isNumber(a)) {
        std::string r = integrateTerm(b);

        if (r == "r not found")
          return r;

        return "(" + a + ")*(" + r + ")";
      }

      if (isNumber(b)) {
        std::string r = integrateTerm(a);

        if (r == "r not found")
          return r;

        return "(" + b + ")*(" + r + ")";
      }
    }

    if (s.back() == ')') {
      size_t p0 = s.find('(');

      if (p0 != std::string::npos) {
        std::string fn = s.substr(0, p0);
        std::string arg = s.substr(
          p0 + 1,
          s.size() - p0 - 2
        );

        std::string da = differentiateRaw(arg);

        if (fn == "ln") {
          if (da == "1")
            return "x*ln(x)-x";

          if (isNumber(da)) {
            return "(" +
            formatNumber(1 / std::stod(da)) +
              ")*(" +
              arg +
              "*ln(" +
              arg +
              ")-(" +
              arg +
              "))";
          }
        }

        if (fn == "log") {
          if (da == "1")
            return "x*log(x)-x/ln(10)";

          if (isNumber(da)) {
            return "(" +
            formatNumber(1 / std::stod(da)) +
              ")*(" +
              arg +
              "*log(" +
              arg +
              ")-(" +
              arg +
              ")/ln(10))";
          }
        }

        if (fn == "asin" && da == "1")
          return arg + "*asin(" + arg +
          ")+sqrt(1-(" + arg + ")^2)";

        if (fn == "acos" && da == "1")
          return arg + "*acos(" + arg +
          ")-sqrt(1-(" + arg + ")^2)";

        if (fn == "atan" && da == "1")
          return arg + "*atan(" + arg +
          ")-0.5*ln(1+(" + arg + ")^2)";

        if (fn == "acot" && da == "1")
          return arg + "*acot(" + arg +
          ")+0.5*ln(1+(" + arg + ")^2)";

        if (fn == "asec" && da == "1")
          return arg + "*asec(" + arg +
          ")-ln(abs(" + arg +
          "+sqrt((" + arg + ")^2-1)))";

        if (fn == "acsc" && da == "1")
          return arg + "*acsc(" + arg +
          ")+ln(abs(" + arg +
          "+sqrt((" + arg + ")^2-1)))";

        auto it = integralRules.find(fn);

        if (it != integralRules.end()) {
          std::string r =
          replaceArgument(it->second, arg);

          if (da == "1")
            return r;

          if (
            isNumber(da) &&
            std::abs(std::stod(da)) > 1e-12
          ) {
            return "(" +
            formatNumber(
              1 / std::stod(da)
            ) +
            ")*(" +
            r +
            ")";
          }
        }
      }
    }

    p = findOperator(s, '^');

    if (p != std::string::npos) {
      std::string b = s.substr(0, p);
      std::string q = s.substr(p + 1);

      if (b == "x" && isNumber(q)) {
        double n = std::stod(q);

        if (std::abs(n + 1) < 1e-12)
          return "ln(abs(x))";

        return "x^" +
        formatNumber(n + 1) +
          "/" +
          formatNumber(n + 1);
      }

      if (isNumber(b) && isNumber(q)) {
        return formatNumber(
          std::pow(
            std::stod(b),
                   std::stod(q)
          )
        ) + "*x";
      }
    }

    if (s == "x")
      return "x^2/2";

    if (
      s.size() > 2 &&
      s[0] == 'x' &&
      s[1] == '^' &&
      isNumber(s.substr(2))
    ) {
      double n = std::stod(s.substr(2));

      if (std::abs(n + 1) < 1e-12)
        return "ln(abs(x))";

      return "x^" +
      formatNumber(n + 1) +
        "/" +
        formatNumber(n + 1);
    }

    if (isNumber(s))
      return s + "*x";

    return "r not found";
  }

  inline std::string integrateRaw(
    const std::string& expression
  ) {
    std::string s = removeSpaces(expression);

    if (s.empty())
      return "C";

    auto terms = splitTerms(s);
    std::string r;

    for (const auto& t : terms) {
      std::string v = integrateTerm(t);

      if (v == "r not found")
        return v;

      if (v == "0")
        continue;

      if (r.empty())
        r = v;
      else if (v[0] == '-')
        r += v;
      else
        r += "+" + v;
    }

    return r.empty() ? "C" : r + "+C";
  }

  inline float evaluateRaw(
    const std::string& input,
    float xvalue
  ) {
    std::string s = removeSpaces(input);
    size_t p = 0;

    std::function<double()> expr;
    std::function<double()> term;
    std::function<double()> power;
    std::function<double()> unary;
    std::function<double()> primary;

    expr = [&] {
      double v = term();

      while (
        p < s.size() &&
        (s[p] == '+' || s[p] == '-')
      ) {
        char o = s[p++];
        double q = term();

        v = o == '+' ? v + q : v - q;
      }

      return v;
    };

    term = [&] {
      double v = power();

      while (
        p < s.size() &&
        (s[p] == '*' || s[p] == '/')
      ) {
        char o = s[p++];
        double q = power();

        v = o == '*' ? v * q : v / q;
      }

      return v;
    };

    power = [&] {
      double v = unary();

      if (
        p < s.size() &&
        s[p] == '^'
      ) {
        ++p;
        v = std::pow(v, power());
      }

      return v;
    };

    unary = [&] {
      if (
        p < s.size() &&
        s[p] == '+'
      ) {
        ++p;
        return unary();
      }

      if (
        p < s.size() &&
        s[p] == '-'
      ) {
        ++p;
        return -unary();
      }

      return primary();
    };

    primary = [&]() -> double {
      if (p >= s.size())
        throw std::runtime_error(
          "invalid expression"
        );

      if (s[p] == '(') {
        ++p;
        double v = expr();

        if (
          p >= s.size() ||
          s[p] != ')'
        )
          throw std::runtime_error(
            "missing )"
          );

          ++p;
          return v;
      }

      if (
        std::isdigit((unsigned char)s[p]) ||
        s[p] == '.'
      ) {
        size_t b = p;

        while (
          p < s.size() &&
          (
            std::isdigit(
              (unsigned char)s[p]
            ) ||
            s[p] == '.'
          )
        )
          ++p;

          return std::stod(
            s.substr(b, p - b)
          );
      }

      if (std::isalpha((unsigned char)s[p])) {
        size_t b = p;

        while (
          p < s.size() &&
          std::isalpha(
            (unsigned char)s[p]
          )
        )
          ++p;

          std::string n =
          s.substr(b, p - b);

          if (n == "x")
            return xvalue;

        if (n == "pi")
          return pi;

        if (n == "e")
          return e;

        if (
          p >= s.size() ||
          s[p] != '('
        )
          throw std::runtime_error(
            "unknown identifier"
          );

          ++p;
          double a = expr();

          if (
            p >= s.size() ||
            s[p] != ')'
          )
            throw std::runtime_error(
              "missing )"
            );

            ++p;

            if (n == "sin")
              return std::sin(a);

        if (n == "cos")
          return std::cos(a);

        if (n == "tan")
          return std::tan(a);

        if (n == "cot")
          return 1 / std::tan(a);

        if (n == "sec")
          return 1 / std::cos(a);

        if (n == "csc")
          return 1 / std::sin(a);

        if (n == "asin")
          return std::asin(a);

        if (n == "acos")
          return std::acos(a);

        if (n == "atan")
          return std::atan(a);

        if (n == "acot")
          return pi / 2 - std::atan(a);

        if (n == "asec")
          return std::acos(1 / a);

        if (n == "acsc")
          return std::asin(1 / a);

        if (n == "sinh")
          return std::sinh(a);

        if (n == "cosh")
          return std::cosh(a);

        if (n == "tanh")
          return std::tanh(a);

        if (n == "coth")
          return 1 / std::tanh(a);

        if (n == "sech")
          return 1 / std::cosh(a);

        if (n == "csch")
          return 1 / std::sinh(a);

        if (n == "exp")
          return std::exp(a);

        if (n == "ln")
          return std::log(a);

        if (n == "log")
          return std::log10(a);

        if (n == "sqrt")
          return std::sqrt(a);

        if (n == "abs")
          return std::abs(a);

        throw std::runtime_error(
          "unknown function"
        );
      }

      throw std::runtime_error(
        "invalid expression"
      );
    };

    double r = expr();

    if (p != s.size())
      throw std::runtime_error(
        "invalid expression"
      );

    return (float)r;
  }

  struct Exact {
    long long n = 0;
    long long d = 1;
    std::string symbolic;
  };

  inline long long gcdll(
    long long a,
    long long b
  ) {
    a = std::llabs(a);
    b = std::llabs(b);

    while (b) {
      long long t = a % b;
      a = b;
      b = t;
    }

    return a ? a : 1;
  }

  inline bool rationalValue(
    double x,
    long long& n,
    long long& d
  ) {
    if (!std::isfinite(x))
      return false;

    double r = std::round(x);

    if (
      std::abs(x - r) < 1e-10 &&
      std::abs(r) < 9000000000000000.0
    ) {
      n = (long long)r;
      d = 1;
      return true;
    }

    for (
      long long den = 1;
    den <= 1000000;
    den *= 10
    ) {
      double num = std::round(x * den);

      if (
        std::abs(x - num / den) < 1e-10 &&
        std::abs(num) < 9000000000000000.0
      ) {
        n = (long long)num;
        d = den;
        return true;
      }

      if (den > 100000)
        break;
    }

    return false;
  }

  inline Exact makeExact(
    long long n,
    long long d = 1
  ) {
    if (d < 0) {
      n = -n;
      d = -d;
    }

    long long g = gcdll(n, d);

    return {
      n / g,
      d / g,
      ""
    };
  }

  inline std::string exactString(
    const Exact& a
  ) {
    if (!a.symbolic.empty())
      return a.symbolic;

    if (a.d == 1)
      return std::to_string(a.n);

    return std::to_string(a.n) +
    "/" +
    std::to_string(a.d);
  }

  inline Exact exactAdd(
    const Exact& a,
    const Exact& b
  ) {
    if (
      a.symbolic.empty() &&
      b.symbolic.empty()
    )
      return makeExact(
        a.n * b.d + b.n * a.d,
        a.d * b.d
      );

      Exact r;
      r.symbolic =
      "(" + exactString(a) +
      ")+(" + exactString(b) + ")";
      return r;
  }

  inline Exact exactSub(
    const Exact& a,
    const Exact& b
  ) {
    if (
      a.symbolic.empty() &&
      b.symbolic.empty()
    )
      return makeExact(
        a.n * b.d - b.n * a.d,
        a.d * b.d
      );

      Exact r;
      r.symbolic =
      "(" + exactString(a) +
      ")-(" + exactString(b) + ")";
      return r;
  }

  inline Exact exactMul(
    const Exact& a,
    const Exact& b
  ) {
    if (
      a.symbolic.empty() &&
      b.symbolic.empty()
    )
      return makeExact(
        a.n * b.n,
        a.d * b.d
      );

      Exact r;
      r.symbolic =
      "(" + exactString(a) +
      ")*(" + exactString(b) + ")";
      return r;
  }

  inline Exact exactDiv(
    const Exact& a,
    const Exact& b
  ) {
    if (
      a.symbolic.empty() &&
      b.symbolic.empty()
    )
      return makeExact(
        a.n * b.d,
        a.d * b.n
      );

      Exact r;
      r.symbolic =
      "(" + exactString(a) +
      ")/(" + exactString(b) + ")";
      return r;
  }

  inline std::string simplifySqrtInteger(
    long long n
  ) {
    if (n < 0)
      return "sqrt(" + std::to_string(n) + ")";

    long long outside = 1;
    long long inside = n;

    for (long long i = 2; i * i <= inside; ++i) {
      while (inside % (i * i) == 0) {
        inside /= i * i;
        outside *= i;
      }
    }

    if (inside == 0)
      return "0";

    if (inside == 1)
      return std::to_string(outside);

    if (outside == 1)
      return "sqrt(" +
      std::to_string(inside) +
      ")";

    return std::to_string(outside) +
    "*sqrt(" +
    std::to_string(inside) +
    ")";
  }

  inline Exact exactSqrt(
    const Exact& a
  ) {
    if (!a.symbolic.empty()) {
      Exact r;
      r.symbolic =
      "sqrt(" +
      exactString(a) +
      ")";
    return r;
    }

    if (a.n < 0) {
      Exact r;
      r.symbolic =
      "sqrt(" +
      exactString(a) +
      ")";
    return r;
    }

    long long sn =
    (long long)std::llround(
      std::sqrt((long double)a.n)
    );

    long long sd =
    (long long)std::llround(
      std::sqrt((long double)a.d)
    );

    if (
      sn * sn == a.n &&
      sd * sd == a.d
    )
      return makeExact(sn, sd);

      if (a.d == 1) {
        Exact r;
        r.symbolic =
        simplifySqrtInteger(a.n);
        return r;
      }

      Exact r;
      r.symbolic =
      "sqrt(" +
      exactString(a) +
      ")";
    return r;
  }

  inline Exact exactPow(
    const Exact& a,
    const Exact& b
  ) {
    if (
      b.symbolic.empty() &&
      b.d == 1 &&
      std::llabs(b.n) <= 20
    ) {
      long long p = b.n;

      if (p == 0)
        return makeExact(1);

      if (p < 0)
        return exactDiv(
          makeExact(1),
                        exactPow(
                          a,
                          makeExact(-p)
                        )
        );

      Exact r = makeExact(1);

      for (long long i = 0; i < p; ++i)
        r = exactMul(r, a);

      return r;
    }

    Exact r;
    r.symbolic =
    "(" + exactString(a) +
    ")^(" + exactString(b) + ")";
    return r;
  }

  inline Exact exactFunction(
    const std::string& n,
    const Exact& a
  ) {
    if (n == "sqrt")
      return exactSqrt(a);

    if (a.symbolic.empty()) {
      double v =
      (double)a.n / a.d;

      if (n == "abs") {
        if (a.n < 0)
          return makeExact(-a.n, a.d);

        return a;
      }

      if (
        n == "sin" &&
        std::abs(v) < 1e-12
      )
        return makeExact(0);

        if (
          n == "cos" &&
          std::abs(v) < 1e-12
        )
          return makeExact(1);

          if (
            n == "tan" &&
            std::abs(v) < 1e-12
          )
            return makeExact(0);

            if (
              n == "exp" &&
              std::abs(v) < 1e-12
            )
              return makeExact(1);
    }

    if (
      n == "sin" ||
      n == "cos" ||
      n == "tan"
    ) {
      if (a.symbolic == "pi") {
        if (n == "sin")
          return makeExact(0);

        if (n == "cos")
          return makeExact(-1);

        Exact r;
        r.symbolic = "tan(pi)";
        return r;
      }

      if (a.symbolic == "(pi)/(2)") {
        if (n == "sin")
          return makeExact(1);

        if (n == "cos")
          return makeExact(0);
      }
    }

    Exact r;
    r.symbolic =
    n + "(" + exactString(a) + ")";
    return r;
  }

  inline std::string exactNormalize(
    const Exact& a
  ) {
    if (!a.symbolic.empty()) {
      std::string s = a.symbolic;
      bool changed = true;

      while (changed) {
        changed = false;

        size_t p;

        while (
          (p = s.find("(1)")) !=
          std::string::npos
        ) {
          s.replace(p, 3, "1");
          changed = true;
        }

        while (
          (p = s.find("(0)")) !=
          std::string::npos
        ) {
          s.replace(p, 3, "0");
          changed = true;
        }
      }

      return s;
    }

    return exactString(a);
  }

  inline std::string evaluateClosed(
    const std::string& input,
    double xvalue
  );

  inline std::string closedParse(
    const std::string& s,
    double xvalue
  ) {
    size_t p = 0;

    std::function<Exact()> expr;
    std::function<Exact()> term;
    std::function<Exact()> power;
    std::function<Exact()> unary;
    std::function<Exact()> primary;

    expr = [&] {
      Exact v = term();

      while (
        p < s.size() &&
        (s[p] == '+' || s[p] == '-')
      ) {
        char o = s[p++];
        Exact q = term();

        v =
        o == '+'
        ? exactAdd(v, q)
        : exactSub(v, q);
      }

      return v;
    };

    term = [&] {
      Exact v = power();

      while (
        p < s.size() &&
        (s[p] == '*' || s[p] == '/')
      ) {
        char o = s[p++];
        Exact q = power();

        v =
        o == '*'
        ? exactMul(v, q)
        : exactDiv(v, q);
      }

      return v;
    };

    power = [&] {
      Exact v = unary();

      if (
        p < s.size() &&
        s[p] == '^'
      ) {
        ++p;
        v = exactPow(v, power());
      }

      return v;
    };

    unary = [&] {
      if (
        p < s.size() &&
        s[p] == '+'
      ) {
        ++p;
        return unary();
      }

      if (
        p < s.size() &&
        s[p] == '-'
      ) {
        ++p;

        Exact v = unary();

        if (v.symbolic.empty())
          v.n = -v.n;
        else
          v.symbolic =
          "-(" + v.symbolic + ")";

        return v;
      }

      return primary();
    };

    primary = [&]() -> Exact {
      if (p >= s.size())
        throw std::runtime_error(
          "invalid expression"
        );

      if (s[p] == '(') {
        ++p;

        Exact v = expr();

        if (
          p >= s.size() ||
          s[p] != ')'
        )
          throw std::runtime_error(
            "missing )"
          );

          ++p;
          return v;
      }

      if (
        std::isdigit((unsigned char)s[p]) ||
        s[p] == '.'
      ) {
        size_t b = p;

        while (
          p < s.size() &&
          (
            std::isdigit(
              (unsigned char)s[p]
            ) ||
            s[p] == '.'
          )
        )
          ++p;

          double v =
          std::stod(
            s.substr(b, p - b)
          );

          long long n;
          long long d;

          if (
            rationalValue(
              v,
              n,
              d
            )
          )
            return makeExact(n, d);

            Exact r;
            r.symbolic =
            s.substr(b, p - b);
            return r;
      }

      if (
        std::isalpha(
          (unsigned char)s[p]
        )
      ) {
        size_t b = p;

        while (
          p < s.size() &&
          std::isalpha(
            (unsigned char)s[p]
          )
        )
          ++p;

          std::string n =
          s.substr(b, p - b);

          if (n == "x") {
            long long a;
            long long b;

            if (
              rationalValue(
                xvalue,
                a,
                b
              )
            )
              return makeExact(a, b);

              Exact r;
              r.symbolic =
              formatNumber(xvalue);
              return r;
          }

          if (n == "pi") {
            Exact r;
            r.symbolic = "pi";
            return r;
          }

          if (n == "e") {
            Exact r;
            r.symbolic = "e";
            return r;
          }

          if (
            p >= s.size() ||
            s[p] != '('
          )
            throw std::runtime_error(
              "unknown identifier"
            );

            ++p;

            Exact a = expr();

            if (
              p >= s.size() ||
              s[p] != ')'
            )
              throw std::runtime_error(
                "missing )"
              );

              ++p;

              return exactFunction(n, a);
      }

      throw std::runtime_error(
        "invalid expression"
      );
    };

    Exact r = expr();

    if (p != s.size())
      throw std::runtime_error(
        "invalid expression"
      );

    return exactNormalize(r);
  }

  inline std::string evaluateClosed(
    const std::string& input,
    double xvalue
  ) {
    try {
      return closedParse(
        removeSpaces(input),
                         xvalue
      );
    } catch (...) {
      return formatNumber(
        evaluateRaw(
          input,
          (float)xvalue
        )
      );
    }
  }

  inline std::string simplifyClosedExpression(
    const std::string& input
  ) {
    std::string s = removeSpaces(input);
    bool changed = true;

    while (changed) {
      changed = false;

      size_t p;

      while ((p = s.find("x^1")) != std::string::npos) {
        s.replace(p, 3, "x");
        changed = true;
      }

      while ((p = s.find("*1")) != std::string::npos) {
        s.erase(p, 2);
        changed = true;
      }

      while ((p = s.find("1*")) != std::string::npos) {
        s.erase(p, 2);
        changed = true;
      }
    }

    return s;
  }

  inline std::string differentiate(
    const std::string& expression,
    bool closedForm = false
  ) {
    std::string r =
    differentiateRaw(expression);

    if (!closedForm)
      return r;

    if (r == "2*x^1")
      return "2*x";

    if (r == "1*x^0")
      return "1";

    size_t p;

    while ((p = r.find("^1")) != std::string::npos)
      r.replace(p, 2, "");

    return r;
  }

  inline std::string integrate(
    const std::string& expression,
    bool closedForm = false
  ) {
    std::string r =
    integrateRaw(expression);

    if (!closedForm)
      return r;

    if (
      r.size() >= 2 &&
      r.substr(r.size() - 2) == "+C"
    )
      r = r.substr(
        0,
        r.size() - 2
      );

      return simplifyClosedExpression(r) + "+C";
  }

  inline float defintegralRaw(
    const std::string& input,
    float lowbound,
    float upbound
  ) {
    std::string expression =
    integrateRaw(input);

    if (expression == "r not found")
      return 0;

    size_t p =
    expression.find("+C");

    if (p != std::string::npos)
      expression =
      expression.substr(0, p);

    return evaluateRaw(
      expression,
      upbound
    ) -
    evaluateRaw(
      expression,
      lowbound
    );
  }

  inline std::string defintegralClosedRaw(
    const std::string& input,
    float lowbound,
    float upbound
  ) {
    std::string antiderivative =
    integrateRaw(input);

    if (antiderivative == "r not found")
      return "r not found";

    size_t p =
    antiderivative.find("+C");

    if (p != std::string::npos)
      antiderivative =
      antiderivative.substr(
        0,
        p
      );

    try {
      std::string hi =
      evaluateClosed(
        antiderivative,
        upbound
      );

      std::string lo =
      evaluateClosed(
        antiderivative,
        lowbound
      );

      auto parseRational =
      [](const std::string& s, Exact& out) -> bool {
        size_t slash = s.find('/');

        if (slash == std::string::npos) {
          if (!isNumber(s))
            return false;

          long long n;
          long long d;

          if (
            !rationalValue(
              std::stod(s),
                           n,
                           d
            )
          )
            return false;

            out = makeExact(n, d);
            return true;
        }

        std::string a =
        s.substr(0, slash);

        std::string b =
        s.substr(slash + 1);

        if (
          !isNumber(a) ||
          !isNumber(b)
        )
          return false;

          long long an;
          long long ad;
          long long bn;
          long long bd;

          if (
            !rationalValue(
              std::stod(a),
                           an,
                           ad
            ) ||
            !rationalValue(
              std::stod(b),
                           bn,
                           bd
            ) ||
            bn == 0
          )
            return false;

            out =
            makeExact(
              an * bd,
              ad * bn
            );

            return true;
      };

      Exact a;
      Exact b;

      if (
        parseRational(hi, a) &&
        parseRational(lo, b)
      )
        return exactNormalize(
          exactSub(a, b)
        );

        if (lo == "0")
          return hi;

      if (hi == "0")
        return "-(" + lo + ")";

      return hi + "-(" + lo + ")";
    } catch (...) {
      return formatNumber(
        defintegralRaw(
          input,
          lowbound,
          upbound
        )
      );
    }
  }

  inline std::string defintegral(
    const std::string& input,
    float lowbound,
    float upbound,
    bool closedForm = false
  ) {
    if (!closedForm)
      return formatNumber(
        defintegralRaw(
          input,
          lowbound,
          upbound
        )
      );

    return defintegralClosedRaw(
      input,
      lowbound,
      upbound
    );
  }

  inline std::string integrate(
    const std::string& expression,
    float lowbound,
    float upbound,
    bool closedForm = false
  ) {
    return defintegral(
      expression,
      lowbound,
      upbound,
      closedForm
    );
  }

  inline std::string evaluate(
    const std::string& expression,
    float xvalue,
    bool closedForm = false
  ) {
    if (!closedForm)
      return formatNumber(
        evaluateRaw(
          expression,
          xvalue
        )
      );

    return evaluateClosed(
      expression,
      xvalue
    );
  }

  struct Polynomial {
    std::vector<Complex> coefficients;

    Polynomial()
    : coefficients(1, Complex(0, 0)) {}

    explicit Polynomial(Complex v)
    : coefficients(1, v) {}

    explicit Polynomial(
      std::vector<Complex> v
    )
    : coefficients(std::move(v)) {
      trim();
    }

    void trim() {
      while (
        coefficients.size() > 1 &&
        std::abs(
          coefficients.back()
        ) < 1e-14
      )
        coefficients.pop_back();
    }

    int degree() const {
      return (int)coefficients.size() - 1;
    }

    bool isConstant() const {
      return degree() == 0;
    }

    Complex evaluate(Complex x) const {
      Complex r = 0;

      for (
        auto i = coefficients.rbegin();
      i != coefficients.rend();
      ++i
      )
        r = r * x + *i;

        return r;
    }
  };

  inline Polynomial addPolynomial(
    const Polynomial& a,
    const Polynomial& b
  ) {
    size_t n =
    std::max(
      a.coefficients.size(),
             b.coefficients.size()
    );

    std::vector<Complex> r(
      n,
      Complex(0, 0)
    );

    for (
      size_t i = 0;
    i < a.coefficients.size();
    ++i
    )
      r[i] += a.coefficients[i];

      for (
        size_t i = 0;
    i < b.coefficients.size();
    ++i
      )
        r[i] += b.coefficients[i];

        return Polynomial(r);
  }

  inline Polynomial subtractPolynomial(
    const Polynomial& a,
    const Polynomial& b
  ) {
    size_t n =
    std::max(
      a.coefficients.size(),
             b.coefficients.size()
    );

    std::vector<Complex> r(
      n,
      Complex(0, 0)
    );

    for (
      size_t i = 0;
    i < a.coefficients.size();
    ++i
    )
      r[i] += a.coefficients[i];

      for (
        size_t i = 0;
    i < b.coefficients.size();
    ++i
      )
        r[i] -= b.coefficients[i];

        return Polynomial(r);
  }

  inline Polynomial multiplyPolynomial(
    const Polynomial& a,
    const Polynomial& b
  ) {
    std::vector<Complex> r(
      a.degree() +
      b.degree() +
      1,
      Complex(0, 0)
    );

    for (
      size_t i = 0;
    i < a.coefficients.size();
    ++i
    ) {
      for (
        size_t j = 0;
      j < b.coefficients.size();
      ++j
      )
        r[i + j] +=
        a.coefficients[i] *
        b.coefficients[j];
    }

    return Polynomial(r);
  }

  inline Polynomial powerPolynomial(
    Polynomial b,
    int n
  ) {
    Polynomial r(
      Complex(1, 0)
    );

    while (n > 0) {
      if (n & 1)
        r =
        multiplyPolynomial(
          r,
          b
        );

      b =
      multiplyPolynomial(
        b,
        b
      );

      n >>= 1;
    }

    return r;
  }

  inline bool parsePolynomial(
    const std::string& input,
    Polynomial& result
  ) {
    std::string s =
    removeSpaces(input);

    if (s.empty())
      return false;

    size_t p = 0;

    std::function<bool(Polynomial&)> expr;
    std::function<bool(Polynomial&)> term;
    std::function<bool(Polynomial&)> power;
    std::function<bool(Polynomial&)> unary;
    std::function<bool(Polynomial&)> primary;

    expr = [&](Polynomial& o) {
      if (!term(o))
        return false;

      while (
        p < s.size() &&
        (s[p] == '+' || s[p] == '-')
      ) {
        char c = s[p++];
        Polynomial q;

        if (!term(q))
          return false;

        o =
        c == '+'
        ? addPolynomial(o, q)
        : subtractPolynomial(o, q);
      }

      return true;
    };

    term = [&](Polynomial& o) {
      if (!power(o))
        return false;

      while (
        p < s.size() &&
        (s[p] == '*' || s[p] == '/')
      ) {
        char c = s[p++];
        Polynomial q;

        if (!power(q))
          return false;

        if (c == '*') {
          o =
          multiplyPolynomial(
            o,
            q
          );
        } else {
          if (
            !q.isConstant() ||
            std::abs(
              q.coefficients[0]
            ) < 1e-14
          )
            return false;

            for (auto& v : o.coefficients)
              v /= q.coefficients[0];
        }
      }

      return true;
    };

    power = [&](Polynomial& o) {
      if (!unary(o))
        return false;

      if (
        p < s.size() &&
        s[p] == '^'
      ) {
        ++p;

        size_t b = p;

        if (
          p < s.size() &&
          (s[p] == '+' || s[p] == '-')
        )
          ++p;

          while (
            p < s.size() &&
            std::isdigit(
              (unsigned char)s[p]
            )
          )
            ++p;

            if (b == p)
              return false;

        int n;

        try {
          n =
          std::stoi(
            s.substr(
              b,
              p - b
            )
          );
        } catch (...) {
          return false;
        }

        if (n < 0)
          return false;

        o =
        powerPolynomial(
          o,
          n
        );
      }

      return true;
    };

    unary = [&](Polynomial& o) {
      if (
        p < s.size() &&
        s[p] == '+'
      ) {
        ++p;
        return unary(o);
      }

      if (
        p < s.size() &&
        s[p] == '-'
      ) {
        ++p;

        if (!unary(o))
          return false;

        for (auto& v : o.coefficients)
          v = -v;

        return true;
      }

      return primary(o);
    };

    primary = [&](Polynomial& o) {
      if (p >= s.size())
        return false;

      if (s[p] == '(') {
        ++p;

        if (!expr(o))
          return false;

        if (
          p >= s.size() ||
          s[p] != ')'
        )
          return false;

          ++p;
          return true;
      }

      if (
        s[p] == 'x' &&
        (
          p + 1 >= s.size() ||
          !std::isalpha(
            (unsigned char)s[p + 1]
          )
        )
      ) {
        ++p;

        o =
        Polynomial(
          std::vector<Complex>{
            0,
            1
          }
        );

        return true;
      }

      if (
        std::isdigit(
          (unsigned char)s[p]
        ) ||
        s[p] == '.'
      ) {
        size_t b = p;

        while (
          p < s.size() &&
          (
            std::isdigit(
              (unsigned char)s[p]
            ) ||
            s[p] == '.'
          )
        )
          ++p;

          try {
            o =
            Polynomial(
              Complex(
                std::stod(
                  s.substr(
                    b,
                    p - b
                  )
                ),
                0
              )
            );

            return true;
          } catch (...) {
            return false;
          }
      }

      if (
        std::isalpha(
          (unsigned char)s[p]
        )
      ) {
        size_t b = p;

        while (
          p < s.size() &&
          std::isalpha(
            (unsigned char)s[p]
          )
        )
          ++p;

          std::string n =
          s.substr(
            b,
            p - b
          );

          if (n == "pi") {
            o =
            Polynomial(
              Complex(pi, 0)
            );

            return true;
          }

          if (n == "e") {
            o =
            Polynomial(
              Complex(e, 0)
            );

            return true;
          }

          return false;
      }

      return false;
    };

    return expr(result) &&
    p == s.size();
  }

  inline Roots polynomialRoots(
    const Polynomial& input
  ) {
    Polynomial p = input;
    p.trim();

    int n = p.degree();

    if (n <= 0)
      return {};

    if (n == 1) {
      return {
        -p.coefficients[0] /
        p.coefficients[1]
      };
    }

    Complex lead =
    p.coefficients.back();

    for (auto& v : p.coefficients)
      v /= lead;

    Roots r(n);
    double radius = 1;

    for (int i = 0; i < n; ++i) {
      radius =
      std::max(
        radius,
        1.0 +
        std::abs(
          p.coefficients[i]
        )
      );
    }

    for (int i = 0; i < n; ++i) {
      r[i] =
      std::polar(
        radius,
        2 * pi * i / n
      );
    }

    for (int k = 0; k < 2000; ++k) {
      double change = 0;

      for (int i = 0; i < n; ++i) {
        Complex d = 1;

        for (int j = 0; j < n; ++j) {
          if (i != j)
            d *= r[i] - r[j];
        }

        if (std::abs(d) < 1e-30)
          d = 1e-30;

        Complex c =
        p.evaluate(r[i]) / d;

        r[i] -= c;

        change =
        std::max(
          change,
          std::abs(c)
        );
      }

      if (change < 1e-12)
        break;
    }

    for (auto& z : r) {
      if (std::abs(z.real()) < 1e-10)
        z.real(0);

      if (std::abs(z.imag()) < 1e-10)
        z.imag(0);
    }

    std::sort(
      r.begin(),
              r.end(),
              [](const Complex& a, const Complex& b) {
                return
                std::abs(
                  a.real() -
                  b.real()
                ) > 1e-10
                ? a.real() < b.real()
                : a.imag() < b.imag();
              }
    );

    return r;
  }

  inline Roots rootsRaw(
    const std::string& expression
  ) {
    Polynomial p;

    if (
      !parsePolynomial(
        expression,
        p
      )
    )
      throw std::runtime_error(
        "bad input"
      );

      return polynomialRoots(p);
  }

  inline std::string formatComplexClosed(
    const Complex& z
  ) {
    long long n;
    long long d;

    if (
      std::abs(z.imag()) < 1e-10 &&
      rationalValue(
        z.real(),
                    n,
                    d
      )
    )
      return exactString(
        makeExact(n, d)
      );

      if (std::abs(z.imag()) < 1e-10)
        return formatNumber(z.real());

    return "(" +
    formatNumber(z.real()) +
      ")+(" +
      formatNumber(z.imag()) +
        ")*i";
  }

  inline bool exactInteger(
    double v,
    long long& out
  ) {
    if (!std::isfinite(v))
      return false;

    double r = std::round(v);

    if (
      std::abs(v - r) > 1e-10 ||
      std::abs(r) > 9000000000000000.0
    )
      return false;

      out = static_cast<long long>(r);
      return true;
  }

  inline long long integerAbs(
    long long v
  ) {
    return v < 0 ? -v : v;
  }

  inline std::vector<long long> integerDivisors(
    long long n
  ) {
    std::vector<long long> r;

    n = integerAbs(n);

    if (n == 0)
      return {0};

    for (long long i = 1; i <= n / i; ++i) {
      if (n % i == 0) {
        r.push_back(i);

        if (i != n / i)
          r.push_back(n / i);
      }
    }

    return r;
  }

  inline std::string exactQuadraticRootString(
    long long a,
    long long b,
    long long c,
    bool plus
  ) {
    long long g =
    gcdll(
      gcdll(a, b),
          c
    );

    if (g > 1) {
      a /= g;
      b /= g;
      c /= g;
    }

    long long d =
    b * b -
    4LL * a * c;

    if (d == 0)
      return exactString(
        makeExact(
          -b,
          2LL * a
        )
      );

    if (d > 0) {
      long long r =
      static_cast<long long>(
        std::sqrt(
          static_cast<long double>(d)
        )
      );

      if (r * r == d) {
        return exactString(
          makeExact(
            plus ? -b + r : -b - r,
            2LL * a
          )
        );
      }

      long long den = 2LL * a;
      long long nb = -b;
      std::string sign =
      plus ? "+" : "-";

      if (den < 0) {
        den = -den;
        nb = -nb;
        sign =
        plus ? "-" : "+";
      }

      std::string first =
      nb < 0
      ? "-" + std::to_string(-nb)
      : std::to_string(nb);

      return "(" +
      first +
      sign +
      "sqrt(" +
      std::to_string(d) +
      "))/" +
      std::to_string(den);
    }

    long long nd = -d;

    long long r =
    static_cast<long long>(
      std::sqrt(
        static_cast<long double>(nd)
      )
    );

    long long den = 2LL * a;
    long long nb = -b;
    std::string sign =
    plus ? "+" : "-";

    if (den < 0) {
      den = -den;
      nb = -nb;
      sign =
      plus ? "-" : "+";
    }

    std::string real =
    std::to_string(nb);

    if (r * r == nd) {
      if (nb == 0 && r == den)
        return plus ? "i" : "-i";

      if (nb == 0)
        return
        "sqrt(" +
        std::to_string(nd) +
        ")" +
        (plus ? "*i/" : "*-i/") +
        std::to_string(den);

      return "(" +
      real +
      sign +
      "sqrt(" +
      std::to_string(nd) +
      ")*i)/" +
      std::to_string(den);
    }

    return "(" +
    real +
    sign +
    "sqrt(" +
    std::to_string(nd) +
    ")*i)/" +
    std::to_string(den);
  }

  inline std::vector<std::string> exactQuadraticRoots(
    long long a,
    long long b,
    long long c
  ) {
    if (a == 0)
      return {};

    std::vector<std::string> r;

    r.push_back(
      exactQuadraticRootString(
        a,
        b,
        c,
        true
      )
    );

    r.push_back(
      exactQuadraticRootString(
        a,
        b,
        c,
        false
      )
    );

    return r;
  }

  inline std::vector<std::string> exactCubicRoots(
    long long a,
    long long b,
    long long c,
    long long d
  ) {
    if (a == 0)
      return {};

    auto numerators =
    integerDivisors(d);

    auto denominators =
    integerDivisors(a);

    for (long long pn : numerators) {
      for (long long qn : denominators) {
        if (qn == 0)
          continue;

        for (int sign : {1, -1}) {
          long long p = pn * sign;
          long long q = qn;

          __int128 x = p;

          __int128 value =
          static_cast<__int128>(a) *
          x *
          x *
          x +
          static_cast<__int128>(b) *
          q *
          x *
          x +
          static_cast<__int128>(c) *
          q *
          q *
          x +
          static_cast<__int128>(d) *
          q *
          q *
          q;

          if (value != 0)
            continue;

          long long bq =
          b * q +
          a * p;

          if (bq % q != 0)
            continue;

          long long qb =
          bq / q;

          long long cq =
          c * q +
          qb * p;

          if (cq % q != 0)
            continue;

          long long qc =
          cq / q;

          std::vector<std::string> result;

          result.push_back(
            exactString(
              makeExact(
                p,
                q
              )
            )
          );

          auto qr =
          exactQuadraticRoots(
            a,
            qb,
            qc
          );

          result.insert(
            result.end(),
                        qr.begin(),
                        qr.end()
          );

          return result;
        }
      }
    }

    double A =
    static_cast<double>(b) / a;

    double B =
    static_cast<double>(c) / a;

    double C =
    static_cast<double>(d) / a;

    double pv =
    B -
    A * A / 3.0;

    double qv =
    2.0 * A * A * A / 27.0 -
    A * B / 3.0 +
    C;

    double delta =
    qv * qv / 4.0 +
    pv * pv * pv / 27.0;

    std::vector<std::string> result;

    if (delta > 1e-14) {
      std::string u =
      "cbrt(" +
      formatNumber(
        -qv / 2.0 +
        std::sqrt(delta)
      ) +
      ")";

    std::string v =
    "cbrt(" +
    formatNumber(
      -qv / 2.0 -
      std::sqrt(delta)
    ) +
    ")";

    std::string shift =
    formatNumber(
      -A / 3.0
    );

    result.push_back(
      u +
      "+" +
      v +
      (
        shift == "0"
        ? ""
        : "+(" + shift + ")"
      )
    );

    std::string real =
    "(-(" +
    u +
    "+" +
    v +
    ")/2)";

    std::string imag =
    "sqrt(3)*((" +
    u +
    ")-(" +
    v +
    "))/2";

    std::string shifted =
    shift == "0"
    ? real
    : "(" + real + ")+" + shift;

    result.push_back(
      shifted +
      "+(" +
      imag +
      ")*i"
    );

    result.push_back(
      shifted +
      "-(" +
      imag +
      ")*i"
    );

    return result;
    }

    if (std::abs(delta) <= 1e-14) {
      double u =
      std::cbrt(
        -qv / 2.0
      );

      double x1 =
      2.0 * u -
      A / 3.0;

      double x2 =
      -u -
      A / 3.0;

      result.push_back(
        formatNumber(x1)
      );

      result.push_back(
        formatNumber(x2)
      );

      result.push_back(
        formatNumber(x2)
      );

      return result;
    }

    double rr =
    2.0 *
    std::sqrt(
      -pv / 3.0
    );

    double theta =
    std::acos(
      (3.0 * qv /
      (2.0 * pv)) *
      std::sqrt(
        -3.0 / pv
      )
    );

    for (int k = 0; k < 3; ++k) {
      double x =
      rr *
      std::cos(
        (theta +
        2.0 * pi * k) /
        3.0
      ) -
      A / 3.0;

      result.push_back(
        formatNumber(x)
      );
    }

    return result;
  }

  inline std::vector<std::string> roots(
    const std::string& expression,
    bool closedForm = false
  ) {
    Polynomial p;

    if (!parsePolynomial(expression, p))
      throw std::runtime_error(
        "bad input"
      );

    p.trim();

    if (p.degree() <= 0)
      return {};

    if (!closedForm) {
      Roots r =
      polynomialRoots(p);

      std::vector<std::string> result;

      for (const auto& z : r)
        result.push_back(
          formatComplexClosed(z)
        );

      return result;
    }

    int degree =
    p.degree();

    if (
      degree == 1 ||
      degree == 2 ||
      degree == 3
    ) {
      long long a;
      long long b;
      long long c;
      long long d;

      if (
        degree == 1 &&
        exactInteger(
          p.coefficients[1].real(),
                     a
        ) &&
        exactInteger(
          p.coefficients[0].real(),
                     b
        )
      ) {
        return {
          exactString(
            makeExact(
              -b,
              a
            )
          )
        };
      }

      if (
        degree == 2 &&
        exactInteger(
          p.coefficients[2].real(),
                     a
        ) &&
        exactInteger(
          p.coefficients[1].real(),
                     b
        ) &&
        exactInteger(
          p.coefficients[0].real(),
                     c
        )
      ) {
        return exactQuadraticRoots(
          a,
          b,
          c
        );
      }

      if (
        degree == 3 &&
        exactInteger(
          p.coefficients[3].real(),
                     a
        ) &&
        exactInteger(
          p.coefficients[2].real(),
                     b
        ) &&
        exactInteger(
          p.coefficients[1].real(),
                     c
        ) &&
        exactInteger(
          p.coefficients[0].real(),
                     d
        )
      ) {
        return exactCubicRoots(
          a,
          b,
          c,
          d
        );
      }
    }

    Roots r =
    polynomialRoots(p);

    std::vector<std::string> result;

    for (const auto& z : r)
      result.push_back(
        formatComplexClosed(z)
      );

    return result;
  }

}
