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

#include <winsock2.h>
#include <windows.h>
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

class MIL_Config;
class MT_ConsoleLogger;
class MT_FileLogger;
class SIM_Dispatcher;
class SIM_NetworkLogger;
class WinArguments;

//=============================================================================
// Created: NLD 2002-08-07
//=============================================================================
class SIM_App : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SIM_App( HINSTANCE hinstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow, int maxConnections ); //!< win32
    virtual ~SIM_App();
    //@}

    //! @name Operations
    //@{
    int Execute();

    static bool CrashWithCoreDump();
    //@}

private:
    //! @name Helpers
    //@{
    int Initialize    ();
    void Run           ();
    void Stop          ();
    bool Tick           ();
    void Cleanup       ();
    int  Test          ();
    void CheckpointTest();

    void RunGUI();
    void RunDispatcher();
    void AnimateIcon();
    void StartIconAnimation();
    void StopIconAnimation();
    void CreateConsoleLog();

    static LRESULT CALLBACK MainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< tools::RealFileLoaderObserver_ABC > observer_;
    std::auto_ptr< MIL_Config > startupConfig_;
    std::auto_ptr< WinArguments > winArguments_;
    std::auto_ptr< SIM_NetworkLogger > pNetworkLogger_;
    std::auto_ptr< MT_FileLogger > logger_;
    std::auto_ptr< MT_ConsoleLogger > console_;

    static bool     bCrashWithCoreDump_;
    static bool     bUserInterrupt_;
    SIM_Dispatcher* pDispatcher_;
    int             maxConnections_;

    HWND                           hWnd_;
    HINSTANCE                      hInstance_;
    NOTIFYICONDATA                  TrayIcon_;
    unsigned int                   nIconIndex_;
    std::auto_ptr< boost::thread > guiThread_;
    std::auto_ptr< boost::thread > dispatcherThread_;
    bool                           dispatcherOk_;
    //@}
};

#include "SIM_App.inl"

#endif // __SIM_App_h_
