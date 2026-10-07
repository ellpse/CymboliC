#pragma once

#include "cymbolic_evaluation.h"

namespace cymbolic {

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

            return "(" + formatNumber(v) +
            ")*(" + d + ")";
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
            return "-(" +
            differentiateTerm(s.substr(1)) +
            ")";

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

}
