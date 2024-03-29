﻿
// ===========================================================================
/// <summary>
/// Input.h
/// GameEngine
/// created by Mehrdad Soleimanimajd on 27.06.2020
/// </summary>
/// <created>ʆϒʅ, 27.06.2020</created>
/// <changed>ʆϒʅ, 06.07.2023</changed>
// ===========================================================================

#ifndef INPUT_H
#define INPUT_H


// DirectX Input wrapper
class Input
{
private:
    /*winrt::com_ptr<IDirectInput8> m_dInput;
    winrt::com_ptr<IDirectInputDevice8> m_keyboard;
    winrt::com_ptr<IDirectInputDevice8> m_mouse;*/

    //DIMOUSESTATE;

    bool initialized; // true if initialization was successful
public:
    Input (void);
    //~Input ( void );

    const bool& Input::isInitialized (void) { return initialized; }; // get the initialized state
};


#endif // !INPUT_H
