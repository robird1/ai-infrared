/******************************************************************************
 *      Copyright (c) 2017-2018 ULSee Corporation.
 *      All Rights Reserved.
 ******************************************************************************/
#ifndef __TIME_H__
#define __TIME_H__

#ifdef _PERF_

#include "uls_infra_log.h"
#include <chrono>

static std::chrono::system_clock::time_point getTimePoint(void)
{
    const std::chrono::system_clock::time_point cur_time_point = std::chrono::system_clock::now();
    return cur_time_point;
}

static std::chrono::system_clock::time_point start_time;
static std::chrono::system_clock::time_point end_time;
static std::chrono::duration<double> elapse_time;

#define PERF_START() \
    start_time = getTimePoint();

#define PERF_END() \
    end_time      = getTimePoint(); \
    elapse_time   = end_time - start_time;

#define PERF_PROFILE(header) \
    LOGD("%10s: %.3f ms\n", header, elapse_time.count()*1000.0f);

#else /* !_PERF_ */

#define PERF_START()
#define PERF_END()
#define PERF_PROFILE(header)

#endif /* _PERF_ */

#endif /* __TIME_H__ */

/* EOF */

