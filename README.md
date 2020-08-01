# cppsixel

# Introduction
A library of sixel encording for C/C++.  
Sixel is a bitmap format for DEC's terminals (refer to [All Abount SIXELs](https://www.digiater.nl/openvms/decus/vax90b1/krypton-nasa/all-about-sixels.text)).

# Usage
Put CIXEL_IMPLEMENTATION to create implementation.

``` cpp
#define CIXEL_IMPLEMENTATION
#include "cixel.h"
```


# License
This software is distributed under two licenses, choose whichever you like. See LICENSE in detail.

# Acknowledgment
## Third parties' libraries

Use libraries for tests and samples.

- [nothings/stb](https://github.com/nothings/stb/blob/master/stb_image.h)
  - stb_image.h
  - stb_image_write.h
- [sheredom/utest.h](https://github.com/sheredom/utest.h)

## Algorithms
I mainly got inspiration about color quantization from the next proceedings.

- Kanjanawanishkul, Kiattisin & Uyyanonvara, Bunyarit, "Novel fast color reduction algorithm for time-constrained applications", Journal of Visual Communication and Image Representation. 16. 311-332. 10.1016/j.jvcir.2004.07.002.
