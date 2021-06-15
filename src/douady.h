#ifndef DOUADY_DOUADY_H
#define DOUADY_DOUADY_H

#include <stdbool.h>

typedef struct options {
    unsigned x;
    unsigned y;
    long double view_x;
    long double view_y;
} options;

unsigned **alloc_buffer(unsigned x, unsigned y);
void free_buffer(unsigned **buffer, unsigned x);
bool output_buffer(unsigned **buf, unsigned x, unsigned y);
options parse_options(int argc, char **argv);
bool init_window_renderer(SDL_Window **window, SDL_Renderer **renderer, int x, int y);

#endif //DOUADY_DOUADY_H
