#pragma once

#include "cymbolic_exact.h"

namespace cymbolic {

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
                throw std::runtime_error("invalid expression");

            if (s[p] == '(') {
                ++p;
                double v = expr();

                if (
                    p >= s.size() ||
                    s[p] != ')'
                )
                    throw std::runtime_error("missing )");

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
                        std::isdigit((unsigned char)s[p]) ||
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
                    std::isalpha((unsigned char)s[p])
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
                        throw std::runtime_error("missing )");

                        ++p;

                        if (n == "sin") return std::sin(a);
                        if (n == "cos") return std::cos(a);
                        if (n == "tan") return std::tan(a);
                        if (n == "cot") return 1 / std::tan(a);
                        if (n == "sec") return 1 / std::cos(a);
                        if (n == "csc") return 1 / std::sin(a);

                        if (n == "asin") return std::asin(a);
                        if (n == "acos") return std::acos(a);
                        if (n == "atan") return std::atan(a);
                        if (n == "acot") return pi / 2 - std::atan(a);
                        if (n == "asec") return std::acos(1 / a);
                        if (n == "acsc") return std::asin(1 / a);

                        if (n == "sinh") return std::sinh(a);
                        if (n == "cosh") return std::cosh(a);
                        if (n == "tanh") return std::tanh(a);
                        if (n == "coth") return 1 / std::tanh(a);
                        if (n == "sech") return 1 / std::cosh(a);
                        if (n == "csch") return 1 / std::sinh(a);

                        if (n == "exp") return std::exp(a);
                        if (n == "ln") return std::log(a);
                        if (n == "log") return std::log10(a);
                        if (n == "sqrt") return std::sqrt(a);
                        if (n == "abs") return std::abs(a);

                        throw std::runtime_error(
                            "unknown function"
                        );
            }

            throw std::runtime_error("invalid expression");
        };

        double r = expr();

        if (p != s.size())
            throw std::runtime_error("invalid expression");

        return (float)r;
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

}
