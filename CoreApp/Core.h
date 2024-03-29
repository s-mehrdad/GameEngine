﻿
// ===========================================================================
/// <summary>
/// Core.h
/// GameEngine
/// created by Mehrdad Soleimanimajd on 01.11.2019
/// </summary>
/// <created>ʆϒʅ, 01.11.2019</created>
/// <changed>ʆϒʅ, 05.07.2023</changed>
// ===========================================================================

#ifndef CORE_H
#define CORE_H


#include "directx/Timer.h"
#include "directx/Direct3D.h"
#include "directx/Direct2D.h"


// game engine framework wrapper
class TheCore
{
    friend class Direct3D;
    friend class Direct2D;
private:
    ::IUnknown* m_appWindow; // pointer to application main window
    int m_outputWidth; // current output width
    int m_outputHeight; // current output height
    // rotation

    Timer* m_timer; // pointer to DirectX high-precision timer application
    int m_FPS; // frames per second
    double m_milliSPF; // render time of a frame in milliseconds

    Direct3D* m_D3D; // pointer to Direct3D application
    Direct2D* m_D2D; // pointer to Direct2D application

    bool m_debug; // if true FPS to screen (true in debug mode)
    //bool resized; // true if the resizing was successful
    bool m_initialized; // true if the initialization was successful
public:
    TheCore (::IUnknown*, const int&, const int&); // constructor and initializer
    //~TheCore ( void );
    const bool& m_isInitialized (void); // get the initialized state

    const bool& m_isDebugging (void); // get current state of debug flag
    const ::IUnknown* m_getWindow (void); // get the main window
    Timer* m_getTimer (void); // get the pointer to application timer
    Direct3D* m_getD3D (void); // get the pointer to application Direct3D
    Direct2D* m_getD2D (void); // get the pointer to application Direct2D
    const int& m_getFPS (void); // get the FPS
    void m_frameStatistics (void); // frame statistics calculations
    void m_setResolution (const bool&, const int& = 0, const int& = 0); // resolution changer
    void m_resizeResources (const bool&); // free and resize the resources
    void m_onSuspending (void); // suspension preparations
    void m_validate (void); // validate the correct state of game resources
    void m_onDeviceLost (void); // clean and reallocate
};


#endif // !CORE_H
