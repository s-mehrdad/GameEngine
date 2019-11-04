// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <created>ʆϒʅ,01.11.2019</created>
/// <changed>ʆϒʅ,05.11.2019</changed>
// ********************************************************************************

#include "pch.h"
//#include "GameEngine.h"
#include "Utilities.h" // string + s,f streams + exception + threads + list + Windows standards
#include "Shared.h"
#include "Game.h"


using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Input;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::System;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Graphics::Display;

//using namespace winrt;
//using namespace winrt::Windows::UI::Xaml;
//using namespace winrt::Windows::UI::Xaml::Controls;
//using namespace winrt::Windows::UI::Xaml::Navigation;


bool running { false };
std::wstring gameState { L"uninitialized" };


class View : public winrt::implements<View, IFrameworkViewSource, IFrameworkView>
{
private:
  static bool failure; // true if exiting with failure

  winrt::agile_ref<CoreWindow> m_appWindow; // reference to application window
  bool m_running;
  bool m_visible;
  // resize
  // DPI
  // Width / Height
  // Orientation
  //std::unique_ptr <Game> game;
  Game* game;
protected:
  void m_onActivated ( CoreApplicationView const& /*applicationView*/,
                       IActivatedEventArgs const& /*args*/ ); // on application window activation
  void m_onVisibilityChanged ( CoreWindow const& /*sender*/,
                               VisibilityChangedEventArgs const& /*args*/ ); // on application window resize
  void m_onWindowResized ( CoreWindow const& /*sender*/,
                           WindowSizeChangedEventArgs const& /*args*/ ); // on application window resize
  void m_onSuspending ( IInspectable const& /*sender*/,
                        SuspendingEventArgs const& /*args*/ ); // on application window entering the suspended state
  void m_onResuming ( IInspectable const& /*sender*/,
                      IInspectable const& /*args*/ ); // on application window resuming from suspended state

  //void m_on Key Pressed
  //void m_on Dpi Changed
  //void m_on Orientation Changed
  //void m_on Display Content Invalidated
  //
public:
  View ( void );
  //~View ( void );

  // implementation documentation:
  // for being able to implement the object which implements IFrameworkView interface,
  // it should made ready by the IFrameworkViewSource factory interface
  // the order in which these methods are called (after defined by a CoreApplication singleton instance):
  // CreateView, Initialize, SetWindow, Load, Run

  // IFrameworkViewSource methods:
  IFrameworkView CreateView ( void ); // creation method overload

  // IFrameworkView methods:
  void Initialize ( CoreApplicationView const& ); // initialization method overload (called on app launch)
  void Load ( winrt::hstring const& /*entryPoint*/ ); // load method overload
  void Run ( void ); // execution method overload (called after load method)
  void SetWindow ( CoreWindow const& ); // window set method overload (after initialization)
  void Uninitialize ( void ); // un-initialization method overload

  static bool exitedWith ( void ); // application exit state provider
};


int WINAPI wWinMain ( _In_ HINSTANCE /*hInstance*/,
                      _In_ HINSTANCE /*hPrevInstance*/,
                      _In_ LPWSTR /*LpCmdLine*/,
                      _In_ int /*nCmdShow*/ )
{

  try
  {

    if (!DirectX::XMVerifyCPUSupport ())
    {
      gameState = L"CPU";
      throw;
    }


    std::shared_ptr<theException> anException { new (std::nothrow) theException () };
    PointerProvider::exceptionProvider ( anException );

    std::shared_ptr<Logger<toFile>> fileLoggerEngine ( new (std::nothrow) Logger<toFile> () );
    PointerProvider::fileLoggerProvider ( fileLoggerEngine );

    std::shared_ptr<Configurations> settings ( new (std::nothrow) Configurations () );
    PointerProvider::configurationProvider ( settings );

    if ((!anException) && (!settings))
    {
      // failure, shut down the game properly
      if (anException)
      {
        PointerProvider::exceptionProvider ( nullptr );
        anException.reset ();
      }
      if (settings)
      {
        PointerProvider::configurationProvider ( nullptr );
        settings.reset ();
      }

      gameState = L"utilities";
      throw;
    }

    if (fileLoggerEngine)
    {
      PointerProvider::getFileLogger ()->push ( logType::info, std::this_thread::get_id (), L"mainThread",
                                                L"Exception, file logger and configuration providers are successfully initialized." );
    } else
    {
      // failure, shut down the game properly
      if (anException)
      {
        PointerProvider::exceptionProvider ( nullptr );
        anException.reset ();
      }
      if (settings)
      {
        PointerProvider::configurationProvider ( nullptr );
        settings.reset ();
      }

      gameState = L"appDebug";
      throw;
    }


    auto view = winrt::make<View> ();
    CoreApplication::Run ( view );
    //CoreApplication::Run ( View () );


    ///
    if (anException)
    {
      PointerProvider::exceptionProvider ( nullptr );
      anException.reset ();
    }

    if (settings)
    {
      PointerProvider::configurationProvider ( nullptr );
      settings.reset ();
    }

    if (fileLoggerEngine)
    {
      PointerProvider::getFileLogger ()->push ( logType::info, std::this_thread::get_id (), L"mainThread",
                                                L"The logging engine is set to shut down..." );

      // failure or success, the logs are somehow to be saved, so give its thread some time
      std::this_thread::sleep_for ( std::chrono::milliseconds { 100 } );

      PointerProvider::fileLoggerProvider ( nullptr );
      fileLoggerEngine.reset ();
    }

    gameState = L"uninitialized";


    if (View::exitedWith ())
      return EXIT_FAILURE;
    else
      return EXIT_SUCCESS;

  }
  catch (const std::exception & ex)
  {

    if (gameState == L"CPU")
    {
      //MessageBoxA ( NULL, "Your CPU isn't supported.", "Error", MB_OK | MB_ICONERROR );
    } else
      if (gameState == L"utilities")
      {
        //MessageBoxA ( NULL, "The Game could not be started...", "Error", MB_OK | MB_ICONERROR );
      } else
        if (gameState == L"appDebug")
        {
          //MessageBoxA ( NULL, "The debug service failed to start.", "Error", MB_OK | MB_ICONERROR );
        } else
        {
          //MessageBoxA ( NULL, ex.what (), "Error", MB_OK | MB_ICONERROR );
          if (PointerProvider::getFileLogger ())
          {
            PointerProvider::getFileLogger ()->push ( logType::error, std::this_thread::get_id (), L"mainThread",
                                                      Converter::strConverter ( ex.what () ) );

            // failure or success, the logs are somehow to be saved, so give its thread some time
            std::this_thread::sleep_for ( std::chrono::milliseconds { 100 } );
          }
        }
        return EXIT_FAILURE;

  }

}


View::View ( void ) :
  m_running ( false ), m_visible ( false )
{

};


//View::~View ( void )
//{
//
//};


void View::m_onActivated ( CoreApplicationView const& /*applicationView*/, IActivatedEventArgs const& /*args*/ )
{

  // visible the activated application window and enables it to receive events
  CoreWindow::GetForCurrentThread ().Activate ();

};


void View::m_onVisibilityChanged ( CoreWindow const& /*sender*/, VisibilityChangedEventArgs const& args )
{
  m_visible = args.Visible ();
  //if(m_visible)
    // Todo: CoreDX, present to screen
};


void View::m_onWindowResized ( CoreWindow const& /*sender*/, WindowSizeChangedEventArgs const& /*args*/ )
{
  // Todo: window behaviour and needed procedure
  // Todo: resize the DirectX resources
};


void View::m_onSuspending ( IInspectable const& /*sender*/, SuspendingEventArgs const& /*args*/ )
{

};


void View::m_onResuming ( IInspectable const& /*sender*/, IInspectable const& /*args*/ )
{

};


IFrameworkView View::CreateView ( void )
{
  return *this;
};


void View::Initialize ( CoreApplicationView const& applicationView )
{

  m_running = true;


  // Todo: more reasearch how to use Xaml controls when implementing the UWP core
  //Frame root { nullptr };
  //root = m_appWindow.get ().try_as<Frame> ();
  //CoreApplicationView newView = CoreApplication::CreateNewView ();
  //auto content = winrt::Windows::UI::Xaml::Window::Current ().Content ();
  //if (!content)
  //{
  //} else
  //{
  //  root = content.try_as<Frame> ();
  //}
  //root = Frame ();
  //winrt::Windows::UI::Xaml::Window::Current ().Content ( root );


  // response to application window activation
  applicationView.Activated ( { this, &View::m_onActivated } );

  // response to application window suspension
  CoreApplication::Suspending ( { this, &View::m_onSuspending } );

  // response to application window resuming from suspension
  CoreApplication::Resuming ( { this, &View::m_onResuming } );

};


void View::Load ( winrt::hstring const& /*entryPoint*/ )
{

};


void View::Run ( void )
{

  // Todo: 
  //CoreDX = std::make_unique<CoreDX> ();


  // game instantiation
  auto windowPtr = static_cast<::IUnknown*>(winrt::get_abi ( m_appWindow.get () ));
  game = new (std::nothrow) Game ( windowPtr ); ///

  if (!game->isReady ())
  {
    game->shutdown (); // failure, shut the application down properly.

    PointerProvider::getFileLogger ()->push ( logType::error, std::this_thread::get_id (), L"mainThread",
                                              L"The game initialization failed!" );

    //MessageBoxA ( NULL, "The Game functionality failed to start...", "Critical-Error", MB_OK | MB_ICONERROR );

    failure = true;
  } else
  {
    gameState = L"initialized";

    PointerProvider::getFileLogger ()->push ( logType::warning, std::this_thread::get_id (), L"mainThread",
                                              L"Entering the game loop..." );

    if (!game->run ())
    {
      failure = true;
      PointerProvider::getFileLogger ()->push ( logType::warning, std::this_thread::get_id (), L"mainThread",
                                                L"One or more errors occurred while running the game!" );
    }

    game->shutdown (); // failure or success, shut the application down properly.
  }


  //while (m_running)
  //{
  //  if (m_visible)
  //  {

  //    // Todo: tick the timer and initiate, update and present the scene

  //    CoreWindow::GetForCurrentThread ().Dispatcher ().ProcessEvents (
  //      CoreProcessEventsOption::ProcessAllIfPresent );

  //  } else
  //  {

  //    // Todo: additional suitable processes for hibernation state

  //    CoreWindow::GetForCurrentThread ().Dispatcher ().ProcessEvents (
  //      CoreProcessEventsOption::ProcessOneIfPresent );
  //    // if no task go to hibernation
  //    std::this_thread::sleep_for ( std::chrono::milliseconds ( 100 ) );

  //    // Microsoft's resource (suspension)
  //    //CoreWindow::GetForCurrentThread ().Dispatcher ().ProcessEvents (
  //    //  CoreProcessEventsOption::ProcessOneAndAllPending );

  //  }
  //}

};


void View::SetWindow ( CoreWindow const& window )
{

  m_appWindow = window;

  // cursor type: standard arrow cursor resource
  m_appWindow.get ().PointerCursor ( CoreCursor { CoreCursorType::Arrow, 0 } );

  // response to window size events
  m_appWindow.get ().SizeChanged ( { this, &View::m_onWindowResized } );

  // response to window close events
  m_appWindow.get ().Closed ( [this]( auto&&, auto&& ) { m_running = false; } );

  // response to window close events
  m_appWindow.get ().VisibilityChanged ( { this, &View::m_onVisibilityChanged } );

};


void View::Uninitialize ( void )
{
  // Todo: free the taken resources
  // shutdown precess
};


bool View::failure = false;
bool View::exitedWith ( void )
{
  return failure;
};
