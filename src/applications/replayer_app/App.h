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
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

class MT_FileLogger;

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
class App : boost::noncopyable
{
public:
     App( bool replayLog );
    ~App();

    void Execute();

private:
    void Initialize();
    void RunGUI();
    void StartIconAnimation();
    void StopIconAnimation();
    void AnimateIcon();
    static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    std::unique_ptr< dispatcher::Config > config_;
    std::unique_ptr< MT_FileLogger > logger_;
    std::unique_ptr< dispatcher::Replayer > replayer_;
    std::unique_ptr< tools::WaitEvent > quit_;
    HWND hWnd_;
    HINSTANCE hInstance_;
    NOTIFYICONDATA trayIcon_;
    unsigned int nIconIndex_;
    std::unique_ptr< boost::thread > gui_;
    bool test_;
};

#endif // __App_h_
