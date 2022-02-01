#pragma once

#include <chrono>

#if defined(_WIN32)
#   include "phnt_windows.h"
#   include <phnt.h>
#elif defined(SLEEPY_HAS_NANOSLEEP)
#   include <time.h>
#elif defined(SLEEPY_HAS_USLEEP)
#   include <unistd.h>
#endif

#if defined(HAVE_PAUSE)
#   include <immintrin.h>
#endif

namespace sleepy {

template<typename Trep, typename Tperiod>
void sleep(std::chrono::duration<Trep, Tperiod> dur) {
    using namespace std::chrono;
    using clock = high_resolution_clock;
    const auto begin = clock::now();
    const auto sleepdur = dur * 0.9; // scale down dur to not overshoot
#if defined(_WIN32)
    const auto timer = CreateWaitableTimer(nullptr, true, nullptr);
    if (timer) {
        using us = duration<long long, std::micro>;
        LARGE_INTEGER wait { .QuadPart = -(duration_cast<us>(sleepdur).count() * 10) };
        SetWaitableTimer(timer, &wait, 0, nullptr, nullptr, false);
        unsigned long previous_resolution;
        NtSetTimerResolution(1, true, &previous_resolution);
        WaitForSingleObject(timer, INFINITE);
        NtSetTimerResolution(previous_resolution, true, &previous_resolution);
        CloseHandle(timer);
    }
#elif defined(SLEEPY_HAS_NANOSLEEP)
    using d_ns = duration<long, std::nano>;
    using d_tt = duration<time_t>;
    const auto s = seconds { 1 } <= dur
                 ? duration_cast<seconds>(sleepdur)
                 : seconds { 0 };
    const auto ns = dur - s;
    timespec t1 { duration_cast<d_tt>(s).count()
                , duration_cast<d_ns>(ns).count() };
    timespec t2;
    nanosleep(&t1, &t2);
#elif defined(SLEEPY_HAS_USLEEP)
    using d_us = duration<useconds_t, std::micro>;
    usleep(duration_cast<d_us>(sleepdur).count());
#endif
    while ((clock::now() - begin) < dur) {
#       if defined(SLEEPY_HAS_PAUSE)
            _mm_pause();
#       endif
    }
}

}