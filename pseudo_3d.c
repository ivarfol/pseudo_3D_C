#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

#define PI 3.1415926535
#define LENGTH 800 
#define H 800
#define SCALE 2.0
#define FOV 0.5 * PI
#define SHIFT FOV / 2
#define STEP FOV / LENGTH
#define MAP_W 10
#define MAP_H 13
#define MAP_SCALE 20
#define HALF_MAP_SCALE 10
#define rad_ch(a) fmod(2.0 * PI + fmod(a, 2.0 * PI), 2.0 * PI)

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

int main(void)
{
    const short int map_arr[MAP_H][MAP_W] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
                                              {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
                                              {1, 1, 0, 0, 1, 0, 0, 0, 0, 1},
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
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LENGTH*SCALE, H, 0);
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
            if (show_map) { show_map = false; }
            else { show_map = true; }
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
 
        // ray casting
        angle = rad_ch(direction + SHIFT);
        float px = location[0] + 0.5;
        float py = location[1] + 0.5;
        for (i = 0; i < LENGTH * 2; i+=2) {
            int mxv,myv,mpv,dofv,mxh,myh,mph,dofh,side; float vx,vy,rxv,ryv,xov,yov,rxh,ryh,xoh,yoh,disV,disH;
            float Cos = cos(angle);
            float Sin = sin(angle);
            dofv=0; side=1; disV=100000;
            float TanV=tan(angle);
            if(Cos> 0.001){ rxv=(int)px+1;      ryv=(px-rxv)*TanV+py; xov= 1; yov=-xov*TanV;}//looking left
            else if(Cos<-0.001){ rxv=(int)px -0.000001; ryv=(px-rxv)*TanV+py; xov=-1; yov=-xov*TanV;}//looking right
            else { rxv=px; ryv=py; dofv=30;}                                                  //looking up or down. no hit

            //---Horizontal---
            dofh=0; disH=100000;
            float TanH=1.0/TanV;
            if(Sin> 0.001){ ryh=(int)py -0.000001; rxh=(py-ryh)*TanH+px; yoh=-1; xoh=-yoh*TanH;}//looking up
            else if(Sin<-0.001){ ryh=(int)py+1;      rxh=(py-ryh)*TanH+px; yoh= 1; xoh=-yoh*TanH;}//looking down
            else{ rxh=px; ryh=py; dofh=30;}                                                   //looking straight left or right

            while(dofh<30 || dofv<30)
            {
                if (dofh < dofv && dofh < 30) {
                    mxh=(int)(rxh); myh=(int)(ryh); mph = myh * MAP_W + mxh;
                    if(mph>-1 && mph<MAP_H*MAP_W && map_arr[myh][mxh]==1){ dofh=30; disH=Cos*(rxh-px)-Sin*(ryh-py);}//hit
                    else{ rxh+=xoh; ryh+=yoh; dofh+=1;}                                               //check next horizontal
                }
                else {
                    if (dofv < 30) {
                        mxv=(int)(rxv); myv=(int)(ryv); mpv = myv * MAP_W + mxv;
                        if(mpv>-1 && mpv<MAP_H*MAP_W && map_arr[myv][mxv]==1){ dofv=30; disV=Cos*(rxv-px)-Sin*(ryv-py);}//hit
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
                start = H / 2 * (1 - 1/disH);
                end = H / 2 * (1 + 1/disH);
            }
            if(start<0) {
                start = 0;
            }
            if(end>H) {
                end = H - 1;
            }
            color = round(242 -8.066666 * hit[i+1]);
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
            int h_position = round((0.5 -tan(rad_ch(rad_ch(angle + STEP) - direction)) / tan(FOV / 2.0) / 2.0) * LENGTH * SCALE);
            int next_h_position = round((0.5 -tan(rad_ch(angle - direction)) / tan(FOV / 2.0) / 2.0) * LENGTH * SCALE + 0.001);
//            if (h_position < 0) { h_position = LENGTH - h_position; }
//            printf("%d\n", h_position);
            SDL_RenderDrawLine(renderer, h_position, start, h_position, end);
            for (j=1; j<-h_position + next_h_position; j++) {
                SDL_RenderDrawLine(renderer, h_position + j, start, h_position + j, end);
            }
        }
//        break;

        if (show_map) {
            SDL_SetRenderDrawColor( renderer, 0, 0, 242, 255 );
            for (i=0; i<MAP_H; i++) {
                for (j=0; j<MAP_W; j++) {
                    if (map_arr[i][j] == 1) {
                        SDL_Rect r;
                        r.x = j * MAP_SCALE;
                        r.y = i * MAP_SCALE;
                        r.w = MAP_SCALE;
                        r.h = MAP_SCALE;
                        SDL_RenderDrawRect( renderer, &r );
                    }
                }
            }
            SDL_SetRenderDrawColor( renderer, 0, 242, 0, 255 );
            SDL_Rect r;
            r.x = round(location[0] * MAP_SCALE)+HALF_MAP_SCALE - (int)HALF_MAP_SCALE/4;
            r.y = round(location[1] * MAP_SCALE)+HALF_MAP_SCALE - (int)HALF_MAP_SCALE/4;
            r.w = (int)HALF_MAP_SCALE/2;
            r.h = (int)HALF_MAP_SCALE/2;
            SDL_RenderDrawRect( renderer, &r );
            for (i=0; i<LENGTH * 2; i+=2) {
                SDL_RenderDrawLine(renderer, round(location[0]*MAP_SCALE + HALF_MAP_SCALE), round(location[1]*MAP_SCALE + HALF_MAP_SCALE), ceil((location[0] + hit[i+1] * cos(hit[i])) * MAP_SCALE+HALF_MAP_SCALE), ceil((location[1] + hit[i+1] * sin(hit[i])) * MAP_SCALE+HALF_MAP_SCALE));
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
