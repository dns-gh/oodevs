//*****************************************************************************
//
// $Created: NLD 2002-08-07 $
// $Archive: /MVW_v10/Build/SDK/SIM/src/SIM_App.h $
// $Author: Jvt $
// $Modtime: 7/04/05 16:01 $
// $Revision: 24 $
// $Workfile: SIM_App.h $
//
//*****************************************************************************

#ifndef __SIM_App_h_
#define __SIM_App_h_

#include <windows.h>
#include <shellapi.h>
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )

namespace tools
{
    class WinArguments;
    class WaitEvent;
}

class MIL_Config;
class MT_FileLogger;

//=============================================================================
// Created: NLD 2002-08-07
//=============================================================================
class SIM_App : private boost::noncopyable
{
public:
     SIM_App( int maxConnections, bool verbose );
    ~SIM_App();

    int Execute();

private:
    void Initialize();
    void RunGUI();
    void AnimateIcon();
    void StartIconAnimation();
    void StopIconAnimation();
    static LRESULT CALLBACK MainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
    const int maxConnections_;
    const bool verbose_;
    std::unique_ptr< MIL_Config > config_;
    std::unique_ptr< tools::WinArguments > winArguments_;
    std::unique_ptr< MT_FileLogger > logger_;
    std::unique_ptr< tools::WaitEvent > quit_;
    std::unique_ptr< boost::thread > gui_;
    std::unique_ptr< boost::thread > dispatcher_;
    int result_;
    HWND hWnd_;
    HINSTANCE hInstance_;
    NOTIFYICONDATA trayIcon_;
    unsigned int nIconIndex_;
};

#endif // __SIM_App_h_
