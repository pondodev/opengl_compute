#ifndef DAN_MATH_H
#define DAN_MATH_H

#define PI 3.14159265359
#define TAU PI*2

float lerp( float a, float b, float t ) {
    return a + t * (b - a);
}

float inverse_lerp( float a, float b, float val ) {
    return (val - a) / (b - a);
}

float remap( float in_min, float in_max, float out_min, float out_max, float val ) {
    float t = inverse_lerp( in_min, in_max, val );
    return lerp( out_min, out_max, t );
}

#endif
