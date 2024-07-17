#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"


const int N_POINTS = 9 * 9 * 9;
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {.x = 0, .y = 0, .z = 2};

bool is_running = false;

void setup(void)
{
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT);

    int point_count = 0;
    for (float x = -1; x <= 1; x += 0.25) {
        for (float y = -1; y <= 1; y += 0.25) {
            for (float z = -1; z <= 1; z += 0.25) {
                cube_points[point_count++] = (vec3_t) { .x = x, .y = y, .z = z};
            }
        }
    }
};

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
        break;
    }
};

vec2_t project(vec3_t p) {
    vec2_t projected_p = {
        .x = fov * p.x / p.z,
        .y = fov * p.y / p.z
    };
    return projected_p;
}

float rotation_angle = 0;
void update(void) {
    rotation_angle += 0.001;

    for (int i=0; i < N_POINTS; i++) {
        vec3_t p = cube_points[i];
        vec3_t transformed_p = vec3_rotate_y(p, rotation_angle);
        transformed_p = vec3_rotate_x(transformed_p, rotation_angle);
        transformed_p = vec3_rotate_z(transformed_p, rotation_angle);
        // Offset point z-component according to the camera position
        transformed_p.z -= camera_position.z;


        projected_points[i] = project(transformed_p);
    }
};

int i = 0;
void render(void)
{
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i=0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i];
        draw_rect(
            projected_point.x + WINDOW_WIDTH / 2,
            projected_point.y + WINDOW_HEIGHT / 2,
            4, 4,
            0x00FF00);
    }

    render_color_buffer();
    clear_color_buffer(0X000000);

    SDL_RenderPresent(renderer);
};

int main(int argc, char **argv)
{
    is_running = initialize_window();

    setup();

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();
    return 0;
};