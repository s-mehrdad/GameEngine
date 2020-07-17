﻿// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <created>ʆϒʅ,27.06.2020</created>
/// <state></state>
// ********************************************************************************

#ifndef DIRECT2D_H
#define DIRECT2D_H


#include "Core.h"
#include "Direct3D.h"


// Direct2D wrapper
class Direct2D
{
  friend class TheCore;
  friend class Direct3D;
private:
  TheCore* m_core; // pointer to the application core

  winrt::com_ptr<IDWriteFactory1> m_writeFactory; // DirectWrite factory
  winrt::com_ptr<ID2D1Factory1> m_factory; // Direct2D factory
  winrt::com_ptr<ID2D1Device> m_device; // Direct2D device
  winrt::com_ptr<ID2D1DeviceContext> m_deviceContext; // Direct2D device context
  winrt::com_ptr<IDXGISurface1> m_dcBuffer; // Direct2D device context buffer
  winrt::com_ptr<ID2D1Bitmap1> m_dcBitmap; // render target bitmap (desired properties)

  winrt::com_ptr<ID2D1SolidColorBrush> m_brushYellow; // brushes
  winrt::com_ptr<ID2D1SolidColorBrush> m_brushWhite;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brushBlack;
  winrt::com_ptr<ID2D1SolidColorBrush> m_brushRed;

  winrt::com_ptr<IDWriteTextFormat> m_textFormatFPS; // text formats
  winrt::com_ptr<IDWriteTextFormat> m_textFormatLogs;
  winrt::com_ptr<IDWriteTextFormat> m_textFormatPointer;
  winrt::com_ptr<IDWriteTextLayout> m_textLayoutFPS; // text layouts
  winrt::com_ptr<IDWriteTextLayout> m_textLayoutLogs;
  winrt::com_ptr<IDWriteTextLayout> m_textLayoutPointer;
  bool m_textLayoutsDebug; // output prevention while updating

  bool m_allocated; // true if resources are allocated successful
  bool m_initialized; // true in case of successful initialization

  bool m_createResources ( void ); // create Direct2D independent resources
  bool m_createDeviceResources ( void ); // create Direct2D device dependent resources
  bool m_createDeviceContextResources ( void ); // create Direct2D device context dependent resources
  bool m_initializeTextFormats ( void ); // different formats initialization
  void m_release ( void ); // suspension preparation
public:
  Direct2D ( TheCore* coreObj );
  //~Direct2D ( void );

  void m_debugInfos ( void ); // FPS infos plus logs to screen

  const bool& m_isReady () { return m_allocated; }; // get the initialized state
};


#endif // !DIRECT2D_H
