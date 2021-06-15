#include <stdio.h>
#include <threads.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "douady.h"
#include "generator.h"
#include "render.h"



int main(int argc, char **argv) {
    options opts = {.x = 800, .y = 800};
    unsigned **global_buffer = alloc_buffer(opts.x, opts.y);
    if (global_buffer == NULL)
        return EXIT_FAILURE;

    iter_buffer_t it_buf = {.buffer = global_buffer, opts.x, opts.y};
    double zoom = 0.5f;
    int res = 1;
    view_t view = {.imag = 0.0, .real = 0.0};

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL init error %s", SDL_GetError());
        free_buffer(global_buffer, opts.x);
        return EXIT_FAILURE;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (!init_window_renderer(&window, &renderer, opts.x, opts.y)) {
        free_buffer(global_buffer, opts.x);
        fprintf(stderr, "Init window render error");
        return EXIT_FAILURE;
    }

    generate(it_buf, view, zoom, res, 32);

    render_data r_data = {.it_buf = &it_buf, .renderer = renderer};
    render(&r_data);
    SDL_Delay(5000);

    //output_buffer(global_buffer, opts.x, opts.y);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free_buffer(global_buffer, opts.x);
    return EXIT_SUCCESS;
}

bool init_window_renderer(SDL_Window **window, SDL_Renderer **renderer, int x, int y) {
    SDL_Window *tmp_window = SDL_CreateWindow("douady", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, x, y,
                                          SDL_WINDOW_SHOWN);
    if (tmp_window == NULL) {
        return false;
    }

    SDL_Renderer *tmp_renderer = SDL_CreateRenderer(tmp_window, 0, 0);
    if (tmp_renderer == NULL) {
        SDL_DestroyWindow(tmp_window);
        return false;
    }

    *window = tmp_window;
    *renderer = tmp_renderer;
    return true;
}

//TODO: FINISH
options parse_options(int argc, char **argv) {
    options opts;

    for (int argi = 1; argi < argc; ++argi) {
        if (argv[argi][0] != '-') {
            fprintf(stderr, "error: unknown option %s", argv[argi]);
            exit(EXIT_FAILURE);
        }

    }
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

bool output_buffer(unsigned **buf, unsigned x, unsigned y) {
    FILE *file = fopen("file", "w");
    for (unsigned i = 0; i < x; ++i) {
        for (unsigned j = 0; j < y; ++j) {
            fprintf(file, "%c ", (buf[i][j] == 1000) ? '@' : '.');
        }
        fprintf(file, "\n");
    }
    fclose(file);
    return true;
}
