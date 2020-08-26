/******************************************************************************
 *      Copyright (c) 2017-2018 ULSee Corporation.
 *      All Rights Reserved.
 ******************************************************************************/
#ifndef __TIME_H__
#define __TIME_H__
#include "uls_infra_log.h"
#include <sys/time.h>

#ifdef _PERF_

static unsigned long long GetTickCountMicroSec(void)
{
    struct timeval g_tv;
    struct timezone g_tz;
    gettimeofday(&g_tv, &g_tz);
    return g_tv.tv_sec*1000 + g_tv.tv_usec/1000;
}

static unsigned long long start_time   = 0;
static unsigned long long end_time     = 0;
static unsigned long long elapse_time  = 0;
static unsigned long long telapse_time = 0;

#define PERF_START() \
    start_time = GetTickCountMicroSec();

#define PERF_END() \
    end_time      = GetTickCountMicroSec(); \
    elapse_time   = end_time - start_time; \
    telapse_time += elapse_time;
#define PERF_PROFILE(header) \
    LOGD("%10s: %lld ms\n", header, elapse_time);

#else /* !_PERF_ */

#define PERF_START()
#define PERF_END()
#define PERF_PROFILE(header)

#endif /* _PERF_ */

#endif /* __TIME_H__ */

/* EOF */

