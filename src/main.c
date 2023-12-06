// Copyright (c) 2023 Austin Lake
// <53884490+austinlucaslake@users.noreply.github.com>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define TWO_PI 2.0f*M_PI
#define RADIUS 8.0f
#define SCREEN_WIDTH 75
#define SCREEN_HEIGHT 75

typedef struct {
    float w;
    float x;
    float y;
    float z;
} quaternion;

#define X_AXIS (quaternion) { 0.0f, 1.0f, 0.0f, 0.0f }
#define Y_AXIS (quaternion) { 0.0f, 0.0f, 1.0f, 0.0f }
#define Z_AXIS (quaternion) { 0.0f, 0.0f, 0.0f, 1.0f }
#define LIGHT_SOURCE (quaternion) { 0.0f, 0.0f, 1.0f, -1.0f }

static inline quaternion conjugate(quaternion * const q) {
    return (quaternion) {q->w, -q->x, -q->y, -q->z};
}

static inline float dot_product(const quaternion q1, const quaternion q2) {
    return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

static inline float magnitude(const quaternion q) {
    return sqrt(dot_product(q, q));
}

static inline float axial_cos(const quaternion q1, const quaternion q2) {
    return dot_product(q1, q2) / (magnitude(q1) * magnitude(q2));
}

static inline quaternion hamilton_product(const quaternion q1, const quaternion q2) {
    return (quaternion) {
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
        q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
        q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
        q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
	};
}

static quaternion rotate_about_axis(const quaternion axis, const float angle) {
    const float half_angle = angle * 0.5f;
	const float sin_half_angle = sin(half_angle);
    return (quaternion) {
		cos(half_angle),
        sin_half_angle * axial_cos(axis, X_AXIS),
        sin_half_angle * axial_cos(axis, Y_AXIS),
        sin_half_angle * axial_cos(axis, Z_AXIS)
	};
}

static void apply_rotation(quaternion * const vector, quaternion * const rotation) {
    *vector = hamilton_product(hamilton_product(*rotation, *vector), conjugate(rotation));
}

static inline char render(const int8_t brightness) {
	return ".,-~:;!=*#$@"[brightness];
}

int main(void) {
	float x_angle = 0.0f,
		  y_angle = 0.0f,
		  z_angle = 0.0f;
	quaternion position,
			   normal,
			   rotation;
	quaternion * const ptr_position = &position,
			   * const ptr_normal = &normal,
			   * const ptr_rotation = &rotation;
 	char display[SCREEN_HEIGHT][SCREEN_WIDTH];
	for (;;) {
        float display_buffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {};
 		memset(display, ' ', SCREEN_HEIGHT*SCREEN_WIDTH);
        rotation = hamilton_product(hamilton_product(rotate_about_axis(X_AXIS, x_angle),
                                                     rotate_about_axis(Y_AXIS, y_angle)),
                                    rotate_about_axis(Z_AXIS, z_angle));
        for (float theta = 0.0f; theta < TWO_PI; theta += 0.05f) {
            const float sin_theta = sin(theta),
				        cos_theta = cos(theta);
			const float z = RADIUS * sin_theta,
				        a0 = RADIUS * (2.0f + cos_theta);
			const float a1 = a0 * RADIUS;
			const float a2 = a1 * sin_theta, 
				        a3 = a1 * cos_theta;
			for (float phi = 0.0f; phi < TWO_PI; phi += 0.05f) {
                const float sin_phi = sin(phi),
					        cos_phi = cos(phi);
                normal = (quaternion) {
                    0.0f,
					a3 * cos_phi,
                    RADIUS * RADIUS * cos_phi * cos_phi * cos_phi,
                    a2 * sin_phi + a3 * cos_phi
				};
                apply_rotation(ptr_normal, ptr_rotation);
                const int8_t brightness = (int8_t) 8.0f * sqrt(2) * axial_cos(normal, LIGHT_SOURCE);
                if (brightness >= 0) {
                    position = (quaternion) {
						0.0f,
						a0 * cos_phi,
                        a0 * sin_phi,
						z	
					};
                    apply_rotation(ptr_position, ptr_rotation);
                    const float z_buffer = position.z - 5.0f*RADIUS,
						        z_inverse = 1.0f / (position.z + 5.0f*RADIUS);
                    const uint16_t x = (uint16_t) (SCREEN_WIDTH/2) * (1.0f + position.x * z_inverse),
						           y = (uint16_t) (SCREEN_HEIGHT/2) * (1.0f + position.y * z_inverse);
                    if (z_buffer <= display_buffer[y][x]) {
                        display_buffer[y][x] = z_buffer;
                        display[y][x] = render(brightness);
                    }
                }
            }
        }
        for (uint16_t i = 0; i < SCREEN_HEIGHT; ++i) {
            for (uint16_t j = 0; j < SCREEN_WIDTH; ++j) {
                printf("%c ", display[i][j]);
            }
            putchar('\n');
        }
        printf("\033[H");
        x_angle += 0.005f;
        y_angle += 0.003f;
        z_angle += 0.002f;
    }
}
