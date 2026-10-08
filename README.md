# CymboliC

CymboliC is a lightweight C++ symbolic algebra and calculus library. It started as a simple antiderivative calculator, and I build off of it.

## Features

* Differentiation
* Symbolic integration
* Definite integrals
* Function evaluation
* Limits
* Function roots
* Exact and closed-form calculations
* More...

## Install

run 
```bash
chmod +x install.sh
sudo ./install.sh
```

## Usage

Include `cymbolic/cymbolic.h` and use the `cymbolic` namespace:

```cpp
#include <iostream>
#include <cymbolic/cymbolic.h>

int main() {
    std::cout << cymbolic::differentiate("x^2") << '\n';
}
```

Expressions use `x` as the variable and require explicit multiplication:

```text
2*x
3*sin(x)
x*(x+1)
```

Not:

```text
2x
3sin(x)
x(x+1)
```

I tried to add implicit multiplication, but it obliterated my tokenizer. greater programmers are welcome to fix this!

used constants are just `pi` and `e`.

## Differentiation

```cpp
cymbolic::differentiate("x^2");          // 2*x
cymbolic::differentiate("2*x^3");        // 6*x^2
cymbolic::differentiate("sin(x)");       // cos(x)
cymbolic::differentiate("x*sin(x)");     // ...
cymbolic::differentiate("x^2+sin(x)");   // idk what this is xD
```

Equivalent to:

```math
\frac{d}{dx}(x^2),\quad
\frac{d}{dx}(2x^3),\quad
\frac{d}{dx}(\sin x),\quad
\frac{d}{dx}(x\sin x),\quad
\frac{d}{dx}(x^2+\sin x)
```

## Integration

```cpp
cymbolic::integrate("x^2");
cymbolic::integrate("sin(x)");
cymbolic::integrate("exp(x)");
cymbolic::integrate("sqrt(x)");
cymbolic::integrate("sin(2*x)");
```

Equivalent to:

```math
\int x^2\,dx,\quad
\int\sin(x)\,dx,\quad
\int e^x\,dx,\quad
\int\sqrt{x}\,dx,\quad
\int\sin(2x)\,dx
```

indefinite integrals include `+C`.

```cpp
cymbolic::integrate("x^2", true);
```

## Definite Integrals

pass the lower and upper bounds directly:

```cpp
cymbolic::integrate("x^2", 0, 3);
```

```math
\int_0^3 x^2\,dx = 9
```

More examples:

```cpp
cymbolic::integrate("x", 0, 5);
cymbolic::integrate("2*x^2", 4.5, 3.14);
cymbolic::integrate("sin(x)", 0, pi);
cymbolic::integrate("exp(x)", 0, 1);
```

Closed-form output:

```cpp
cymbolic::integrate("x^2", 0, 3, true);
```

`defintegral` is also available:
it does the same thing as `integrate`. it is an artifact from an older version.

```cpp
cymbolic::defintegral("x^2", 0, 3);
```

## Evaluation

Evaluate an expression at a given value of `x`:

```cpp
cymbolic::evaluate("x^2+2*x", 5);
cymbolic::evaluate("sin(x)", 5);
cymbolic::evaluate("sqrt(x)", 5);
cymbolic::evaluate("exp(x)", 5);
```

For example:

```math
x^2+2x\quad\text{at }x=5
```

Closed-form evaluation can be requested with `true`:

```cpp
cymbolic::evaluate("x^2", 5, true);
```

## Limits

```cpp
cymbolic::limit("1/x", 0);
```

```math
\lim_{x\to0}\frac{1}{x}
```

one-sided limits:

```cpp
cymbolic::limitleft("1/x", 0);
cymbolic::limitright("1/x", 0);
```

specify the direction directly:

```cpp
cymbolic::limit("1/x", 0, "left", true);
cymbolic::limit("1/x", 0, "right", true);
```

limits at infinity:

```cpp
cymbolic::limit("1/x", "infinity", "right", true);
cymbolic::limit("1/x", "-infinity", "left", true);
```

```math
\lim_{x\to\infty}\frac1x
\qquad
\lim_{x\to-\infty}\frac1x
```

## Function Roots

Find the roots of a function by solving `f(x) = 0`:

```cpp
auto roots = cymbolic::roots("x^2-4");

for (const auto& root : roots)
    std::cout << root << '\n';
```

Examples:

```cpp
cymbolic::roots("x^2-4");
cymbolic::roots("x^2+2*x-3");
cymbolic::roots("x^3-x");
cymbolic::roots("x^4-16");
```

Equivalent equations:

```math
x^2-4=0
\qquad
x^2+2x-3=0
\qquad
x^3-x=0
\qquad
x^4-16=0
```

polynomial roots are solved symbolically when possible, but numerical methods are used as a fallback.

closed form:

```cpp
cymbolic::roots("x^2-4", true);
```

## Contributors

Contributors are welcome! feel welcome to fix my crappy programming.

## License

CymboliC is free and unencumbered software released into the public domain.

You may copy, modify, publish, use, compile, sell, or distribute it for any purpose, commercial or non-commercial.

See the [Unlicense](https://unlicense.org) website or the accompanying `LICENSE` file.
