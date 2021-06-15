#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "render.h"


int render(void *data) {
    render_data *r_data = (render_data *) data;
    SDL_SetRenderDrawColor(r_data->renderer, 0xff, 0xff, 0xff, 0xff);

    for (unsigned x = 0; x < r_data->it_buf->x; ++x) {
        for (unsigned y = 0; y < r_data->it_buf->y; ++y) {
            if (r_data->it_buf->buffer[x][y] == 1000) {
                SDL_RenderDrawPoint(r_data->renderer, x, y);
            }
        }
    }

    SDL_RenderPresent(r_data->renderer);
}
