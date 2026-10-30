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
    inline float defintegralRaw(const std::string&, float, float);

    inline std::string evaluateClosed(
        const std::string&,
        double
    );

    inline std::string limit(
        const std::string&,
        double,
        bool
    );

    inline std::string limit(
        const std::string&,
        double,
        const std::string&,
        bool
    );

    inline std::string limit(
        const std::string&,
        const std::string&,
        const std::string&,
        bool
    );

    inline std::string limitleft(
        const std::string&,
        double,
        bool
    );

    inline std::string limitright(
        const std::string&,
        double,
        bool
    );

    inline std::string rationalDefiniteIntegralClosed( //sigh...
        const std::string&,
        float,
        float
    );

}
