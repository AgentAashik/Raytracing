#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#define PI 3.141592653589793

#define WIDTH 1200
#define HEIGHT 680
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_SUN 0xffffaaff // Light yellow for the sun
#define COLOR_RAY 0xffd43bff // Color for rays
#define RAYS_NUMBER 360 // Number of rays to emit

struct Circle {
    double x;
    double y;
    double r;
};

// Function to draw a filled circle
void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color) {
    double radius_squared = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++) {
            double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distance_squared < radius_squared) {
                SDL_Rect pixel = { (int)x, (int)y, 1, 1 };
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

// Function to emit rays from the light source
void EmitRays(SDL_Surface* surface, struct Circle light, struct Circle obstacle) {
    for (int i = 0; i < RAYS_NUMBER; i++) {
        double angle = (2 * PI * i) / RAYS_NUMBER; // Angle in radians
        double dx = cos(angle);
        double dy = sin(angle);

        // Start from the light source
        double x = light.x;
        double y = light.y;

        // Cast the ray until it hits the screen boundary or the obstacle
        while (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            // Check if the ray hits the obstacle
            double distance_to_obstacle = sqrt(pow(x - obstacle.x, 2) + pow(y - obstacle.y, 2));
            if (distance_to_obstacle < obstacle.r) {
                break; // Stop the ray if it hits the obstacle
            }

            // Draw the ray
            SDL_Rect pixel = { (int)x, (int)y, 1, 1 };
            SDL_FillRect(surface, &pixel, COLOR_RAY);

            // Move the ray forward
            x += dx;
            y += dy;
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Raycasting Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    struct Circle sun = { 200, 200, 20 }; // Light source (sun)
    struct Circle obstacle = { 650, 500, 120 }; // Obstacle casting shadow
    SDL_Rect erase_rect = { 0, 0, WIDTH, HEIGHT };

    double obstacle_speed_y = 1;

    int simulation_running = 1;
    SDL_Event event;
    while (simulation_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                simulation_running = 0;
            }
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0) {
                sun.x = event.motion.x;
                sun.y = event.motion.y;
            }
        }

        // Clear the screen
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);

        // Emit rays from the light source
        EmitRays(surface, sun, obstacle);

        // Draw the obstacle
        FillCircle(surface, obstacle, COLOR_WHITE);

        // Draw the sun (light source)
        FillCircle(surface, sun, COLOR_SUN);

        // Update obstacle position and check boundaries
        obstacle.y += obstacle_speed_y;
        if (obstacle.y - obstacle.r < 0 || obstacle.y + obstacle.r > HEIGHT) {
            obstacle_speed_y = -obstacle_speed_y;
        }

        // Update the window
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}