#pragma once

#include "cymbolic_roots.h"

namespace cymbolic {

    inline bool limitFiniteValue(double v) {
        return std::isfinite(v) && !std::isnan(v);
    }

    inline std::string limitDirectionNormalize(
        const std::string& direction
    ) {
        std::string d = removeSpaces(direction);

        std::transform(
            d.begin(),
                       d.end(),
                       d.begin(),
                       [](unsigned char c) {
                           return (char)std::tolower(c);
                       }
        );

        return d;
    }

    inline bool limitTopLevelDivision(
        const std::string& expression,
        std::string& numerator,
        std::string& denominator
    ) {
        std::string s = removeSpaces(expression);
        size_t p = findOperator(s, '/');

        if (p == std::string::npos)
            return false;

        numerator = s.substr(0, p);
        denominator = s.substr(p + 1);

        return
        !numerator.empty() &&
        !denominator.empty();
    }

    inline std::string limitPolynomialInfinity(
        const std::string& expression,
        int sign,
        bool closedForm
    ) {
        std::string numerator;
        std::string denominator;

        if (
            !limitTopLevelDivision(
                expression,
                numerator,
                denominator
            )
        )
            return "";

            Polynomial np;
            Polynomial dp;

            if (
                !parsePolynomial(numerator, np) ||
                !parsePolynomial(denominator, dp)
            )
                return "";

                np.trim();
                dp.trim();

                if (
                    dp.degree() == 0 &&
                    std::abs(dp.coefficients[0]) < 1e-14
                )
                    return "undefined";

                    int nd = np.degree();
                    int dd = dp.degree();
                    int difference = nd - dd;

                    if (difference < 0)
                        return "0";

        Complex nc = np.coefficients.back();
        Complex dc = dp.coefficients.back();

        if (difference == 0) {
            Complex r = nc / dc;

            if (
                std::abs(r.imag()) < 1e-12 &&
                std::abs(r.real()) < 1e-12
            )
                return "0";

                if (
                    std::abs(r.imag()) < 1e-12 &&
                    closedForm
                ) {
                    long long n;
                    long long d;

                    if (
                        rationalValue(
                            r.real(),
                                      n,
                                      d
                        )
                    )
                        return exactString(
                            makeExact(n, d)
                        );
                }

                if (std::abs(r.imag()) < 1e-12)
                    return formatNumber(r.real());

            return formatComplexClosed(r);
        }

        double coefficient =
        (nc / dc).real();

        if (sign < 0 && (difference & 1))
            coefficient = -coefficient;

        if (coefficient > 0)
            return "infinity";

        if (coefficient < 0)
            return "-infinity";

        return "0";
    }

    inline std::string limitAtInfinity(
        const std::string& expression,
        int sign,
        bool closedForm
    ) {
        std::string rational =
        limitPolynomialInfinity(
            expression,
            sign,
            closedForm
        );

        if (!rational.empty())
            return rational;

        std::string s = removeSpaces(expression);

        if (s == "0")
            return "0";

        const double values[] = {
            1e3,
            1e4,
            1e5,
            1e6,
            1e7
        };

        double previous = 0;
        bool havePrevious = false;
        bool shrinkingToZero = true;

        for (double magnitude : values) {
            double x =
            sign > 0
            ? magnitude
            : -magnitude;

            double value;

            try {
                value =
                evaluateRaw(
                    expression,
                    (float)x
                );
            } catch (...) {
                return "undefined";
            }

            if (std::isnan(value))
                return "does not exist";

            if (std::isinf(value))
                return value > 0
                ? "infinity"
                : "-infinity";

            if (std::abs(value) > 1e30)
                shrinkingToZero = false;

            if (
                havePrevious &&
                std::abs(value) >=
                std::abs(previous)
            )
                shrinkingToZero = false;

                previous = value;
                havePrevious = true;
        }

        if (
            shrinkingToZero &&
            std::abs(previous) < 1e-6
        )
            return "0";

            double a =
            evaluateRaw(
                expression,
                (float)(
                    sign > 0
                    ? 1e6
                    : -1e6
                )
            );

            double b =
            evaluateRaw(
                expression,
                (float)(
                    sign > 0
                    ? 1e7
                    : -1e7
                )
            );

            if (
                limitFiniteValue(a) &&
                limitFiniteValue(b) &&
                std::abs(a - b) < 1e-8
            ) {
                if (closedForm)
                    return evaluateClosed(
                        expression,
                        sign > 0
                        ? 1e7
                        : -1e7
                    );

                return formatNumber(b);
            }

            return "does not exist";
    }

    inline std::string limitSideFinite(
        const std::string& expression,
        double point,
        int side,
        bool closedForm
    ) {
        double direct;

        try {
            direct =
            evaluateRaw(
                expression,
                (float)point
            );
        } catch (...) {
            direct =
            std::numeric_limits<double>::quiet_NaN();
        }

        if (limitFiniteValue(direct)) {
            if (closedForm)
                return evaluateClosed(
                    expression,
                    point
                );

            return formatNumber(direct);
        }

        const double deltas[] = {
            1e-3,
            1e-4,
            1e-5,
            1e-6
        };

        double values[4]{};

        for (int i = 0; i < 4; ++i) {
            double x =
            point +
            (
                side < 0
                ? -deltas[i]
                : deltas[i]
            );

            try {
                values[i] =
                evaluateRaw(
                    expression,
                    (float)x
                );
            } catch (...) {
                values[i] =
                std::numeric_limits<double>::quiet_NaN();
            }

            if (std::isinf(values[i]))
                return values[i] > 0
                ? "infinity"
                : "-infinity";
        }

        if (
            limitFiniteValue(values[0]) &&
            limitFiniteValue(values[1]) &&
            limitFiniteValue(values[2]) &&
            limitFiniteValue(values[3])
        ) {
            bool grows =
            std::abs(values[1]) >
            std::abs(values[0]) * 5.0 &&
            std::abs(values[2]) >
            std::abs(values[1]) * 5.0 &&
            std::abs(values[3]) >
            std::abs(values[2]) * 5.0;

            if (grows) {
                bool positive =
                values[0] > 0 &&
                values[1] > 0 &&
                values[2] > 0 &&
                values[3] > 0;

                bool negative =
                values[0] < 0 &&
                values[1] < 0 &&
                values[2] < 0 &&
                values[3] < 0;

                if (positive)
                    return "infinity";

                if (negative)
                    return "-infinity";
            }

            double a = values[2];
            double b = values[3];

            if (
                std::abs(a - b) <
                1e-7 *
                std::max(1.0, std::abs(a))
            ) {
                if (closedForm) {
                    try {
                        std::string exact =
                        evaluateClosed(
                            expression,
                            point
                        );

                        if (
                            exact.find("/0") ==
                            std::string::npos
                        )
                            return exact;
                    } catch (...) {}
                }

                return formatNumber(b);
            }
        }

        return "does not exist";
    }

    inline std::string limitleft(
        const std::string& expression,
        double point,
        bool closedForm = false
    ) {
        return limitSideFinite(
            expression,
            point,
            -1,
            closedForm
        );
    }

    inline std::string limitright(
        const std::string& expression,
        double point,
        bool closedForm = false
    ) {
        return limitSideFinite(
            expression,
            point,
            1,
            closedForm
        );
    }

    inline std::string limit(
        const std::string& expression,
        double point,
        bool closedForm = false
    ) {
        std::string left =
        limitleft(
            expression,
            point,
            closedForm
        );

        std::string right =
        limitright(
            expression,
            point,
            closedForm
        );

        if (left == right)
            return left;

        if (
            (
                left == "infinity" &&
                right == "infinity"
            ) ||
            (
                left == "-infinity" &&
                right == "-infinity"
            )
        )
            return left;

            return "does not exist";
    }

    inline std::string limit(
        const std::string& expression,
        double point,
        const std::string& direction,
        bool closedForm = false
    ) {
        std::string d =
        limitDirectionNormalize(direction);

        if (d == "left")
            return limitleft(
                expression,
                point,
                closedForm
            );

        if (d == "right")
            return limitright(
                expression,
                point,
                closedForm
            );

        if (
            d == "both" ||
            d == "two-sided" ||
            d == "twosided"
        )
            return limit(
                expression,
                point,
                closedForm
            );

            return "undefined";
    }

    inline std::string limit(
        const std::string& expression,
        const std::string& point,
        const std::string& direction,
        bool closedForm = false
    ) {
        std::string p = removeSpaces(point);
        std::string d =
        limitDirectionNormalize(direction);

        if (
            p == "infinity" ||
            p == "+infinity"
        ) {
            if (
                d != "left" &&
                d != "right" &&
                d != "both" &&
                d != "two-sided" &&
                d != "twosided"
            )
                return "undefined";

                return limitAtInfinity(
                    expression,
                    1,
                    closedForm
                );
        }

        if (p == "-infinity") {
            if (
                d != "left" &&
                d != "right" &&
                d != "both" &&
                d != "two-sided" &&
                d != "twosided"
            )
                return "undefined";

                return limitAtInfinity(
                    expression,
                    -1,
                    closedForm
                );
        }

        if (isNumber(p)) {
            double value = std::stod(p);

            return limit(
                expression,
                value,
                d,
                closedForm
            );
        }

        return "undefined";
    }

}
