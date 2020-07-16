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

static unsigned long long start   = 0;
static unsigned long long end     = 0;
static unsigned long long elapse  = 0;
static unsigned long long telapse = 0;

#define PERF_START() \
    start = GetTickCountMicroSec();

#define PERF_END() \
    end      = GetTickCountMicroSec(); \
    elapse   = end - start; \
    telapse += elapse;
#define PERF_PROFILE(header) \
    LOGD("%10s: %lld ms\n", header, elapse);

#else /* !_PERF_ */

#define PERF_START()
#define PERF_END()
#define PERF_PROFILE(header)

#endif /* _PERF_ */

#endif /* __TIME_H__ */

/* EOF */

