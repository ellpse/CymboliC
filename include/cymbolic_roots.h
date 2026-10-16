#pragma once

#include "cymbolic_polynomial.h"

namespace cymbolic {

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

        Complex lead = p.coefficients.back();

        for (auto& v : p.coefficients)
            v /= lead;

        Roots r(n);
        double radius = 1;

        for (int i = 0; i < n; ++i)
            radius =
            std::max(
                radius,
                1.0 +
                std::abs(
                    p.coefficients[i]
                )
            );

        for (int i = 0; i < n; ++i)
            r[i] =
            std::polar(
                radius,
                2 * pi * i / n
            );

        for (int k = 0; k < 2000; ++k) {
            double change = 0;

            for (int i = 0; i < n; ++i) {
                Complex d = 1;

                for (int j = 0; j < n; ++j)
                    if (i != j)
                        d *= r[i] - r[j];

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

        if (!parsePolynomial(expression, p))
            throw std::runtime_error("bad input");

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

    inline long long integerAbs(long long v) {
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

            if (r * r == d)
                return exactString(
                    makeExact(
                        plus ? -b + r : -b - r,
                        2LL * a
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

        return {
            exactQuadraticRootString(
                a, b, c, true
            ),
            exactQuadraticRootString(
                a, b, c, false
            )
        };
    }

    inline std::vector<std::string> exactCubicRoots(
        long long a,
        long long b,
        long long c,
        long long d
    ) {
        if (a == 0)
            return {};

        auto numerators = integerDivisors(d);
        auto denominators = integerDivisors(a);

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
                    x * x * x +
                    static_cast<__int128>(b) *
                    q * x * x +
                    static_cast<__int128>(c) *
                    q * q * x +
                    static_cast<__int128>(d) *
                    q * q * q;

                    if (value != 0)
                        continue;

                    long long bq =
                    b * q +
                    a * p;

                    if (bq % q != 0)
                        continue;

                    long long qb = bq / q;

                    long long cq =
                    c * q +
                    qb * p;

                    if (cq % q != 0)
                        continue;

                    long long qc = cq / q;

                    std::vector<std::string> result;

                    result.push_back(
                        exactString(
                            makeExact(p, q)
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
            std::cbrt(-qv / 2.0);

            double x1 =
            2.0 * u -
            A / 3.0;

            double x2 =
            -u -
            A / 3.0;

            result.push_back(formatNumber(x1));
            result.push_back(formatNumber(x2));
            result.push_back(formatNumber(x2));

            return result;
        }

        double rr =
        2.0 *
        std::sqrt(-pv / 3.0);

        double theta =
        std::acos(
            (3.0 * qv /
            (2.0 * pv)) *
            std::sqrt(-3.0 / pv)
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
            throw std::runtime_error("bad input");

        p.trim();

        if (p.degree() <= 0)
            return {};

        if (!closedForm) {
            Roots r = polynomialRoots(p);
            std::vector<std::string> result;

            for (const auto& z : r)
                result.push_back(
                    formatComplexClosed(z)
                );

            return result;
        }

        int degree = p.degree();

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
            )
                return {
                    exactString(
                        makeExact(-b, a)
                    )
                };

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
                )
                    return exactQuadraticRoots(
                        a, b, c
                    );

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
                    )
                        return exactCubicRoots(
                            a, b, c, d
                        );
        }

        Roots r = polynomialRoots(p);
        std::vector<std::string> result;

        for (const auto& z : r)
            result.push_back(
                formatComplexClosed(z)
            );

        return result;
    }

}
