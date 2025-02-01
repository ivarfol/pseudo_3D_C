#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

#define PI 3.1415926535
#define LENGTH 200
#define H 500
#define SCALE 3
#define SHIFT -0.25 * PI
#define STEP 0.0025 * PI

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

void move_f( const short int map_arr[][10], float location[], float direction, float rot, float mod, bool noclip)
{
    float angle = -direction;
    rad_ch(&angle, rot);
//    printf("dir %f rot %f fin %f\n", direction/PI, rot/PI, angle/PI);
//    rad_ch(&angle, 1.5 * PI);
    float x = location[0] + 0.125 * cos(angle) * mod;
    float y = location [1] + 0.125 * sin(angle) * mod;
    if (map_arr[(int)round(y)][(int)round(x)] == 0) {
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
    float location[2] = {2, 2};
    float direction = 1.5 * PI;
    bool show_map, noclip, quit;
    float mod = 1;
    short int move_tic = 1;
    int color, start, end, i, j, wall_hit;
    float angle;
    float hit[LENGTH * 2] = {};
    bool KEYS[322];
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
        if (KEYS[SDL_SCANCODE_A]) {
            move_f(map_arr, location, direction, 0.5 * PI, mod, false);
        }
        if (KEYS[SDL_SCANCODE_D]) {
            move_f(map_arr, location, direction, 1.5 * PI, mod, false);
        }
        if (KEYS[SDL_SCANCODE_W]) {
            move_f(map_arr, location, direction, 0.0, mod, false);
        }
        if (KEYS[SDL_SCANCODE_S]) {
            move_f(map_arr, location, direction, 1.0 * PI, mod, false);
        }
        if (KEYS[SDL_SCANCODE_Q]) {
            rad_ch(&direction, -0.01 * PI);
        }
        if (KEYS[SDL_SCANCODE_E]) {
            rad_ch(&direction, 0.01 * PI);
        }
        // clear window
 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
 
        // ray casting
        angle = direction;
        rad_ch(&angle, SHIFT);
        float px = location[0] + 0.5;
        float py = location[1] + 0.5;
        for (i = 0; i < LENGTH * 2; i+=2) {
            int r,mx,my,mp,dof,side; float vx,vy,rx,ry,xo,yo,disV,disH;
            dof=0; side=0; disV=100000;
            float Tan=tan(angle);
            if(cos(angle)> 0.001){ rx=(int)px+1;      ry=(px-rx)*Tan+py; xo= 1; yo=-xo*Tan;}//looking left
            else if(cos(angle)<-0.001){ rx=(int)px -0.0001; ry=(px-rx)*Tan+py; xo=-1; yo=-xo*Tan;}//looking right
            else { rx=px; ry=py; dof=30;}                                                  //looking up or down. no hit

            while(dof<30)
            {
            mx=(int)(rx); my=(int)(ry); mp = my * 10 + mx;
            if(mp>0 && mp<13*10 && map_arr[my][mx]==1){ dof=30; disV=cos(angle)*(rx-px)-sin(angle)*(ry-py);}//hit
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
            mx=(int)(rx); my=(int)(ry); mp = my * 10 + mx;
            if(mp>0 && mp<13*10 && map_arr[my][mx]==1){ dof=30; disH=cos(angle)*(rx-px)-sin(angle)*(ry-py);}//hit
            else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
            }

            if(disV<disH){ rx=vx; ry=vy; disH=disV;}                  //horizontal hit first

            float tmp_angle = -angle + 2.0 * PI;
            rad_ch(&tmp_angle, 0.0);
            hit[i] = tmp_angle;
            hit[i+1] = (disH);
//            float ca = direction;
//            rad_ch(&ca, -angle); disH=disH*cos(ca);                            //fix fisheye
            rad_ch(&angle, STEP);
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
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            j = 0;
            for (j=0; j<=SCALE*2; j++) {
                SDL_RenderDrawLine(renderer, i * SCALE + j, start, i * SCALE + j, end);
            }
        }

        SDL_SetRenderDrawColor( renderer, 0, 0, 242, 255 );
        for (i=0; i<13; i++) {
            for (j=0; j<10; j++) {
                if (map_arr[i][j] == 1) {
                    SDL_Rect r;
                    r.x = (9 - j) * 10;
                    r.y = (12 - i) * 10;
                    r.w = 10;
                    r.h = 10;
                    SDL_RenderDrawRect( renderer, &r );
                }
            }
        }
        SDL_SetRenderDrawColor( renderer, 0, 242, 0, 255 );
        SDL_Rect r;
        r.x = round(-location[0] * 10)+9*10 +4;
        r.y = round(-location[1] * 10)+12 *10 +4;
        r.w = 3;
        r.h = 3;
        SDL_RenderDrawRect( renderer, &r );
        for (i=0; i<LENGTH * 2; i+=2) {
            SDL_RenderDrawLine(renderer, round(10*9 -location[0]*10 + 5), round(12 * 10 -location[1]*10 + 5), ceil((-location[0] - hit[i+1] * cos(hit[i]) + 10) * 10-5), ceil((-location[1] - hit[i+1] * sin(hit[i]) + 13) * 10-5));
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
