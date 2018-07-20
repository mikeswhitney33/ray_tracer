#ifndef TIMING_H
#define TIMING_H

#ifdef __WIN32__
#include <Windows.h>
double get_wall_time() {
    LARGE_INTEGER time, freq;
    if(!QueryPerformanceFrequency(&freq)) {
        return 0;
    }
    if(!QueryPerformanceCounter(&time)) {
        return 0;
    }
    return (double) time.QuadPart / freq.QuadPart;
}

double get_cpu_time() {
    FILETIME a, b, c, d;
    if(GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
        return (double)(d.dwLowDateTime | ((unsigned long long) d.dwHighDataeTime << 32)) * 0.0000001;
    }
    else {
        return 0;
    }
}

#else
#include <time.h>
#include <sys/time.h>
double get_wall_time() {
    struct timeval time;
    if(gettimeofday(&time, NULL)) {
        return 0;
    }
    return (double)time.tv_sec + (double) time.tv_usec * 0.000001;
}
double get_cpu_time() {
    return (double) clock() / CLOCKS_PER_SEC;
}
#endif

void time_function(void (*f)(const char*), const char* arg, double(*timer)()) {
    double start_time = (*timer)();
    (*f)(arg);
    std::cout << "Function took: " << ((*timer)() - start_time) << " time units" << std::endl;
}

void time_function(void (*f)(void*, const char*), void* context, const char* arg, double(*timer)()) {
    double start_time = (*timer)();
    (*f)(context, arg);
    std::cout << "Function took: " << ((*timer)() - start_time) << " time units" << std::endl;
}

void print_time(double time_sec) {
    int total_sec = time_sec;
    double milli = time_sec - total_sec;
    int total_min = total_sec / 60;
    int total_hour = total_min / 60;
    int sec = total_sec % 60;
    int min = total_min % 60;
    std::cout << "<" << ((total_hour > 9)? "":"0") << total_hour << ":" << ((min > 9)? "":"0") << min << ":" << ((sec > 9)? "":"0") << sec << ">";
}

class ProgressBar {
    //time_left = (time_taken / _total_ticks) * (_total_ticks - _ticks_occured)
public:
    ProgressBar(int n_iters) {
        num_iters = n_iters;
        current_iter = 0;
        wall_start_time = get_wall_time();
        cpu_start_time = get_cpu_time();
    }
    double time_left(double time_take) {
        return (time_take / (double)num_iters) * ((double)num_iters - (double)current_iter);
    }
    void tick(bool progress=true) {
        double elapsed_wall_time = get_wall_time() - wall_start_time;
        double elapsed_cpu_time = get_cpu_time() - cpu_start_time;
        if(progress) current_iter++;
        std::cout << current_iter << "/" << num_iters << " -- ";
        std::cout << "Wall: ";
        print_time(elapsed_wall_time);
        std::cout << " -- CPU: ";
        print_time(elapsed_cpu_time);
        std::cout << "          \r" << std::flush;
    }
private:
    double wall_start_time;
    double cpu_start_time;
    int current_iter;
    int num_iters;
};

#endif
