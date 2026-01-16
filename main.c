#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 1600
#define HEIGHT 900

#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0xff000000
#define COLOR_RED   0xffff0000
#define COLOR_GREEN 0xff00ff00
#define COLOR_BLUE  0xff0000ff

#define COLOR_COUNT 4

int current_color = 0;

struct Circle
{
    int x;
    int y;
    int r;
};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color)
{
    int r2 = circle.r * circle.r;

    for (int y = circle.y - circle.r; y <= circle.y + circle.r; y++)
    {
        for (int x = circle.x - circle.r; x <= circle.x + circle.r; x++)
        {
            int dx = x - circle.x;
            int dy = y - circle.y;

            if (dx*dx + dy*dy <= r2)
            {
                if ((unsigned)x < surface->w && (unsigned)y < surface->h)
                {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[y * surface->w + x] = color;
                }
            }
        }
    }
}

void FillCircleOutline(SDL_Surface* surface, struct Circle circle, int border, Uint32 color)
{
    int rOuter2 = circle.r * circle.r;
    int rInner = circle.r - border;
    int rInner2 = rInner * rInner;

    for (int y = circle.y - circle.r; y <= circle.y + circle.r; y++)
    {
        for (int x = circle.x - circle.r; x <= circle.x + circle.r; x++)
        {
            int dx = x - circle.x;
            int dy = y - circle.y;
            int d2 = dx*dx + dy*dy;

            if (d2 <= rOuter2 && d2 >= rInner2)
            {
                if ((unsigned)x < surface->w && (unsigned)y < surface->h)
                {
                    Uint32* pixels = (Uint32*)surface->pixels;
                    pixels[y * surface->w + x] = color;
                }
            }
        }
    }
}

void increment_color(void)
{
    current_color = (current_color + 1) % COLOR_COUNT;
}

int main(int argc, char** argv)
{
    Uint32 colors[COLOR_COUNT] = {
        COLOR_BLACK,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_BLUE
    };

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "SDL Paint",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    SDL_Surface* canvas = SDL_CreateRGBSurface(
        0, WIDTH, HEIGHT, 32,
        0x00ff0000,
        0x0000ff00,
        0x000000ff,
        0xff000000
    );

    SDL_FillRect(canvas, NULL, COLOR_WHITE);

    int running = 1;
    int painting = 0;

    int brush_size = 10;

    int last_x = 0;
    int last_y = 0;

    struct Circle brush = { WIDTH / 2, HEIGHT / 2, brush_size};

    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            if (event.type == SDL_KEYDOWN)
            {
                running = 0;
            }

            // Right click: change color
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
            {
                increment_color();
            }
            
            // Left click: start painting
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                painting = 1;

                brush.x = event.button.x;
                brush.y = event.button.y;

                last_x = brush.x;
                last_y = brush.y;

                FillCircle(canvas, brush, colors[current_color]);
            }

            // Scroll down: decrease brush size
            if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
            {
                brush_size++;
                brush.r = brush_size;
            }

            // Scroll up: increase brush size
            if (event.type == SDL_MOUSEWHEEL && event.wheel.y < 0)
            {
                if (brush_size > 0)
                {
                    brush_size--;
                    brush.r = brush_size;
                }
            }

            // Stop painting
            if (event.type == SDL_MOUSEBUTTONUP &&
                event.button.button == SDL_BUTTON_LEFT)
            {
                painting = 0;
            }

            // Mouse motion
            if (event.type == SDL_MOUSEMOTION)
            {
                brush.x = event.motion.x;
                brush.y = event.motion.y;

                if (painting)
                {
                    int x0 = last_x;
                    int y0 = last_y;
                    int x1 = brush.x;
                    int y1 = brush.y;
                    
                    int dx = x1 - x0;
                    int dy = y1 - y0;

                    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
                    if (steps == 0) steps = 1;

                    for (int i = 1; i <= steps; i++)
                    {
                        struct Circle c = {
                            x0 + dx * i / steps,
                            y0 + dy * i / steps,
                            brush.r
                        };
                        FillCircle(canvas, c, colors[current_color]);
                    }
                    last_x = x1;
                    last_y = y1;
                }
            }
        }
        SDL_BlitSurface(canvas, NULL, surface, NULL);
        FillCircle(surface, brush, colors[current_color]);
        SDL_UpdateWindowSurface(window);
    }
    SDL_FreeSurface(canvas);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
