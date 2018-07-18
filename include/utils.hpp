#ifndef UTILS_H
#define UTILS_H

float random_float(float low, float high) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = high - low;
    float r = random * diff;
    return low + r;
}


#endif
