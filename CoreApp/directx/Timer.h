﻿
// ===========================================================================
/// <summary>
/// Timer.h
/// GameEngine
/// created by Mehrdad Soleimanimajd on 01.11.2019
/// </summary>
/// <created>ʆϒʅ, 01.11.2019</created>
/// <changed>ʆϒʅ, 05.07.2023</changed>
// ===========================================================================

#ifndef TIMER_H
#define TIMER_H


// high-precision timer container (time is measured in counts -QPC units-)
class Timer
{
private:
    // timing of the following is measured in QPC units
    uint64_t m_startTime; // application start time (start of the game loop)
    uint64_t m_lastStop; // the time of the last pause (user pauses, minimized, inactive)
    ///
    uint64_t m_frequency; // current frequency (constant counts per second)
    uint64_t m_lastTime; // queried time of last tick (frame)
    uint64_t m_deltaTime; // the automatically updated time between two frames (essential for a robust physics engine)
    uint64_t m_maxDeltaTime; // 1/10 of the counts within a second

    // timing of the following is measured in canonical tick format
    uint64_t m_totalTimeIdle; // a total of all idle time (paused, minimized, inactive)
    ///
    uint64_t m_elapsedTicks; // targeted elapsed ticks (frames)
    uint64_t m_totalTicks; // a total number of ticks (frames)
    int64_t m_leftOver; // leftover ticks (frames)

    // frame rate tracking in counts
    uint64_t m_currentFrameTime; // the time of the current frame
    uint64_t m_previousFrameTime; // the time of the previous frame
    ///
    uint32_t m_frameCounter; // wished frame count
    uint32_t m_FPS; // frames per second
    uint32_t m_framesThisSecond;
    uint64_t m_secondsCounter;

    // frame rate tracking in seconds
    double m_milliSPF; // render time of a frame in milliseconds
    double m_secondsPerCount; // frequency reciprocal (seconds per count)

    bool m_frameIt; // if true game engine screens a frame

    bool m_initialized; // true if the initialization was successful
public:
    Timer (void);
    //~Timer ( void );
    const bool& m_isInitialized (void); // get the initialized state

    void m_tick (void); // the time never stops! (the time of every frame i.e. a cycle of game loop)
    void m_event (const std::string& type); // start, reset, stop
    //const bool& m_isPaused ( void ); // get the paused state
    const double m_getTotalTime (void); // total time minus the idle time
    const double& m_getDeltaTime (void); // get the delta time
};


#endif // !TIMER_H
