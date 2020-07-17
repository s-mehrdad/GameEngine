﻿// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <created>ʆϒʅ,10.07.2020</created>
/// <state></state>
// ********************************************************************************

#ifndef MAINPAGETYPES_H
#define MAINPAGETYPES_H


class MainPageTypes
{
private:
  struct Display // resolution/orientation wrapper
  {
    float windowWidthDips; // current window width (Dips)
    float windowHeightDips; // current window height (Dips)
    uint32_t windowWidthPixels; // current window width (Pixels) (feed from configuration)
    uint32_t windowHeightPixels; // current window height (Pixels) (feed from configuration)

    float panelWidthDips; // current swap chain panel width (Dips)
    float panelHeightDips; // current swap chain panel height (Dips)
    uint32_t panelWidthPixels; // current swap chain panel width (Pixels)
    uint32_t panelHeightPixels; // current swap chain panel height (Pixels)

    bool fullscreen; // current full screen state (feed from configuration)

    float Dpi; // current DPI
    float compositionScaleX; // current composition scale
    float compositionScaleY; // current composition scale

    // high resolution considerations:
    // considering battery life on phones and having high resolutions supported
    float effectiveDpi; // effective DPI
    float effectiveCompositionScaleX; // effective composition scale
    float effectiveCompositionScaleY; // effective composition scale
    const bool supportHighResolution { false }; // a deliberate decision thus false
    const float DpiThreshold { 192.0f }; // 200% of standard thus 192.0f (phone considerations)

    winrt::Windows::Graphics::Display::DisplayOrientations orientationNative; // native orientation
    winrt::Windows::Graphics::Display::DisplayOrientations orientationCurrent; // current orientation

    D2D1::Matrix3x2F orientationTransform2D; // orientation 2D transform to render on rotated swap chain
    DirectX::XMFLOAT4X4 orientationTransform3D; // orientation 3D transform to render on rotated swap chain

    DXGI_MODE_ROTATION displayRotation; // calculated screen rotation to be set
    bool swapedDimensions; // true if dimensions are swapped.

    void updatePixels ( void ); // called after each input to Dips based dimensions
    void updateDips ( void ); // called after each input to Pixels based dimensions
    void computeRotation ( void ); // computation of rotation based on native and current orientation
  } m_display;

  winrt::hstring m_pointer;

public:
  MainPageTypes ();
  //~MainPageTypes ();

  Display* m_getDisplay ( void ) { return &m_display; };
  winrt::hstring& m_getPointer ( void ) { return m_pointer; };
};


namespace ScreenRotation
{
  static const DirectX::XMFLOAT4X4 Rotation0
  { 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f };
  static const DirectX::XMFLOAT4X4 Rotation90
  { 0.0f, 1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f };
  static const DirectX::XMFLOAT4X4 Rotation180
  { -1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f };
  static const DirectX::XMFLOAT4X4 Rotation270
  { 0.0f, -1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f };
};


#endif // !MAINPAGETYPES_H
