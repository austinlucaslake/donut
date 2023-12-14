// Copyright (c) 2023 Austin Lucas Lake
// <53884490+austinlucaslake@users.noreply.github.com>
#include "quaternion.h"

float __q_dot(const void* const v1, const constexpr size_t v1_dim, const constexpr size_t v1_type, const void* const v2, const constexpr size_t v2_dim, const constexpr size_t v2_type) {
    float total = 0;
    for (size_t i = 0; i < v1_dim && v1_dim == v2_dim; ++i) {
        total += *((float*)v1 + i*v1_type) * *((float*)v2 + i*v2_type);
    }
    return total;
}

float __q_magnitude(const void* const vector, const constexpr size_t dim, const constexpr size_t type) {
    return sqrt(q_dot(vector, dim, type, vector, dim, type));
}

float __q_axial_cos(const void* const restrict v1, const size_t v1_dim, const size_t v1_type, const void* const restrict v2, const size_t v2_dim, const size_t v2_type) {
    return q_dot(v1, v1_dim, v1_type, v2, v2_dim, v2_type) /
        (q_magnitude(v1, v1_dim, v1_type) * q_magnitude(v2, v2_dim, v2_type));
}

float* __q_about_axis(const void* const axis, const constexpr size_t dim, const constexpr size_t type, const float angle) {
    const float half_angle = angle * 0.5f;
    const float sin_half_over_mag = sin(half_angle) /
        q_magnitude(axis, dim, type);
    static float buff[4];
    for (size_t i = 0; i < dim; ++i) {
        *(buff+i+1) = *((float*)axis + i*type) * sin_half_over_mag;
    }
    buff[0] = cos(half_angle);
    return buff;
}

float* __q_from_euler(const void* const angles, const constexpr size_t dim, const constexpr size_t type) {
    if (dim==3) {
        float half_angles[3];
        for (size_t i = 0; i < dim; ++i) {
            half_angles[i] = *((float*)angles + i*type) * 0.5f;
        }
        const float sin_roll = sin(half_angles[dim - 3]);
        const float cos_roll = cos(half_angles[dim - 3]);
        const float sin_pitch = sin(half_angles[dim - 2]);
        const float cos_pitch = cos(half_angles[dim - 2]);
        const float sin_yaw = sin(half_angles[dim - 1]);
        const float cos_yaw = cos(half_angles[dim - 1]);
        static float buff[4] = {
            cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw,
            sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw,
            cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw,
            cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw
        };
        return buff;
    }
}

float* __q_conjugate(const void* const q, const constexpr size_t dim, const constexpr size_t type) {
    static float buff[4];
    for (size_t i = 0; i < dim - 1; ++i) {
        *(buff+i+1) = -*((float*)q + (i+1)*type); 
    }
    buff[0] = *((float*)q);
    return buff;
}

float* __q_hamilton(const void* const restrict q1, const constexpr size_t q1_dim, const constexpr size_t q1_type, const void* const restrict q2, const constexpr size_t q2_dim, const constexpr size_t q2_type) {
    if (q2_dim==4) {
        static float buff[4] = {
            *((float*)q1 + 0*q1_type) * *((float*)q2 + 0*q2_type)
                - *((float*)q1 + 1*q1_type) * *((float*)q2 + 1*q2_type)
                - *((float*)q1 + 2*q1_type) * *((float*)q2 + 2*q2_type)
                - *((float*)q1 + 3*q1_type) * *((float*)q2 + 3*q2_type),
            *((float*)q1 + 0*q1_type) * *((float*)q2 + 1*q2_type)
                + *((float*)q1 + 1*q1_type) * *((float*)q2 + 0*q2_type)
                + *((float*)q1 + 2*q1_type) * *((float*)q2 + 3*q2_type)
                - *((float*)q1 + 3*q1_type) * *((float*)q2 + 2*q2_type),
            *((float*)q1 + 0*q1_type) * *((float*)q2 + 2*q2_type)
                - *((float*)q1 + 1*q1_type) * *((float*)q2 + 3*q2_type)
                + *((float*)q1 + 2*q1_type) * *((float*)q2 + 0*q2_type)
                + *((float*)q1 + 3*q1_type) * *((float*)q2 + 1*q2_type),
            *((float*)q1 + 0*q1_type) * *((float*)q2 + 3*q2_type)
                + *((float*)q1 + 1*q1_type) * *((float*)q2 + 2*q2_type)
                - *((float*)q1 + 2*q1_type) * *((float*)q2 + 1*q2_type)
                + *((float*)q1 + 3*q1_type) * *((float*)q2 + 0*q2_type)
        };
        return buff;
    } else {
        static float buff[4] = {
            - *((float*)q1 + 1*q1_type) * *((float*)q2 + 1*q2_type)
                - *((float*)q1 + 2*q1_type) * *((float*)q2 + 2*q2_type)
                - *((float*)q1 + 3*q1_type) * *((float*)q2 + 3*q2_type),
            *((float*)q1 + 0*q1_type) * *((float*)q2 + 1*q2_type)
                + *((float*)q1 + 2*q1_type) * *((float*)q2 + 3*q2_type)
                - *((float*)q1 + 3*q1_type) * *((float*)q2 + 2*q2_type),
            *((float*)q1 + 0*q1_type) * *((float*)q2 + 2*q2_type)
                - *((float*)q1 + 1*q1_type) * *((float*)q2 + 3*q2_type)
                + *((float*)q1 + 3*q1_type) * *((float*)q2 + 1*q2_type),
            *((float*)q1 + 0*q1_type) * *((float*)q2 + 3*q2_type)
                + *((float*)q1 + 1*q1_type) * *((float*)q2 + 2*q2_type)
                - *((float*)q1 + 2*q1_type) * *((float*)q2 + 1*q2_type)
        };
        return buff;
    }
}

void __q_vec_from_quat(void * restrict buff, const void* const restrict q, const constexpr size_t dim, const constexpr size_t type) {
    for (size_t i = 0; i < dim - 1; ++i) {
        *((float*)buff + i*type)= *((float*)q + (i+1)*type);
    }
}

void __q_rotate(void* restrict v, const constexpr size_t v_dim, const constexpr size_t v_type, const void* const restrict q, const constexpr size_t q_dim, const constexpr size_t q_type) {
    q_vec_from_quat(v,
        q_hamilton(q_hamilton(q, q_dim, q_type, v, v_dim, v_type), q_dim, q_type
            q_conjugate(q, q_dim, q_type), q_dim, q_type), q_dim, q_type);
}
