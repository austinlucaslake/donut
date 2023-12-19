// Copyright (c) 2023 Austin Lucas Lake
// <53884490+austinlucaslake@users.noreply.github.com>
#include "quaternion.h"

float q_inv_sqrt(float number) {
	union {
		float f;
    	uint32_t i;
    } conv = { .f = number };
    conv.i  = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5f - (number * 0.5f * conv.f * conv.f);
    return conv.f;
}

float q_dot(const float * const q1, const float * const q2) {
	float total = 0;
	for (size_t i = 0; i < 4; ++i) { total += *(q1+i) * *(q2+i); }
    return total;
}

float q_magnitude(const float * const q) {
    return sqrt(q_dot(q, q));
}

float q_axial_cos(const float * const restrict q1,
		const float * const restrict q2) {
    return q_dot(q1, q2) * q_inv_sqrt(q_dot(q1, q1) * q_dot(q2, q2));
}

float * q_about_axis(const float * const axis, const float angle) {
    const float half_angle = angle * 0.5f;
    const float sin_half_over_mag = sin(half_angle) * q_inv_sqrt(q_magnitude(axis));
	static float buffer[4];
	buffer[0] = cos(half_angle);
	for (size_t i = 0; i < 3; ++i) {
		buffer[i+1] = *(axis+i) * sin_half_over_mag;
	}
    return buffer;
}

float * q_from_euler(const float * const angles) {
    float half_angles[3];
    for (size_t i = 0; i < 3; ++i) { half_angles[i] = *(angles+i) * 0.5f; }
    const float sin_roll = sin(half_angles[0]);
    const float cos_roll = cos(half_angles[0]);
    const float sin_pitch = sin(half_angles[1]);
    const float cos_pitch = cos(half_angles[1]);
    const float sin_yaw = sin(half_angles[2]);
    const float cos_yaw = cos(half_angles[2]);
    static float buffer[4];
	buffer[0] = cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw;
	buffer[1] = sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw;
    buffer[2] = cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw;
    buffer[3] = cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw;
    return buffer;
}

float * q_conjugate(const float * const q) {
	static float buffer[4];
	buffer[0] = *q;
	for (size_t i = 1; i < 4; ++i) { buffer[i] = - *(q+i); }
    return buffer;
}

float * q_hamilton(const float * const restrict q1,
		const float * const restrict q2) {
	static float buffer[4];
	buffer[0] = *(q1+0) * *(q2+0)
		- *(q1+1) * *(q2+1)
		- *(q1+2) * *(q2+2)
		- *(q1+3) * *(q2+3);
    buffer[1] = *(q1+0) * *(q2+1)
		+ *(q1+1) * *(q2+0)
		+ *(q1+2) * *(q2+3)
		- *(q1+3) * *(q2+2);
    buffer[2] = *(q1+0) * *(q2+2)
		- *(q1+1) * *(q2+3)
		+ *(q1+2) * *(q2+0)
		+ *(q1+3) * *(q2+1);
    buffer[3] = *(q1+0) * *(q2+3)
		+ *(q1+1) * *(q2+2)
		- *(q1+2) * *(q2+1)
		+ *(q1+3) * *(q2+0);
	return buffer;
}

void q_rotate(float * const restrict vector,
		const float * const restrict rotation) {
    const float * const buffer = q_hamilton(q_hamilton(rotation, vector),
			q_conjugate(rotation));
	for (size_t i = 0; i < 4; ++i) { *(vector+i) = *(buffer+i); }
}
