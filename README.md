# CymboliC

CymboliC is a lightweight C++ symbolic calculus library. It was created because I didn't like the other C++ symbolic math libraries, so I built my own calculus engine and expanded it from there.

## Features

* Differentiation
* Function evaluation
* Analytic integration
* Definite and indefinite integrals
* Limits
* Finding function roots
* Exact and closed-form calculations
* More...

## Usage

Include `CymboliC.h` and use the `cymbolic` namespace:

```cpp
#include <iostream>
#include "CymboliC.h"

int main() {
    std::cout << cymbolic::differentiate("x^2") << '\n';
    return 0;
}
```

Expressions use `x` as the variable and require explicit multiplication, as implicit multiplication obliterates up my tokenizer

```text
2*x
3*sin(x)
x*(x+1)
```

rather than:

```text
2x
3sin(x)
x(x+1)
```

The current constants are `pi` and `e`.

### Differentiation

for:

```cpp
cymbolic::differentiate("x^2");
```

the written equivalent is:

```math
\frac{d}{dx}(x^2)
```

other examples:

```cpp
cymbolic::differentiate("2*x^3");
cymbolic::differentiate("sin(x)");
cymbolic::differentiate("x*sin(x)");
cymbolic::differentiate("x^2+sin(x)");
```

```math
\frac{d}{dx}(2x^3)
\qquad
\frac{d}{dx}(\sin x)
\qquad
\frac{d}{dx}(x\sin x)
\qquad
\frac{d}{dx}(x^2+\sin x)
```

### Integration

code:

```cpp
cymbolic::integrate("x^2");
```

the written equivalent:

```math
\int x^2\,dx
```

other examples:

```cpp
cymbolic::integrate("sin(x)");
cymbolic::integrate("exp(x)");
cymbolic::integrate("sqrt(x)");
cymbolic::integrate("sin(2*x)");
```

```math
\int \sin(x)\,dx
\qquad
\int e^x\,dx
\qquad
\int \sqrt{x}\,dx
\qquad
\int \sin(2x)\,dx
```

The result of an indefinite integral appends `+C`

closed form output can be requested with `true`:

```cpp
cymbolic::integrate("x^2", true);
```

### Definite Integrals

code:

```cpp
cymbolic::integrate("x^2", 0, 3);
```

written equivalent:

```math
\int_0^3 x^2\,dx
```

for example:

```cpp
std::cout << cymbolic::integrate("x^2", 0, 3) << '\n';
```

returns:

```text
9
```

other examples:

```cpp
cymbolic::integrate("x", 0, 5);
cymbolic::integrate("2*x^2", 4.5, 3.14);
cymbolic::integrate("sin(x)", 0, pi);
cymbolic::integrate("exp(x)", 0, 1);
```

```math
\int_0^5 x\,dx
\qquad
\int_{4.5}^{3.14}2x^2\,dx
\qquad
\int_0^\pi\sin(x)\,dx
\qquad
\int_0^1e^x\,dx
```

Closed-form output can also be requested:

```cpp
cymbolic::integrate("x^2", 0, 3, true);
```

`defintegral` is also available for compatibility:

```cpp
cymbolic::defintegral("x^2", 0, 3);
```

### Evaluation

code:

```cpp
cymbolic::evaluate("x^2+2*x", 5);
```

written equivalent:

```math
x^2+2x\quad\text{at }x=5
```

other examples:

```cpp
cymbolic::evaluate("sin(x)", 5);
cymbolic::evaluate("sqrt(x)", 5);
cymbolic::evaluate("exp(x)", 5);
cymbolic::evaluate("x^2", 5, true);
```

```math
\sin(x)\quad\text{at }x=5
\qquad
\sqrt{x}\quad\text{at }x=5
\qquad
e^x\quad\text{at }x=5
\qquad
x^2\quad\text{at }x=5
```

### Limits

code:

```cpp
cymbolic::limit("1/x", 0);
```

written equivalent:

```math
\lim_{x\to0}\frac{1}{x}
```

one-sided limits:

```cpp
cymbolic::limitleft("1/x", 0);
cymbolic::limitright("1/x", 0);
```

```math
\lim_{x\to0^-}\frac{1}{x}
\qquad
\lim_{x\to0^+}\frac{1}{x}
```

the direction can also be specified directly:

```cpp
cymbolic::limit("1/x", 0, "left", true);
cymbolic::limit("1/x", 0, "right", true);
```

limits at infinity are supported:

```cpp
cymbolic::limit("1/x", "infinity", "right", true);
cymbolic::limit("1/x", "-infinity", "left", true);
```

```math
\lim_{x\to\infty}\frac{1}{x}
\qquad
\lim_{x\to-\infty}\frac{1}{x}
```

### Function Roots

code:

```cpp
auto roots = cymbolic::roots("x^2-4");
```

written equivalent:

```math
x^2-4=0
```

for example:

```cpp
auto roots = cymbolic::roots("x^2-4");

for (const auto& root : roots)
    std::cout << root << '\n';
```

other examples:

```cpp
cymbolic::roots("x^2+2*x-3");
cymbolic::roots("x^3-x");
cymbolic::roots("x^4-16");
```

```math
x^2+2x-3=0
\qquad
x^3-x=0
\qquad
x^4-16=0
```

Polynomial roots are solved symbolically when possible, with numerical methods available as a fallback.

Closed-form processing can be requested:

```cpp
cymbolic::roots("x^2-4", true);
```

## License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code or compiled binary form, for any purpose, commercial or non-commercial, and by any means.

For more information, see the [Unlicense](https://unlicense.org) website or the accompanying `LICENSE` file.
