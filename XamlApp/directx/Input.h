// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <created>ʆϒʅ,27.06.2020</created>
/// <state></state>
// ********************************************************************************

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
  Input ( void );
  //~Input ( void );

  const bool& Input::isInitialized ( void ) { return initialized; }; // get the initialized state
};


#endif // !INPUT_H
