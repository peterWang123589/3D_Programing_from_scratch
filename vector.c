#include "vector.h"
#include <math.h>


//implement 2d vector  function
float vec2_length(vec2_t v) {

	return sqrt(v.x * v.x + v.y + v.y);

}
vec2_t vec2_add(vec2_t a, vec2_t b) {
	vec2_t res = {
		.x = a.x + b.x,
		.y = a.y + b.y
	};
	return res;
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
	vec2_t res = {
		.x = a.x - b.x,
		.y = a.y -b.y
	};
	return res;
}
vec2_t vec2_mul(vec2_t v, float factor) {
	vec2_t res = {
		.x = v.x * factor,
		.y = v.y * factor
	};
	return res;
}

vec2_t vec2_div(vec2_t v, float factor) {
	vec2_t res = {
		.x = v.x / factor,
		.y = v.y / factor
	};
	return res;
}

float vec2_dot(vec2_t a, vec2_t b) {
	return (a.x * b.x) + (a.y * b.y) ;
};

void vec2_normalize(vec2_t* normal) {
	float length = vec2_length(*normal);
	normal->x /= length;
	normal->y /= length;
}

//implement 3d vector funciton
float vec3_length(vec3_t v) { 

	return sqrt(v.x * v.x + v.y *v.y+v.z*v.z);

}
vec3_t vec3_add(vec3_t a, vec3_t b) {
	vec3_t res = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z=a.z+b.z
	};
	return res;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
	vec3_t res = {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z=a.z-b.z
	};
	return res;
}

vec3_t vec3_mul(vec3_t v, float factor) {
	vec3_t res = {
		.x = v.x * factor,
		.y = v.y * factor,
		.z=v.z*factor
	};
	return res;
}

vec3_t vec3_div(vec3_t v, float factor) {
	vec3_t res = {
		.x = v.x / factor,
		.y = v.y / factor,
		.z=v.z/factor
	};
	return res;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
	vec3_t res = {
	.x=a.y*b.z-a.z*b.y,
	.y=a.z*b.x-a.x*b.z,
	.z=a.x*b.y-a.y*b.x

	};
	return res;
}

float vec3_dot(vec3_t a, vec3_t b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
};

void vec3_normalize(vec3_t* normal) {
	float length = vec3_length(*normal);
	normal->x /= length;
	normal->y /= length;
	normal->z /= length;
}

vec3_t vec3_rotate_x(vec3_t v,float angle) {
	vec3_t rotated_vector = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;

}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.z * sin(angle),
		.y = v.y,
		.z = v.x * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;

}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y * cos(angle),
		.z =v.z 
	};
	return rotated_vector;

}

//vec4 implemetion
vec4_t  vec4_from_vec3(vec3_t v) {

	vec4_t res = { v.x,v.y,v.z,1.0 };
	return res;

};
vec3_t vec3_from_vec4(vec4_t v) {
	vec3_t res = { v.x,v.y,v.z };
	return res;
}

vec2_t vec2_from_vec4(vec4_t v) {
	vec2_t res = { v.x,v.y };
	return res;
}