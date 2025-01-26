#include "triangle.h"
#include "display.h"
#include "swap.h"
#include <math.h>



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

void draw_filled_triangle(int x0, int y0,float z0,float w0, int x1, int y1,float z1,float w1, int x2, int y2,float z2,float w2,uint32_t color) {
	//we need to sort the vertices by y-coordinate ascending (y0<y1<y2)
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&z1, &z2);
		float_swap(&w1, &w2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
	}
	vec4_t a = { x0,y0,z0,w0 };
	vec4_t b = { x1,y1,z1,w1 };
	vec4_t c = { x2,y2,z2,w2 };
	//Rendder the upper part of the triangle (flat bottom)
	float inverse_slope_1 = 0;
	float inverse_slope_2 = 0;
	if (y1 - y0 != 0)inverse_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0)inverse_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y1 - y0 != 0) {
		for (int y = y0; y <= y1; y++) {
			int x_start = x1 + (y - y1) * inverse_slope_1;
			int x_end = x0 + (y - y0) * inverse_slope_2;
			if (x_end < x_start) {
				int_swap(&x_start, &x_end);
			}
			for (int x = x_start; x < x_end; x++) {

				draw_triangle_pixel(x, y, color, a, b, c);


			}



		}
	}

	//render the bottom part of the triangle (flat-top)
	inverse_slope_1 = 0;
	inverse_slope_2 = 0;
	if (y2 - y1 != 0)inverse_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
	if (y2 - y0 != 0)inverse_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y2 - y1 != 0) {
		for (int y = y1; y <= y2; y++) {
			int x_start = x1 + (y - y1) * inverse_slope_1;
			int x_end = x0 + (y - y0) * inverse_slope_2;
			if (x_end < x_start) {
				int_swap(&x_start, &x_end);
			}
			for (int x = x_start; x < x_end; x++) {

				/*draw_pixel(x, y, 0xff00ffff);*/
				draw_triangle_pixel(x, y, color, a, b, c);

			}



		}
	}



};

//return the barycentruc weights alpha,beta,and gamma for point p
////////////////////////////////
//
//               B
//              /|\
//             / | \
//            /  |  \
//           /  (p)  \
//          /  /   \  \
//         /  /     \  \
//        A-------------C


vec3_t  barycenteric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {
	vec2_t ac = vec2_sub(c, a);
	vec2_t ab = vec2_sub(b, a);
	vec2_t ap = vec2_sub(p, a);
	vec2_t pc = vec2_sub(c, p);
	vec2_t pb = vec2_sub(b, p);
	//COMPUTE the area of the full parallegram /trangle ABC using 2D cross product
	float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x);
	//alpha is the area og the small parallelogram/triangle PBC divided by the area of the full parallelgram/triangle ABC
	float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;
	//Bets is the area of the small parallelogram/triangle APC divided by the area other full parallelogram/triangle ABC
	float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;
	//weight gamma is easily found  since barycentric coordinates always add up to 1.0
	float gamma = 1 - alpha - beta;
	vec3_t weights = { alpha,beta,gamma };
	return weights;


}


void draw_triangle_pixel(int x, int y, uint32_t color, vec4_t a, vec4_t b, vec4_t c) {
	vec2_t p = { x,y };
	vec2_t a_point = vec2_from_vec4(a);
	vec2_t b_point = vec2_from_vec4(b);
	vec2_t c_point = vec2_from_vec4(c);
	vec3_t weights = barycenteric_weights(a_point, b_point, c_point, p);
	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;
	float interpolated_reciprocal_w;
	interpolated_reciprocal_w = (1 / a.w) * alpha + (1 / b.w) * beta + (1 / c.w) * gamma;
	//adjust 1/w so the pixels that are closer to  the camera have smaller values
	interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

	//only draw the pixel if the current depth value is less than the one previously stored in the z-buffer
	if (interpolated_reciprocal_w < z_buffer[(window_width * y) + x]) {
		draw_pixel(x, y, color);
		//update the z-buffer value the 1/w of this current pixel
		z_buffer[(window_width * y) + x] = interpolated_reciprocal_w;
	}
}

void draw_texel(int x,int y,uint32_t* texture,vec4_t a,vec4_t b,vec4_t c,float u0,float v0,float u1,float v1,float u2,float v2 ) {

	vec2_t p = { x,y };
	vec2_t a_point = vec2_from_vec4(a);
	vec2_t b_point = vec2_from_vec4(b);
	vec2_t c_point = vec2_from_vec4(c);
	vec3_t weights = barycenteric_weights(a_point, b_point, c_point, p);
	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;
	float interpolated_u;
	float interpolated_v;
	float interpolated_reciprocal_w;

	//peform the interpolation of all U/w and v/w values using barycentric weights and a factor of 1/w
	interpolated_u=(u0/a.w)*alpha+(u1/b.w)*beta+(u2/c.w)*gamma;
    interpolated_v=(v0/a.w)*alpha+(v1/b.w)*beta+(v2/c.w)*gamma;
	//also interpolate the value of 1/w for the current pixel
	interpolated_reciprocal_w= (1 / a.w) * alpha + (1 / b.w) * beta + (1 / c.w) * gamma;

	//now we can divide back both interpolated values by 1/w
	interpolated_u /= interpolated_reciprocal_w;
	interpolated_v /= interpolated_reciprocal_w;
	
	//Map th uv coordinates to the  full texture width and height
	int tex_x = abs((int)(interpolated_u * texture_width)) % texture_width;
	int tex_y = abs((int)(interpolated_v * texture_height))%texture_height;
	//adjust 1/w so the pixels that are closer to  the camera have smaller values
	interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

	//only draw the pixel if the current depth value is less than the one previously stored in the z-buffer
	if (interpolated_reciprocal_w < z_buffer[(window_width * y) + x]) {
		draw_pixel(x, y, texture[(texture_width * tex_y) + tex_x]);
		//update the z-buffer value the 1/w of this current pixel
		z_buffer[(window_width * y) + x] = interpolated_reciprocal_w;
	}
	

	/*draw_pixel(x, y, texture[(texture_width * tex_y) + tex_x]);*/

}


void draw_textured_triangle(int x0, int y0, float z0,float w0, float u0, float v0,
	int x1, int y1, float z1,float w1,float u1, float v1,
	int x2, int  y2,float z2,float w2, float u2, float v2,
	uint32_t* texture) {
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
		float_swap(&u0, &u1);
		
		float_swap(&v0, &v1); 
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&z1, &z2);
		float_swap(&w1, &w2);
		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}
	//create vector points after we sort the vertices
	vec4_t a = { x0,y0,z0,w0 };
	vec4_t b = { x1,y1,z1,w1 };
	vec4_t c = { x2,y2,z2,w2 };

	//flip the V component to account for inverted uv_coordinates(v grows downwards)
	v0 = 1.0 - v0;
	v1 = 1.0 - v1;
	v2 = 1.0 - v2;

	//Rendder the upper part of the triangle (flat bottom)
	float inverse_slope_1 = 0;
	float inverse_slope_2 = 0;
	if(y1-y0!=0)inverse_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
    if(y2-y0!=0)inverse_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
	
	if (y1 - y0 != 0) {
		for (int y = y0; y <= y1; y++) {
			int x_start = x1 + (y - y1) * inverse_slope_1;
			int x_end = x0 + (y - y0) * inverse_slope_2;
			if (x_end < x_start) {
				int_swap(&x_start, &x_end);
			}
			for (int x = x_start; x < x_end; x++) {

				draw_texel(x, y, texture, a, b, c, u0, v0, u1, v1, u2, v2);


			}



		}
	}

	//render the bottom part of the triangle (flat-top)
	 inverse_slope_1 = 0;
	 inverse_slope_2 = 0;
	if (y2 - y1 != 0)inverse_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
	if (y2 - y0 != 0)inverse_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y2 - y1 != 0) {
		for (int y = y1; y <= y2; y++) {
			int x_start = x1 + (y - y1) * inverse_slope_1;
			int x_end = x0 + (y - y0) * inverse_slope_2;
			if (x_end < x_start) {
				int_swap(&x_start, &x_end);
			}
			for (int x = x_start; x < x_end; x++) {

				/*draw_pixel(x, y, 0xff00ffff);*/
				draw_texel(x, y,texture,a,b,c,u0,v0,u1, v1,u2,v2);

			}



		}
	}

}