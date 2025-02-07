#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

#define PI 3.1415926535
#define LENGTH 600 
#define H 600
#define SCALE 2.0
#define NO_PI_FOV 0.5
#define FOV NO_PI_FOV * PI
#define SHIFT FOV / 2
#define STEP FOV / LENGTH
#define RATIO LENGTH * SCALE / H / NO_PI_FOV / 2
#define DOOR_NUM 2
#define MAP_W 64
#define MAP_H 64
#define MAP_SCALE 6
#define HALF_MAP_SCALE 3
#define TARGET_FPS 80
#define top_l(r, x, y) SDL_RenderDrawLine(r, x, y + 2, x+ 8, y + 2)
#define bottom_l(r, x, y) SDL_RenderDrawLine(r, x, y + 18, x_offset + 8, y + 18)
#define middle_l(r, x, y) SDL_RenderDrawLine(r, x, y + 9, x + 8, y + 9)
#define left_l(r, x, y) SDL_RenderDrawLine(r, x, y + 2, x, y + 18)
#define right_l(r, x, y) SDL_RenderDrawLine(r, x + 8, y + 2, x + 8, y + 18)
#define left_top_l(r, x, y) SDL_RenderDrawLine(r, x, y + 2, x, y + 9)
#define right_bottom_l(r, x, y) SDL_RenderDrawLine(r, x + 8, y + 9, x + 8, y + 18)
#define left_bottom_l(r, x, y) SDL_RenderDrawLine(r, x, y + 9, x, y + 18)
#define right_top_l(r, x, y) SDL_RenderDrawLine(r, x + 8, y + 2, x + 8, y + 9)

float rad_ch(float a)
{
    if (a > 2 * PI) {
        a -= 2 * PI;
    }
    else {
        if (a < 2 * PI) {
            a += 2 * PI;
        }
    }
    return a;
}

void move_f( const short int map_arr[][MAP_W], float location[], float direction, float rot, float mod, bool noclip)
{
    float angle = -direction;
    angle = rad_ch(angle + rot);
    float x = location[0] + 0.125 * cos(angle) * mod;
    float y = location [1] + 0.125 * sin(angle) * mod;
    if (map_arr[(int)round(y)][(int)round(location[0])] == 0) {
        location[1] = y - 0.0625 * sin(angle) * mod;
    }
    else {
        if (sin(angle) < 0) {
            location[1] = round(y) + 0.51;
        }
        else {
            location[1] = round(y) - 0.51;
        }
    }
    if (map_arr[(int)round(location[1])][(int)round(x)] == 0) {
        location[0] = x - 0.0625 * cos(angle) * mod;
    }
    else {
        if (cos(angle) < 0) {
            location[0] = round(x) + 0.51;
        }
        else {
            location[0] = round(x) - 0.51;
        }
    }
}

void print_number(int number, int x_offset, int y_offset, SDL_Renderer* renderer)
{
    // _
    //| |
    // -
    //| |
    // -
    switch (number) {
        case 0:
            top_l(renderer, x_offset, y_offset);
            bottom_l(renderer, x_offset, y_offset);
            left_l(renderer, x_offset, y_offset);
            right_l(renderer, x_offset, y_offset);
            break;
        case 1:
            right_l(renderer, x_offset, y_offset);
            break;
        case 2:
            top_l(renderer, x_offset, y_offset);
            bottom_l(renderer, x_offset, y_offset);
            middle_l(renderer, x_offset, y_offset);
            left_bottom_l(renderer, x_offset, y_offset);
            right_top_l(renderer, x_offset, y_offset);
            break;
        case 3:
            top_l(renderer, x_offset, y_offset);
            bottom_l(renderer, x_offset, y_offset);
            middle_l(renderer, x_offset, y_offset);
            right_l(renderer, x_offset, y_offset);
            break;
        case 4:
            right_l(renderer, x_offset, y_offset);
            left_top_l(renderer, x_offset, y_offset);
            middle_l(renderer, x_offset, y_offset);
            break;
        case 5:
            top_l(renderer, x_offset, y_offset);
            left_top_l(renderer, x_offset, y_offset);
            middle_l(renderer, x_offset, y_offset);
            right_bottom_l(renderer, x_offset, y_offset);
            bottom_l(renderer, x_offset, y_offset);
            break;
        case 6:
            top_l(renderer, x_offset, y_offset);
            left_l(renderer, x_offset, y_offset);
            middle_l(renderer, x_offset, y_offset);
            bottom_l(renderer, x_offset, y_offset);
            right_bottom_l(renderer, x_offset, y_offset);
            break;
        case 7:
            top_l(renderer, x_offset, y_offset);
            right_l(renderer, x_offset, y_offset);
            break;
        case 8:
            top_l(renderer, x_offset, y_offset);
            right_l(renderer, x_offset, y_offset);
            left_l(renderer, x_offset, y_offset);
            middle_l(renderer, x_offset, y_offset);
            bottom_l(renderer, x_offset, y_offset);
            break;
        case 9:
            top_l(renderer, x_offset, y_offset);
            middle_l(renderer, x_offset, y_offset);
            bottom_l(renderer, x_offset, y_offset);
            right_l(renderer, x_offset, y_offset);
            left_top_l(renderer, x_offset, y_offset);
            break;
    }
}

int main(void)
{
    const short int map_arr[MAP_H][MAP_W] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
    int door_location[DOOR_NUM][2] = {{5, 10},
                                      {6, 9}};
    int door_type[DOOR_NUM] = {2, 3}; // 2 is - (horizontal), 3 is | (vertical)
    float door_extencion[DOOR_NUM] = {0.25, 0.25};
    float door_ext_rate[DOOR_NUM] = {0, 0};
    float location[2] = {1, 2};
    float direction = 1.5 * PI;
    bool show_map, noclip, show_fps, quit, try_door;
    float mod = 1.0;
    short int move_tic = 1;
    int color, start, end, i, j, k, wall_hit;
    float angle, move_direction_h, move_direction_v;
    float hit[LENGTH * 2] = {};
    bool KEYS[322];
    bool OLD_KEYS[322];
    unsigned int ticks, old_ticks;
    int delta = 0;
    ticks = 1000;
    old_ticks = 16.6;
    int frames = 0;
    unsigned int frame_tick = 0;
    int fps[3];
    show_map = noclip = show_fps = quit = try_door = false;
    for (i=0;i<322;i++) { KEYS[i] = false; }
 
    // init SDL
 
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Pseudo 3D",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LENGTH*SCALE, H, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;
    // handle events
 
    while (!quit) {
        delta = ticks - old_ticks;
        if (delta < 1000/TARGET_FPS) {
            SDL_Delay(1000/TARGET_FPS - delta);
        }
        ticks = SDL_GetTicks();
        if (ticks >= frame_tick + 1000) {
            fps[0] = fps[1] = fps[2] = 0;
            frame_tick = ticks;
            if (frames > 999) {
                frames = 999;
            }
            fps[2] = frames % 10;
            fps[1] = frames % 100 / 10;
            fps[0] = frames / 100;
//            printf("%d%d%d\n", fps[0], fps[1], fps[2]);
            frames = 0;
        }
        for (i=0;i<322;i++) { OLD_KEYS[i] = KEYS[i]; }
 
        frames++;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    KEYS[event.key.keysym.scancode] = true;
                    break;
                case SDL_KEYUP:
                    KEYS[event.key.keysym.scancode] = false;
                    break;
            }
        }
        if (KEYS[SDL_SCANCODE_LSHIFT]) {
            mod = 2.0;
        }
        else {
            mod = 1.0;
        }
        move_direction_h = -1;
        move_direction_v = -1;
        if (KEYS[SDL_SCANCODE_A] && !KEYS[SDL_SCANCODE_D]) {
            move_direction_h = 1.5;
        }
        else {
            if (KEYS[SDL_SCANCODE_D] && !KEYS[SDL_SCANCODE_A]) {
                move_direction_h = 0.5;
            }
        }
        if (KEYS[SDL_SCANCODE_W] && !KEYS[SDL_SCANCODE_S]) {
            move_direction_v = 0.0;
        }
        else {
            if (KEYS[SDL_SCANCODE_S] && !KEYS[SDL_SCANCODE_W]) {
                move_direction_v = 1.0;
            }
        }
        if (KEYS[SDL_SCANCODE_Q]) {
            direction = rad_ch(direction + 0.01 * PI * mod);
        }
        if (KEYS[SDL_SCANCODE_E]) {
            direction = rad_ch(direction - 0.01 * PI * mod);
        }
        if (!OLD_KEYS[SDL_SCANCODE_M] && KEYS[SDL_SCANCODE_M]) {
            show_map -= 1;
        }
        if (!OLD_KEYS[SDL_SCANCODE_F] && KEYS[SDL_SCANCODE_F]) {
            show_fps -= 1;
        }
        if (!OLD_KEYS[SDL_SCANCODE_O] && KEYS[SDL_SCANCODE_O]) {
            try_door -= 1;
        }
        if (move_direction_h > 0) {
            if (move_direction_v >= 0) {
                move_direction_h = (move_direction_h + move_direction_v) / 2;
                if (move_direction_v == 0.0 && move_direction_h == 0.75) {
                    move_direction_h = 1.75;
                }
            }
            move_f(map_arr, location, direction, rad_ch(move_direction_h * PI), mod, false);
        }
        else {
            if (move_direction_v >=0) {
                move_f(map_arr, location, direction, move_direction_v * PI, mod, false);
            }
        }
       // clear window
 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
 

        //toggle door
        if (try_door) {
            float dist = 0.1;
            for (i=0; i<10; i++) {
                int tile_x = (int)round(location[0] + dist * cos(direction));
                int tile_y = (int)round(location[1] - dist * sin(direction));
                short int tile = map_arr[tile_y][tile_x];
                if (tile == 2 || tile == 3) {
                    for (j=0;j<DOOR_NUM;j++) {
                        if (door_location[j][0] = tile_x && door_location[j][1] == tile_y) {
                            if (door_ext_rate[j] == 0) {
                                door_ext_rate[j] = -0.01;
                            }
                            break;
                        }
                    }
                    break;
                }
                else {
                    if (tile != 0) {
                        break;
                    }
                }
                dist += 0.1;
            }
            try_door = false;
        }

        // ray casting
        angle = rad_ch(direction + SHIFT);
        float px = location[0] + 0.5;
        float py = location[1] + 0.5;
        int h_position = round((0.5 -tan(rad_ch(angle - direction)) / tan(FOV / 2.0) / 2.0) * LENGTH * SCALE);
        int door_index;
        for (i = 0; i < LENGTH * 2; i+=2) {
            int mxv,myv,mpv,dofv,mxh,myh,mph,dofh,side; float vx,vy,xov,yov,rxv,ryv,rxh,ryh,xoh,yoh,disV,disH;
            float Cos = cos(angle);
            float Sin = sin(angle);
            //---Vertical---
            dofv=0; side=1; disV=30;
            float TanV=tan(angle);
            if(Cos> 0.001){ rxv=(int)px+1;      ryv=(px-rxv)*TanV+py; xov= 1; yov=-xov*TanV;}//looking left
            else if(Cos<-0.001){ rxv=(int)px -0.00001; ryv=(px-rxv)*TanV+py; xov=-1; yov=-xov*TanV;}//looking right
            else { rxv=px; ryv=py; dofv=30;}                                                  //looking up or down. no hit

            //---Horizontal---
            dofh=0; disH=30;
            float TanH=1.0/TanV;
            if(Sin> 0.001){ ryh=(int)py -0.00001; rxh=(py-ryh)*TanH+px; yoh=-1; xoh=-yoh*TanH;}//looking up
            else if(Sin<-0.001){ ryh=(int)py+1;      rxh=(py-ryh)*TanH+px; yoh= 1; xoh=-yoh*TanH;}//looking down
            else{ rxh=px; ryh=py; dofh=30;}                                                   //looking straight left or right

            while(dofh<30 || dofv<30)
            {
                if (dofh < dofv && dofh < 30) {
                    mxh=(int)(rxh); myh=(int)(ryh); mph = myh * MAP_W + mxh;
                    if (mph>-1 && mph<MAP_H*MAP_W && map_arr[myh][mxh] == 2) {
                        for(k=0; k<DOOR_NUM; k++) {
                            if (door_location[k][0] == mxh && door_location[k][1] == myh) {
                                door_index = k;
                                break;
                            }
                        }
//                        printf("%f %f\n", door_location[door_index][0] + 0.25, rxh + 0.5 * xoh);
                    }
                    if(mph>-1 && mph<MAP_H*MAP_W && (map_arr[myh][mxh]==1 || (map_arr[myh][mxh] == 2 && (door_location[door_index][0] + door_extencion[door_index] < rxh + 0.5 * xoh)))){ //hit
                        dofh=30; disH=Cos*(rxh-px)-Sin*(ryh-py);
                        if (map_arr[myh][mxh] == 2) {
                            disH += Sin*(-0.5 * yoh) + Cos*(0.5 * xoh);
                        }
                    }
                    else{ rxh+=xoh; ryh+=yoh; dofh+=1;}                                               //check next horizontal
                }
                else {
                    if (dofv < 30) {
                        mxv=(int)(rxv); myv=(int)(ryv); mpv = myv * MAP_W + mxv;
                        if (mpv>-1 && mpv<MAP_H*MAP_W && map_arr[myv][mxv] == 3) {
                            for(k=0; k<DOOR_NUM; k++) {
                                if (door_location[k][0] == mxv && door_location[k][1] == myv) {
                                    door_index = k;
                                    break;
                                }
                            }
                        }
//                        if (map_arr[myv][mxv] == 3) {
//                            for(k=0; k<DOOR_NUM; k++) {
//                                if (door_location[k][0] == mxv && door_location[k][1] == myv) {
//                                    door_index = k;
//                                    break;
//                                }
//                            }
//                        }
                        if(mpv>-1 && mpv<MAP_H*MAP_W && (map_arr[myv][mxv]==1 || (map_arr[myv][mxv] == 3 && (door_location[door_index][1] + door_extencion[door_index] < ryv + 0.5 * yov)))){ //hit
                            dofv=30; disV=Cos*(rxv-px)-Sin*(ryv-py);
                            if (map_arr[myv][mxv] == 3) {
                                disV += Cos*(0.5 * xov) + Sin*(-0.5 * yov);
                            }
                        }
                        else{ rxv+=xov; ryv+=yov; dofv+=1;}                                               //check next horizontal
                    }
                }
            }
//            printf("ang %f ryh %f rxh %f yoh %f xoh %f disH %f\n", angle / PI, ryh, rxh, yoh, xoh,  disH);
//            printf("ang %f ryv %f rxv %f yov %f xov %f disV %f\n", angle / PI, ryv, rxv, yov, xov, disV);

            if(disV<disH){ rxh=rxv; ryh=ryv; disH=disV; side=0; }                  //horizontal hit first

            hit[i] = rad_ch(2.0 * PI - angle);
            hit[i+1] = (disH);
            disH=disH*cos(direction - angle);                            //fix fisheye
            angle = rad_ch(angle - STEP);
                        // vertical lines for the screen output created
            start = 0;
            end = H - 1;
            if(disH != 0) {
                start = H / 2 * (1 - 0.5/disH * RATIO);
                end = H / 2 * (1 + 0.5/disH * RATIO);
            }
            if(start<0) {
                start = 0;
            }
            if(end>H) {
                end = H - 1;
            }
            color = round(240 -8 * hit[i+1]);
            if (color < 0) {color = 0;}
//            if (side == 1) { color += 10; }
            if (side == 1) {
                if (Sin > 0) { SDL_SetRenderDrawColor(renderer, 0, color, color, 255); }
                else { SDL_SetRenderDrawColor(renderer, 0, color, 0, 255); }
            }
            else {
                if (Cos > 0) { SDL_SetRenderDrawColor(renderer, color, 0, color, 255); }
                else { SDL_SetRenderDrawColor(renderer, color, color, 0, 255); }
            }
//            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            int next_h_position = round((0.5 -tan(rad_ch(angle - direction)) / tan(FOV / 2.0) / 2.0) * LENGTH * SCALE + 0.001);
//            if (h_position < 0) { h_position = LENGTH - h_position; }
//            printf("%d\n", h_position);
            SDL_RenderDrawLine(renderer, h_position, start, h_position, end);
            for (j=1; j<-h_position + next_h_position; j++) {
                SDL_RenderDrawLine(renderer, h_position + j, start, h_position + j, end);
            }
            h_position = next_h_position;
        }
//        break;

        if (show_map) {
            SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
            SDL_Rect r;
            r.x = 0;
            r.y = 0;
            r.w = MAP_W * MAP_SCALE;
            r.h = MAP_H * MAP_SCALE;
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor( renderer, 0, 0, 242, 255 );
            for (i=0; i<MAP_H; i++) {
                for (j=0; j<MAP_W; j++) {
                    if (map_arr[i][j] == 1) {
                        r.x = j * MAP_SCALE;
                        r.y = i * MAP_SCALE;
                        r.w = MAP_SCALE;
                        r.h = MAP_SCALE;
                        SDL_RenderDrawRect( renderer, &r );
                    }
                }
            }
            SDL_SetRenderDrawColor( renderer, 0, 242, 0, 255 );
            r.x = round(location[0] * MAP_SCALE)+HALF_MAP_SCALE - (int)HALF_MAP_SCALE/4;
            r.y = round(location[1] * MAP_SCALE)+HALF_MAP_SCALE - (int)HALF_MAP_SCALE/4;
            r.w = (int)HALF_MAP_SCALE/2;
            r.h = (int)HALF_MAP_SCALE/2;
            SDL_RenderDrawRect( renderer, &r );
            for (i=0; i<LENGTH * 2; i+=2) {
                SDL_RenderDrawLine(renderer, round(location[0]*MAP_SCALE + HALF_MAP_SCALE), round(location[1]*MAP_SCALE + HALF_MAP_SCALE), ceil((location[0] + hit[i+1] * cos(hit[i])) * MAP_SCALE+HALF_MAP_SCALE), ceil((location[1] + hit[i+1] * sin(hit[i])) * MAP_SCALE+HALF_MAP_SCALE));
            }
        }
        if (show_fps) {
            int x_offset = MAP_W * MAP_SCALE + 10;
            SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
            SDL_Rect r;
            r.x = x_offset;
            r.y = 0;
            r.w = 36;
            r.h = 70;
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor( renderer, 242, 242, 242, 255 );
            if (fps[0] > 0) {
                print_number(fps[0], x_offset, 0, renderer);
                x_offset += 12;
            }
            print_number(fps[1], x_offset + 12, 0, renderer);
            print_number(fps[2], x_offset + 24, 0, renderer);
            print_number((int)round(location[0]) / 10, x_offset, 25, renderer);
            print_number((int)round(location[0]) % 10, x_offset + 12, 25, renderer);
            print_number((int)round(location[1]) / 10, x_offset, 50, renderer);
            print_number((int)round(location[1]) % 10, x_offset + 12, 50, renderer);
        }
        // render window
 
        SDL_RenderPresent(renderer);
        old_ticks = ticks;
    }
 
    // cleanup SDL
 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
