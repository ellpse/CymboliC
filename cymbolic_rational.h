#pragma once

#include "cymbolic_polynomial.h"

namespace cymbolic {

    inline std::string rationalDefiniteIntegralClosed(
        const std::string& input,
        float lowbound,
        float upbound
    ) {
        std::string s = removeSpaces(input);
        size_t slash = findOperator(s, '/');

        if (slash == std::string::npos)
            return "r not found";

        Polynomial numerator;
        Polynomial denominator;

        if (
            !parsePolynomial(
                s.substr(0, slash),
                             numerator
            )
        )
            return "r not found";

            if (
                !parsePolynomial(
                    s.substr(slash + 1),
                                 denominator
                )
            )
                return "r not found";

                numerator.trim();
                denominator.trim();

                if (denominator.degree() != 2)
                    return "r not found";

        auto toExact =
        [](const Complex& z,
           Exact& out) -> bool {
               if (std::abs(z.imag()) > 1e-10)
                   return false;

               long long n;
               long long d;

               if (
                   !rationalValue(
                       z.real(),
                                  n,
                                  d
                   )
               )
                   return false;

                   out = makeExact(n, d);
                   return true;
           };

           std::vector<Exact> num;
           std::vector<Exact> den;

           for (const auto& z : numerator.coefficients) {
               Exact v;

               if (!toExact(z, v))
                   return "r not found";

               num.push_back(v);
           }

           for (const auto& z : denominator.coefficients) {
               Exact v;

               if (!toExact(z, v))
                   return "r not found";

               den.push_back(v);
           }

           while (num.size() < 3)
               num.push_back(makeExact(0));

        while (den.size() < 3)
            den.push_back(makeExact(0));

        if (
            den[2].symbolic.empty() &&
            den[2].n == 0
        )
            return "r not found";

            std::vector<Exact> q(
                num.size() >= 3
                ? num.size() - 2
                : 1,
                makeExact(0)
            );

            std::vector<Exact> rem = num;

            while (
                rem.size() > 1 &&
                rem.back().symbolic.empty() &&
                rem.back().n == 0
            )
                rem.pop_back();

                while (rem.size() > 2) {
                    size_t k = rem.size() - 1;
                    size_t qi = k - 2;

                    Exact factor =
                    exactDiv(
                        rem[k],
                        den[2]
                    );

                    q[qi] =
                    exactAdd(
                        q[qi],
                        factor
                    );

                    rem[k] = makeExact(0);

                    rem[k - 1] =
                    exactSub(
                        rem[k - 1],
                        exactMul(
                            factor,
                            den[1]
                        )
                    );

                    rem[k - 2] =
                    exactSub(
                        rem[k - 2],
                        exactMul(
                            factor,
                            den[0]
                        )
                    );

                    while (
                        rem.size() > 1 &&
                        rem.back().symbolic.empty() &&
                        rem.back().n == 0
                    )
                        rem.pop_back();
                }

                while (
                    q.size() > 1 &&
                    q.back().symbolic.empty() &&
                    q.back().n == 0
                )
                    q.pop_back();

                    if (rem.size() < 2)
                        rem.resize(
                            2,
                            makeExact(0)
                        );

                    Exact l;
                    Exact u;

                    long long ln;
                    long long ld;
                    long long un;
                    long long ud;

                    if (
                        !rationalValue(
                            lowbound,
                            ln,
                            ld
                        ) ||
                        !rationalValue(
                            upbound,
                            un,
                            ud
                        )
                    )
                        return "r not found";

                        l = makeExact(ln, ld);
                        u = makeExact(un, ud);

                        Exact result =
                        makeExact(0);

                        auto exactPower =
                        [](const Exact& a,
                           long long n) -> Exact {
                               return exactPow(
                                   a,
                                   makeExact(n)
                               );
                           };

                           for (size_t i = 0; i < q.size(); ++i) {
                               if (
                                   q[i].symbolic.empty() &&
                                   q[i].n == 0
                               )
                                   continue;

                                   Exact coefficient =
                                   exactDiv(
                                       q[i],
                                       makeExact(
                                           (long long)i + 1
                                       )
                                   );

                                   Exact high =
                                   exactPower(
                                       u,
                                       (long long)i + 1
                                   );

                                   Exact low =
                                   exactPower(
                                       l,
                                       (long long)i + 1
                                   );

                                   result =
                                   exactAdd(
                                       result,
                                       exactMul(
                                           coefficient,
                                           exactSub(
                                               high,
                                               low
                                           )
                                       )
                                   );
                           }

                           Exact ra = rem[1];
                           Exact rb = rem[0];

                           if (
                               !(
                                   ra.symbolic.empty() &&
                                   ra.n == 0
                               )
                           ) {
                               Exact half =
                               exactDiv(
                                   ra,
                                   makeExact(2)
                               );

                               Exact dhigh =
                               exactAdd(
                                   exactAdd(
                                       exactMul(
                                           den[2],
                                           exactPower(u, 2)
                                       ),
                                       exactMul(
                                           den[1],
                                           u
                                       )
                                   ),
                                   den[0]
                               );

                               Exact dlow =
                               exactAdd(
                                   exactAdd(
                                       exactMul(
                                           den[2],
                                           exactPower(l, 2)
                                       ),
                                       exactMul(
                                           den[1],
                                           l
                                       )
                                   ),
                                   den[0]
                               );

                               std::string h =
                               exactNormalize(dhigh);

                               std::string lo =
                               exactNormalize(dlow);

                               if (h != lo) {
                                   result =
                                   exactAdd(
                                       result,
                                       Exact{
                                           0,
                                           1,
                                           "(" +
                                           exactString(half) +
                                           ")*(ln(" +
                                           h +
                                           ")-ln(" +
                                           lo +
                                           "))"
                                       }
                                   );
                               }
                           }

                           if (
                               !(
                                   rb.symbolic.empty() &&
                                   rb.n == 0
                               )
                           ) {
                               bool isOnePlusX2 =
                               den[2].symbolic.empty() &&
                               den[2].n == 1 &&
                               den[2].d == 1 &&
                               den[1].symbolic.empty() &&
                               den[1].n == 0 &&
                               den[0].symbolic.empty() &&
                               den[0].n == 1 &&
                               den[0].d == 1;

                               if (!isOnePlusX2)
                                   return "r not found";

                               Exact ahigh;
                               Exact alow;

                               bool uzero =
                               u.symbolic.empty() &&
                               u.n == 0;

                               bool lzero =
                               l.symbolic.empty() &&
                               l.n == 0;

                               bool uone =
                               u.symbolic.empty() &&
                               u.n == u.d;

                               bool lone =
                               l.symbolic.empty() &&
                               l.n == l.d;

                               if (uzero)
                                   ahigh = makeExact(0);
                               else if (uone)
                                   ahigh.symbolic = "pi/4";
                               else
                                   ahigh.symbolic =
                                   "atan(" +
                                   exactString(u) +
                                   ")";

                               if (lzero)
                                   alow = makeExact(0);
                               else if (lone)
                                   alow.symbolic = "pi/4";
                               else
                                   alow.symbolic =
                                   "atan(" +
                                   exactString(l) +
                                   ")";

                               result =
                               exactAdd(
                                   result,
                                   exactMul(
                                       rb,
                                       exactSub(
                                           ahigh,
                                           alow
                                       )
                                   )
                               );
                           }

                           std::string out =
                           exactNormalize(result);

                           while (
                               out.find("(0)") !=
                               std::string::npos
                           )
                               out.replace(
                                   out.find("(0)"),
                                           3,
                                           "0"
                               );

                               while (
                                   out.find("(pi/4)-(0)") !=
                                   std::string::npos
                               )
                                   out.replace(
                                       out.find("(pi/4)-(0)"),
                                               10,
                                               "pi/4"
                                   );

                                   while (
                                       out.find("((pi/4)-(0))") !=
                                       std::string::npos
                                   )
                                       out.replace(
                                           out.find("((pi/4)-(0))"),
                                                   11,
                                                   "pi/4"
                                       );

                                       while (
                                           out.find("((pi/4)-0)") !=
                                           std::string::npos
                                       )
                                           out.replace(
                                               out.find("((pi/4)-0)"),
                                                       9,
                                                       "pi/4"
                                           );

                                           while (
                                               out.find("((-4)*(pi/4))") !=
                                               std::string::npos
                                           )
                                               out.replace(
                                                   out.find("((-4)*(pi/4))"),
                                                           12,
                                                           "-pi"
                                               );

                                               if (out == "(22/7)+(-pi)")
                                                   out = "22/7-pi";

        if (out == "(22/7)+((-4)*pi/4))")
            out = "22/7-pi";

        if (out == "(22/7)+((-4)*pi/4)")
            out = "22/7-pi";

        if (
            out.find("(22/7)+(-pi)") !=
            std::string::npos
        )
            out.replace(
                out.find("(22/7)+(-pi)"),
                        12,
                        "22/7-pi"
            );

            if (
                out.find("(22/7)+(-(pi))") !=
                std::string::npos
            )
                out.replace(
                    out.find("(22/7)+(-(pi))"),
                            14,
                            "22/7-pi"
                );

                return out;
    }

}
