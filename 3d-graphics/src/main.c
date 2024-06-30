#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
bool is_running = false;

uint32_t *color_buffer = NULL;

bool initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // Use SDL to retrieve the max w/h of the display
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    WINDOW_WIDTH = display_mode.w;
    WINDOW_HEIGHT = display_mode.h;

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

    // TODO: Create a SDL Renderer
    renderer = SDL_CreateRenderer(
        window,
        -1, 0);

    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

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

void update(void) {
    // TODO
};

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
            color_buffer[WINDOW_WIDTH * y + x] = color;
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
            color_buffer[WINDOW_WIDTH * y + x] = color;
        }
    }
}

int i = 0;
void render(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw_grid(10);
    draw_rect(100, 100, 800, 400, 0XFF00FF00);
    render_color_buffer();
    clear_color_buffer(0XFFFFFF00);

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