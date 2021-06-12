#ifndef DOUADY_GENERATOR_H
#define DOUADY_GENERATOR_H

#include <stdbool.h>


typedef struct iter_buffer {
    unsigned **buffer;
    unsigned x;
    unsigned y;
} iter_buffer_t;

typedef struct pixel {
    unsigned x;
    unsigned y;
} pixel_t;

typedef struct bounds {
    pixel_t ul;
    pixel_t lr;
} bounds_t;

typedef struct view {
    long double real;
    long double imag;
} view_t;

bool generate(iter_buffer_t it_buffer, view_t view, int zoom, int resolution, int thread_count);

typedef struct build_opts {
    iter_buffer_t it_buffer;
    view_t view;
    bounds_t *bounds;
    int resolution;
    int zoom;
} build_opts_t;

int generate_bounds(void *opts);
long double scale(long double start, long double end, unsigned size, unsigned position);

#endif //DOUADY_GENERATOR_H
