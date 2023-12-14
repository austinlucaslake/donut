// Copyright (c) 2023 Austin Lucas Lake
// <53884490+austinlucaslake@users.noreply.github.com>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "quaternion.h"

#define dot(v1, v2) \
    q_dot(v1, sizeof(v1)/sizeof(v1[0]), sizeof(v1[0]), v2, sizeof(v2)/sizeof(v2[0]), sizeof(v1[0]))

#define magnitude(v) \
    q_magnitude(v, sizeof(v)/sizeof(v[0]), sizeof(v[0]))

#define axial_cos(v1, v2) \
    q_axial_cos(v1, sizeof(v1)/sizeof(v1[0]), sizeof(v1[0]), v2, sizeof(v2)/sizeof(v2[0]), sizeof(v2[0]))

#define about_axis(axis, angle) \
    q_about_axis(axis, sizeof(axis)/sizeof(axis[0]), sizeof(axis[0]), angle)

#define from_euler(angles) \
    q_from_euler(angles, sizeof(angles)/sizeof(angles[0]), sizeof(angles[0]))

#define conjugate(q) \
    q_conjugate(q, sizeof(q)/sizeof(q[0]), sizeof(q[0]))

#define hamilton(q1, q2) \
    q_hamilton(q1, sizeof(q1)/sizeof(q1[0]), sizeof(q1[0]), q2, sizeof(q2)/sizeof(q2[0]), sizeof(q2[0]))

#define vec_from_quat(v_buff, q) \
    q_vec_from_quat(v_buff, q, sizeof(q)/sizeof(q[0]), sizeof(q[0]))

#define rotate(v, q) \
    q_rotate(v, sizeof(v)/sizeof(v[0]), sizeof(v[0]), q, sizeof(q)/sizeof(q[0]), sizeof(q[0]))

#define RADIUS 10
#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 50

int main(void) {
    size_t RADIUS_SQ = RADIUS * RADIUS;
    size_t FIVE_RADIUS = 5 * RADIUS;
    float euler_angles[3] = { 0.0f, 0.0f, 0.0f };
    float light_source[3] = { 0.0f, 0.0f, -1.0f };
    char display[SCREEN_HEIGHT][SCREEN_WIDTH];
    for (;;) {
        memset(display, ' ', sizeof display);
        float display_buffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {};
        const float rotation[4] = from_euler(euler_angles);
        for (float theta = 0; theta < 2*M_PI; theta += 0.05) {
            const float sin_theta = sin(theta);
            const float cos_theta = cos(theta);
            const float z = RADIUS * sin_theta;
            const float a0 = RADIUS * (2 + cos_theta);
            const float a1 = RADIUS * a0;
            const float a2 = sin_theta * a1;
            const float a3 = cos_theta * a1;
            for (float phi = 0; phi < 2*M_PI; phi += 0.05) {
                const float sin_phi = sin(phi);
                const float cos_phi = cos(phi);
                float position[3] = { a0 * cos_phi, a0 * sin_phi, z };
                rotate(position, &rotation);
                const float z_buffer = position[2] - FIVE_RADIUS;
                const float z_inverse = 1 / (position[2] + FIVE_RADIUS);
                const size_t x = (size_t) (SCREEN_WIDTH * 0.5) *
                    (1 + position[0] * z_inverse);
                const size_t y = (size_t) (SCREEN_HEIGHT * 0.5) *
                    (1 + position[1] * z_inverse);
                if (z_buffer <= display[y][x]) {
                    float normal[3] = {
                        a1 * cos_phi,
                        RADIUS_SQ * cos_phi * cos_phi * cos_phi,
                        a2 * sin_phi + a3 * cos_phi
                    };
                    rotate(normal, &rotation);
                    const int8_t brightness = (int8_t) 8 * sqrt(2) *
                        axial_cos(normal, light_source);
                    if (brightness >= 0) {
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
        euler_angles[0] += 0.0011;
        euler_angles[1] += 0.0007;
        euler_angles[2] += 0.0005;
    }
}
