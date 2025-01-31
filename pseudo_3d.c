#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

#define PI 3.1415926535

void rad_ch(float *direction, float rot)
{
	*direction += rot;
	if(*direction>=2 * PI) {
		*direction -=  2 * PI;
	}
	else { if(*direction<0) {
		*direction += 2 * PI;
        }
    }
}

void move_f( const short int map_arr[][10], float *location, float direction, float rot, float mod, bool noclip)
{
    float angle = direction;                                                    
    rad_ch(&angle, rot);                                                        
    float x = location[0] + 0.125 * cos(angle) * mod;                           
    float y = location [1] + 0.125 * sin(angle) * mod;                          
    int round_x = round(x);                                                     
    int round_y = round(y);                                                     
    if (map_arr[round_y][round_x] == 0) {                                       
        location[0] = x;                                                        
        location[1] = y;
    }
}

int main(void)
{
    const short int map_arr[13][10] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
    const short int length = 100;
    const short int h = 500;
    const short int scale = 5;
    float location[2] = {2, 2};
    float direction = 1.5 * PI;
    bool show_map = false;
    bool noclip = false;
    float mod = 1;
    short int move_tic = 1;
    int color;
    bool quit = false;
    int start;
    int end;
    float dist;
    int j;
    int i;
    SDL_Event event;
 
    // init SDL
 
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("SDL2 line drawing",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, length*scale*2, h, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
 
    // handle events
 
    while (!quit) {
        SDL_Delay(10);
        SDL_PollEvent(&event);
 
        if (event.type == SDL_QUIT) {
            quit = true;
            break;
        }
//        switch (event.type)
//        {
//            case SDL_QUIT:
//                quit = true;
//                break;
            // TODO input handling code goes here
//        }
 
        rad_ch(&direction, -0.01 * PI);
        move_f(map_arr, &*location, direction, 0.5 * PI, mod, false);
        // clear window
 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
 
        // get hits
        float shift = -0.25 * PI;
        float step = 0.005 * PI;
        float angle = direction;
        rad_ch(&angle, shift);
        int k;
        int index[2] = {};
        float len_x;
        float hit[200] = {};
        int wall_hit;
        for (k = 0; k < 200; k+=2) {
            wall_hit = 0;
            len_x = 0;
            while (wall_hit == 0 && len_x < 30) {
                len_x += 0.01;
                index[1] = round(location[1] + len_x * sin(angle));
                index[0] = round(location[0] + len_x * cos(angle));
                if (map_arr[index[1]][index[0]] == 1) {wall_hit = 1;}
            }
            hit[k] = angle;
            hit[k+1] = (len_x);
            rad_ch(&angle, step);
            // lines
            start = 0;
            end = h - 1;
            dist = len_x;
            if(dist != 0) {
                start = h / 2 * (1 - 1/dist);
                end = h / 2 * (1 + 1/dist);
            }
            if(start<0) {
                start = 0;
            }
            if(end>h) {
                end = h - 1;
            }
            color = round(242 -8.066666 * dist);
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            j = 0;
            for (j=0; j<=scale*2; j++) {
                SDL_RenderDrawLine(renderer, k * scale + j, start, k * scale + j, end);
            }
        }

        SDL_SetRenderDrawColor( renderer, 0, 0, 242, 255 );
        for (i=0; i<13; i++) {
            for (j=0; j<10; j++) {
                if (map_arr[i][j] == 1) {
                    SDL_Rect r;
                    r.x = j * 10;
                    r.y = i * 10;
                    r.w = 10;
                    r.h = 10;
                    SDL_RenderDrawRect( renderer, &r );
                }
            }
        }
        SDL_SetRenderDrawColor( renderer, 0, 242, 0, 255 );
        SDL_Rect r;
        r.x = round(location[0] * 10)+4;
        r.y = round(location[1] * 10)+4;
        r.w = 3;
        r.h = 3;
        SDL_RenderDrawRect( renderer, &r );
        for (i=0; i<200; i+=2) {
            SDL_RenderDrawLine(renderer, round(location[0]*10 + 5), round(location[1]*10 + 5), ceil((location[0] + hit[i+1] * cos(hit[i])) * 10) + 5, ceil((location[1] + hit[i+1] * sin(hit[i])) * 10) + 5);
        }
        // render window
 
        SDL_RenderPresent(renderer);
    }
 
    // cleanup SDL
 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
