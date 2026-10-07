# CymboliC

CymboliC was created because I didn't like the other C++ symbolic math libraries. I had previously written a calculus engine, and I built off of that.

## Features

* Differentiation
* Function evaluation
* Integration (fully analytic, definite or indefinite)
* Finding the roots of functions
* And more...

## Usage

First, include the main header, `CymboliC.h`, and the library is already set up. It uses the namespace `cymbolic`.

For integrals, here is the math syntax:

\[\int_{a}^{b} (3x^2 + 2x + 5) \ dx\]

It is equivalent to this C++ code:

```cpp
cymbolic::integrate("3*x^2+2*x+5", a, b); 

// For it to show the closed form:
cymbolic::integrate("3*x^2+2*x+5", a, b, true);
```

## License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.

For more information, please refer to the [Unlicense](https://unlicense.org) website or see the accompanying `LICENSE` file.
