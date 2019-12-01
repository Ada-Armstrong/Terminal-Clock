
/* Header for the Brensenham line and circle algorithms. Uses only 
 * integer operations and works for any slope. For more info check out
 * https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm and
 * https://en.wikipedia.org/wiki/Midpoint_circle_algorithm. */

#include <stdlib.h>
#include <stdio.h>


struct posn {
	int x;
	int y;
};

typedef struct posn *Posn;


/* Returns an array of struct posns that approximate the line between (x0, y0)
 * and (x1, y1). Allocates these posns to the heap, client is responsible of
 * freeing. */
Posn draw_line(int x0, int y0, int x1, int y1, int *len);


/* Returns an array of struct posns that approximates the circle centered at
 * the point (x0, y0). Allocates these posns to the heap, client is responsible
 * of freeing. */
Posn draw_circle(int x0, int y0, int radius, int *len);

