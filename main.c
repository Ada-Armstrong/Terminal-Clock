/*
 * A simple analog clock for the terminal using the ncurses library.
 */

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>

#include "bresenham_line.h"

const int num_hour = 12;
const int num_min = 60;
const int num_sec = 60;

void place_pixels(Posn pixels, int num_pixels, char color);
void place_time_hand(int cur_time, int amt_time, float hand_len, 
		int origin_x, int origin_y, char color);
void place_border(int radius, int origin_x, int origin_y, char color);
void draw_clock(struct tm *cur_time, int origin_x, int origin_y,
		float sec_hand, float min_hand, float hour_hand);

int is_numeric(const char *string);


int main(int argc, char *argv[])
{
	int WIDTH = 37;

	if (argc == 2) {
		if (!is_numeric(argv[1])) {
			fprintf(stderr, "Non numeric argument.\n");
			return 1;
		}

		int ret = atoi(argv[1]);
		WIDTH = ret % 2 == 0 ? ret : ret + 1;
	} else if (argc > 2) {
		fprintf(stderr, "Too many arguments.\n");
		return 1;
	}

	int HEIGHT = WIDTH;

	int origin_x = (WIDTH - 1) >> 1;
	int origin_y = (HEIGHT - 1) >> 1;

	float sec_hand = 0.9 *  ((WIDTH - 1) >> 1);
	float min_hand = 0.75 * ((WIDTH - 1) >> 1);
	float hour_hand = 0.6 * ((WIDTH - 1) >> 1);

	time_t current_time;
	time_t last_time = time(NULL);
	struct tm *local_time;

	initscr(); // init the ncurses screen
	noecho();

	while (1) {

		current_time = time(NULL);
		/* checks if the time has changed */	
		if (last_time < current_time) {
			local_time = localtime(&current_time);
			draw_clock(local_time, origin_x, origin_y,
					sec_hand, min_hand, hour_hand);
			last_time = current_time;
		}
		refresh();
	}
	
	endwin(); // free the memory for the screen

	return 0;	
}

void place_pixels(Posn pixels, int num_pixels, char color)
{
	const int offset = 2; // to resolve screen stretching
	
	for (int i = 0; i < num_pixels; ++i) {
		mvaddch(pixels[i].y, pixels[i].x * offset, color);
	}
}

void place_time_hand(int cur_time, int amt_time, float hand_len, 
		int origin_x, int origin_y, char color)
{
	const float time_ang = 2 * M_PI / amt_time;
	const float angle = time_ang * (float)(cur_time % amt_time);
	const int x = round((float)origin_x + (hand_len * sinf(angle)));
	const int y = round((float)origin_y - (hand_len * cosf(angle)));
	
	int num_pixels = 0;
	Posn line = draw_line(origin_x, origin_y, x, y, &num_pixels);
	place_pixels(line, num_pixels, color);
	free(line);
}

void place_border(int radius, int origin_x, int origin_y, char color)
{
	const char empty = '.';	
	int num_pixels;
	Posn circle;
	char clr;

	for (int r = 2; r <= radius; ++r) {
		clr = r == radius? color: empty;
		num_pixels = 0;
		circle = draw_circle(origin_x, origin_y, r, &num_pixels);
		place_pixels(circle, num_pixels, clr);
		free(circle);
	}
}

void draw_clock(struct tm *cur_time, int origin_x, int origin_y,
		float sec_hand, float min_hand, float hour_hand)
{
	place_border(origin_x, origin_x, origin_y, '*');

	place_time_hand(cur_time->tm_sec, num_sec, sec_hand,
			origin_x, origin_y, 's');
	place_time_hand(cur_time->tm_min, num_min, min_hand,
			origin_x, origin_y, 'm'); 			
	place_time_hand(cur_time->tm_hour, num_hour, hour_hand,
			origin_x, origin_y, 'h');
}

int is_numeric(const char *string)
{
	if (string == NULL || *string == '\0')
		return 0;
	for (; *string; ++string) {
		if ('0' > *string || *string > '9')
			return 0;
	}

	return 1;
}
