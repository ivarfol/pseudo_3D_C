#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

void move_f( const short int map_arr[], float *location, float direction, float rot, float mod, bool noclip, const float pi ) {
	float tmp[] = { location[0], location[1] };
//    printf("all %fx %fy\n", tmp[0], tmp[1]);
	tmp[0] += 0.125 * sin(direction * pi) * mod;
// 	printf("%f\n", 0.125 * sin(direction * pi) * mod);
// 	printf("%f %f %f\n", sin(direction * pi), direction, pi);
	tmp[1] += 0.125 * cos(direction * pi) * mod;
//	printf("%f\n", 0.125 * cos(direction * pi) * mod);
//    printf("all %fx %fy\n", tmp[0], tmp[1]);
    int index[] = {ceil(tmp[0]), ceil(tmp[1])};
	if(index[0]<9 && index[0]>0 && index[1]<12 && map_arr[index[0]*10+index[1]] != 1) {
        printf("tmp %fx %fy\n", tmp[0], tmp[1]);
        location[0] = tmp[0];
        location[1] = tmp[1];
    }
	printf("move\n");
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

float* raycast(float direction, const short int map_arr[], float *location, const float step, const float shift, const float pi) {
//	float hit[200] = {};
    float *hit = (float *)malloc(200*sizeof(float));
    memset(hit, -1, 200*sizeof(*hit));
    float angle = direction;
	rad_ch(&angle, shift);
	int i = 0;
	int j = 0;
    int index[2] = {};
    float mov = 0;
	for (i = 0; i < 200; i+=2) {
		mov = 0; // change for const step per ray
		for (j = 0; j < 100; j++) {
            index[0] = ceil(location[0] + mov * sin(angle * pi));
            index[1] = ceil(location[1] + mov * cos(angle * pi));
            //map_arr[ceil(location[0] + mov * sin(angle * pi))][ceil(location[1] + mov * cos(angle * pi))] == "#"
            //printf("index %dx %dy\n", index[0], index[1]);
            if(map_arr[index[0]*10+index[1]] == 1) {
                hit[i] = angle;
                hit[i+1] = mov;
                break;
            }
			mov += 0.1;
		}
		rad_ch(&angle, step);
//        printf("%f %f\n", hit[i], hit[i+1]);
//        if (hit[i] != hit[i]) { will check if nan
	}
    return hit;
}

//void print_map( const short int map_arr[], float direction, float location[], float hit[]) {
//	printf("map");
//}

int* visual(float direction, const short int map_arr, float *location, const short int h, bool show_map, bool noclip, const float step, const float shift, const float pi) {
	int start = 0;
	int end = 0;
//	int lines[200] = {};
    int *lines = (int *)malloc(200*sizeof(int));
    memset(lines, -1, 200*sizeof(*lines));
    float dist = 0;
    int i = 0;
    float* hit = raycast(direction, &map_arr, location, 0.005, -0.25, pi);
    int index = 0;
    //printf("%f\n", hit);
    for (i = 0; i < 200; i+=2) {
        if(hit[i]!=hit[i]) { continue; }
        else {
            start = 0;
            end = h - 1;
            dist = hit[i+1];
            if(dist != 0) {
                start = h / 2 * (1 - 1/dist);
                end = h / 2 * (1 - 1/dist);
            }
            if(start<0) {
                start = 0;
            }
            if(end>h) {
                end = h - 1;
            }
            lines[i] = start;
            lines[i+1] = end;
            //printf("%d %d\n", lines[i], lines[i+1]);
            //printf("%d %d\n", start, end);
        }
    }
    free(hit);
    hit = NULL;
    return lines;
}

int main(void) {
	const float pi = 3.141592;
	const short int map_arr[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	const short int length = 100;
	const short int h = 500;
	const short int scale = 5;
	float location[2] = {2, 2};
	float direction = 0;
	bool show_map = false;
	bool noclip = false;
	float mod = 1;
	short int move_tic = 1;
	printf("main\n%fx %fy\n", location[0], location[1]);
    move_f(map_arr, &*location, direction, 0.5, mod, false, pi);
	printf("%fx %fy\n", location[0], location[1]);
//    raycast(direction, map_arr, &*location, 0.005, -0.25, pi);
//    visual(direction, *map_arr, &*location, h, show_map, noclip, 0.005, -0.25, pi);
    bool quit = false;
    SDL_Event event;
 
    // init SDL
 
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("SDL2 line drawing",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, length*scale, h, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
 
    // handle events
 
    while (!quit)
    {
        SDL_Delay(10);
        SDL_PollEvent(&event);
 
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            // TODO input handling code goes here
        }
 
        // clear window
 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
 
        SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
        int* line = visual(direction, *map_arr, &*location, h, show_map, noclip, 0.005, -0.25, pi);
        int i = 0;
        int j = 0;
        for (i=0; i<200; i+=2) {
            if (line[i]!=line[i] || line[i] < 0 || line[i+1] < 0) { continue; }
            printf("drawing a line\n%d %d\n", line[i], line[i+1]);
            for (j=0; j<scale; j++) {
                SDL_RenderDrawLine(renderer, i*scale+j, line[i], i*scale+j, line[i+1]);
            }
        }
        free(line);
        line = NULL;
        // TODO rendering code goes here
 
        // render window
 
        SDL_RenderPresent(renderer);
    }
 
    // cleanup SDL
 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
