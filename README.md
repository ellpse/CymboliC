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

Expressions use `x` as the variable and require explicit multiplication.

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

The supported constants are `pi` and `e`.

### Differentiation

CymboliC:

```cpp
cymbolic::differentiate("x^2");
```

Mathematics:

```math
\frac{d}{dx}(x^2)
```

More examples:

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

CymboliC:

```cpp
cymbolic::integrate("x^2");
```

Mathematics:

```math
\int x^2\,dx
```

More examples:

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

The result of an indefinite integral includes `+C`.

Closed-form output can be requested with `true`:

```cpp
cymbolic::integrate("x^2", true);
```

### Definite Integrals

CymboliC:

```cpp
cymbolic::integrate("x^2", 0, 3);
```

Mathematics:

```math
\int_0^3 x^2\,dx
```

For example:

```cpp
std::cout << cymbolic::integrate("x^2", 0, 3) << '\n';
```

returns:

```text
9
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

CymboliC:

```cpp
cymbolic::evaluate("x^2+2*x", 5);
```

Mathematics:

```math
x^2+2x\quad\text{at }x=5
```

Other examples:

```cpp
cymbolic::evaluate("sin(x)", 5);
cymbolic::evaluate("sqrt(x)", 5);
cymbolic::evaluate("exp(x)", 5);
```

### Limits

CymboliC:

```cpp
cymbolic::limit("1/x", 0);
```

Mathematics:

```math
\lim_{x\to0}\frac{1}{x}
```

One-sided limits:

```cpp
cymbolic::limitleft("1/x", 0);
cymbolic::limitright("1/x", 0);
```

```math
\lim_{x\to0^-}\frac{1}{x}
\qquad
\lim_{x\to0^+}\frac{1}{x}
```

Limits at infinity are supported:

```cpp
cymbolic::limit("1/x", "infinity", "right", true);
```

```math
\lim_{x\to\infty}\frac{1}{x}
```

### Function Roots

CymboliC:

```cpp
auto roots = cymbolic::roots("x^2-4");
```

Mathematics:

```math
x^2-4=0
```

For example:

```cpp
auto roots = cymbolic::roots("x^2-4");

for (const auto& root : roots)
    std::cout << root << '\n';
```

Polynomial roots are solved symbolically when possible, with numerical methods available as a fallback.

Closed-form processing can be requested:

```cpp
cymbolic::roots("x^2-4", true);
```

## License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, in source code or compiled binary form, for any purpose, commercial or non-commercial, and by any means.

For more information, see the [Unlicense](https://unlicense.org) website or the accompanying `LICENSE` file.
