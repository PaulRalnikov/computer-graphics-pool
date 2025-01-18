#pragma once
#include <chrono>

class FrameTimeObserver
{
public:
    FrameTimeObserver();

    // returs time elapsed from previous frame
    float start_frame();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> now;
};