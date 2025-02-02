#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

#define PI 3.1415926535
#define LENGTH 320
#define H 500
#define SCALE 2
#define SHIFT -0.25 * PI
#define STEP 0.0015625 * PI
#define MAP_W 10
#define MAP_H 13
#define MAP_SCALE 20
#define HALF_MAP_SCALE 10
#define rad_ch(a) fmod(2 * PI + fmod(a, 2 * PI), 2 * PI)

void move_f( const short int map_arr[][MAP_W], float location[], float direction, float rot, float mod, bool noclip)
{
    float angle = -direction;
    angle = rad_ch(angle + rot);
    float x = location[0] + 0.125 * cos(angle) * mod;
    float y = location [1] + 0.125 * sin(angle) * mod;
    if (map_arr[(int)round(y)][(int)round(location[0])] == 0) {
        location[1] = y;
    }
    else {
        if (sin(angle) < 0) {
            location[1] = round(y) + 0.50001;
        }
        else {
            location[1] = round(y) - 0.50001;
        }
    }
    if (map_arr[(int)round(location[1])][(int)round(x)] == 0) {
        location[0] = x;
    }
    else {
        if (cos(angle) < 0) {
            location[0] = round(x) + 0.50001;
        }
        else {
            location[0] = round(x) - 0.50001;
        }
    }
}

int main(void)
{
    const short int map_arr[MAP_H][MAP_W] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
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
    float location[2] = {2, 2};
    float direction = 1.5 * PI;
    bool show_map, noclip, quit;
    float mod = 1.0;
    short int move_tic = 1;
    int color, start, end, i, j, wall_hit;
    float angle, move_direction_h, move_direction_v;
    float hit[LENGTH * 2] = {};
    bool KEYS[322];
    bool OLD_KEYS[322];
    show_map = noclip = quit = false;
    for (i=0;i<322;i++) { KEYS[i] = false; }
 
    // init SDL
 
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Pseudo 3D",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LENGTH*SCALE*2, H, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;
    // handle events
 
    while (!quit) {
        for (i=0;i<322;i++) { OLD_KEYS[i] = KEYS[i]; }
        SDL_Delay(10);
 
//        if (event.type == SDL_QUIT) {
//            quit = true;
//            break;
//        }
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
            move_direction_h = 0.5;
        }
        else {
            if (KEYS[SDL_SCANCODE_D] && !KEYS[SDL_SCANCODE_A]) {
                move_direction_h = 1.5;
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
            direction = rad_ch(direction - 0.01 * PI * mod);
        }
        if (KEYS[SDL_SCANCODE_E]) {
            direction = rad_ch(direction + 0.01 * PI * mod);
        }
        if (!OLD_KEYS[SDL_SCANCODE_M] && KEYS[SDL_SCANCODE_M]) {
            if (show_map) { show_map = false; }
            else { show_map = true; }
        }
        if (move_direction_h > 0) {
            if (move_direction_v >= 0) {
                move_direction_h = (move_direction_h + move_direction_v) / 2;
            }
            move_f(map_arr, location, direction, move_direction_h * PI, mod, false);
        }
        else {
            if (move_direction_v >=0) {
                move_f(map_arr, location, direction, move_direction_v * PI, mod, false);
            }
        }
       // clear window
 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
 
        // ray casting
        angle = rad_ch(direction + SHIFT);
        float px = location[0] + 0.5;
        float py = location[1] + 0.5;
        for (i = 0; i < LENGTH * 2; i+=2) {
            int r,mx,my,mp,dof,side; float vx,vy,rx,ry,xo,yo,disV,disH;
            dof=0; side=1; disV=100000;
            float Tan=tan(angle);
            if(cos(angle)> 0.001){ rx=(int)px+1;      ry=(px-rx)*Tan+py; xo= 1; yo=-xo*Tan;}//looking left
            else if(cos(angle)<-0.001){ rx=(int)px -0.0001; ry=(px-rx)*Tan+py; xo=-1; yo=-xo*Tan;}//looking right
            else { rx=px; ry=py; dof=30;}                                                  //looking up or down. no hit

            while(dof<30)
            {
            mx=(int)(rx); my=(int)(ry); mp = my * MAP_W + mx;
            if(mp>0 && mp<MAP_H*MAP_W && map_arr[my][mx]==1){ dof=30; disV=cos(angle)*(rx-px)-sin(angle)*(ry-py);}//hit
            else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
            }
            vx=rx; vy=ry;

            //---Horizontal---
            dof=0; disH=100000;
            Tan=1.0/Tan;
            if(sin(angle)> 0.001){ ry=(int)py -0.0001; rx=(py-ry)*Tan+px; yo=-1; xo=-yo*Tan;}//looking up
            else if(sin(angle)<-0.001){ ry=(int)py+1;      rx=(py-ry)*Tan+px; yo= 1; xo=-yo*Tan;}//looking down
            else{ rx=px; ry=py; dof=30;}                                                   //looking straight left or right

            while(dof<30)
            {
            mx=(int)(rx); my=(int)(ry); mp = my * MAP_W + mx;
            if(mp>0 && mp<MAP_H*MAP_W && map_arr[my][mx]==1){ dof=30; disH=cos(angle)*(rx-px)-sin(angle)*(ry-py);}//hit
            else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
            }

            if(disV<disH){ rx=vx; ry=vy; disH=disV; side=0; }                  //horizontal hit first

            hit[i] = rad_ch(2.0 * PI - angle);
            hit[i+1] = (disH);
//            float ca = direction;
//            ca = rad_ch(ca -angle); disH=disH*cos(ca);                            //fix fisheye
            angle = rad_ch(angle + STEP);
                        // vertical lines for the screen output created
            start = 0;
            end = H - 1;
            if(disH != 0) {
                start = H / 2 * (1 - 1/disH);
                end = H / 2 * (1 + 1/disH);
            }
            if(start<0) {
                start = 0;
            }
            if(end>H) {
                end = H - 1;
            }
            color = round(242 -8.066666 * disH);
            if (side == 1) { color += 10; }
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            j = 0;
            for (j=0; j<=SCALE*2 - 1; j++) {
                SDL_RenderDrawLine(renderer, i * SCALE + j, start, i * SCALE + j, end);
            }
        }

        if (show_map) {
            SDL_SetRenderDrawColor( renderer, 0, 0, 242, 255 );
            for (i=0; i<MAP_H; i++) {
                for (j=0; j<MAP_W; j++) {
                    if (map_arr[i][j] == 1) {
                        SDL_Rect r;
                        r.x = j * MAP_SCALE;
                        r.y = (MAP_H - i - 1) * MAP_SCALE;
                        r.w = MAP_SCALE;
                        r.h = MAP_SCALE;
                        SDL_RenderDrawRect( renderer, &r );
                    }
                }
            }
            SDL_SetRenderDrawColor( renderer, 0, 242, 0, 255 );
            SDL_Rect r;
            r.x = round(location[0] * MAP_SCALE)+HALF_MAP_SCALE - (int)HALF_MAP_SCALE/4;
            r.y = round((MAP_H - location[1] - 1) * MAP_SCALE)+HALF_MAP_SCALE - (int)HALF_MAP_SCALE/4;
            r.w = (int)HALF_MAP_SCALE/2;
            r.h = (int)HALF_MAP_SCALE/2;
            SDL_RenderDrawRect( renderer, &r );
            for (i=0; i<LENGTH * 2; i+=2) {
                SDL_RenderDrawLine(renderer, round(location[0]*MAP_SCALE + HALF_MAP_SCALE), round((MAP_H - location[1])*MAP_SCALE - HALF_MAP_SCALE), ceil((location[0] + hit[i+1] * cos(hit[i])) * MAP_SCALE+HALF_MAP_SCALE), ceil((MAP_H - location[1] - hit[i+1] * sin(hit[i])) * MAP_SCALE-HALF_MAP_SCALE));
            }
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
