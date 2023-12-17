// Copyright (c) 2023 Austin Lucas Lake
// <53884490+austinlucaslake@users.noreply.github.com>
#include "quaternion.h"

float inv_sqrt(float number) {
	union {
		float f;
    	uint32_t i;
    } conv = { .f = number };
    conv.i  = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5F - (number * 0.5f * conv.f * conv.f);
    return conv.f;
}

float dot(const Quaternion q1, const Quaternion q2) {
    return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

float magnitude(const Quaternion q) {
    return sqrt(dot(q, q));
}

float axial_cos(const Quaternion q1, const Quaternion q2) {
    return dot(q1, q2) * inv_sqrt(dot(q1, q1) * dot(q2, q2));
}

Quaternion about_axis(const Quaternion axis, const float angle) {
    const float half_angle = angle * 0.5f;
    const float sin_half_over_mag = sin(half_angle) * inv_sqrt(magnitude(axis));
    return (Quaternion) {
        cos(half_angle),
        sin_half_over_mag*axis.x,
        sin_half_over_mag*axis.y,
        sin_half_over_mag*axis.z,
    };
}

Quaternion from_euler(const float angles[3]) {
    float half_angles[3];
    for (size_t i = 0; i < 3; ++i) {
        half_angles[i] = angles[i] * 0.5f;
    }
    const float sin_roll = sin(half_angles[0]);
    const float cos_roll = cos(half_angles[0]);
    const float sin_pitch = sin(half_angles[1]);
    const float cos_pitch = cos(half_angles[1]);
    const float sin_yaw = sin(half_angles[2]);
    const float cos_yaw = cos(half_angles[2]);
    return (Quaternion) {
        cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw,
        sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw,
        cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw,
        cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw
    };
}

Quaternion conjugate(const Quaternion q) {
    return (Quaternion) { q.w, -q.x, -q.y, -q.z };
}

Quaternion hamilton(const Quaternion q1, const Quaternion q2) {
    return (Quaternion) {
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
    	q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
    	q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
    	q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
	};
}

Quaternion rotate(const Quaternion vector, const Quaternion rotation) {
    return hamilton(hamilton(rotation, vector), conjugate(rotation));
}
