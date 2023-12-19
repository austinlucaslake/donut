// Copyright (c) 2023 Austin Lucas Lake
// <53884490+austinlucaslake@users.noreply.github.com>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "quaternion.h"

#define RADIUS 10
#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 50

int main(void) {
    const float RADIUS_SQ = RADIUS * RADIUS;
    const float FIVE_RADIUS = 5.0f * RADIUS;
    float euler_angles[3] = { 0.0f, 0.0f, 0.0f };
    float light_source[4] = { 0.0f, 0.0f, 0.0f, -1.0f };
    char display[SCREEN_HEIGHT][SCREEN_WIDTH];
    for (;;) {
        memset(display, ' ', sizeof display);
        float display_buffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {};
        const float * const rotation = q_from_euler(euler_angles);
        for (float theta = 0.0f; theta < 2.0f*M_PI; theta += 0.05f) {
            const float sin_theta = sin(theta);
            const float cos_theta = cos(theta);
            const float z = RADIUS * sin_theta;
            const float a0 = RADIUS * (2.0f + cos_theta);
            const float a1 = RADIUS * a0;
            const float a2 = sin_theta * a1;
            const float a3 = cos_theta * a1;
            for (float phi = 0.0f; phi < 2.0f*M_PI; phi += 0.05f) {
                const float sin_phi = sin(phi);
                const float cos_phi = cos(phi);
				float normal[4] = {
					0.0f,
					a1 * cos_phi,
					RADIUS_SQ * cos_phi * cos_phi * cos_phi,
					a2 * sin_phi + a3 * cos_phi
				};
				q_rotate(normal, rotation);
				const int8_t brightness = (int8_t) (8.0f * sqrt(2) *
					q_axial_cos(normal, light_source));
                if (brightness >= 0) {
					float position[4] = { 0.0f, a0 * cos_phi, a0 * sin_phi, z };
					q_rotate(position, rotation);
					const float z_buffer = position[3] - FIVE_RADIUS;
					const float z_inverse = 1.0f / (position[3] + FIVE_RADIUS);
					const size_t x = (size_t) ((SCREEN_WIDTH * 0.5f) *
						(1.0f + position[1] * z_inverse));
					const size_t y = (size_t) ((SCREEN_HEIGHT * 0.5f) *
						(1.0f + position[2] * z_inverse));
                    if (z_buffer <= display[y][x]) {
                        display[y][x] = ".,-~:;!=*#$@"[brightness];
                        display_buffer[y][x] = z_buffer;
                    }
                }
            }
        }
        for (size_t i = 0; i < SCREEN_HEIGHT; ++i) {
            for (size_t j = 0; j < SCREEN_WIDTH; ++j) {
                printf("%c ", display[i][j]);
            }
            putchar('\n');
        }
        printf("\033[H");
        euler_angles[0] += 0.0011f;
        euler_angles[1] += 0.0007f;
        euler_angles[2] += 0.0005f;
    }
}
