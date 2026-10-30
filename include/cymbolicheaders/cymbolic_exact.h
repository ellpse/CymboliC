#pragma once

#include "cymbolic_core.h"

namespace cymbolic {

    struct Exact {
        long long n = 0;
        long long d = 1;
        std::string symbolic;
    };

    inline long long gcdll(long long a, long long b) {
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

        for (long long den = 1; den <= 1000000; den *= 10) {
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

    inline std::string exactString(const Exact& a) {
        if (!a.symbolic.empty())
            return a.symbolic;

        if (a.d == 1)
            return std::to_string(a.n);

        return std::to_string(a.n) + "/" +
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

    inline std::string simplifySqrtInteger(long long n) {
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
            return "sqrt(" + std::to_string(inside) + ")";

        return std::to_string(outside) +
        "*sqrt(" +
        std::to_string(inside) +
        ")";
    }

    inline Exact exactSqrt(const Exact& a) {
        if (!a.symbolic.empty()) {
            Exact r;
            r.symbolic =
            "sqrt(" + exactString(a) + ")";
            return r;
        }

        if (a.n < 0) {
            Exact r;
            r.symbolic =
            "sqrt(" + exactString(a) + ")";
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
                r.symbolic = simplifySqrtInteger(a.n);
                return r;
            }

            Exact r;
            r.symbolic =
            "sqrt(" + exactString(a) + ")";

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

            if (n == "sin" && std::abs(v) < 1e-12)
                return makeExact(0);

            if (n == "cos" && std::abs(v) < 1e-12)
                return makeExact(1);

            if (n == "tan" && std::abs(v) < 1e-12)
                return makeExact(0);

            if (n == "exp" && std::abs(v) < 1e-12)
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

    inline std::string exactNormalize(const Exact& a) {
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
                throw std::runtime_error("invalid expression");

            if (s[p] == '(') {
                ++p;

                Exact v = expr();

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

                    double v =
                    std::stod(s.substr(b, p - b));

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

            if (std::isalpha((unsigned char)s[p])) {
                size_t b = p;

                while (
                    p < s.size() &&
                    std::isalpha((unsigned char)s[p])
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
                        throw std::runtime_error("unknown identifier");

                        ++p;

                        Exact a = expr();

                        if (
                            p >= s.size() ||
                            s[p] != ')'
                        )
                            throw std::runtime_error("missing )");

                            ++p;

                            return exactFunction(n, a);
            }

            throw std::runtime_error("invalid expression");
        };

        Exact r = expr();

        if (p != s.size())
            throw std::runtime_error("invalid expression");

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

}
