#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"


triangle_t triangles_to_render[N_MESH_FACES];
vec3_t camera_position = {.x = 0, .y = 0, .z = -2};
Uint32 previous_frame_time = 0;

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
    // Naive limiter, keeps CPU busy.
    // while(!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));
    // Better release process and resume in `time_to_wait` ms
    const Uint32 time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }
    previous_frame_time = SDL_GetTicks();

    rotation_angle += 0.02;

    // Loop over all the mesh faces
    for (int i=0; i < N_MESH_FACES; i++) {
        face_t mesh_face = mesh_faces[i];
        vec3_t face_vertices[3];
        face_vertices[0] = mesh_vertices[mesh_face.a - 1];
        face_vertices[1] = mesh_vertices[mesh_face.b - 1];
        face_vertices[2] = mesh_vertices[mesh_face.c - 1];

        triangle_t projected_triangle;
        // Apply transformations to vertices
        for (int j = 0; j < 3; j ++) {
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate_y(transformed_vertex, rotation_angle);
            transformed_vertex = vec3_rotate_x(transformed_vertex, rotation_angle);
            transformed_vertex = vec3_rotate_z(transformed_vertex, rotation_angle);
            // Offset point z-component according to the camera position
            transformed_vertex.z -= camera_position.z;

            vec2_t projected_point = project(transformed_vertex);
            projected_point.x += projected_point.x + WINDOW_WIDTH / 2;
            projected_point.y += projected_point.y + WINDOW_HEIGHT / 2;
            projected_triangle.points[j] = projected_point;
            triangles_to_render[i] = projected_triangle;

        }
    }
};

int i = 0;
void render(void)
{
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);
    // Loop over all projected triangles and render them
    for (int i=0; i < N_MESH_FACES; i++) {
        triangle_t triangle = triangles_to_render[i];
        for (int j=0; j<3; j++) {
            draw_rect(
                triangle.points[j].x,
                triangle.points[j].y,
                8, 8,
                0xFFFF00
            );
        }
        draw_triangle(triangle, 0x00FF00);
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