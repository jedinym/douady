#ifndef DOUADY_DOUADY_H
#define DOUADY_DOUADY_H

#include <stdbool.h>

typedef struct options {
    unsigned x;
    unsigned y;
    bool interactive;
} options;

unsigned **alloc_buffer(unsigned x, unsigned y);
void free_buffer(unsigned **buffer, unsigned x);

#endif //DOUADY_DOUADY_H
