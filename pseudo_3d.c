#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

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
	}
	}
}

void raycast(float direction, const short int map_arr[], const float step, const float shift) {
	float hit[200] = {};
    float angle = direction;
	rad_ch(&angle, shift);
	int i = 0;
	for (i = 0; i < 200; i+=2) {
		printf("i");
		float mov = 0; // change for const step per ray
		for (i = 0; i < 100; i++) {
			printf("add");
			mov += 0.1;
		}
		rad_ch(&angle, step);
	}
	printf("raycast");
}

void line(float dist, const short int h, short int num, const short int scale) {
	short int start = 0;
	short int end = h - 1;
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
	printf("line"); // output line
}

//void print_map( const short int map_arr[], float direction, float location[], float hit[]) {
//	printf("map");
//}

void visual(float direction, const short int map_arr, float location, const short int h, const short int scale, bool show_map, bool noclip, const float step, const float shift) {
	printf("visual");
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
	const short int sacle = 5;
	float location[2] = {2, 2};
	float direction = 0;
	bool show_map = false;
	bool noclip = false;
	float mod = 1;
	short int move_tic = 1;
	printf("main\n%fx %fy\n", location[0], location[1]);
    move_f(map_arr, &*location, direction, 0.5, mod, false, pi);
	printf("%fx %fy\n", location[0], location[1]);
    return 0;
}
