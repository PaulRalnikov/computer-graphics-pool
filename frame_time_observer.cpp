#include "frame_time_observer.h"

FrameTimeObserver::FrameTimeObserver()
{
    now = std::chrono::high_resolution_clock::now();
}

float FrameTimeObserver::start_frame()
{
    auto new_now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration_cast<std::chrono::duration<float>>(new_now - now).count();
    now = new_now;
    return dt;
}