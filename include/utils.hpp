#ifndef UTILS_H
#define UTILS_H

float randf(float low, float high) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = high - low;
    float r = random * diff;
    return low + r;
}

float randf(float high) {
    return randf(0, high);
}

float randf() {
    return randf(0, 1);
}


#endif
