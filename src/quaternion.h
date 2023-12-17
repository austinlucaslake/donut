// Copyright (c) 2023 Austin Lucas Lake
// <53884490+austinlucaslake@users.noreply.github.com>
#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Quaternion {
	float w;
	float x;
	float y;
	float z;
} Quaternion;

float inv_sqrt(float number);

float dot(const Quaternion q1, const Quaternion q2);

float magnitude(const Quaternion q);

float axial_cos(const Quaternion q1, const Quaternion q2);

Quaternion about_axis(const Quaternion axis, const float angle);

Quaternion from_euler(const float angles[3]);

Quaternion conjugate(const Quaternion q);

Quaternion hamilton(const Quaternion q1, const Quaternion q2);

Quaternion rotate(const Quaternion vector, const Quaternion rotation);

#ifdef __cplusplus
}
#endif

#endif // QUATERNION_H
