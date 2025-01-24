#include "light.h"
#include <stdint.h>
uint32_t light_apply_intensity(uint32_t original_color, float percent_factor){
	if (percent_factor > 1) percent_factor = 1;
	if (percent_factor < 0) percent_factor = 0;
	uint32_t a = (original_color & 0xFF000000);
	uint32_t r = (original_color & 0x00FF0000) * percent_factor;
	uint32_t g = (original_color & 0x0000FF00) * percent_factor;
	uint32_t b = (original_color & 0x000000FF) * percent_factor;
	uint32_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
	return new_color;
}