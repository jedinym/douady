#include <stdlib.h>
#include <stdbool.h>
#include <complex.h>
#include <stdio.h>
#include <threads.h>
#include <math.h>
#include <assert.h>
#include "generator.h"

#define MAX_ITERATIONS 1000

static unsigned get_iterations(long double scaled_x, long double scaled_y, unsigned max_iters) {
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


static void create_bound_list(bounds_t *bounds_arr, unsigned chunk_count, unsigned size) {
    unsigned x_step = size / chunk_count;
    unsigned rest = size % chunk_count;
    unsigned x_pos = 0;

    for (unsigned i = 0; i < chunk_count; ++i) {
        unsigned x_step_end = x_pos + x_step;
        if (i == chunk_count - 1) {
            x_step_end += rest;
        }
        bounds_t bounds = {{.x = x_pos, .y = 0}, {.x = x_step_end, .y = size}};
        //printf("[%d, %d] [%d, %d]\n", bounds.ul.x, bounds.ul.y, bounds.lr.x, bounds.lr.y);
        bounds_arr[i] = bounds;
        x_pos += x_step;
    }
}

static void create_opts(build_opts_t *buff,
                        double zoom,
                        int resolution,
                        iter_buffer_t it_buf,
                        bounds_t *bound_arr,
                        int bound_count,
                        view_t view) {
    for (int i = 0; i < bound_count; ++i) {
        buff[i].bounds = &bound_arr[i];
        buff[i].zoom = zoom;
        buff[i].resolution = resolution;
        buff[i].view = view;
        buff[i].it_buffer = it_buf;
    }
}

bool generate(iter_buffer_t it_buffer, view_t view, double zoom, int resolution, int thread_count) {
    thrd_t threads[thread_count];
    build_opts_t opts[thread_count];
    bounds_t bound_arr[thread_count];
    create_bound_list(bound_arr, thread_count, it_buffer.x);
    create_opts(opts, zoom, resolution, it_buffer, bound_arr, thread_count, view);

    for (int i = 0; i < thread_count; ++i) {
        if (thrd_create(&threads[i], generate_bounds, &opts[i]) != thrd_success) {
            perror("thrd_create");
            return false;
        }
    }

    for (int i = 0; i < thread_count; ++i) {
        thrd_join(threads[i], NULL);
    }

    return true;
}

long double scale(long double start, long double end, unsigned size, unsigned position) {
    long double coeff = fabsl(end - start) / size;
    return start + position * coeff;
}

int generate_bounds(void *opts) {
    assert(opts != NULL);
    //puts("THREAD!");
    build_opts_t *b_opts = (build_opts_t*) opts;

    long double real_low = b_opts->view.real - 1.0 / b_opts->zoom;
    long double real_high = b_opts->view.real + 1.0 / b_opts->zoom;
    long double imag_low = b_opts->view.imag - 1.0 / b_opts->zoom;
    long double imag_high = b_opts->view.imag + 1.0 / b_opts->zoom;

    //calculate iterations for each pixel within bounds
    for (unsigned x = b_opts->bounds->ul.x; x < b_opts->bounds->lr.x; x += b_opts->resolution) {
        long double scaled_x = scale(real_low, real_high, b_opts->it_buffer.x, x);
        for (unsigned y = b_opts->bounds->ul.y; y < b_opts->bounds->lr.y; y += b_opts->resolution) {
            long double scaled_y = scale(imag_low, imag_high, b_opts->it_buffer.y, y);
            unsigned iterations = get_iterations(scaled_x, scaled_y, MAX_ITERATIONS);
            b_opts->it_buffer.buffer[x][y] = iterations;
        }
    }

    return 1;
}
