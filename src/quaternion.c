#ifndef QUATERNION_C
#define QUATERNION_C

#include <math.h>
#include "quaternion.h"

quaternion conjugate(const quaternion q) {
    return (quaternion) { q.w, -q.x, -q.y, -q.z };
}

float dot_product(const quaternion q1, const quaternion q2) {
    return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

float magnitude(const quaternion v) {
    return sqrt(dot_product(v, v));
}

float axial_cos(const quaternion q1, const quaternion q2) {
    return dot_product(q1, q2) / (magnitude(q1) * magnitude(q2));
}

quaternion hamilton_product(const quaternion q1, const quaternion q2) {
    return (quaternion) {
        q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
        q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
        q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
        q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
    };
}

quaternion rotate_about_axis(const quaternion axis, const float angle) {
    return (quaternion) {
        cos(angle * 0.5),
        sin(angle * 0.5)*axial_cos(axis, X_AXIS),
        sin(angle * 0.5)*axial_cos(axis, Y_AXIS),
        sin(angle * 0.5)*axial_cos(axis, Z_AXIS),
    };
}

quaternion from_euler(const float roll, const float pitch, const float yaw) {
    const float sin_roll = sin(roll * 0.5);
    const float cos_roll = cos(roll * 0.5);
    const float sin_pitch = sin(pitch * 0.5);
    const float cos_pitch = cos(pitch * 0.5);
    const float sin_yaw = sin(yaw * 0.5);
    const float cos_yaw = cos(yaw * 0.5);
    return (quaternion) {
        cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw,
        sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw,
        cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw,
        cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw
    };
}

quaternion apply_rotation(const quaternion vector, const quaternion rotation) {
    return hamilton_product(hamilton_product(rotation, vector), conjugate(rotation));
}

#endif
