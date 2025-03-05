#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "triangle.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;

uint32_t *color_buffer = NULL;

const float fov = 100;

bool initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // Use SDL to retrieve the max w/h of the display
    // SDL_DisplayMode display_mode;
    // SDL_GetCurrentDisplayMode(0, &display_mode);
    // WINDOW_WIDTH = display_mode.w;
    // WINDOW_HEIGHT = display_mode.h;

    // Create a SDL Window
    window = SDL_CreateWindow(
        "HelloWindow",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);

    if (!window)
    {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1, 0);

    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void destroy_window(void)
{
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            draw_pixel(x, y, color);
        }
    }
}

void render_color_buffer(void)
{
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)WINDOW_WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_grid(int n)
{
    for (int y=0; y < WINDOW_HEIGHT; y++)
    {
        for (int x=0; x < WINDOW_WIDTH; x++)
        {
            if (y % n == 0 || x % n == 0 )
            color_buffer[WINDOW_WIDTH * y + x] = 0XFFFF0000;
        }
    }
}

void draw_rect(int originX, int originY, int w, int h, uint32_t color) {
    for (int y= originY; y < originY + h; y++)
    {
        for (int x=originX; x < originX + w; x++)
        {
            draw_pixel(x, y, color);
        }
    }
}

void draw_pixel(int x, int y, uint32_t color) {
    if (x >=0 && x < WINDOW_WIDTH && y >=0 && y < WINDOW_HEIGHT) {
        color_buffer[(WINDOW_HEIGHT  * y) +  x] = color;
    }
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

    // Find the increments for each dimensions
    float x_inc = delta_x / (float) side_length;
    float y_inc = delta_y / (float) side_length;

    float curr_x = x0;
    float curr_y = y0;

    for (int i=0; i <= side_length; i++, curr_x+=x_inc, curr_y+=y_inc) {
        draw_pixel(round(curr_x), round(curr_y), color);
    }
}

void draw_triangle(triangle_t triangle, uint32_t color) {
    vec2_t* points = triangle.points;
    draw_line(points[0].x, points[0].y, points[1].x, points[1].y, color);
    draw_line(points[1].x, points[1].y, points[2].x, points[2].y, color);
    draw_line(points[2].x, points[2].y, points[0].x, points[0].y, color);
}