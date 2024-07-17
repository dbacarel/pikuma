#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *color_buffer_texture;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern uint32_t *color_buffer;

extern const float fov;

// Functions
bool initialize_window(void);
void destroy_window(void);

void clear_color_buffer(uint32_t color);
void render_color_buffer(void);

void draw_grid(int n);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int originX, int originY, int w, int h, uint32_t color);

#endif