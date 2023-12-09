#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define X_AXIS (quaternion) { 0, 1, 0, 0 }
#define Y_AXIS (quaternion) { 0, 0, 1, 0 }
#define Z_AXIS (quaternion) { 0, 0, 0, 1 }

typedef struct {
    float w;
    float x;
    float y;
    float z;
} quaternion;

quaternion conjugate(const quaternion q);

float dot_product(const quaternion q1, const quaternion q2);

float magnitude(const quaternion v);

float axial_cos(const quaternion q1, const quaternion q2);

quaternion hamilton_product(const quaternion q1, const quaternion q2);

quaternion rotate_about_axis(const quaternion axis, const float angle);

quaternion from_euler(const float roll, const float pitch, const float yaw);

quaternion apply_rotation(const quaternion vector, const quaternion rotation);

#endif
