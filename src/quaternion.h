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

typedef struct {
    float w;
    float x;
    float y;
    float z;
} N(Quaternion);

float N(fast_inv_sqrt)(float number);

float N(dot)(const N(Quaternion) q1, const N(Quaternion) q2);

float N(magnitude)(const N(Quaternion) q);

float N(axial_cos)(const N(Quaternion) q1,
		const N(Quaternion) q2);

N(Quaternion) N(about_axis)(const N(Quaternion) axis,
		const float angle);

N(Quaternion) N(from_euler)(const N(Quaternion) angles);

N(Quaternion) N(conjugate)(const N(Quaternion) q);

N(Quaternion) N(hamilton)(const N(Quaternion) q1,
		const N(Quaternion) q2);

N(Quaternion) N(rotate)(const N(Quaternion) vector,
		const N(Quaternion) rotation);

#endif // QUATERNION_NAMESPACE

#ifdef __cplusplus
}
#endif

#endif // QUATERNION_H
