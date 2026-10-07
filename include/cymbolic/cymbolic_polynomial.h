#pragma once

#include "cymbolic_differentiation.h"

namespace cymbolic {

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
                std::abs(coefficients.back()) < 1e-14
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

        for (size_t i = 0; i < a.coefficients.size(); ++i)
            r[i] += a.coefficients[i];

        for (size_t i = 0; i < b.coefficients.size(); ++i)
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

        for (size_t i = 0; i < a.coefficients.size(); ++i)
            r[i] += a.coefficients[i];

        for (size_t i = 0; i < b.coefficients.size(); ++i)
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

        for (size_t i = 0; i < a.coefficients.size(); ++i)
            for (size_t j = 0; j < b.coefficients.size(); ++j)
                r[i + j] +=
                a.coefficients[i] *
                b.coefficients[j];

            return Polynomial(r);
    }

    inline Polynomial powerPolynomial(
        Polynomial b,
        int n
    ) {
        Polynomial r(Complex(1, 0));

        while (n > 0) {
            if (n & 1)
                r = multiplyPolynomial(r, b);

            b = multiplyPolynomial(b, b);
            n >>= 1;
        }

        return r;
    }

    inline bool parsePolynomial(
        const std::string& input,
        Polynomial& result
    ) {
        std::string s = removeSpaces(input);
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
                    o = multiplyPolynomial(o, q);
                } else {
                    if (
                        !q.isConstant() ||
                        std::abs(q.coefficients[0]) < 1e-14
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
                        std::isdigit((unsigned char)s[p])
                    )
                        ++p;

                        if (b == p)
                            return false;

                int n;

                try {
                    n = std::stoi(
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

                o = powerPolynomial(o, n);
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

                o = Polynomial(
                    std::vector<Complex>{0, 1}
                );

                return true;
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

                    try {
                        o = Polynomial(
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
                        o = Polynomial(
                            Complex(pi, 0)
                        );
                        return true;
                    }

                    if (n == "e") {
                        o = Polynomial(
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

}
