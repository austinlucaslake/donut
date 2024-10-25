// Copyright (c) 2023 Austin Lucas Lake
// <53884490+austinlucaslake@users.noreply.github.com>
#include "quaternion.h"

float q_fast_inv_sqrt(float number) {
	union {
		float f;
    	uint32_t i;
    } conv = { .f = number };
    conv.i  = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5f - (number * 0.5f * conv.f * conv.f);
    return conv.f;
}

float q_dot(const q_Quaternion q1, const q_Quaternion q2) {
    return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

float q_magnitude(const q_Quaternion q) {
    return sqrt(q_dot(q, q));
}

float q_axial_cos(const q_Quaternion q1,
		const q_Quaternion q2) {
    return q_dot(q1, q2) * q_inv_sqrt(q_dot(q1, q1) * q_dot(q2, q2));
}

q_Quaternion q_about_axis(const q_Quaternion axis, const float angle) {
    const float sin_half_over_mag = sin(angle*0.5f) * q_fast_inv_sqrt(q_magnitude(axis));
	return (q_Quaternion) {
        .w = axis.w * sin_half_over_mag,
        .x = axis.x * sin_half_over_mag,
        .y = axis.y * sin_half_over_mag,
        .z = axis.z * sin_half_over_mag,
	}
}

q_Quaternion quaternion_from_angles(const float row, const float pitch, const float yaw) {
    const float sin_roll = sin(row*0.5f);
    const float cos_roll = cos(row*0.5f);
    const float sin_pitch = sin(pitch*0.5f);
    const float cos_pitch = cos(pitch*0.5f);
    const float sin_yaw = sin(yaw*0.5f);
    const float cos_yaw = cos(yaw*0.5f);
    return (q_Quaternion) {
        .w = cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw;
	    .x = sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw;
        .y = cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw;
        .z = cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw;
    };
}

q_Quaternion q_conjugate(const float * const q) {
	return (q_Quaternion) {
        .w = - q.w,
        .x = - q.x,
        .y = - q.y,
        .z = - q.z,
    }
}

q_Quaternion q_hamilton(const q_Quaternion q1,
		const q_Quaternion q2) {
	return (q_Quaternion) {
        .w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
        .x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
        .y = q1.w * q2.y + q1.x * q2.z + q1.y * q2.w - q1.z * q2.x;
        .z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    };
}

q_Quaternion q_rotate(const q_Quaternion vector,
		const q_Quaternion rotation) {
    return q_hamilton(q_hamilton(rotation, vector),
			q_conjugate(rotation));
}
