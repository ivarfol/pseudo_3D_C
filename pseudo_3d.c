#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

#define pi 3.1415926535

void move_f( const short int map_arr[][10], float *location, float direction, float rot, float mod, bool noclip) {
	float tmp[] = { location[0], location[1] };
//    printf("all %fx %fy\n", tmp[0], tmp[1]);
	tmp[0] += 0.125 * cos(direction * pi) * mod;
// 	printf("%f\n", 0.125 * sin(direction * pi) * mod);
// 	printf("%f %f %f\n", sin(direction * pi), direction, pi);
	tmp[1] += 0.125 * sin(direction * pi) * mod;
//	printf("%f\n", 0.125 * cos(direction * pi) * mod);
//    printf("all %fx %fy\n", tmp[0], tmp[1]);
    int index[] = {ceil(tmp[0]), ceil(tmp[1])};
	if(index[0]<9 && index[0]>0 && index[1]<10 && map_arr[index[1]][index[0]] != 1) {
//        printf("tmp %fx %fy\n", tmp[0], tmp[1]);
        location[0] = tmp[0];
        location[1] = tmp[1];
    }
}

void rad_ch(float *direction, float rot) {
	*direction += rot;
	if(*direction>=2) {
		*direction -=  2;
	}
	else { if(*direction<0) {
		*direction += 2;
	}}
//    printf("%f\n", *direction);
}

float* raycast(float direction, const short int map_arr[][10], float *location, const float step, const float shift, int *hit_size) {
    float *hit = (float *)malloc(200*sizeof(float));
    if(hit==NULL) {
        printf("Failed to allocate memory!\n");
        exit(1);
    }
    float angle = direction;
	rad_ch(&angle, shift);
    short int rays_num = 0;
	int i;
    int index[2] = {};
    float len_x;
    int wall_hit;
	for (i = 0; i < 200; i+=2) {
	wall_hit = 0;
	len_x = 0;
        while (wall_hit == 0) {
                len_x += 0.01;
            index[1] = round(location[1] + len_x * sin(angle * pi));
            index[0] = round(location[0] + len_x * cos(angle * pi));
            if (map_arr[index[1]][index[0]] == 1) {wall_hit = 1;}
        }
        hit[i] = angle;
		hit[i+1] = (len_x);
        rays_num += 2;
	rad_ch(&angle, step);
    }
    *hit_size = rays_num;
    return hit;
}

int* visual(float direction, float *location, const short int h, bool show_map, bool noclip, const float step, const float shift, float *hit, int hit_size) {
	int start = 0;
	int end = 0;
//	int lines[200] = {};
    int *lines = (int *)malloc(hit_size*sizeof(int));
    if(lines==NULL) {
        printf("Failed to allocate memory!\n");
        exit(1);
    }
    float dist = 0;
    int i = 0;
    //printf("%f\n", hit);
    for (i = 0; i < hit_size; i+=2) {
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
        lines[i] = start;
        lines[i+1] = end;
//            printf("%d %d\n", lines[i], lines[i+1]);
//            printf("%d %d\n", start, end);
    }
    return lines;
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
	float direction = 0;
	bool show_map = false;
	bool noclip = false;
	float mod = 1;
	short int move_tic = 1;
    int hit_size = 0;
	printf("main\n%fx %fy\n", location[0], location[1]);
//    move_f(map_arr, &*location, direction, 0.5, mod, false, pi);
	printf("%fx %fy\n", location[0], location[1]);
//    raycast(direction, map_arr, &*location, 0.005, -0.25, pi);
//    visual(direction, *map_arr, &*location, h, show_map, noclip, 0.005, -0.25, pi);
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
//        move_f(map_arr, &*location, direction, 0.0, mod, false, pi);
        // clear window
 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
 
        SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
        float* hit = raycast(direction, *&map_arr, location, 0.005, -0.25, &hit_size);
//        printf("hit size %d\n", hit_size);
        int* line = visual(direction, &*location, h, show_map, noclip, 0.005, -0.25, hit, hit_size);
        int i = 0;
        int j = 0;
        for (i=0; i<hit_size; i+=2) {
//            printf("drawing a line\n%d %d\n", line[i], line[i+1]);
            for (j=0; j<=scale*2; j++) {
                SDL_RenderDrawLine(renderer, i*scale + j, line[i], i * scale + j, line[i+1]);
            }
        }
        free(line);
        line = NULL;
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
//                printf("%f %f\n", line[i], line[i + 1]);
        }
        free(hit);
        hit = NULL;
        // render window
 
        SDL_RenderPresent(renderer);
    }
 
    // cleanup SDL
 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
