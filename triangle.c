#include "triangle.h"
#include "display.h"

void int_swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}


/************************************************
 * Draw a filled triangle with a flat bottom    *
 ************************************************
 *           (x0, y0)                           *
 *              /\                              *
 *             /  \                             *
 *            /    \                            *
 *           /      \                           *
 *      (x1, y1)----(x2, y2)                    *
 *                                              *
 * Flat-bottom triangles have two vertices      *
 * (x1, y1) and (x2, y2) on the same horizontal *
 * line. Filling can be done using horizontal   *
 * scanlines between (x0, y0) and the base.     *
 ************************************************/
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int  y2, uint32_t color) {
	//find the two slope (two triangle legs)
	float inverse_slope_1 =(float)(x1-x0)/(y1 - y0);
	float inverse_slope_2 = (float)(x2 - x0) / (y2 - y0);


	//Start x_start and x_end from the top vertex (x0,y0)
	float x_start = x0;
	float x_end = x0;
	
	//loop all the scanlines from top to bottom
	for (int y = y0; y <= y2; y++) {
		draw_line(x_start, y, x_end, y, color);
		x_start += inverse_slope_1;
		x_end += inverse_slope_2;
	}



};
/************************************************
 * Draw a filled triangle with a flat top       *
 ************************************************
 *  (x0, y0)----(x1, y1)                        *
 *       \      /                               *
 *        \    /                                *
 *         \  /                                 *
 *         (x2, y2)                             *
 *                                              *
 * Flat-top triangles have two vertices         *
 * (x0, y0) and (x1, y1) on the same horizontal *
 * line. Filling can be done using horizontal   *
 * scanlines between (x2, y2) and the base.     *
 ************************************************/

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int  y2, uint32_t color) {
	//find the two slope (two triangle legs)
	float inverse_slope_1 = (float)(x2 - x0) / (y2 - y0);
	float inverse_slope_2 = (float)(x2 - x1) / (y2 - y1);


	//Start x_start and x_end from the bottom vertex (x2,y2)
	float x_start = x2;
	float x_end = x2;

	//loop all the scanlines from top to bottom
	for (int y = y2; y >= y1; y--) {
		draw_line(x_start, y, x_end, y, color);
		x_start -= inverse_slope_1;
		x_end -= inverse_slope_2;
	}

};

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2,uint32_t color) {
	//we need to sort the vertices by y-coordinate ascending (y0<y1<y2)
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y1 == y2) {
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else if (y0 == y1) {
		fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else {
		/*
Draw a filled triangle using the flat-top/flat-bottom method.
We split the original triangle into two parts: half flat-bottom and half flat-top.

Original triangle vertices:
(x0, y0)
   /\
  /  \
 /    \
(x1, y1)----(Mx, My)  <- Split the triangle here
   \      /
	\    /
	 \  /
	  \/
   (x2, y2)

   Steps:
   1. Sort vertices by y-coordinate: (x0, y0) < (x1, y1) < (x2, y2).
   2. Split the triangle into two parts at (Mx, My):
	  a. Flat-bottom triangle: (x0, y0), (x1, y1), (Mx, My).
	  b. Flat-top triangle: (x2, y2), (x1, y1), (Mx, My).
   3. Use horizontal scanlines to fill each part.
*/

//calculate  the new vertex (Mx,My) using triangle similarity
		int My = y1;
		int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;
		//draw flat-bottom triangle
		fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);

		//draw flat-top triangle
		fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);



	}


};
