#include "bresenham_line.h" 


static inline void draw_high(int x0, int y0, int x1, int y1, 
		Posn pixels, int *len) {

	int dx, dy, p, x, y, xi;
   	struct posn pixel;

	dx = x1 - x0;
    	dy = y1 - y0;
	
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	} else {
		xi = 1;
	}

	x = x0;
	y = y0;
 
	p = (dx << 1) - dy;
	 
	while(y <= y1) {
		pixel.x = x;
		pixel.y = y;	
		pixels[*len] = pixel;
		if(p >= 0) {
			x = x + xi; 
			p = p - (dy << 1);
		}

		p = p + (dx << 1);
		y = y + 1;
		*len += 1;
	}
}


static inline void draw_low(int x0, int y0, int x1, int y1, 
		Posn pixels, int *len) {

	int dx, dy, p, x, y, yi;
   	struct posn pixel;

	dx = x1 - x0;
    	dy = y1 - y0;

	if (dy < 0) {
		yi = -1;
		dy = -dy;
	} else {
		yi = 1;
	}

	x = x0;
	y = y0;
 
	p = (dy << 1) - dx;
	 
	while(x <= x1) {
		pixel.x = x;
		pixel.y = y;	
		pixels[*len] = pixel;
		if(p >= 0) {
			y = y + yi; 
			p = p - (dx << 1);
		}

		p = p + (dy << 1);
		x = x + 1;
		*len += 1;
	}
}


static inline void draw_dia(int x0, int y0, int x1, int y1, 
		Posn pixels, int *len) {

	int dy, yi;
	struct posn pixel;

	dy = y1 - y0;
	if (dy < 0) {
		yi = -1;
	} else {
		yi = 1;
	}

	while (x0 <= x1) {
		pixel.x = x0;
		pixel.y = y0;
		pixels[*len] = pixel;
		x0 += 1;
		y0 += yi;
		*len += 1;
	}	
}


/* Returns an array of struct posns that approximate the line between (x0, y0)
 * and (x1, y1). Allocates these posns to the heap, client is responsible of
 * freeing. */
Posn draw_line(int x0, int y0, int x1, int y1, int *len) {

	int max = x0 > y0? x0: y0;	
	Posn pixels = malloc(max * sizeof(struct posn));

	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1) {
			draw_low(x1, y1, x0, y0, pixels, len);
		} else {
			draw_low(x0, y0, x1, y1, pixels, len);
		}
	} else if (abs(y1 - y0) == abs(x1 - x0)) {
		if (x0 > x1) {
			draw_dia(x1, y1, x0, y0, pixels, len);
		} else {
			draw_dia(x0, y0, x1, y1, pixels, len);
		}
	} else {
		if (y0 > y1) {
			draw_high(x1, y1, x0, y0, pixels, len);
		} else {
			draw_high(x0, y0, x1, y1, pixels, len);
		}
	}

	return pixels;
}


static inline struct posn ret_posn(int x, int y) {
	struct posn p = {x, y};
	return p;
}


Posn draw_circle(int x0, int y0, int radius, int *len) {

	int n, x, y, dx, dy, err;

	n = radius << 3;
	Posn pixels = malloc(n * sizeof(struct posn));

	x = radius - 1;
	y = 0;
	dx = 1;
	dy = 1;
	err = dx - (radius << 1);

	while (x >= y) {
		/* mirrors the point for all eight octants */	
		pixels[(*len)++] = ret_posn(x0 + x, y0 + y);
		pixels[(*len)++] = ret_posn(x0 + y, y0 + x);	
		pixels[(*len)++] = ret_posn(x0 - y, y0 + x);	
		pixels[(*len)++] = ret_posn(x0 - x, y0 + y);	
		pixels[(*len)++] = ret_posn(x0 - x, y0 - y);	
		pixels[(*len)++] = ret_posn(x0 - y, y0 - x);	
		pixels[(*len)++] = ret_posn(x0 + y, y0 - x);	
		pixels[(*len)++] = ret_posn(x0 + x, y0 - y);	

		if (err < 0) {
			++y;
			err += dy;
			dy += 2;
		} else {
			--x;
			dx += 2;
			err += dx - (radius << 1);
		}
	}

	return pixels;	
}
