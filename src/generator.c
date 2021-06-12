#include <stdlib.h>
#include <stdbool.h>
#include <complex.h>
#include <math.h>
#include "generator.h"

static unsigned c_get_iterations(long double scaled_x, long double scaled_y, unsigned max_iters) {
    unsigned iters = 0;

    long double complex c = scaled_x + scaled_y * I;
    long double complex z = 0.0 + 0.0 * I;

    //try no square root after
    while ((creal(csqrt(z * z)) <= 2) && (iters < max_iters)) {
        z = (z * z) + c;
        ++iters;
    }

    return iters;
}

bool generate(unsigned **buffer, unsigned x, unsigned y, unsigned thread_count) {

}
