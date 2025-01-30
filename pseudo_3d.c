#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

#define pi 3.1415926535

void rad_ch(float *direction, float rot) {
	*direction += rot;
	if(*direction>=2) {
		*direction -=  2;
	}
	else { if(*direction<0) {
		*direction += 2;
	}}
}

void move_f( const short int map_arr[][10], float *location, float direction, float rot, float mod, bool noclip) {
	float angle = direction;
	rad_ch(&angle, rot);
	float tmp[] = { location[0], location[1] };
	tmp[0] += 0.125 * cos(angle * pi) * mod;
	tmp[1] += 0.125 * sin(angle * pi) * mod;
    int index[] = {round(tmp[0]), round(tmp[1])};
    if (map_arr[index[1]][index[0]] == 0) {
        location[0] = tmp[0];
        location[1] = tmp[1];
    }
}

int main(void) {
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
	float direction = 1.5;
	bool show_map = false;
	bool noclip = false;
	float mod = 1;
	short int move_tic = 1;
	int color;
    int hit_size = 0;
    bool quit = false;
    SDL_Event event;
 
    // init SDL
 
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("SDL2 line drawing",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, length*scale*2, h, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
 
    // handle events
 
    while (!quit)
    {
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
 
        rad_ch(&direction, -0.01);
        move_f(map_arr, &*location, direction, 0.5, mod, false);
        // clear window
 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
 
	// get hits
	float shift = -0.25;
	float step = 0.005;
	    float angle = direction;
		rad_ch(&angle, shift);
	    short int rays_num = 0;
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
		    index[1] = round(location[1] + len_x * sin(angle * pi));
		    index[0] = round(location[0] + len_x * cos(angle * pi));
		    if (map_arr[index[1]][index[0]] == 1) {wall_hit = 1;}
		}
		hit[k] = angle;
			hit[k+1] = (len_x);
		rays_num += 2;
		rad_ch(&angle, step);
	    }
	    hit_size = rays_num;

	    // get line start and end
		int start = 0;
		int end = 0;
	    float dist = 0;
		int j;
		int i;
	    for (i = 0; i < 200; i+=2) {
		start = 0;
		end = h - 1;
		dist = hit[i+1];
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
		color = round(242 -8.066666 * hit[i+1]);
		SDL_SetRenderDrawColor(renderer, color, color, color, 255);
		j = 0;
	        for (j=0; j<=scale*2; j++) {
                SDL_RenderDrawLine(renderer, i*scale + j, start, i * scale + j, end);
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
        for (i=0; i<hit_size; i+=2) {
            SDL_RenderDrawLine(renderer, round(location[0]*10 + 5), round(location[1]*10 + 5), ceil((location[0] + hit[i+1] * cos(hit[i] * pi)) * 10) + 5, ceil((location[1] + hit[i+1] * sin(hit[i] * pi)) * 10) + 5);
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
