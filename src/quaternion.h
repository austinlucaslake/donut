// Copyright (c) 2023 Austin Lucas Lake
// <53884490+austinlucaslake@users.noreply.github.com>
#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef QUATERNION_NAMESPACE
#define QUATERNION_NAMESPACE q_

#define C_(a,b) a##b
#define C(a,b) C_(a,b)
#define N(a) C(QUATERNION_NAMESPACE, a)

float N(inv_sqrt)(float number);

float N(dot)(const float * const q1, const float * const q2);

float N(magnitude)(const float * const q);

float N(axial_cos)(const float * const restrict q1,
		const float * const restrict q2);

float * N(about_axis)(const float * const axis,
		const float angle);

float * N(from_euler)(const float * const angles);

float * N(conjugate)(const float * const q);

float * N(hamilton)(const float * const restrict q1,
		const float * const restrict q2);

void N(rotate)(float * const restrict vector,
		const float * const restrict rotation);

#endif // QUATERNION_NAMESPACE

#ifdef __cplusplus
}
#endif

#endif // QUATERNION_H
