#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL_image.h>

#define PI 3.1415926535
#define DOF 15
#define FADE 10
#define NO_PI_FOV 0.5
#define FOV NO_PI_FOV * PI
#define SHIFT FOV / 2
#define DOOR_NUM 2
#define MAP_W 64
#define MAP_H 64
#define TARGET_FPS 60.0
#define MAXLENGTH 6
#define COMMENTMAXLENGTH 80
#define LINES 5

#define top_l(r, x, y) SDL_RenderDrawLine(r, x, y + 2, x+ 8, y + 2)
#define bottom_l(r, x, y) SDL_RenderDrawLine(r, x, y + 18, x_offset + 8, y + 18)
#define middle_l(r, x, y) SDL_RenderDrawLine(r, x, y + 9, x + 8, y + 9)
#define left_l(r, x, y) SDL_RenderDrawLine(r, x, y + 2, x, y + 18)
#define right_l(r, x, y) SDL_RenderDrawLine(r, x + 8, y + 2, x + 8, y + 18)
#define left_top_l(r, x, y) SDL_RenderDrawLine(r, x, y + 2, x, y + 9)
#define right_bottom_l(r, x, y) SDL_RenderDrawLine(r, x + 8, y + 9, x + 8, y + 18)
#define left_bottom_l(r, x, y) SDL_RenderDrawLine(r, x, y + 9, x, y + 18)
#define right_top_l(r, x, y) SDL_RenderDrawLine(r, x + 8, y + 2, x + 8, y + 9)
#define top_l_bot_r(r, x, y) SDL_RenderDrawLine(r, x, y + 2, x + 8, y + 18)
#define top_r_bot_l(r, x, y) SDL_RenderDrawLine(r, x, y + 18, x + 8, y + 2)
#define y_left_top(r, x, y) SDL_RenderDrawLine(r, x + 4, y + 9, x, y + 2)

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

float rad_ch(float a) // stops the a from going over 2 * PI (radians)
{
    if (a > 2 * PI) {
        a -= 2 * PI;
    }
    else {
        if (a < 0) {
            a += 2 * PI;
        }
    }
    return a;
}

void move_f( short int map_arr[][MAP_W], float location[], float direction, float rot, float mod, bool noclip, int door_location[][2], float door_extencion[], int door_num)
{
    float angle = -direction;                            //this function is responsible for moving the player
    angle = rad_ch(angle + rot);
    float x = location[0] + 0.125 * cos(angle) * mod;
    float y = location [1] + 0.125 * sin(angle) * mod;
    int i, door_ext_x, door_ext_y;
    door_ext_x = door_ext_x = 0;
    for (i=0; i<door_num; i++) { // get the door extencion, to let the player throught if it is open
        if (door_location[i][1] == (int)round(y) && door_location[i][0] == (int)round(location[0])) {
            door_ext_y = (int)round(door_extencion[i]);
        }
        if (door_location[i][1] == (int)round(location[1]) && door_location[i][0] == (int)round(x)) {
            door_ext_x = (int)round(door_extencion[i]);
        }
    }
    if (map_arr[(int)round(y)][(int)round(location[0])] == 0 || ((map_arr[(int)round(y)][(int)round(location[0])] == 2 || map_arr[(int)round(y)][(int)round(location[0])] == 3) && door_ext_y == 1)) {
        location[1] = y - 0.0625 * sin(angle) * mod;
    }
    else if (map_arr[(int)round(y)][(int)round(location[0])] != 2 && map_arr[(int)round(y)][(int)round(location[0])] != 3) {
        if (sin(angle) < 0) {
            location[1] = round(y) + 0.51;
        }
        else {
            location[1] = round(y) - 0.51;
        }
    }
    if (map_arr[(int)round(location[1])][(int)round(x)] == 0 || ((map_arr[(int)round(location[1])][(int)round(x)] == 2 || map_arr[(int)round(location[1])][(int)round(x)] == 3) && door_ext_x == 1)) {
        location[0] = x - 0.0625 * cos(angle) * mod;
    }
    else if (map_arr[(int)round(location[1])][(int)round(x)] != 2 && map_arr[(int)round(location[1])][(int)round(x)] != 3) {
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
    switch (number) { //outputs a number on the coordinates, looks like the digital clock
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
    short int map_arr[MAP_H][MAP_W] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1},
                                        {1, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1},
                                        {1, 0, 1, 0, 1, 2, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
                                        {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 7, 6, 1, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1, 4, 1, 0, 8, 8, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
    float direction = 1.6 * PI; //direction the player is facing
    int color, start, end, i, j, k, wall_hit;
    bool show_map, noclip, show_fps, quit, try_door;
    show_map = noclip = show_fps = quit = try_door = false;
    FILE *fptr;
    char contents[COMMENTMAXLENGTH];
    int config[LINES];
    fptr = fopen("conf", "r");
    j = 1;
    int length = 600;
    int hight = 600;
    float scale = 2;
    float floor_scale = 2;
    int map_scale = 6;
    if (fptr != NULL) {
        for (i=0;i<LINES && j>0;i++) {
            fgets(contents, COMMENTMAXLENGTH, fptr);
            config[i] = 0;
            for (j=0; j<MAXLENGTH-1 && contents[j] != EOF && contents[j] >= '0' && contents[j] <= '9'; j++);
            for (k=0; k<j; k++)
                config[i] += (contents[k] - '0') * pow(10, j - k - 1);
        }
        if (i==LINES && config[0] > 0 && config[1] > 0 && config[2] > 0 && config[3] > 0 && config[4] > 0) {
            length = config[0];
            hight = config[1];
            scale = config[2];
            floor_scale = config[3];
            map_scale = config[4];
        }
    }
    float half_map_scale = map_scale / 2;
    float mod = 1.0; // variable for speeding up the player movement if shift is pressed
    float angle, move_direction_h, move_direction_v;
    float hit_ang[length + 3];
    float hit_len[length + 3];
    bool KEYS[322]; //keys that are currently pressed
    bool OLD_KEYS[322]; //keys that were pressed on the previous frame
    unsigned int ticks, old_ticks, frame_tick;
    int delta = 0; // delta time
    ticks = 1000;
    old_ticks = 16.6;
    frame_tick = 0;
    int frames = 0; // num of frames for the fps
    int fps[3]; // fps as an array of numbers, used to output the fps with print_num()
    int door_num = 0; // number of doors
    int sprite_num = 0;

    for (i=0; i<MAP_H; i++) {
        for (j=0; j<MAP_W; j++) {
            if (map_arr[i][j] == 2 || map_arr[i][j] == 3) door_num++; // 2 - horizontal door, 3 - vertical door
            else if (map_arr[i][j] == 4) sprite_num++;
        }
    }

    for (i=0;i<322;i++) { KEYS[i] = false; }

    int door_location[door_num][2];
    int door_wait[door_num];
    float door_extencion[door_num];
    float door_ext_rate[door_num];

    float location[2]; // player position

    float floor_x, floor_y;
    float vx,vy,xov,yov,rxv,ryv,rxh,ryh,xoh,yoh,disV,disH;
    int last_offset, last_side;
    last_offset = last_side = 0;
    int door_indexH, door_indexV;
    int mxv,myv,mpv,dofv,mxh,myh,mph,dofh,side;

    float sprite_location[sprite_num][2];
    float sprite_dist[sprite_num];
    float sprite_angle[sprite_num];
    int sprite_index[sprite_num];
    for (i=0; i<sprite_num; i++) {
        sprite_dist[i] = -1;
        sprite_index[i] = i;
    }

    door_num = sprite_num = 0;
    for (i=0; i<MAP_H; i++) {
        for (j=0; j<MAP_W; j++) {
            if (map_arr[i][j] == 2 || map_arr[i][j] == 3) { // 2 - horizontal door, 3 - vertical door
                door_location[door_num][0] = j;
                door_location[door_num][1] = i;
                door_extencion[door_num] = 0.0;
                door_ext_rate[door_num] = 0.0;
                door_wait[door_num] = 0;
                door_num++;
            }
            else if (map_arr[i][j] == 9) { //the player
                location[0] = j;
                location[1] = i;
                map_arr[i][j] = 0;
            }
            else if (map_arr[i][j] == 4) {
                sprite_location[sprite_num][0] = j;
                sprite_location[sprite_num][1] = i;
                map_arr[i][j] = 0;
                sprite_num++;
            }
        }
    }

    // init SDL
 
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Pseudo 3D",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, length * scale, hight, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;
    IMG_Init(IMG_INIT_PNG);

    SDL_Texture* buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, length * scale, length );
    // set the textures, if no texture with the name is found, set to missing.png
    SDL_Texture* wall_texture;
    if (fopen("wall.png", "r")!=NULL) { wall_texture = IMG_LoadTexture(renderer, "wall.png"); }
    else { wall_texture = IMG_LoadTexture(renderer, "missing.png"); }
    SDL_SetTextureBlendMode(wall_texture, SDL_BLENDMODE_BLEND);

    SDL_Texture* door_texture;
    if (fopen("door.png", "r")!=NULL) { door_texture = IMG_LoadTexture(renderer, "door.png"); }
    else { door_texture = IMG_LoadTexture(renderer, "missing.png"); }
    SDL_SetTextureBlendMode(door_texture, SDL_BLENDMODE_BLEND);

    SDL_Texture* sprite_texture;
    if (fopen("sprite.png", "r")!=NULL) { sprite_texture = IMG_LoadTexture(renderer, "sprite.png"); }
    else { sprite_texture = IMG_LoadTexture(renderer, "missing.png"); }
    SDL_SetTextureBlendMode(sprite_texture, SDL_BLENDMODE_BLEND);

    SDL_Texture* floor_texture;
    if (fopen("floor.png", "r")!=NULL) { floor_texture = IMG_LoadTexture(renderer, "floor.png"); }
    else { floor_texture = IMG_LoadTexture(renderer, "missing.png"); }
    SDL_SetTextureBlendMode(floor_texture, SDL_BLENDMODE_BLEND);

    SDL_Texture* six_texture;
    if (fopen("texture6.png", "r")!=NULL) { six_texture = IMG_LoadTexture(renderer, "texture6.png"); }
    else { six_texture = IMG_LoadTexture(renderer, "missing.png"); }
    SDL_SetTextureBlendMode(six_texture, SDL_BLENDMODE_BLEND);

    SDL_Texture* seven_texture;
    if (fopen("texture7.png", "r")!=NULL) { seven_texture = IMG_LoadTexture(renderer, "texture7.png"); }
    else { seven_texture = IMG_LoadTexture(renderer, "missing.png"); }
    SDL_SetTextureBlendMode(seven_texture, SDL_BLENDMODE_BLEND);

    SDL_Texture* eight_texture;
    if (fopen("texture8.png", "r")!=NULL) { eight_texture = IMG_LoadTexture(renderer, "texture8.png"); }
    else { eight_texture = IMG_LoadTexture(renderer, "missing.png"); }
    SDL_SetTextureBlendMode(eight_texture, SDL_BLENDMODE_BLEND);
 
    const double base_angles = rad_ch(0.5 * PI - SHIFT);
    const double side_len = length * sin(base_angles) / sin(FOV);
    const float floor_ray_temp = sin(base_angles) * length / 4 / sin(SHIFT) * scale; // sin(base_angles) * length / 2 / sin(SHIFT) is the distance to trhe screen
    const float target_fps = 1000.0 / TARGET_FPS;
    const double base_angles_cos = cos(base_angles);
    const double side_len_squared = side_len * side_len;
    const double denom_temp = 2 * side_len * base_angles_cos;
    const short int delta_fade = DOF - FADE;
    const float color_intercept = 255 * (FADE / (delta_fade) + 1); // make the tiles fade between FADE and DOF, with DOF being transparent
    const float ratio = length * scale / hight / NO_PI_FOV / 2;
    const float half_h = hight / 2;
    // the main loop
    while (!quit) {
        SDL_SetRenderTarget(renderer, buffer);
        delta = SDL_GetTicks() - old_ticks; // time for the last frame in ms.
        if (delta < target_fps) {
            SDL_Delay((int)(target_fps) - delta); // cap the fps
        }
        ticks = SDL_GetTicks();
        if (ticks >= frame_tick + 1000) { // get the fps
            fps[0] = fps[1] = fps[2] = 0;
            frame_tick = ticks;
            if (frames > 999) {
                frames = 999;
            }
            fps[2] = frames % 10;
            fps[1] = frames % 100 / 10;
            fps[0] = frames / 100;
            frames = 0;
        }
        for (i=0;i<322;i++) { OLD_KEYS[i] = KEYS[i]; }
        frames++;
        while (SDL_PollEvent(&event)) { // handle events
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
        // handle user inputs
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
        if (KEYS[SDL_SCANCODE_Q] || KEYS[SDL_SCANCODE_LEFT]) {
            direction = rad_ch(direction + 0.01 * PI * mod);
        }
        if (KEYS[SDL_SCANCODE_E] || KEYS[SDL_SCANCODE_RIGHT]) {
            direction = rad_ch(direction - 0.01 * PI * mod);
        }
        if (!OLD_KEYS[SDL_SCANCODE_M] && KEYS[SDL_SCANCODE_M]) { // toggle, does not activate if the key is held down
            show_map -= 1;
        }
        if (!OLD_KEYS[SDL_SCANCODE_F] && KEYS[SDL_SCANCODE_F]) {
            show_fps -= 1;
        }
        if (!OLD_KEYS[SDL_SCANCODE_SPACE] && KEYS[SDL_SCANCODE_SPACE]) {
            try_door -= 1;
        }
        if (move_direction_h > 0) { // get the correct direction for movement
            if (move_direction_v >= 0) {
                move_direction_h = (move_direction_h + move_direction_v) / 2;
                if (move_direction_v == 0.0 && move_direction_h == 0.75) {
                    move_direction_h = 1.75;
                }
            }
            move_f(map_arr, location, direction, rad_ch(move_direction_h * PI), mod, false, door_location, door_extencion, door_num);
        }
        else {
            if (move_direction_v >=0) {
                move_f(map_arr, location, direction, move_direction_v * PI, mod, false, door_location, door_extencion, door_num);
            }
        }
 
        // clear window
        SDL_SetRenderDrawColor(renderer, 0, 240, 240, 255);
        SDL_RenderClear(renderer);
//        SDL_Rect r; // draw the ground
//        r.x = 0;
//        r.y = 0;
//        r.w = length * scale;                                   // less overdrawing
//        r.h = hight / 2 * (1 + 0.5 / (DOF - FADE + 8) / 2 * RATIO) + 2; // set the ground upper limit, to the lowwer limit of the fading tiles
//        SDL_RenderFillRect(renderer, &r);

        // toggles door if found in front
        if (try_door) {
            float dist = 0.1;
            for (i=0; i<10; i++) {
                int tile_x = (int)round(location[0] + dist * cos(direction));
                int tile_y = (int)round(location[1] - dist * sin(direction));
                short int tile = map_arr[tile_y][tile_x];
                if (tile == 2 || tile == 3) {
                    for (j=0;j<door_num;j++) {
                        if (door_location[j][0] == tile_x && door_location[j][1] == tile_y) {
                            if (door_ext_rate[j] == 0.0) {
                                door_ext_rate[j] = 0.01;
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

        // ray casting DDA
        float _angle = rad_ch(direction + SHIFT); // ray direction
        float px = location[0] + 0.5;
        float py = location[1] + 0.5;
        door_indexH = door_indexV = 0;
        int offset = 0;
        int last_symbolH = 0;
        for (i = 0; i < length + 3; i++) {
            angle = rad_ch(_angle - acos((side_len - (i - 3) * base_angles_cos) / sqrt(side_len_squared + (i - 3) * (i - 3 - denom_temp))));
            bool is_doorV = false;
            bool is_doorH = false;
            int symbolV = 9; // tile type of the tile that was hit by the ray from the map_arr
            int symbolH = 9;
            float Cos = cos(angle);
            float Sin = sin(angle);
            // Vertical
            dofv=0; side=1; disV=DOF; // side 1 - vertical, 0 - horizontal
            float TanV=tan(angle);
            if(Cos> 0.001){ rxv=(int)px+1;      ryv=(px-rxv)*TanV+py; xov= 1; yov=-xov*TanV;}//looking left
            else if(Cos<-0.001){ rxv=(int)px -0.00001; ryv=(px-rxv)*TanV+py; xov=-1; yov=-xov*TanV;}//looking right
            else { rxv=px; ryv=py; dofv=DOF;}                                                  //looking up or down. no hit

            // Horizontal
            dofh=0; disH=DOF;
            float TanH=1.0/TanV;
            if(Sin> 0.001){ ryh=(int)py -0.00001; rxh=(py-ryh)*TanH+px; yoh=-1; xoh=-yoh*TanH;}//looking up
            else if(Sin<-0.001){ ryh=(int)py+1;      rxh=(py-ryh)*TanH+px; yoh= 1; xoh=-yoh*TanH;}//looking down
            else{ rxh=px; ryh=py; dofh=DOF;}                                                   //looking straight left or right

            // DDA
            while(dofh<DOF || dofv<DOF)
            {
                if (dofh < dofv && dofh < DOF) { // use the shortest ray
                    mxh=(int)(rxh); myh=(int)(ryh); mph = myh * MAP_W + mxh;
                    if (mph>-1 && mph<MAP_H*MAP_W && map_arr[myh][mxh] == 2) {
                        for(k=0; k<door_num; k++) { // get the door index
                            if (door_location[k][0] == mxh && door_location[k][1] == myh) {
                                door_indexH = k;
                                break;
                            }
                        }
                    }
                    // let the ray pass through the door, if the x mosition of the ray is more than x position of the doors end
                    if(mph>-1 && mph<MAP_H*MAP_W && ((map_arr[myh][mxh]!=0 && map_arr[myh][mxh]!=2 && map_arr[myh][mxh]!=3) || (map_arr[myh][mxh] == 2 && (door_location[door_indexH][0] + door_extencion[door_indexH] < rxh + 0.5 * xoh)))){ //hit
                        dofh=DOF; disH=Cos*(rxh-px)-Sin*(ryh-py);
                        symbolH = map_arr[myh][mxh];
                        if (symbolH == 2) { // make the door look embedded in the wall
                            disH += Sin*(-0.5 * yoh) + Cos*(0.5 * xoh);
                            rxh += 0.5 * xoh;
                            ryh += 0.5 * yoh;
                            is_doorH = true;
                        }
                    }
                    else{ rxh+=xoh; ryh+=yoh; dofh+=1;} //check next horizontal
                }
                else if (dofv < DOF) {
                    mxv=(int)(rxv); myv=(int)(ryv); mpv = myv * MAP_W + mxv;
                    if (mpv>-1 && mpv<MAP_H*MAP_W && map_arr[myv][mxv] == 3) {
                        for(k=0; k<door_num; k++) { //get the door index
                            if (door_location[k][0] == mxv && door_location[k][1] == myv) {
                                door_indexV = k;
                                break;
                            }
                        }
                    }
                    // let the ray pass through the door, if the y mosition of the ray is more than y position of the doors end
                    if(mpv>-1 && mpv<MAP_H*MAP_W && ((map_arr[myv][mxv]!=0 && map_arr[myv][mxv]!=2 && map_arr[myv][mxv]!=3) || (map_arr[myv][mxv] == 3 && (door_location[door_indexV][1] + door_extencion[door_indexV] < ryv + 0.5 * yov)))){ //hit
                        dofv=DOF; disV=Cos*(rxv-px)-Sin*(ryv-py);
                        symbolV = map_arr[myv][mxv];
                        if (symbolV == 3) { // make the door look embedded in the wall
                            disV += Cos*(0.5 * xov) + Sin*(-0.5 * yov);
                            rxv += 0.5 * xov;
                            ryv += 0.5 * yov;
                            is_doorV = true;
                        }
                    }
                    else{ rxv+=xov; ryv+=yov; dofv+=1;} //check next horizontal
                }
            }

            if(disV<disH){ rxh=rxv; ryh=ryv; disH=disV; side=0; is_doorH = is_doorV; door_indexH = door_indexV; symbolH = symbolV; } // the horizontal is set to the shortest ray
            hit_ang[i] = rad_ch(2.0 * PI - angle);
            hit_len[i] = (disH);
            float fisheye_correction = cos(direction - angle);
            disH=disH * fisheye_correction ; //fix fisheye

            // vertical lines for the screen output
            start = 0;
            end = hight - 1;
            if(disH != 0 && disH == disH) {
                float delta_h = 0.5 / disH * ratio;
                start = half_h * (1 - delta_h);
                end = half_h * (1 + delta_h);
            }
            color = (int)round(color_intercept - 255.0 / delta_fade * hit_len[i]); // make the tiles fade between FADE and DOF, with DOF being transparent
            if (color < 0) {color = 0;}
            else if (color > 255) {color = 255;}
            SDL_Rect r; // the column on the screen
            r.x = (i - 3) * scale;
            r.y = start;
            r.w = scale;
            r.h = end - start;
            SDL_Rect texture_rect;
            if (!is_doorH) {
                if (symbolH == 1) {
                    SDL_SetTextureAlphaMod(wall_texture, color);
                }
                else if (symbolH == 6) {
                    SDL_SetTextureAlphaMod(six_texture, color);
                }
                else if (symbolH == 7) {
                    SDL_SetTextureAlphaMod(seven_texture, color);
                }
                else if (symbolH == 8) {
                    SDL_SetTextureAlphaMod(eight_texture, color);
                }
                // the texture part to be used for the column
                if (side == 1) {
                    if (Sin > 0) {
                        offset = (int)((rxh - (int)rxh) * 1024); // offset from the side * 1024 (1024 is the picture resolution)
                    }
                    else {
                        offset = (int)((1 - rxh + (int)rxh) * 1024);
                    }
                }
                else {
                    if (Cos > 0) {
                        offset = (int)((ryh - (int)ryh) * 1024);
                    }
                    else {
                        offset = (int)((1 - ryh + (int)ryh) * 1024);
                    }
                }
            }
            else {
                SDL_SetTextureAlphaMod(door_texture, color); // same, but for doors
                if (side == 1) { // offset from the doors end (the "moving" part)
                    offset = (int)((1 - rxh + (int)rxh + door_extencion[door_indexH]) * 1024);
                }
                else {
                    offset = (int)((1 - ryh + (int)ryh + door_extencion[door_indexH]) * 1024);
                }
            }
            texture_rect.x = offset;
            texture_rect.y = 0;
            texture_rect.h = 1024;
            if (!is_doorH) {
                if (offset - last_offset + 1 > 0 || last_offset == 0) {
                    if (last_offset == 0) {
                        texture_rect.w = 1;
                    }
                    else {
                        texture_rect.w = offset - last_offset + 1;
                    }
                }
                else if (last_symbolH != 2 && last_symbolH != 3) {
                        texture_rect.w = 1025 + offset - last_offset;
                        offset = 0;
                }
            }
            else
                texture_rect.w = 1;
            if (texture_rect.x < 0 || texture_rect.x > 1023) {
                texture_rect.x = 0;
                texture_rect.w = 1;
            }
            if (i > 2) {
                if (is_doorH) {
                    SDL_RenderCopy(renderer, door_texture, &texture_rect, &r);
                }
                else {
                    if (symbolH == 1) SDL_RenderCopy(renderer, wall_texture, &texture_rect, &r);
                    else if (symbolH == 6) SDL_RenderCopy(renderer, six_texture, &texture_rect, &r);
                    else if (symbolH == 7) SDL_RenderCopy(renderer, seven_texture, &texture_rect, &r);
                    else if (symbolH == 8) SDL_RenderCopy(renderer, eight_texture, &texture_rect, &r);
                }
            }
            last_side = side;
            last_offset = offset;
            last_symbolH = symbolH;
            r.h = floor_scale;
            r.w = scale;
            texture_rect.w = scale;
            texture_rect.h = floor_scale;
            for (j=end; j<hight; j += floor_scale) {
                float floor_ray = floor_ray_temp / (j - hight / 2) / fisheye_correction;
                floor_x = floor_ray * Cos + px;
                floor_y = - floor_ray * Sin + py;
//                if (floor_x > 0 && floor_x < MAP_W && floor_y > 0 && floor_y < MAP_H)  // check if tile is in the map
                color = (int)round(color_intercept - 255.0 / delta_fade * floor_ray); // make the tiles fade between FADE and DOF, with DOF being transparent
                if (color < 0) color = 0;
                else if (color > 255) color = 255;
                SDL_SetTextureAlphaMod(floor_texture, color);
                r.y = j;
                texture_rect.x = (int)((floor_x - (int)floor_x) * 1024);
                texture_rect.y = (int)((floor_y - (int)floor_y) * 1024);
                SDL_RenderCopy(renderer, floor_texture, &texture_rect, &r);
            }
        }

        for (i=0; i<door_num; i++) { // handle the door movement
            if (door_ext_rate[i] > 0) { // opens the doors
                door_extencion[i] += door_ext_rate[i];
                if (door_extencion[i] > 1) {
                    door_ext_rate[i] = -0.01;
                    door_extencion[i] = 1.0;
                    door_wait[i] = 300; // wait 300 frames
                }
            }
            else if (door_wait[i] > 0) door_wait[i] -= 1; //wait
            else {
                if (round(location[0]) != door_location[i][0] || round(location[1]) != door_location[i][1]) {
                    door_extencion[i] += door_ext_rate[i]; // closes the doors
                    if (door_extencion[i] < 0) {
                        door_ext_rate[i] = 0.0;
                        door_extencion[i] = 0.0;
                    }
                }
            }
        }

        float tmp_x, tmp_y;

        for (i=0; i<sprite_num; i++) {
            tmp_x = sprite_location[i][0] - location[0];
            tmp_y = - sprite_location[i][1] + location[1];
            sprite_dist[i] = sqrt(tmp_x * tmp_x + tmp_y * tmp_y);
            if (tmp_x == 0)
                tmp_x = 0.00001;
            if (tmp_y == 0)
                tmp_y = - 0.00001;
            sprite_angle[i] = rad_ch(atan(tmp_y / tmp_x));
            if (tmp_x < 0)
                sprite_angle[i] = rad_ch(sprite_angle[i] + PI);
        }

        bool swapped;
        for (i=0; i<sprite_num - 1; i++) {
            swapped = false;
            for (j=0; j< sprite_num - i - 1; j++) {
                if (sprite_dist[sprite_index[j]] < sprite_dist[sprite_index[j + 1]]) {
                    swap(&sprite_index[j], &sprite_index[j+1]);
                    swapped = true;
                }
            }
            if (swapped == false)
                break;
        }

        for (i=0; i<sprite_num; i++) {
            bool rad_skip = false;
            float border_one = direction + SHIFT;
            if (border_one < 0 || border_one > 2 * PI) {
                border_one = rad_ch(border_one);
                rad_skip = true;
            }

            float border_two = direction - SHIFT;
            if (border_two < 0 || border_two > 2 * PI) {
                border_two = rad_ch(border_two);
                rad_skip = true;
            }

            int h_position = round((0.5 -tan(rad_ch(sprite_angle[sprite_index[i]] - direction)) / tan(FOV / 2.0) / 2.0) * length * scale+ 0.001);
            float tmp_dist = sprite_dist[sprite_index[i]] * cos(rad_ch(direction - sprite_angle[sprite_index[i]]));
            int dimention = hight / 2 * (1 / tmp_dist) * ratio;
            if (sprite_dist[sprite_index[i]] < DOF && (((rad_skip && (sprite_angle[sprite_index[i]] < border_one || sprite_angle[sprite_index[i]] > border_two)) || (!rad_skip && sprite_angle[sprite_index[i]] < border_one && sprite_angle[sprite_index[i]] > border_two)) || (h_position - dimention / 2 < length * scale&& h_position + dimention / 2 > 0))) {
                color = (int)round(color_intercept - 255.0 / delta_fade * sprite_dist[sprite_index[i]]); // make the tiles fade between FADE and DOF, with DOF being transparent
                if (color < 0) color = 0;
                else if (color > 255) color = 255;
                SDL_SetTextureAlphaMod(sprite_texture, color);
                int slices = ceil(dimention / scale);
                int start_pos = h_position - dimention / 2;
                int start_width = ceil(start_pos / scale) - start_pos;
                SDL_Rect r;
                r.x = start_pos;
                r.y= hight / 2 * (1 - 0.5 / tmp_dist * ratio);
                r.w = start_width;
                r.h = dimention;
                SDL_Rect texture_rect;
                texture_rect.y = 0;
                texture_rect.h = 1024;
                texture_rect.x = 0;
                texture_rect.w = 1;
                SDL_RenderCopy(renderer, sprite_texture, &texture_rect, &r);
                float texture_width = 1024 / (dimention - ceil(start_pos / scale) * scale + start_pos) * scale;
                int column;
                //float texture_width = 1024 / (dimention - ceil(start_pos / scale) * scale + start_pos) * scale;
                r.w = scale;
                texture_rect.w = texture_width;
                for (j=0;j<=slices;j++) {
                    column = ceil(start_pos / scale) + j;
                    if (column < length && sprite_dist[sprite_index[i]] < hit_len[column + 3]) {
                        texture_rect.x = j * texture_width;
                        r.x = scale * ceil(start_pos / scale + j);
                        if (j < slices)
                            SDL_RenderCopy(renderer, sprite_texture, &texture_rect, &r);
                    }
                }
                if (column < length && sprite_dist[sprite_index[i]] < hit_len[column + 3]) {
                    r.w = h_position + dimention / 2 - scale * ceil(start_pos / scale + slices);
                    texture_rect.w = 1024 - texture_rect.x;
                    SDL_RenderCopy(renderer, sprite_texture, &texture_rect, &r);
                }
            }
        }

        if (show_map) { // render the map
            SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 ); // a background
            SDL_Rect r;
            r.x = 0;
            r.y = 0;
            r.w = MAP_W * map_scale;
            r.h = MAP_H * map_scale;
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor( renderer, 0, 0, 242, 255 );
            for (i=0; i<MAP_H; i++) {
                for (j=0; j<MAP_W; j++) {
                    if (map_arr[i][j] != 0) {
                        r.x = j * map_scale;
                        r.y = i * map_scale;
                        r.w = map_scale;
                        r.h = map_scale;
                        SDL_RenderDrawRect( renderer, &r );
                    }
                }
            }
            SDL_SetRenderDrawColor( renderer, 0, 242, 0, 255 ); // the player rectangle
            r.x = round(location[0] * map_scale)+half_map_scale- (int)half_map_scale/4;
            r.y = round(location[1] * map_scale)+half_map_scale- (int)half_map_scale/4;
            r.w = (int)half_map_scale/2;
            r.h = (int)half_map_scale/2;
            SDL_RenderDrawRect( renderer, &r );
            for (i=2; i<length + 3; i++) { // shows the rays on the map
                SDL_RenderDrawLine(renderer, round(location[0]*map_scale+ half_map_scale), round(location[1]*map_scale+ half_map_scale), ceil((location[0] + hit_len[i] * cos(hit_ang[i])) * map_scale+half_map_scale), ceil((location[1] + hit_len[i] * sin(hit_ang[i])) * map_scale+half_map_scale));
            }
        }
        if (show_fps) { // show fps and player position
            int x_offset = MAP_W * map_scale+ 10;
            SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 ); // a background
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
            print_number(fps[1], x_offset + 12, 0, renderer); // fps
            print_number(fps[2], x_offset + 24, 0, renderer);
            print_number((int)round(location[0]) / 10, x_offset, 25, renderer); // x position
            print_number((int)round(location[0]) % 10, x_offset + 12, 25, renderer);
            top_l_bot_r(renderer, x_offset + 24, 25);
            top_r_bot_l(renderer, x_offset + 24, 25);
            print_number((int)round(location[1]) / 10, x_offset, 50, renderer); // y position
            print_number((int)round(location[1]) % 10, x_offset + 12, 50, renderer);
            top_r_bot_l(renderer, x_offset + 24, 50);
            y_left_top(renderer, x_offset + 24, 50);
        }

        // render window
        SDL_RenderPresent(renderer);
        old_ticks = ticks;
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, buffer, NULL, NULL);
    }
 
    // cleanup SDL
    SDL_DestroyTexture(wall_texture);
    wall_texture = NULL;
    SDL_DestroyTexture(door_texture);
    door_texture = NULL;
    SDL_DestroyTexture(sprite_texture);
    sprite_texture = NULL;
    SDL_DestroyTexture(floor_texture);
    floor_texture = NULL;
    SDL_DestroyTexture(six_texture);
    six_texture = NULL;
    SDL_DestroyTexture(seven_texture);
    seven_texture = NULL;
    SDL_DestroyTexture(eight_texture);
    eight_texture = NULL;
    SDL_DestroyTexture(buffer);
    buffer = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
    return 0;
}
