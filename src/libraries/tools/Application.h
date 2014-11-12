// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef TOOLS_APPLICATION_H
#define TOOLS_APPLICATION_H

#include <boost/noncopyable.hpp>
#include <windows.h>
#include <shellapi.h>
#include <memory>
#include <string>

namespace boost
{
    class thread;
}

namespace tools
{
    class WaitEvent;
    class WinArguments;

// =============================================================================
/** @class  Application
    @brief  Server side application base class
*/
// Created: MCO 2014-11-12
// =============================================================================
class Application : private boost::noncopyable
{
public:
    int Execute();

protected:
     Application( int icon1, int icon2, int quit );
    ~Application();

    void Initialize( const std::string& module );

    virtual bool Update() = 0;
    virtual void Initialize() = 0;

private:
    void RunGUI();
    void AnimateIcon();
    void StartIconAnimation();
    void StopIconAnimation();
    static LRESULT CALLBACK MainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:
    std::unique_ptr< tools::WinArguments > arguments_;
    std::unique_ptr< tools::WaitEvent > quit_;
    int result_;

private:
    std::unique_ptr< boost::thread > gui_;
    std::unique_ptr< boost::thread > dispatcher_;
    int icon1_, icon2_, q_;
    HWND hWnd_;
    HINSTANCE hInstance_;
    NOTIFYICONDATA trayIcon_;
    unsigned int nIconIndex_;
};

}

#endif // TOOLS_APPLICATION_H
