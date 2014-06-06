// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __App_h_
#define __App_h_

#include <string>
#include <windows.h>
#include <shellapi.h>
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )

namespace tools
{
    class WaitEvent;
}

namespace dispatcher
{
    class Config;
    class Replayer;
}

// =============================================================================
/** @class  App
    @brief  My first application
*/
// Created: AGE 2007-04-10
// =============================================================================
class App
{
public:
    //! @name Constructors/Destructor
    //@{
             App( HINSTANCE hinstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, bool replayLog );
    virtual ~App();
    //@}

    //! @name Operations
    //@{
    void Execute();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    App( const App& );            //!< Copy constructor
    App& operator=( const App& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    void RunGUI( HINSTANCE hinstance );
    void StartIconAnimation();
    void StopIconAnimation();
    void AnimateIcon();
    static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< dispatcher::Config >   config_;
    std::unique_ptr< dispatcher::Replayer > replayer_;
    std::unique_ptr< tools::WaitEvent > quit_;
    //@}

    //! @name GUI Member data
    //@{
    HWND                           hWnd_ ;
    HINSTANCE                      hInstance_ ;
    NOTIFYICONDATA                 TrayIcon_;
    unsigned int                   nIconIndex_;
    std::unique_ptr< boost::thread > guiThread_ ;
    bool                           test_;
    //@}
};

#endif // __App_h_
