#pragma once

#include <string>
#include <cctype>

namespace cymbolic {

    inline std::string latex(const std::string& input) {
        class Parser {
            const std::string& s;
            std::size_t p = 0;

            bool end() const {
                return p >= s.size();
            }

            char peek() const {
                return end() ? '\0' : s[p];
            }

            bool consume(char c) {
                if (peek() == c) {
                    ++p;
                    return true;
                }
                return false;
            }

            std::string group(const std::string& value) {
                return "{" + value + "}";
            }

            std::string parseExpression() {
                std::string out = parseTerm();

                while (!end()) {
                    if (consume('+')) {
                        out += "+" + parseTerm();
                    } else if (consume('-')) {
                        out += "-" + parseTerm();
                    } else {
                        break;
                    }
                }

                return out;
            }

            std::string parseTerm() {
                std::string out = parseUnary();

                while (!end()) {
                    if (consume('*')) {
                        out += parseUnary();
                    } else if (consume('/')) {
                        out = "\\frac" + group(out) + group(parseUnary());
                    } else {
                        break;
                    }
                }

                return out;
            }

            std::string parseUnary() {
                if (consume('+'))
                    return parseUnary();

                if (consume('-'))
                    return "-" + parseUnary();

                return parsePower();
            }

            std::string parsePower() {
                std::string base = parsePrimary();

                if (consume('^')) {
                    std::string exponent = parseUnary();
                    return group(base) + "^" + group(exponent);
                }

                return base;
            }

            std::string parsePrimary() {
                if (consume('(')) {
                    std::string inside = parseExpression();
                    consume(')');
                    return inside;
                }

                if (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '.') {
                    std::string number;
                    while (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '.') {
                        number += peek();
                        ++p;
                    }
                    return number;
                }

                if (std::isalpha(static_cast<unsigned char>(peek()))) {
                    std::string word;
                    while (std::isalpha(static_cast<unsigned char>(peek()))) {
                        word += peek();
                        ++p;
                    }

                    if (word == "pi")
                        return "\\pi";
                    if (word == "e")
                        return "e";
                    if (word == "i")
                        return "i";

                    if (word == "sqrt" || word == "cbrt") {
                        if (consume('(')) {
                            std::string inside = parseExpression();
                            consume(')');
                            return word == "sqrt"
                            ? "\\sqrt" + group(inside)
                            : "\\sqrt[3]" + group(inside);
                        }
                    }

                    if (word == "abs" && consume('(')) {
                        std::string inside = parseExpression();
                        consume(')');
                        return "\\left|" + inside + "\\right|";
                    }

                    if (consume('(')) {
                        std::string inside = parseExpression();
                        consume(')');
                        static const char* functions[] = {
                            "sin", "cos", "tan", "cot", "sec", "csc",
                            "asin", "acos", "atan", "acot", "asec", "acsc",
                            "sinh", "cosh", "tanh", "coth", "sech", "csch",
                            "asinh", "acosh", "atanh", "acoth", "asech", "acsch",
                            "ln", "log", "exp"
                        };

                        bool known = false;
                        for (const char* f : functions) {
                            if (word == f) {
                                known = true;
                                break;
                            }
                        }

                        if (known) {
                            if (word == "ln")
                                return "\\ln\\left(" + inside + "\\right)";
                            if (word == "log")
                                return "\\log\\left(" + inside + "\\right)";
                            if (word == "exp")
                                return "e^{" + inside + "}";

                            std::string name = word;
                            if (name.size() > 1 && name[0] == 'a' &&
                                (name.substr(0, 3) == "asi" ||
                                name.substr(0, 3) == "aco" ||
                                name.substr(0, 3) == "ata" ||
                                name.substr(0, 3) == "ase" ||
                                name.substr(0, 3) == "acs")) {
                                name = "\\" + name.substr(1);
                            return name + "^{-1}\\left(" + inside + "\\right)";
                                }

                                return "\\" + name + "\\left(" + inside + "\\right)";
                        }

                        return word + "\\left(" + inside + "\\right)";
                    }

                    return word;
                }

                if (!end()) {
                    char c = s[p++];
                    return std::string(1, c);
                }

                return "";
            }

        public:
            explicit Parser(const std::string& text) : s(text) {}

            std::string parse() {
                std::string out = parseExpression();

                if (p < s.size()) {
                    out += s.substr(p);
                }

                return out;
            }
        };

        return Parser(input).parse();
    }

}
