// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <created>ʆϒʅ,27.06.2020</created>
/// <state></state>
// ********************************************************************************

#ifndef TIMER_H
#define TIMER_H


// Todo 
// timer event
//enum logType { start = 0, pause, reset };


// high-precision timer container
class Timer
{
private:
  // timing of the following is measured in -QPC units- known as counts
  uint64_t m_frequency; // current frequency (a constant counts per second across all processors)
  uint64_t m_deltaTime; // the automatically updated time between two frames (essential for a robust physics engine)
  uint64_t m_deltaTimeMax; // one tenth of the counts per a second (CPU)


  // timing of the following is measured in canonical tick format
  uint64_t m_timeStart; // application start time (queried at the game loop's start)
  uint64_t m_timeLastIdle; // the time of the last pause (user pauses, minimized, inactive)
  uint64_t m_timeTotalIdle; // a total of all idle time (paused, minimized, inactive)

  // frame rate tracking in counts
  uint64_t m_currentFrame; // the time of the current frame (current tick)
  uint64_t m_previousFrame; // the time of the previous frame (last tick)


  uint64_t m_elapsedTicks; // targeted elapsed ticks (frames)
  uint64_t m_totalTicks; // a total number of ticks (frames)
  int64_t m_leftOver; // leftover ticks (frames)


  uint32_t m_sleepFactor; // amount of time that main engine sleeps
  uint32_t m_split; // calculated split of sleep factor


  uint32_t m_framesCounter; // frame counter
  uint32_t m_FPS; // frames per second
  uint32_t m_framesThisSecond;
  uint64_t m_secondsCounter;

  // frame rate tracking in seconds
  double m_milliSPF; // render time of a frame in milliseconds
  double m_secondsPerCount; // frequency reciprocal (seconds per count)

  bool m_screenIt; // screen a frame only once a second

  bool m_initialized; // true if the initialization was successful
public:
  Timer ( void );
  //~Timer ( void );

  void m_tick ( void ); // the time never stops! (the time of every frame i.e. a cycle of game loop)
  void m_event ( const std::string& type ); // start, reset, stop
  const double m_getTotalTime ( void ); // total time minus the idle time

  const bool& m_isInitialized ( void ) { return m_initialized; }; // get the initialized state
  bool& m_isNewFrame ( void ) { return m_screenIt; }; // true if FPS got updated
  const double& m_getDeltaTime ( void ) { return m_deltaTime; }; // get the delta time
  uint32_t& const m_getFPS ( void ) { return m_FPS; }; // get frame per seconds
  double& const m_getMilliSPF ( void ) { return m_milliSPF; }; // get render time of a frame
};


#endif // !TIMER_H
