#pragma once

#include <sys/time.h>
#include <android/log.h>

static struct timeval tv_begin;
static struct timeval tv_end;
static double elasped;

static void bench_start() {
    gettimeofday(&tv_begin, NULL);
}

static void bench_end(const char *comment) {
    gettimeofday(&tv_end, NULL);
    elasped = ((tv_end.tv_sec - tv_begin.tv_sec) * 1000000.0f + tv_end.tv_usec - tv_begin.tv_usec) /
              1000.0f;
    __android_log_print(ANDROID_LOG_DEBUG, "duration=", "%.2fms   %s", elasped, comment);
}

#ifdef __cplusplus
#endif