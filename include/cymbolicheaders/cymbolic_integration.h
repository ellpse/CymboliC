#pragma once

#include "cymbolic_limits.h"
#include "cymbolic_rational.h"

namespace cymbolic {

    inline std::string integrateTerm(
        const std::string& term
    ) {
        std::string s = removeSpaces(term);

        if (s.empty())
            return "0";

        if (s[0] == '+')
            return integrateTerm(s.substr(1));

        if (s[0] == '-') {
            std::string r =
            integrateTerm(s.substr(1));

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
                std::string r =
                integrateTerm(b);

                if (r == "r not found")
                    return r;

                return "(" + a + ")*(" + r + ")";
            }

            if (isNumber(b)) {
                std::string r =
                integrateTerm(a);

                if (r == "r not found")
                    return r;

                return "(" + b + ")*(" + r + ")";
            }
        }

        if (s.back() == ')') {
            size_t p0 = s.find('(');

            if (p0 != std::string::npos) {
                std::string fn =
                s.substr(0, p0);

                std::string arg =
                s.substr(
                    p0 + 1,
                    s.size() - p0 - 2
                );

                std::string da =
                differentiateRaw(arg);

                if (fn == "ln") {
                    if (da == "1")
                        return
                        "x*ln(x)-x";

                    if (isNumber(da))
                        return
                        "(" +
                        formatNumber(
                            1 / std::stod(da)
                        ) +
                        ")*(" +
                        arg +
                        "*ln(" +
                        arg +
                        ")-(" +
                        arg +
                        "))";
                }

                if (fn == "log") {
                    if (da == "1")
                        return
                        "x*log(x)-x/ln(10)";

                    if (isNumber(da))
                        return
                        "(" +
                        formatNumber(
                            1 / std::stod(da)
                        ) +
                        ")*(" +
                        arg +
                        "*log(" +
                        arg +
                        ")-(" +
                        arg +
                        ")/ln(10))";
                }

                if (
                    fn == "asin" &&
                    da == "1"
                )
                    return
                    arg +
                    "*asin(" +
                    arg +
                    ")+sqrt(1-(" +
                    arg +
                    ")^2)";

                if (
                    fn == "acos" &&
                    da == "1"
                )
                    return
                    arg +
                    "*acos(" +
                    arg +
                    ")-sqrt(1-(" +
                    arg +
                    ")^2)";

                if (
                    fn == "atan" &&
                    da == "1"
                )
                    return
                    arg +
                    "*atan(" +
                    arg +
                    ")-0.5*ln(1+(" +
                    arg +
                    ")^2)";

                if (
                    fn == "acot" &&
                    da == "1"
                )
                    return
                    arg +
                    "*acot(" +
                    arg +
                    ")+0.5*ln(1+(" +
                    arg +
                    ")^2)";

                if (
                    fn == "asec" &&
                    da == "1"
                )
                    return
                    arg +
                    "*asec(" +
                    arg +
                    ")-ln(abs(" +
                    arg +
                    "+sqrt((" +
                    arg +
                    ")^2-1)))";

                    if (
                        fn == "acsc" &&
                        da == "1"
                    )
                        return
                        arg +
                        "*acsc(" +
                        arg +
                        ")+ln(abs(" +
                        arg +
                        "+sqrt((" +
                        arg +
                        ")^2-1)))";

                        auto it =
                        integralRules.find(fn);

                        if (it != integralRules.end()) {
                            std::string r =
                            replaceArgument(
                                it->second,
                                arg
                            );

                            if (da == "1")
                                return r;

                            if (
                                isNumber(da) &&
                                std::abs(
                                    std::stod(da)
                                ) > 1e-12
                            )
                                return
                                "(" +
                                formatNumber(
                                    1 /
                                    std::stod(da)
                                ) +
                                ")*(" +
                                r +
                                ")";
                        }
            }
        }

        p = findOperator(s, '^');

        if (p != std::string::npos) {
            std::string b =
            s.substr(0, p);

            std::string q =
            s.substr(p + 1);

            if (
                b == "x" &&
                isNumber(q)
            ) {
                double n =
                std::stod(q);

                if (
                    std::abs(n + 1) <
                    1e-12
                )
                    return "ln(abs(x))";

                    return
                    "x^" +
                    formatNumber(n + 1) +
                        "/" +
                        formatNumber(n + 1);
            }

            if (
                isNumber(b) &&
                isNumber(q)
            )
                return
                formatNumber(
                    std::pow(
                        std::stod(b),
                             std::stod(q)
                    )
                ) +
                "*x";
        }

        if (s == "x")
            return "x^2/2";

        if (
            s.size() > 2 &&
            s[0] == 'x' &&
            s[1] == '^' &&
            isNumber(s.substr(2))
        ) {
            double n =
            std::stod(s.substr(2));

            if (
                std::abs(n + 1) <
                1e-12
            )
                return "ln(abs(x))";

                return
                "x^" +
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
        std::string s =
        removeSpaces(expression);

        if (s.empty())
            return "C";

        auto terms = splitTerms(s);
        std::string r;

        for (const auto& t : terms) {
            std::string v =
            integrateTerm(t);

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
            r =
            r.substr(
                0,
                r.size() - 2
            );

            return
            simplifyClosedExpression(r) +
            "+C";
    }

    inline float numericalDefiniteIntegral(
        const std::string& input,
        float lowbound,
        float upbound
    ) {
        if (lowbound == upbound)
            return 0;

        double a = lowbound;
        double b = upbound;
        double sign = 1;

        if (b < a) {
            std::swap(a, b);
            sign = -1;
        }

        auto f = [&](double x) -> double {
            return evaluateRaw(
                input,
                (float)x
            );
        };

        std::function<
        double(
            double,
            double,
            double,
            double,
            double,
            int
        )
        > simpson;

        simpson =
        [&](double l,
            double r,
            double fl,
            double fm,
            double fr,
            int depth) -> double {
                double m =
                (l + r) / 2;

                double lm =
                (l + m) / 2;

                double rm =
                (m + r) / 2;

                double flm = f(lm);
                double frm = f(rm);

                double whole =
                (r - l) *
                (fl + 4 * fm + fr) /
                6;

                double left =
                (m - l) *
                (fl + 4 * flm + fm) /
                6;

                double right =
                (r - m) *
                (fm + 4 * frm + fr) /
                6;

                double delta =
                left + right - whole;

                if (
                    depth <= 0 ||
                    std::abs(delta) <=
                    1e-12 *
                    (
                        1 +
                        std::abs(
                            left + right
                        )
                    )
                )
                    return
                    left +
                    right +
                    delta / 15;

                    return
                    simpson(
                        l,
                        m,
                        fl,
                        flm,
                        fm,
                        depth - 1
                    ) +
                    simpson(
                        m,
                        r,
                        fm,
                        frm,
                        fr,
                        depth - 1
                    );
            };

            double fa = f(a);
            double fb = f(b);
            double fm = f((a + b) / 2);

            return (float)(
                sign *
                simpson(
                    a,
                    b,
                    fa,
                    fm,
                    fb,
                    20
                )
            );
    }

    inline float defintegralRaw(
        const std::string& input,
        float lowbound,
        float upbound
    ) {
        std::string s =
        removeSpaces(input);

        if (
            findOperator(s, '/') !=
            std::string::npos
        )
            return numericalDefiniteIntegral(
                input,
                lowbound,
                upbound
            );

            std::string expression =
            integrateRaw(input);

            if (expression == "r not found")
                return numericalDefiniteIntegral(
                    input,
                    lowbound,
                    upbound
                );

            size_t p =
            expression.find("+C");

            if (p != std::string::npos)
                expression =
                expression.substr(0, p);

            return
            evaluateRaw(
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
        std::string rational =
        rationalDefiniteIntegralClosed(
            input,
            lowbound,
            upbound
        );

        if (rational != "r not found")
            return rational;

        std::string antiderivative =
        integrateRaw(input);

        if (antiderivative == "r not found")
            return formatNumber(
                numericalDefiniteIntegral(
                    input,
                    lowbound,
                    upbound
                )
            );

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
            [](const std::string& s,
               Exact& out) -> bool {
                   size_t slash =
                   s.find('/');

                   if (slash ==
                       std::string::npos) {
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

                       out =
                       makeExact(n, d);

                       return true;
                       }

                       std::string a =
                       s.substr(
                           0,
                           slash
                       );

                       std::string b =
                       s.substr(
                           slash + 1
                       );

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

    inline std::string defintegral(
        const std::string& input,
        const std::string& lowbound,
        const std::string& upbound,
        bool closedForm = false
    ) {
        bool lowinf =
        lowbound == "infinity" ||
        lowbound == "-infinity";

        bool upinf =
        upbound == "infinity" ||
        upbound == "-infinity";

        if (!lowinf && !upinf) {
            float lo = std::stof(lowbound);
            float hi = std::stof(upbound);

            return defintegral(
                input,
                lo,
                hi,
                closedForm
            );
        }

        std::string antiderivative =
        integrate(
            input,
            closedForm
        );

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

        auto evalbound =
        [&](const std::string& bound)
        -> std::string {
            if (bound == "infinity")
                return limit(
                    antiderivative,
                    "infinity",
                    "left",
                    closedForm
                );

            if (bound == "-infinity")
                return limit(
                    antiderivative,
                    "-infinity",
                    "right",
                    closedForm
                );

            return evaluate(
                antiderivative,
                std::stof(bound),
                            closedForm
            );
        };

        std::string lo =
        evalbound(lowbound);

        std::string hi =
        evalbound(upbound);

        if (
            lo == "infinity" &&
            hi == "infinity"
        )
            return "undefined";

            if (
                lo == "-infinity" &&
                hi == "-infinity"
            )
                return "undefined";

                if (lo == "0")
                    return hi;

        if (hi == "0")
            return "-(" + lo + ")";

        return hi + "-(" + lo + ")";
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

}
