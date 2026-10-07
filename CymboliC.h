#pragma once

/*
 * syntax!!
 *
 * only current constants are pi and e, written respectively
 * pi e
 *
 * we use x for the variable
 *
 * normal arithmetic
 * x+y
 * x-y
 * x*y
 * x/y
 * x^y
 * (x+y)
 * -(x)
 * abs(x)
 *
 * trig functions
 * sin(x)
 * cos(x)
 * tan(x)
 * cot(x)
 * sec(x)
 * csc(x)
 *
 * inverse trig functs.
 * asin(x)
 * acos(x)
 * atan(x)
 * acot(x)
 * asec(x)
 * acsc(x)
 *
 * hyperbolic trig functs.
 * sinh(x)
 * cosh(x)
 * tanh(x)
 * coth(x)
 * sech(x)
 * csch(x)
 *
 * misc cmath functions
 * exp(x)
 * ln(x)
 * log(x)
 * sqrt(x)
 * abs(x)
 *
 * differentiation
 * differentiate("x")
 * differentiate("x^2")
 * differentiate("2*x^3")
 * differentiate("sin(x)")
 * differentiate("cos(x)")
 * differentiate("tan(x)")
 * differentiate("exp(x)")
 * differentiate("ln(x)")
 * differentiate("sqrt(x)")
 * differentiate("sin(2*x)")
 * differentiate("x*sin(x)")
 * differentiate("x^2+sin(x)")
 * differentiate("x^2", true)
 *
 * indefinite integration
 * integrate("x")
 * integrate("x^2")
 * integrate("2*x^3")
 * integrate("sin(x)")
 * integrate("cos(x)")
 * integrate("tan(x)")
 * integrate("sec(x)")
 * integrate("csc(x)")
 * integrate("sinh(x)")
 * integrate("cosh(x)")
 * integrate("tanh(x)")
 * integrate("coth(x)")
 * integrate("sech(x)")
 * integrate("csch(x)")
 * integrate("exp(x)")
 * integrate("ln(x)")
 * integrate("log(x)")
 * integrate("sqrt(x)")
 * integrate("sin(2*x)")
 * integrate("exp(3*x)")
 * integrate("sec^2(x)")
 * integrate("csc^2(x)")
 * integrate("sech^2(x)")
 * integrate("csch^2(x)")
 * integrate("x^2", true)
 *
 * evaluation!
 * not made to be used as a function like this, only as a helper subroutine, but it works just fine.
 * evaluate("x", 5)
 * evaluate("x", 5, true)
 * evaluate("x^2", 5)
 * evaluate("x^2", 5, true)
 * evaluate("2*x^2+3*x", 5)
 * evaluate("sin(x)", 5)
 * evaluate("sqrt(x)", 5)
 *
 * definite integrals
 * yes, defintegral is a artifact from before i merged it into integrate. last time i removed it i broke something, so for now it stays xD
 * integrate("x", 0, 5)
 * integrate("x", 0, 5, true)
 * integrate("x^2", 0, 3)
 * integrate("x^2", 0, 3, true)
 * integrate("2*x^2", 4.5, 3.14)
 * integrate("sin(x)", 0, pi)
 * integrate("exp(x)", 0, 1)
 * defintegral("x", 0, 5)
 * defintegral("x", 0, 5, true)
 *
 * limit syntax.
 * limit("1/x", 0)
 * limit("1/x", 0, true)
 * limitleft("1/x", 0)
 * limitright("1/x", 0)
 * limit("1/x", 0, "left", true)
 * limit("1/x", 0, "right", true)
 * limit("1/x", "infinity", "left", true)
 * limit("1/x", "-infinity", "right", true)
 *
 * function roots
 * for quartics and under, it is guaranteed to be exact, or at least symbolically done. I use a numeric solution as a fallback. kind of cheating, but it is ok.
 * roots("x^2-4")
 * roots("x^2-4", true)
 *
 * normal integration
 * integrate("x^2")
 * x^3/3+C
 *
 * definite integration returns a number, this isn't guaranteed if you pass true into the function.
 * defintegrate is an artifact from before i merged it into integrate, so it really doesnt matter if you use it.
 * integrate("x^2", 0, 3)
 * 9
 *
 * function names:
 * differentiate
 * integrate
 * evaluate
 * defintegral
 * limit
 * limitleft
 * limitright
 * roots
 *
 * closed form is false by default.
 *
 * all function arguments are strings.
 * all numeric evaluation types are floats.
 *
 * implicit multiplication is BANNED. it completely decimates up my tokenizer, so use these:
 * 2*x
 * 3*sin(x)
 *
 * instead of:
 * 2x
 * 3sin(x)
 */

#include "cymbolic_core.h"
#include "cymbolic_exact.h"
#include "cymbolic_evaluation.h"
#include "cymbolic_differentiation.h"
#include "cymbolic_polynomial.h"
#include "cymbolic_roots.h"
#include "cymbolic_limits.h"
#include "cymbolic_rational.h"
#include "cymbolic_integration.h"
