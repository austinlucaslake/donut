#include <math.h>
#include <stdio.h>
#include <stdlibh>
#include <string.h>

#define x_axis {0,1,0,0}
#define y_axis {0,0,1,0}
#define z_axis {0,0,0,1}

typedef struct {
	float w;
	float x;
	float y;
	float z;
} quaternion;

quaternion conjugate(const quaternion q) {
	return {q.w, -q.x, -q.y, -q.z};
}

float dot_product(const quaternion q1, const quaternion q2) {
	return q1.w*q2.w+q1.x*q2.x+q1.y*q2.y+q1.z*q2.z;
}

float magnitude(const quaternion q) {
	return sqrt(dot_product(q,q));
}

float axial_cos(const quaternion& q1, const quaternion& q2) {
	return dot_product(q1,q2)/(magnitude(q1)*magnitude(q2));
}

quaternion multiply(const quaternion q1, const quaternion q2) {
	return {q1.w*q2.w-q1.x*q2.x-q1.y*q2.y-q1.z*q2.z,
			q1.w*q2.x+q1.x*q2.w+q1.y*q2.z-q1.z*q2.y,
			q1.w*q2.y-q1.x*q2.z+q1.y*q2.w+q1.z*q2.x,
			q1.w*q2.z+q1.x*q2.y-q1.y*q2.x+q1.z*q2.w};
}

const quaternion from_euler(const quaternion& axis, const float& angle) {
	float half_angle = angle/2;
	return {cos(half_angle),
			sin(half_angle)*axial_cos(axis,x_axis),
			sin(half_angle)*axial_cos(axis,y_axis),
			sin(half_angle)*axial_cos(axis,z_axis)};
}

quaternion rotate(const quaternion& vector, const quaternion& rotation) {
	return multiply(multiply(rotation, vector), conjugate(rotation));
}

int main(void) {
	const float two_pi = 2*M_PI, max_shift = 0.005;
    const int radius = 8;
    const int display_size = 5*radius, radius_squared = radius*radius;
    const float delta_theta = two_pi/100, delta_phi = two_pi/100;
    const quaternion light_source = {0,0,0,-1};
    float x_angle = 0, y_angle = 0, z_angle = 0;
    for (;;) {
        float display_buffer[display_size][display_size] = {};
        memset(display_buffer, float(display_size), sizeof display_buffer);
        char display[display_size][display_size];
        memset(display, ' ', sizeof display);
        const quaternion rotation = multiply(multiply(from_euler(x_axis,x_angle),from_euler(y_axis,y_angle)),from_euler(z_axis,z_angle));
        for (float theta = 0; theta < two_pi; theta+=delta_theta) {
            const float sin_theta = sin(theta), cos_theta = cos(theta);
            const float z = radius*sin_theta, rad_two_plus_cos_theta = radius*(2+cos_theta);
            const float rad_sq_two_plus_cos_theta = radius*rad_two_plus_cos_theta;
            for (float phi = 0; phi < two_pi; phi+=delta_phi) {
                const float sin_phi = sin(phi),
                            cos_phi = cos(phi);
				quaternion normal = {0,cos_phi*cos_theta*rad_sq_two_plus_cos_theta,powf(cos_phi,3)*radius_squared,(sin_theta*sin_phi+cos_theta*cos_phi)*rad_sq_two_plus_cos_theta};
                normal = rotate(normal,rotation);
                const int brightness = int(8*sqrt(2)*axial_cos(normal,light_source));
                if (brightness >= 0) {
					quaternion position = {0,rad_two_plus_cos_theta*cos_phi,rad_two_plus_cos_theta*sin_phi,z};
                    position = rotate(position,rotation);
					const float z_buffer = position.z+display_size;
                    const int half_display = display_size/2;
                    const float z_inverse = 1/(z_buffer);
                    const int x = int(half_display*(1+position.x*z_inverse)), y = int(half_display*(1+position.y*z_inverse));
                    if (position.z <= display_buffer[y][x]) {
                        display_buffer[y][x] = position.z;
                        display[y][x] = ".,-~:;!=*#$@"[brightness];
                    }
                }             
            }
        }
        for (int i = 0; i < display_size; ++i) {
            for (int j = 0; j < display_size; ++j) {
                putchar(display[i][j]);
                putchar(' ');
            }
            printf("\n");
        }
        printf("\033[H");
        x_angle += float(rand()/(RAND_MAX/max_shift));
        y_angle += float(rand()/(RAND_MAX/max_shift));
        z_angle += float(rand()/(RAND_MAX/max_shift));
    }

}
