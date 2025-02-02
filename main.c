#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "triangle.h"
#include "upng.h"
#include "camera.h"
#define MAX_TRIANGLES_PER_MESH 10000

triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render = 0;

//vec3_t cube_points[N_POINTS];
//vec2_t projected_points[N_POINTS];
int previous_frame_time = 0;

bool is_running = false;
float delta_time = 0;


//vec3_t cube_rotation = { .x = 0,.y = 0,.z = 0 };
/*triangle_t triangles_to_render[N_MESH_FACES];*///plane tranigle be projected
//triangle_t* triangles_to_render = NULL;
mat4_t world_matrix;
mat4_t view_matrix;
mat4_t proj_matrix;
light light_ray = { .direction = {.x = 3,.y = 2,.z = 3} };

void setup(void) {


	//initialize render mode and triangle culling method
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;

	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) *window_width * window_height);
	z_buffer = (float*)malloc(sizeof(float) * window_width * window_height);

	//creating a sdl texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

	//int point_count = 0;
	//for (float x = -1; x <= 1; x += 0.25) {
	//	for (float y = -1; y <= 1; y += 0.25) {
	//		for (float z = -1; z <= 1; z += 0.25) {
	//			vec3_t new_point = { .x = x,.y = y,.z = z };
	//			cube_points[point_count++] = new_point;
	//		}
	//	}
	//}
	//initialize the perspective projection matrix
	float fov = M_PI / 3.0; //60 degrees
	float aspect = ((float)window_height / (float)window_width);
	float znear = 0.1;
	float zfar = 100;


	proj_matrix = mat4_make_perspective(fov,aspect,znear,zfar);

	//Manually load the hardcoded texture data from the static array
	//mesh_texture = (uint32_t*)REDBRICK_TEXTURE;


	//load_cube_mesh_data();
	load_obj_file_data("./assets/f22.obj");
	//load the texture information from an external PNG file
	load_png_texture_data("./assets/f22.png");


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
			if (event.key.keysym.sym == SDLK_5)
				render_method = RENDER_TEXTURED;
			if (event.key.keysym.sym == SDLK_6)
				render_method = RENDER_TEXTURED_WIRE;
			if (event.key.keysym.sym == SDLK_c)
				cull_method = CULL_BACKFACE;
			if (event.key.keysym.sym == SDLK_x)
				cull_method = CULL_NONE;
			if (event.key.keysym.sym == SDLK_j)
				camera.position.y +=3.0* delta_time;
			if (event.key.keysym.sym == SDLK_k)
				camera.position.y -= 3.0*delta_time;
			if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT)
				camera.yaw += 1.0 * delta_time;
			if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
				camera.yaw -= 1.0 * delta_time;
			if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP)
			{
				camera.forward_velocity = vec3_mul(camera.direction, 5.0 * delta_time);
				camera.position = vec3_add(camera.position, camera.forward_velocity);
			}
			if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
			{
				camera.forward_velocity = vec3_mul(camera.direction, 5.0 * delta_time);
				camera.position = vec3_sub(camera.position, camera.forward_velocity);
			}
			   
			break;



	}

}
//vec2_t project(vec3_t point) {
//	vec2_t projected_point = {
//		.x = (fov_factor * point.x)/point.z,
//		.y = (fov_factor * point.y)/point.z
//	};
//	return projected_point;
//
//}

void update(void) {
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + Frame_Target_Time));
	//wait some time until the reach the target frame time in milliseconds
	int time_to_wait = Frame_Target_Time - (SDL_GetTicks() - previous_frame_time);

	//only delay execution iff we are running too fast
	if (time_to_wait > 0 && time_to_wait <= Frame_Target_Time) {
		SDL_Delay(time_to_wait);
	}
	//get a delta time factor converted  to seconds to be used to update our game  objects
  delta_time = (SDL_GetTicks() - previous_frame_time)/1000.0;
	previous_frame_time = SDL_GetTicks();
	//INitialize the counter of triangles to render for the current frame
	num_triangles_to_render = 0;
	//mesh.rotation.y += 0.01;
	/*mesh.rotation.y += 0.01;*/
	//mesh.rotation.z += 0.01;
	//mesh.rotation.y += 0.01;

	
	
	/*mesh.translation.x += 0.01;
	mesh.translation.y += 0.01;*/
	mesh.translation.z = 4;
	//create a scale,rotation and translation matrix that will be used to multiply the mesh vertices

	//change  the camera position per animation per animation frame
	/*camera.position.x += 0.8 * delta_time;
	camera.position.y += 0.8 * delta_time;*/
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);

	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

	//create the view matrix 

	vec3_t up_direction = { 0,1,0 };
	//compute the camera target looking at the positive z-axis
	vec3_t target = { 0,0,1 };
	mat4_t camera_yaw_rotation = mat4_make_rotation_y(camera.yaw);
	 camera.direction=  vec3_from_vec4(mat4_mul_vec4(camera_yaw_rotation, vec4_from_vec3( target)));
	 target = vec3_add(camera.position, camera.direction);


	 view_matrix = mat4_look_at(camera.position,target,up_direction);


	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++) {

		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];



		vec4_t transformed_vertices[3];
		for (int j = 0; j < 3; j++) {
			//tranform before  project
	/*		vec3_t transformed_vertice = vec3_rotate_x(face_vertices[j], mesh.rotation.x);
			transformed_vertice = vec3_rotate_y(transformed_vertice,mesh.rotation.y);
			transformed_vertice = vec3_rotate_z(transformed_vertice, mesh.rotation.z);*/
			vec4_t transformed_vertice =vec4_from_vec3(face_vertices[j]);

		/*transformed_vertice= mat4_mul_vec4(scale_matrix, transformed_vertice);
			transformed_vertice = mat4_mul_vec4(rotation_matrix_x, transformed_vertice);

			transformed_vertice = mat4_mul_vec4(rotation_matrix_y, transformed_vertice);
		
			transformed_vertice = mat4_mul_vec4(rotation_matrix_z, transformed_vertice);
			transformed_vertice = mat4_mul_vec4(translation_matrix, transformed_vertice);*/
			
			//create a world matrix combining scale,rotation,and translation matrices

			//order matter:First scale,then rotate,then translate. [T]*[R]*[S]*v
	        world_matrix = mat4_identity();
			world_matrix = mat4_mul_mat4(scale_matrix,world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

			transformed_vertice = mat4_mul_vec4(world_matrix, transformed_vertice);
										
			//multiply the view matrix by the vector to transform the scene to the camera space
			transformed_vertice = mat4_mul_vec4(view_matrix, transformed_vertice);

			
		//move the points away from the camera

	
		transformed_vertices[j] = transformed_vertice; 
	
		

		}
		vec3_t va = vec3_from_vec4(transformed_vertices[0]);/*  A   */
		vec3_t vb = vec3_from_vec4(transformed_vertices[1]);/* /  \*/
		vec3_t vc = vec3_from_vec4(transformed_vertices[2]);/*C---_B*/ //clockwiseŁ¨relative with left hands coordinateŁ©   
		vec3_t ab = vec3_sub(vb, va);
		vec3_t ac = vec3_sub(vc, va);
		vec3_normalize(&ab);
		vec3_normalize(&ac);
		//compute the face normal (using cross product to find )
		vec3_t normal_vector = vec3_cross(ab, ac);
		//normalize the face normal vector
		vec3_normalize(&normal_vector);
		vec3_t origin = { 0,0,0 };
		vec3_t camera_ray = vec3_sub(origin, va);
		float dot_normal_camera = vec3_dot(normal_vector, camera_ray);

		//check backface culling before project and after transform
		if (cull_method == CULL_BACKFACE) {
		

			//Bypass tge triangles that are looking away from camera
			if (dot_normal_camera < 0) continue;
		}
		
		vec4_t projected_points[3];

		//loop all three vertices to perform projection
		for (int j = 0; j < 3; j++) {
			//project
			
			vec4_t projected_point =mat4_mul_vec4_project(proj_matrix,transformed_vertices[j]);
			projected_points[j] = projected_point;

			//Scale into the view
			projected_points[j].x *= (window_width / 2.0);
			projected_points[j].y *= (window_height / 2.0);

			//Invert the y values to account for flipped screen y coordinate
			projected_points[j].y *= -1;

			// translate the projected points to the middle of the screen
			projected_points[j].x += (window_width / 2.0);
			projected_points[j].y += (window_height / 2.0);
		
		
		
		}
		//calculate the average depth for each face based on the vertices after transform
		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

		//calculate color base light intensity
	
		vec3_t zero_vector = { 0 };
		vec3_t reverse_light_ray = vec3_sub(zero_vector, light_ray.direction);
		vec3_normalize(&reverse_light_ray);
		float percent = vec3_dot(normal_vector, reverse_light_ray);
		
		uint32_t color = light_apply_intensity(mesh_face.color, percent);
		
	

		triangle_t plane_triangle = {
		.points = {
			{projected_points[0].x,projected_points[0].y,projected_points[0].z,projected_points[0].w},
			{projected_points[1].x,projected_points[1].y,projected_points[1].z,projected_points[1].w},
			{projected_points[2].x,projected_points[2].y,projected_points[2].z,projected_points[2].w}
		},
			.texcoords = {
				{
				mesh_face.a_uv.u,mesh_face.a_uv.v,
			
				},
			{mesh_face.b_uv.u,mesh_face.b_uv.v},
			{mesh_face.c_uv.u,mesh_face.c_uv.v}
		
		
		},
			.color = color,
			.avg_depth=avg_depth

		};
		/*array_push(triangles_to_render,plane_triangle);*/
		if (num_triangles_to_render < MAX_TRIANGLES_PER_MESH) {
			triangles_to_render[num_triangles_to_render++] = plane_triangle;
		}
		

	}
	//sort the triangles to render by theur avf_depth

	//int num_triangles = array_length(triangles_to_render);
	//for (int i = 0; i < num_triangles; i++) {
	//	for (int j = i; j < num_triangles; j++) {
	//		if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth) {
	//			triangle_t temp = triangles_to_render[i];
	//			triangles_to_render[i] = triangles_to_render[j];
	//			triangles_to_render[j] = temp;
	//		}
	//	}
	//}

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
	
	for (int i = 0; i < num_triangles_to_render; i++) {
		triangle_t plane_triangle = triangles_to_render[i];
	

		
		if(render_method==RENDER_FILL_TRIANGLE||render_method==RENDER_FILL_TRIANGLE_WIRE){
			//draw filled triangle(solid color)
			draw_filled_triangle(

				plane_triangle.points[0].x,
				plane_triangle.points[0].y,
				plane_triangle.points[0].z,
				plane_triangle.points[0].w,
				plane_triangle.points[1].x,
				plane_triangle.points[1].y,
				plane_triangle.points[1].z,
				plane_triangle.points[1].w,
				plane_triangle.points[2].x,
				plane_triangle.points[2].y,
				plane_triangle.points[2].z,
				plane_triangle.points[2].w,
				plane_triangle.color
			);
		
		}
		if (render_method == RENDER_TEXTURED|| render_method==  RENDER_TEXTURED_WIRE) {
			draw_textured_triangle(
				plane_triangle.points[0].x,
				plane_triangle.points[0].y,
				plane_triangle.points[0].z,
				plane_triangle.points[0].w,
				plane_triangle.texcoords[0].u,
				plane_triangle.texcoords[0].v,
				plane_triangle.points[1].x,
				plane_triangle.points[1].y,
				plane_triangle.points[1].z,
				plane_triangle.points[1].w,
				plane_triangle.texcoords[1].u,
				plane_triangle.texcoords[1].v,
				plane_triangle.points[2].x,
				plane_triangle.points[2].y,
				plane_triangle.points[2].z,
				plane_triangle.points[2].w,
				plane_triangle.texcoords[2].u,
				plane_triangle.texcoords[2].v,
				mesh_texture

			);
		}
		if (render_method==RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE||render_method==RENDER_TEXTURED_WIRE) {
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
	
	render_color_buffer();
	clear_color_buffer(0xFF000000);
	clear_z_buffer();
	SDL_RenderPresent(renderer);
}

//free memory that was dynamically allocated by the program
void free_resources(void) {
	free(color_buffer);
	free(z_buffer);
	upng_free(png_texture);
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