#include <SDL2/SDL.h>
#include <stdio.h>

// Window width / height
#define WIDTH 1600
#define HEIGHT 900

// 240 FPS (to match my native refresh rate)
#define FPS 10000

// Color definitions
#define WHITE 0xffffffff
#define BLACK 0xff000000
#define RED   0xffff0000
#define GREEN 0xff00ff00
#define BLUE  0xff0000ff

// Number of usable colors in the colors array
#define COLOR_COUNT 4
// Set array of colors
Uint32 colors[COLOR_COUNT] = {BLACK, RED, GREEN, BLUE};

int current_color = 0;

struct Circle
{
    int x;
    int y;
    int r;
};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color)
{
    int r_squared = circle.r * circle.r;

    for (int y = circle.y - circle.r; y <= circle.y + circle.r; y++)
    {
        for (int x = circle.x - circle.r; x <= circle.x + circle.r; x++)
        {
            int dx = x - circle.x;
            int dy = y - circle.y;

            if (dx * dx + dy * dy <= r_squared)
            {
                if (x >= 0 && x < surface->w && y >= 0 && y < surface->h)
                {
                    SDL_Rect pixel = { x, y, 1, 1 };
                    SDL_FillRect(surface, &pixel, color);
                }
            }
        }
    }
}

// Circle outline function (not used currently)
void FillCircleOutline(SDL_Surface* surface, struct Circle circle, int border, Uint32 color)
{
    int r_outer_squared = circle.r * circle.r;
    int r_inner = circle.r - border;
    int r_inner_squared = r_inner * r_inner;

    for (int y = circle.y - circle.r; y <= circle.y + circle.r; y++)
    {
        for (int x = circle.x - circle.r; x <= circle.x + circle.r; x++)
        {
            int dx = x - circle.x;
            int dy = y - circle.y;
            int d2 = dx * dx + dy * dy;

            if (d2 <= r_outer_squared && d2 >= r_inner_squared)
            {
                if (x >= 0 && x < surface->w && y >= 0 && y < surface->h)
                {
                    SDL_Rect pixel = { x, y, 1, 1 };
                    SDL_FillRect(surface, &pixel, color);
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
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Hide cursor
    SDL_ShowCursor(SDL_DISABLE);

    // Create window, background surface, and canvas surface
    SDL_Window* window = SDL_CreateWindow("SDL Paint", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_Surface* canvas = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    // Fill canvas background
    SDL_FillRect(canvas, NULL, WHITE);

    int running = 1;
    int painting = 0;

    int brush_size = 10;
    int last_x = 0;
    int last_y = 0;

    struct Circle brush = { WIDTH / 2, HEIGHT / 2, brush_size };

    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            // Handle quitting (User closes window)
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            // Key input
            else if (event.type == SDL_KEYDOWN)
            {
                // Escape: Quit
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }
            }
            // Mouse down
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                // Right click: change color
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    increment_color();
                }
                 // Left click: paint
                else if (event.button.button == SDL_BUTTON_LEFT)
                {
                    painting = 1;
                    brush.x = event.button.x;
                    brush.y = event.button.y;
                    last_x = brush.x;
                    last_y = brush.y;
                    FillCircle(canvas, brush, colors[current_color]);
                }
            }
            // Mouse up
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                // Left click release: stop painting
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    painting = 0;
                }
            }
            // Mouse wheel: change brush size
            else if (event.type == SDL_MOUSEWHEEL)
            {
                if (event.wheel.y > 0)
                    brush_size++;
                else if (event.wheel.y < 0 && brush_size > 1)
                    brush_size--;

                brush.r = brush_size;
            }
            // Mouse movement handling (draw circles if painting)
            else if (event.type == SDL_MOUSEMOTION)
            {
                brush.x = event.motion.x;
                brush.y = event.motion.y;
                if (painting)
                {
                    int dx = brush.x - last_x;
                    int dy = brush.y - last_y;
                    // Fill in the the biggest change (x or y) worth of steps with circles to smooth the painting
                    int steps = abs(dx);
                    if (abs(dy) > steps)
                    {
                        steps = abs(dy);
                    }
                    // Fill each step between last mouse pos and current mouse pos for smooth painting
                    for (int i = 1; i <= steps; i++)
                    {
                        struct Circle c = {last_x + dx * i / steps, last_y + dy * i / steps, brush.r};
                        FillCircle(canvas, c, colors[current_color]);
                    }
                    last_x = brush.x;
                    last_y = brush.y;
                }
            }
        }
        // Re-draw canvas to surface
        SDL_BlitSurface(canvas, NULL, surface, NULL);
        // Draw brush
        FillCircle(surface, brush, colors[current_color]);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(1000/FPS);
    }
    SDL_FreeSurface(canvas);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
