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

#define X_AXIS (quaternion) { 0.0f, 1.0f, 0.0f, 0.0f }
#define Y_AXIS (quaternion) { 0.0f, 0.0f, 1.0f, 0.0f }
#define Z_AXIS (quaternion) { 0.0f, 0.0f, 0.0f, 1.0f }
#define LIGHT_SOURCE (quaternion) { 0.0f, 0.0f, 1.0f, -1.0f }

typedef struct {
    float w;
    float x;
    float y;
    float z;
} quaternion;

static inline quaternion conjugate(const quaternion q) {
    return (quaternion) { q.w, -q.x, -q.y, -q.z };
}

static inline float dot_product(const quaternion q1, const quaternion q2) {
    return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

static inline float magnitude(const quaternion v) {
    return sqrt(dot_product(v, v));
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

static quaternion from_euler(const float roll, const float pitch, const float yaw) {
    const float sin_roll = sin(roll * 0.5f);
    const float cos_roll = cos(roll * 0.5f);
	const float sin_pitch = sin(pitch * 0.5f);
    const float cos_pitch = cos(pitch * 0.5f);
	const float sin_yaw = sin(yaw * 0.5f);
    const float cos_yaw = cos(yaw * 0.5f);
    return (quaternion) {
        cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw,
        sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw,
        cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw,
        cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw
    };
}

static quaternion apply_rotation(const quaternion vector, const quaternion rotation) {
    return hamilton_product(hamilton_product(rotation, vector), conjugate(rotation));
}

int main(void) {
    float roll = 0.0f;
    float pitch = 0.0f;
	float yaw = 0.0f;
    float display_buffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {};
    char display[SCREEN_HEIGHT][SCREEN_WIDTH];
    for (;;) {
        memset(display, ' ', sizeof(char) * SCREEN_HEIGHT * SCREEN_WIDTH);
        const quaternion rotation = from_euler(roll, pitch, yaw);
        for (float theta = 0.0f; theta < TWO_PI; theta += 0.05f) {
            const float sin_theta = sin(theta);
            const float cos_theta = cos(theta);
            for (float phi = 0.0f; phi < TWO_PI; phi += 0.05f) {
                const float sin_phi = sin(phi);
                const float cos_phi = cos(phi);
                quaternion normal = (quaternion) {
					0.0f,
                    RADIUS * RADIUS * (2.0f + cos_theta) * cos_phi,
                    RADIUS * RADIUS * cos_phi * cos_phi * cos_phi,
                    RADIUS * RADIUS * (2.0f + cos_theta) * (sin_theta * sin_phi + cos_theta * cos_phi)
                };
                normal = apply_rotation(normal, rotation);
                const int8_t brightness = (int8_t) 8.0f * sqrt(2) * axial_cos(normal, LIGHT_SOURCE);
                if (brightness >= 0) {
                    quaternion position = (quaternion) {
						0.0f,
                        RADIUS * (2.0f + cos_theta) * cos_phi,
                        RADIUS * (2.0f + cos_theta) * sin_phi,
                        RADIUS * sin_theta
                    };
                    position = apply_rotation(position, rotation);
                    const float z_buffer = position.z - 5.0f*RADIUS;
					const float z_inverse = 1.0f / (position.z + 5.0f*RADIUS);
                    const uint16_t x = (uint16_t) (SCREEN_WIDTH * 0.5f) * (1.0f + position.x * z_inverse);
                    const uint16_t y = (uint16_t) (SCREEN_HEIGHT * 0.5f) * (1.0f + position.y * z_inverse);
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
        roll += 0.005f;
        pitch += 0.003f;
		yaw += 0.002f;
        memset(display_buffer, 0, sizeof(float) * SCREEN_HEIGHT * SCREEN_WIDTH);
    }
}
