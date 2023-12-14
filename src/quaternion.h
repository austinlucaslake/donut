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

#ifndef QUATERNION_NAMESPACE
#define QUATERNION_NAMESPACE q_

#define C_(a,b) a##b
#define C(a,b) C_(a,b)
#define N(a) C(QUATERNION_NAMESPACE,a)

float N(dot)(const void * const v1, const constexpr size_t v1_dim, const constexpr size_t v1_type, const void * const v2, const constexpr size_t v2_dim, const constexpr size_t v2_type);

float N(magnitude)(const void * const vector, const constexpr size_t dim, const constexpr size_t type);

float N(axial_cos)(const void * const restrict v1, const constexpr size_t v1_dim, const constexpr size_t v1_type, const void * const restrict v2, const constexpr size_t v2_dim, const constexpr size_t v2_type);

float* N(about_axis)(const void * const axis, const constexpr size_t dim, const constexpr size_t type, const float angle);

float* N(from_euler)(const void * const angles, const constexpr size_t dim, const constexpr size_t type);

float* N(conjugate)(const void * const q, const constexpr size_t dim, const constexpr size_t type);

float* N(hamilton)(const void * const restrict q1, const constexpr size_t q1_dim, const constexpr size_t q1_type, const void * const restrict q2, const constexpr size_t q2_dim, const constexpr size_t q2_type);

void N(vec_from_quat)(void * restrict buff, const void * const restrict q, const constexpr size_t dim, const constexpr size_t type);

void N(rotate)(void * restrict v, const constexpr size_t v_dim, const constexpr size_t v_type, const void * const restrict q, const constexpr size_t q_dim, const constexpr size_t q_type);



#endif // QUATERNION_NAMESPACE

#ifdef __cplusplus
}
#endif

#endif // QUATERNION_H
