#ifndef DOUADY_RENDER_H
#define DOUADY_RENDER_H

#include "generator.h"

typedef struct render_data {
    iter_buffer_t *it_buf;
    SDL_Renderer *renderer;
} render_data;

int render(void *data);

#endif //DOUADY_RENDER_H
