#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include "vector.h"
#include <stdint.h>
typedef struct {

	vec3_t direction;
} light;

uint32_t light_apply_intensity(uint32_t original_color,float percent_factor); 
#endif // !LIGHT_H

