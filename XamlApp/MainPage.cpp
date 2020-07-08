// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <created>ʆϒʅ,27.06.2020</created>
/// <state></state>
// ********************************************************************************

#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"


using namespace winrt;
using namespace Windows::UI::Xaml;


namespace winrt::GameEngine::implementation
{

  MainPage::MainPage () :
    m_appWindow ( nullptr ), m_visible ( false ), m_inResizeMove ( false ),
    m_core ( nullptr ), m_game ( nullptr ),
    m_inputLoop ( nullptr ), m_inputCore ( nullptr ),
    m_initialized ( false ), test ( L"" )
  {
    InitializeComponent ();

    // Register event handlers for page life cycle and keep a pointer to it.
    m_appWindow = Window::Current ().CoreWindow ();

    m_appWindow.get ().PointerCursor ( winrt::Windows::UI::Core::CoreCursor
                                       { winrt::Windows::UI::Core::CoreCursorType::Arrow, 0 } );

    // response to window size events
    m_appWindow.get ().SizeChanged ( { this, &MainPage::m_onWindowResized } );

#if defined (NTDDI_WIN10_RS2) && (NTDDI_VERSION >= NTDDI_WIN10_RS2)
    try
    {
      m_appWindow.get ().ResizeStarted ( [this]( auto&&, auto&& ) { m_inResizeMove = true; } );
      m_appWindow.get ().ResizeCompleted ( [this]( auto&&, auto&& ) { m_inResizeMove = false; } );
    }
    catch (...) {}
#endif // requires Windows 10 Creators Update (10.0.15063) or later

    // response to window focus events
    m_appWindow.get ().Activated ( { this, &MainPage::m_onFocused } );

    // response to window visibility events
    m_appWindow.get ().VisibilityChanged ( { this, &MainPage::m_onVisibilityChanged } );

    //auto dispatcher = CoreWindow
    m_appWindow.get ().Dispatcher ().AcceleratorKeyActivated ( { this, &MainPage::m_onAcceleratorKeyActivated } );

    // response to window close events // because of suspension procedure, following event rarely occurs.
    //m_appWindow.get ().Closed ( [this]( auto&&, auto&& ) { PointerProvider::getVariables ()->running = false; } );

    // response to client display DPI events
    auto currentDisplayInfos = Windows::Graphics::Display::DisplayInformation::GetForCurrentView ();
    currentDisplayInfos.DpiChanged ( { this, &MainPage::m_onDpiChanged } );

    currentDisplayInfos.OrientationChanged ( { this, &MainPage::m_onOrientationChanged } );

    Windows::Graphics::Display::DisplayInformation::DisplayContentsInvalidated ( { this, &MainPage::m_onDisplayContentInvalidated } );

    m_appWindow.get ().PointerMoved ( { this, &MainPage::m_onPointerMoved2 } );

    // current display DPI
    m_display.Dpi = Windows::Graphics::Display::DisplayInformation::GetForCurrentView ().LogicalDpi ();

    // current window size
    m_display.windowWidthDips = static_cast<float> (PointerProvider::getConfiguration ()->m_getSettings ().Width);
    m_display.windowHeightDips = static_cast<float> (PointerProvider::getConfiguration ()->m_getSettings ().Height);
    auto size = winrt::Windows::Foundation::Size ( m_display.windowWidthDips, m_display.windowHeightDips );

    // Debug problem
    // current swap chain panel size
    m_display.panelWidthDips = swapchainpanel ().RenderSize ().Width;
    m_display.panelHeightDips = swapchainpanel ().RenderSize ().Height;

    // Todo 
    // user settings
    m_display.updatePixels ();
    m_display.fullscreen = PointerProvider::getConfiguration ()->m_getSettings ().fullscreen;

    auto view = winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView ();
    if (m_display.fullscreen)
    {

      if (view.IsFullScreen ())
        view.ExitFullScreenMode ();
      else
        view.TryEnterFullScreenMode ();
    }

    //winrt::Windows::UI::ViewManagement::ApplicationView::PreferredLaunchWindowingMode ( ApplicationViewWindowingMode::PreferredLaunchViewSize );
    //winrt::Windows::UI::ViewManagement::ApplicationView::PreferredLaunchViewSize ( size );
    view.PreferredLaunchWindowingMode ( winrt::Windows::UI::ViewManagement::ApplicationViewWindowingMode::Auto );
    view.PreferredLaunchViewSize ( size );

    // minimum client window size
    size.Width = 640.0f; size.Height = 480.0f;
    view.SetPreferredMinSize ( size );

    // set and save
    view.FullScreenSystemOverlayMode ( winrt::Windows::UI::ViewManagement::FullScreenSystemOverlayMode::Minimal );
    view.TryResizeView ( view.PreferredLaunchViewSize () );

    swapchainpanel ().CompositionScaleChanged ( { this, &MainPage::m_onCompositionScaleChanged } );

    swapchainpanel ().SizeChanged ( { this, &MainPage::m_onSwapChainPanelSizeChanged } );

    m_initialized = true;



    // application core instantiation
    m_core = new (std::nothrow) TheCore ( m_display.windowWidthPixels, m_display.windowHeightPixels );

    if (!m_core->m_isInitialized ())
    {
      PointerProvider::getFileLogger ()->m_push ( logType::error, std::this_thread::get_id (), "mainThread",
                                                  "Initialization of application core failed!" );
    } else
    {

      // game instantiation
      m_game = std::make_unique<Game> ( m_core );

      if (!m_game->m_isReady ())
      {
        m_game->m_onSuspending (); // failure, shut the application down properly.

        PointerProvider::getFileLogger ()->m_push ( logType::error, std::this_thread::get_id (), "mainThread",
                                                    "The game initialization failed!" );

        //MessageBoxA ( NULL, "The Game functionality failed to start...", "Critical-Error", MB_OK | MB_ICONERROR );

        //failure = true;
      } else
      {
        PointerProvider::getVariables ()->currentState = "initialized";

        PointerProvider::getFileLogger ()->m_push ( logType::warning, std::this_thread::get_id (), "mainThread",
                                                    "Entering the game loop..." );
        // Todo 
        // reconstruction:
        // add window dependent concepts (note: pass to app core)
        //
        // set swap chain panel independently (adjusting the logical size)
        // run game on thread
        // if no errors adaptation of Microsoft template
      }

    }



    // register swap chain panel to receive independent input events on pointer device
    auto workItemHandler =
      winrt::Windows::System::Threading::WorkItemHandler
      ( [this]( winrt::Windows::Foundation::IAsyncAction )
        {

          // procedure will raise pointer events for specified devices on eider thread it is created on
          m_inputCore = swapchainpanel ().CreateCoreIndependentInputSource (
            winrt::Windows::UI::Core::CoreInputDeviceTypes::Mouse |
            winrt::Windows::UI::Core::CoreInputDeviceTypes::Touch |
            winrt::Windows::UI::Core::CoreInputDeviceTypes::Pen
          );

          // pointer events registration, raised on background thread
          m_inputCore.PointerPressed ( winrt::Windows::Foundation::TypedEventHandler
                                       <Windows::Foundation::IInspectable, winrt::Windows::UI::Core::PointerEventArgs>
                                       ( this, &MainPage::m_onPointerPressed ) );
          m_inputCore.PointerMoved ( winrt::Windows::Foundation::TypedEventHandler
                                     <Windows::Foundation::IInspectable, winrt::Windows::UI::Core::PointerEventArgs>
                                     ( this, &MainPage::m_onPointerMoved ) );
          m_inputCore.PointerReleased ( winrt::Windows::Foundation::TypedEventHandler
                                        <Windows::Foundation::IInspectable, winrt::Windows::UI::Core::PointerEventArgs>
                                        ( this, &MainPage::m_onPointerReleased ) );

          // process input messages upon their deliverances
          m_inputCore.Dispatcher ().ProcessEvents ( Windows::UI::Core::CoreProcessEventsOption::ProcessUntilQuit );

        } );
    // run the task on a high priority background thread
    m_inputLoop = winrt::Windows::System::Threading::ThreadPool::RunAsync (
      workItemHandler,
      winrt::Windows::System::Threading::WorkItemPriority::High,
      winrt::Windows::System::Threading::WorkItemOptions::TimeSliced );



    m_game->m_getCore ()->m_setSwapChainPanel ( &swapchainpanel () );

    winrt::Windows::System::Threading::ThreadPool::RunAsync (
      winrt::Windows::System::Threading::WorkItemHandler
      ( [this]( winrt::Windows::Foundation::IAsyncAction )
        {

          if (!m_game->m_run ())
          {
            //failure = true;
            PointerProvider::getFileLogger ()->m_push ( logType::warning, std::this_thread::get_id (), "mainThread",
                                                        "One or more errors occurred while running the game!" );
          }

        } ),
      winrt::Windows::System::Threading::WorkItemPriority::High,
          winrt::Windows::System::Threading::WorkItemOptions::TimeSliced );



    PointerProvider::getVariables ()->running = true;

  }


  //MainPage::~MainPage ()
  //{
  //  // tear down
  //}


  int32_t MainPage::MyProperty ()
  {
    throw hresult_not_implemented ();
  }
  void MainPage::MyProperty ( int32_t /* value */ )
  {
    throw hresult_not_implemented ();
  }


  MainPage::Display::Display ()
  {
    Dpi = 96.f;
    windowWidthDips = 0.0f;
    windowHeightDips = 0.0f;
    windowWidthPixels = 0;
    windowHeightPixels = 0;
    panelWidthDips = 0.0f;
    panelHeightDips = 0.0f;
    panelWidthPixels = 0;
    panelHeightPixels = 0;
    fullscreen = false;
  };
  void MainPage::Display::updatePixels ()
  {
    // to pixels ( dips * c_DPI / 96.f + 0.5f )
    windowWidthPixels = static_cast<int>(windowWidthDips * Dpi / 96.f + 0.5f);
    windowHeightPixels = static_cast<int>(windowHeightDips * Dpi / 96.f + 0.5f);
    panelWidthPixels = static_cast<int>(panelWidthDips * Dpi / 96.f + 0.5f);
    panelHeightPixels = static_cast<int>(panelHeightDips * Dpi / 96.f + 0.5f);
  };
  void MainPage::Display::updateDips ()
  {
    // to dips ( float (pixels) * 96.f / c_DPI )
    windowWidthDips = (static_cast<float> (windowWidthPixels) * 96.f / Dpi);
    windowHeightDips = (static_cast<float> (windowHeightPixels) * 96.f / Dpi);
    panelWidthDips = (static_cast<float> (panelWidthPixels) * 96.f / Dpi);
    panelHeightDips = (static_cast<float> (panelHeightPixels) * 96.f / Dpi);
  };


  void MainPage::m_onRendering ( Windows::Foundation::IInspectable const& sender,
                                 Windows::Foundation::IInspectable const& args )
  {
    //
  };


  void MainPage::m_onActivated ( Windows::UI::Core::CoreWindow const& sender,
                                 Windows::UI::Core::WindowActivatedEventArgs const& args )
  {
    Window::Current ().Activate ();
  };


  void MainPage::m_onFocused ( Windows::UI::Core::CoreWindow const& sender,
                               Windows::UI::Core::WindowActivatedEventArgs const& args )
  {
    if (Window::Current ().Visible ())
    {
      m_game->m_isPaused () = false;
      m_game->m_getCore ()->m_getTimer ()->m_event ( "start" );
    } else
    {
      m_game->m_isPaused () = true;
      m_game->m_getCore ()->m_getTimer ()->m_event ( "pause" );
    }
  };


  void MainPage::m_onWindowResized ( Windows::Foundation::IInspectable const& sender,
                                     Windows::UI::Core::WindowSizeChangedEventArgs const& args )
  {
    m_display.windowWidthDips = Window::Current ().Bounds ().Width;
    m_display.windowHeightDips = Window::Current ().Bounds ().Height;
    m_display.updatePixels ();

    // Todo: drag and drop resizing (resolution needs to be sent)
    //if (args.Handled () && m_initialized)
    //{
    //  m_game->m_getCore ()->m_setResolution ( false,
    //                                          m_resolution.m_currentWidth,
    //                                          m_resolution.m_currentHeight );
    //}
  };


  void MainPage::m_onVisibilityChanged ( Windows::Foundation::IInspectable const& sender,
                                         Windows::UI::Core::VisibilityChangedEventArgs const& args )
  {
    //m_visible = Window::Current ().Visible ();
    m_visible = args.Visible ();

    //if (m_visible)
    //{
    //  m_game->m_isPaused () = false;
    //  m_game->m_getCore ()->m_getTimer ()->m_event ( "start" );
    //  startRenderLoop();
    //} else
    //{
    //  m_game->m_isPaused () = true;
    //  m_game->m_getCore ()->m_getTimer ()->m_event ( "pause" );
    //  stopRenderLoop();
    //}
  };


  void MainPage::m_onDpiChanged ( Windows::Graphics::Display::DisplayInformation const& sender,
                                  Windows::Foundation::IInspectable const& args )
  {
    //concurrency::critical_section::scoped_lock lock ( m_core->getCriticalSection () );
    // note that below acquired Dpi may not match the app's effective Dpi, when it is scaled for high resolution devices
    // therefore the effective one is retrievable upon logical one being set on device resources
    //m_core->m_getD3D ()->setDpi ( sender.LogicalDpi () );
    //m_core->createWindowSizeDependentResources (); // matrices
    m_display.Dpi = sender.LogicalDpi ();
  };


  void MainPage::m_onOrientationChanged ( Windows::Graphics::Display::DisplayInformation const& sender,
                                          Windows::Foundation::IInspectable const& args )
  {
    //concurrency::critical_section::scoped_lock lock ( m_core->getCriticalSection () );
    //m_core->m_getD3D ()->setCurrentOrientation ( sender.CurrentOrientation () );
    //m_core->createWindowSizeDependentResources (); // matrices
  };


  void MainPage::m_onDisplayContentInvalidated ( Windows::Graphics::Display::DisplayInformation const& sender,
                                                 Windows::Foundation::IInspectable const& args )
  {
    //concurrency::critical_section::scoped_lock lock ( m_core->getCriticalSection () );
    m_game->m_validate ();
  };


  void MainPage::m_onCompositionScaleChanged ( Windows::UI::Xaml::Controls::SwapChainPanel const& sender,
                                               Windows::Foundation::IInspectable const& args )
  {
    //concurrency::critical_section::scoped_lock lock ( m_core->getCriticalSection () );
    //m_core->m_getD3D ()->setCompositionScale ( sender.CompositionScaleX, sender.CompositionScaleY );
    //m_core->createWindowSizeDependentResources (); // matrices
  };


  void MainPage::m_onSwapChainPanelSizeChanged ( Windows::Foundation::IInspectable const& sender,
                                                 Windows::UI::Xaml::SizeChangedEventArgs const& e )
  {
    //concurrency::critical_section::scoped_lock lock ( m_core->getCriticalSection () );
    //m_core->m_getD3D ()->setCompositionScale ( e.NewSize() );
    //m_core->createWindowSizeDependentResources (); // matrices
  };


  void MainPage::m_onAcceleratorKeyActivated ( winrt::Windows::UI::Core::CoreDispatcher const& sender,
                                               winrt::Windows::UI::Core::AcceleratorKeyEventArgs const& e )
  {

    // ALT+Enter: fullscreen + highest/lowest resolution switch
    if (e.EventType () == winrt::Windows::UI::Core::CoreAcceleratorKeyEventType::SystemKeyDown
         && e.VirtualKey () == winrt::Windows::System::VirtualKey::Enter
         && e.KeyStatus ().IsMenuKeyDown)
    {
      auto view = winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView ();

      if (view.IsFullScreen ())
      {
        m_game->m_getCore ()->m_setResolution ( false );
        view.ExitFullScreenMode ();
      } else
      {
        m_game->m_getCore ()->m_setResolution ( true );
        view.TryEnterFullScreenMode ();
      }

      e.Handled ( true );
    }


    // Todo: DirectInput
    if (e.EventType () == winrt::Windows::UI::Core::CoreAcceleratorKeyEventType::KeyDown)
    {

      // Escape: up
      if (e.VirtualKey () == winrt::Windows::System::VirtualKey::Up)
      {
        m_game->m_getUniverse ()->m_getCamera ()->forwardBackward ( 0.05f );
      }

      // Escape: down
      if (e.VirtualKey () == winrt::Windows::System::VirtualKey::Down)
      {
        m_game->m_getUniverse ()->m_getCamera ()->forwardBackward ( -0.05f );
      }

      // Escape: exit
      if (e.VirtualKey () == winrt::Windows::System::VirtualKey::Escape)
      {
        m_game->m_isPaused () = true;
        m_game->m_getCore ()->m_getTimer ()->m_event ( "pause" );
        //if (MessageBoxA ( handle, "Exit the Game?", "Exit", MB_YESNO | MB_ICONQUESTION ) == IDYES)
        //{
        //CoreApplication::Exit (); ///
        m_appWindow.get ().Close ();
        //} else
        //{
        //  m_game->m_getCore ()->m_getTimer ()->m_event ( "start" );
        //}
      }
    }

  };


  void MainPage::m_onPointerPressed ( Windows::Foundation::IInspectable const& sender,
                                      winrt::Windows::UI::Core::PointerEventArgs const& e )
  {
    // Todo sample: start pointer movement tracking
    // add interactions caused by courser
  };


  void MainPage::m_onPointerMoved ( Windows::Foundation::IInspectable const& sender,
                                    winrt::Windows::UI::Core::PointerEventArgs const& e )
  {
    // Todo 
    // add interactions caused by courser
    //trackingUpdate(e.CurrentPoint().Position().X)

    std::string a { "" };
    a += std::to_string ( e.CurrentPoint ().RawPosition ().X );
    a += ", ";
    a += std::to_string ( e.CurrentPoint ().RawPosition ().Y );

    test = (Converter::strConverter ( a ));

    //textBlock1 ().Text ( test );

  };


  void MainPage::m_onPointerReleased ( Windows::Foundation::IInspectable const& sender,
                                       winrt::Windows::UI::Core::PointerEventArgs const& e )
  {
    // Todo 
    // add interactions caused by courser
  };


  void MainPage::m_onPointerMoved2 ( Windows::Foundation::IInspectable const& sender,
                                     winrt::Windows::UI::Core::PointerEventArgs const& e )
  {
    textBlock1 ().Text ( test );
  };


  void MainPage::SaveInternalState ( winrt::Windows::Foundation::Collections::IPropertySet& state )
  {
    //concurrency::critical_section::scoped_lock lock ( m_core->getCriticalSection () );
    //m_core->m_getD3D ()->Trim ();

    // stopRenderLoop();

    // save app state

    //m_inputCore->Dispatcher ().StopProcessEvents ();

    if (m_game)
    {
      m_game->m_onSuspending ();
      m_game.release ();
    }

    if (m_core)
    {
      m_core->m_onSuspending ();
      delete m_core;
      m_core = nullptr;
    }

  };


  void MainPage::LoadInternalState ( winrt::Windows::Foundation::Collections::IPropertySet& state )
  {
    // load app state

    // startRenderLoop();
  };


  //void MainPage::ClickHandler ( IInspectable const& sender,
  //                              RoutedEventArgs const& e )
  //{

  //};


  void MainPage::AppBarButton_Click ( Windows::Foundation::IInspectable const& sender,
                                      Windows::UI::Xaml::RoutedEventArgs const& e )
  {
    // Use the app bar if it is appropriate for your app. Design the app bar, 
    // then fill in event handlers (like this one).
  };


  void MainPage::Button_Click ( winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e )
  {

    winrt::Windows::UI::Text::FontWeight a;
    a.Weight = 1;
    textBlock2 ().FontWeight ( a );
    textBlock1 ().Text ( test );
  }
};


namespace winrt::GameEngine::factory_implementation
{

};
