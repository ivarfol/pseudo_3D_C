#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

#define PI 3.1415926535

void rad_ch(float *direction, float rot) {
	*direction += rot;
	if(*direction>=2 * PI) {
		*direction -=  2 * PI;
	}
	else { if(*direction<0) {
		*direction += 2 * PI;
	}}
}

void move_f( const short int map_arr[][10], float *location, float direction, float rot, float mod, bool noclip) {
	float angle = direction;
	rad_ch(&angle, rot);
	float x = location[0] + 0.125 * cos(angle) * mod;
	float y = location [1] + 0.125 * sin(angle) * mod;
    int round_x = round(x);
    int round_y = round(y);
    if (map_arr[round_x][round_y] == 0) {
        location[0] = x;
        location[1] = y;
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
	float direction = 1.5 * PI;
	bool show_map = false;
	bool noclip = false;
	float mod = 1;
	short int move_tic = 1;
	int color;
    int hit_size = 1;
    bool quit = false;
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
	    short int rays_num = 0;
		int k;
	    int index[2] = {};
	    float len_x;
	    float hit[200] = {};
	    int wall_hit;
        int map_pos[2] = {(int)location[0], (int)location[1]};
        float cos_a;
        float sin_a;
        float x_vert;
        int dx;
        float y_vert;
        float depth_vert;
        float delta_depth;
        float dy;
        int tile_vert[2] = {};
        int l = 0;
        float y_hor;
        float x_hor;
        float depth_hor;
        int tile_hor[2] = {};
		for (k = 0; k < 200; k+=2) {
            wall_hit = 0;
            len_x = 0;
            sin_a = sin(angle);
            cos_a = cos(angle);
            //horizontals
            if (sin_a > 0) {
                y_hor = map_pos[1] + 1;
                dy = 1;
            }
            else {
                y_hor = map_pos[1] - 0.000001;
                dy = -1;
            }
            depth_hor = (y_hor - location[1]) / sin_a;
            x_hor = location[0] + depth_hor * cos_a;
            delta_depth = dy / sin_a;
            dx = delta_depth * cos_a;
            for (l=0; l<30; l++) {
                tile_hor[0] = (int)x_hor;
                tile_hor[1] = (int)y_hor;
                if (tile_hor[1] > 13 || tile_hor[1] < 0 || tile_hor[0] > 10 || tile_hor[0] < 0 || map_arr[tile_hor[1]][tile_hor[0]] == 1) { break; }
                x_hor += dx;
                y_hor += dy;
                depth_hor += delta_depth;
            }
            // verticals
            if (cos_a > 0) {
                x_vert = map_pos[0] + 1;
                dx = 1;
            }
            else {
                x_vert = map_pos[0] - 0.000001;
                dx = -1;
            }
            depth_vert = (x_vert - location[0]) / cos_a;
            y_vert = location[1] + depth_vert * sin_a;
            delta_depth = dx / cos_a;
            dy = delta_depth * sin_a;
            for (l=0; l<30; l++) {
                tile_vert[0] = (int)x_vert;
                tile_vert[1] = (int)y_vert;
                if (tile_vert[1] > 13 || tile_vert[1] < 0 || tile_vert[0] > 10 || tile_vert[0] < 0 || map_arr[tile_vert[1]][tile_vert[0]] == 1) { break; }
                x_vert += dx;
                y_vert += dy;
                depth_vert += delta_depth;

            }
//            if (!(tile_vert[1] > 13 || tile_vert[1] < 0 || tile_vert[0] > 10 || tile_vert[0] < 0)) {printf("%d\n", map_arr[tile_vert[1]][tile_vert[0]]);}
//            printf("x_hor %f y_hot %f x_vert %f y_vert %f\nh %f v %f %fa\n", x_hor, y_hor, x_vert, y_vert, depth_hor, depth_vert, angle);
            if (depth_vert < depth_hor) {
                hit[k+1] = depth_vert;
            }
            else {
                hit[k+1] = depth_hor;
            }

            hit[k] = angle;
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
            SDL_RenderDrawLine(renderer, round(location[0]*10 + 5), round(location[1]*10 + 5), ceil((location[0] + hit[i+1] * cos(hit[i])) * 10), ceil((location[1] + hit[i+1] * sin(hit[i])) * 10));
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
