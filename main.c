
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"


//vec3_t cube_points[N_POINTS];
//vec2_t projected_points[N_POINTS];
int previous_frame_time = 0;

bool is_running = false;
float fov_factor = 640;
vec3_t camera_position = {
	.x = 0,
	.y = 0,.z = 0
};
//vec3_t cube_rotation = { .x = 0,.y = 0,.z = 0 };
/*triangle_t triangles_to_render[N_MESH_FACES];*///plane tranigle be projected
triangle_t* triangles_to_render = NULL;

void setup(void) {


	//initialize render mode and triangle culling method
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;

	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) *window_width * window_height);

	//creating a sdl texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

	//int point_count = 0;
	//for (float x = -1; x <= 1; x += 0.25) {
	//	for (float y = -1; y <= 1; y += 0.25) {
	//		for (float z = -1; z <= 1; z += 0.25) {
	//			vec3_t new_point = { .x = x,.y = y,.z = z };
	//			cube_points[point_count++] = new_point;
	//		}
	//	}
	//}
	load_cube_mesh_data();
	//load_obj_file_data("./assets/cube.obj");


}
void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);
	//key c or key d must be uppercase when you run the appliaction
	switch(event.type){
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			if (event.key.keysym.sym == SDLK_1)
				render_method = RENDER_WIRE_VERTEX;
			if (event.key.keysym.sym == SDLK_2)
				render_method = RENDER_WIRE;
			if (event.key.keysym.sym == SDLK_3)
				render_method = RENDER_FILL_TRIANGLE;
			if (event.key.keysym.sym == SDLK_4)
				render_method = RENDER_FILL_TRIANGLE_WIRE;
			if (event.key.keysym.sym == SDLK_c)
				cull_method = CULL_BACKFACE;
			if (event.key.keysym.sym == SDLK_d)
				cull_method = CULL_NONE;
			break;



	}

}
vec2_t project(vec3_t point) {
	vec2_t projected_point = {
		.x = (fov_factor * point.x)/point.z,
		.y = (fov_factor * point.y)/point.z
	};
	return projected_point;

}

void update(void) {
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + Frame_Target_Time));
	//wait some time until the reach the target frame time in milliseconds
	int time_to_wait = Frame_Target_Time - (SDL_GetTicks() - previous_frame_time);

	//only delay execution iff we are running too fast
	if (time_to_wait > 0 && time_to_wait <= Frame_Target_Time) {
		SDL_Delay(time_to_wait);
	}
	if(time_to_wait)

	previous_frame_time = SDL_GetTicks();
	//initialize the array of triangles to render
	triangles_to_render = NULL;
	mesh.rotation.y += 0.01;
	mesh.rotation.x += 0.01;
	mesh.rotation.z += 0.01;
	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++) {

		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];



		vec3_t transformed_vertices[3];
		for (int j = 0; j < 3; j++) {
			//tranform before  project
			vec3_t transformed_vertice = vec3_rotate_x(face_vertices[j], mesh.rotation.x);
			transformed_vertice = vec3_rotate_y(transformed_vertice,mesh.rotation.y);
			transformed_vertice = vec3_rotate_z(transformed_vertice, mesh.rotation.z);

			
		//move the points away from the camera

		transformed_vertice.z += 5;
		transformed_vertices[j] = transformed_vertice;
	
		

		}
		//check backface culling before project and after transform
		if (cull_method == CULL_BACKFACE) {
			vec3_t va = transformed_vertices[0];/*  A   */
			vec3_t vb = transformed_vertices[1];/* /  \*/
			vec3_t vc = transformed_vertices[2];/*C---_B*/ //clockwise£¨relative with left hands coordinate£©   
			vec3_t ab = vec3_sub(vb, va);
			vec3_t ac = vec3_sub(vc, va);
			vec3_normalize(&ab);
			vec3_normalize(&ac);
			//compute the face normal (using cross product to find )
			vec3_t normal_vector = vec3_cross(ab, ac);
			//normalize the face normal vector
			vec3_normalize(&normal_vector);
			vec3_t camera_ray = vec3_sub(camera_position, va);
			float dot_normal_camera = vec3_dot(normal_vector, camera_ray);

			//Bypass tge triangles that are looking away from camera
			if (dot_normal_camera < 0) continue;
		}
		
		vec2_t projected_points[3];

		//loop all three vertices to perform projection
		for (int j = 0; j < 3; j++) {
			//project
			vec3_t transformed_vertice = transformed_vertices[j];
			vec2_t projected_point = project(transformed_vertice);
			projected_points[j] = projected_point;
			//scale and translate the projected points to the middle of the screen
			projected_points[j].x += (window_width / 2);
			projected_points[j].y += (window_height / 2);
		
		
		}
		//calculate the average depth for each face based on the vertices after transform
		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;
		triangle_t plane_triangle = {
		.points = {
			{projected_points[0].x,projected_points[0].y},
			{projected_points[1].x,projected_points[1].y},
			{projected_points[2].x,projected_points[2].y}
		},
			.color = mesh_face.color,
			.avg_depth=avg_depth

		};
		array_push(triangles_to_render,plane_triangle);

	}
	//sort the triangles to render by theur avf_depth

	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++) {
		for (int j = i; j < num_triangles; j++) {
			if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth) {
				triangle_t temp = triangles_to_render[i];
				triangles_to_render[i] = triangles_to_render[j];
				triangles_to_render[j] = temp;
			}
		}
	}

}



void render(void) {

	//SDL_SetRenderDrawColor(renderer,255,0,0,255) ;  //rgba
	//SDL_RenderClear(renderer);
	draw_grid();
	//draw_rect(200,50,100,100, 0xffff0000);
	//for (int i = 0; i < N_POINTS; i++) {
	//	vec2_t projected_point = projected_points[i];

	//	draw_rect((int)(projected_point.x)+(window_width/2), (int)(projected_point.y)+(window_height/2), 4, 4, 0xffffff00);
	//}
	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++) {
		triangle_t plane_triangle = triangles_to_render[i];
	

		
		if(render_method==RENDER_FILL_TRIANGLE||render_method==RENDER_FILL_TRIANGLE_WIRE){
			//draw filled triangle(solid color)
			draw_filled_triangle(

				plane_triangle.points[0].x,
				plane_triangle.points[0].y,
				plane_triangle.points[1].x,
				plane_triangle.points[1].y,
				plane_triangle.points[2].x,
				plane_triangle.points[2].y,
				plane_triangle.color
			);
		
		}
		if (render_method==RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			//draw unfilled triangle(wireframe)
			draw_triangle(

				plane_triangle.points[0].x,
				plane_triangle.points[0].y,
				plane_triangle.points[1].x,
				plane_triangle.points[1].y,
				plane_triangle.points[2].x,
				plane_triangle.points[2].y,
				0xffffffff
			);
		}
		if (render_method == RENDER_WIRE_VERTEX) {
			draw_rect(plane_triangle.points[0].x-3, plane_triangle.points[0].y-3, 6, 6, 0xffffff00);
			draw_rect(plane_triangle.points[1].x-3, plane_triangle.points[1].y-3, 6, 6, 0xffffff00);
			draw_rect(plane_triangle.points[2].x-3, plane_triangle.points[2].y-3, 6, 6, 0xffffff00);
		}
		//draw vertex points



		

	}
	//draw_filled_triangle(300,100,50,400,500,700,0xffffff00);
	array_free(triangles_to_render);
	render_color_buffer();
	clear_color_buffer(0xFF000000);
	SDL_RenderPresent(renderer);
}

//free memory that was dynamically allocated by the program
void free_resources(void) {
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}


int main(int argc, char* args[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	printf("hello,world!!");
	is_running = initialize_window();
	setup();
	//start loading my array of vectors
	//from -1 to 1( in this 9*9*9 cube)

	while (is_running) {
		process_input();
		update();
		render();
	}
	destroy_window();
	free_resources();
	//printf("%d",sizeof(int));
	return 0;
}