#include <stdio.h>
#include <threads.h>
#include <stdlib.h>

#include "douady.h"
#include "generator.h"

unsigned **global_buffer;

int main(int argc, char **argv) {
    options opts = {.x = 200, .y = 200, .interactive = false};
    global_buffer = alloc_buffer(opts.x, opts.y);
    if (global_buffer == NULL)
        return EXIT_FAILURE;

    generate(global_buffer, opts.x, opts.y);

    free_buffer(global_buffer, opts.x);
    return EXIT_SUCCESS;
}

unsigned **alloc_buffer(unsigned x, unsigned y) {
    unsigned  **iteration_buffer = malloc(x * sizeof(unsigned int*));
    if (iteration_buffer == NULL) {
        perror("malloc");
        return NULL;
    }

    for (unsigned i = 0; i < x; ++i) {
        iteration_buffer[i] = malloc(y * sizeof(unsigned int));
        if (iteration_buffer[i] == NULL) {
            perror("malloc");
            free_buffer(iteration_buffer, i - 1);
            free(iteration_buffer);
            return NULL;
        }
    }

    return iteration_buffer;
}

void free_buffer(unsigned **buffer, unsigned x) {
    for (unsigned i = 0; i < x; ++i) {
        free(buffer[i]);
    }
    free(buffer);
}
