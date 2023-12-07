// Copyright (c) 2023 Austin Lake
// <53884490+austinlucaslake@users.noreply.github.com>

#include "quaternion.h"

#define RADIUS 8
#define SCREEN_WIDTH 75
#define SCREEN_HEIGHT 75
#define LIGHT_SOURCE (quaternion) { 0, 0, 1, -1 }

int main(void) {
    float roll = 0;
    float pitch = 0;
    float yaw = 0;
    float display_buffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {};
    char display[SCREEN_HEIGHT][SCREEN_WIDTH];
    for (;;) {
        memset(display, ' ', sizeof(char) * SCREEN_HEIGHT * SCREEN_WIDTH);
        const quaternion rotation = from_euler(roll, pitch, yaw);
        for (float theta = 0; theta < 2*M_PI; theta += 0.05) {
            const float sin_theta = sin(theta);
            const float cos_theta = cos(theta);
            for (float phi = 0; phi < 2*M_PI; phi += 0.05) {
                const float sin_phi = sin(phi);
                const float cos_phi = cos(phi);
                quaternion normal = (quaternion) {
                    0,
                    RADIUS * RADIUS * (2 + cos_theta) * cos_phi,
                    RADIUS * RADIUS * cos_phi * cos_phi * cos_phi,
                    RADIUS * RADIUS * (2 + cos_theta) * (sin_theta * sin_phi + cos_theta * cos_phi)
                };
                normal = apply_rotation(normal, rotation);
                const int8_t brightness = (int8_t) 8 * sqrt(2) * axial_cos(normal, LIGHT_SOURCE);
                if (brightness >= 0) {
                    quaternion position = (quaternion) {
                        0,
                        RADIUS * (2 + cos_theta) * cos_phi,
                        RADIUS * (2 + cos_theta) * sin_phi,
                        RADIUS * sin_theta
                    };
                    position = apply_rotation(position, rotation);
                    const float z_buffer = position.z - 5*RADIUS;
                    const float z_inverse = 1 / (position.z + 5*RADIUS);
                    const uint16_t x = (uint16_t) (SCREEN_WIDTH * 0.5) * (1 + position.x * z_inverse);
                    const uint16_t y = (uint16_t) (SCREEN_HEIGHT * 0.5) * (1 + position.y * z_inverse);
                    if (z_buffer <= display_buffer[y][x]) {
                        display_buffer[y][x] = z_buffer;
                        display[y][x] = ".,-~:;!=*#$@"[brightness];
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
        roll += 0.005;
        pitch += 0.003;
        yaw += 0.002;
        memset(display_buffer, 0, sizeof(float) * SCREEN_HEIGHT * SCREEN_WIDTH);
    }
}
