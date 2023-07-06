
// ===========================================================================
/// <summary>
/// App.h
/// GameEngine
/// created by Mehrdad Soleimanimajd on 27.06.2020
/// </summary>
/// <created>ʆϒʅ, 27.06.2020</created>
/// <changed>ʆϒʅ, 06.07.2023</changed>
// ===========================================================================

#ifndef APP_H
#define APP_H


#include "App.xaml.g.h"
#include "MainPage.h"
#include "common/Utilities.h"


namespace winrt::GameEngine::implementation
{
    struct App : AppT<App>
    {
    private:
        winrt::GameEngine::implementation::MainPage* m_mainPage; // pointer to main page

        void OnSuspending (IInspectable const& sender,
                           Windows::ApplicationModel::SuspendingEventArgs const& e); // on suspension
        void OnResuming (IInspectable const& sender,
                         IInspectable const& args); // when returning from a suspended state
        void OnNavigationFailed (IInspectable const& sender,
                                 Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const& e); // on ZAML failures

        static bool failure; // true if exiting with failure
    public:
        App (); // first to run (class constructor plus application initialization)
        //~App ();

        void OnLaunched (Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const& e); // after initialization

        //static bool exitedWith ( void ); // application exit state provider
    };
}


#endif // !APP_H
