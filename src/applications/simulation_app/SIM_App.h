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

#include "Sim.h"
#include "WinArguments.h"
#include "simulation_kernel/tools/MIL_Config.h"
#pragma warning( push )
#pragma warning( disable : 4512 4244 )
#include <boost/thread.hpp>
#pragma warning( pop )

class SIM_NetworkLogger;
class SIM_Dispatcher;
class MT_FileLogger;

//=============================================================================
// Created: NLD 2002-08-07
//=============================================================================
class SIM_App : private boost::noncopyable
{
public:

    class QuitException : public std::exception
    {

    } ;

    //! @name Constructors/Destructor
    //@{
             SIM_App( HINSTANCE hinstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow, int maxConnections ); //!< win32
    virtual ~SIM_App();
    //@}

    int Execute();

    static bool CrashWithCoreDump();

private:
    //! @name Tools
    //@{
    void Initialize        ();
    void Run               ();
    void Stop              ();
    bool Tic               ();
    void Cleanup           ();
    int  Test              ();
    void CheckpointTest    ();

    std::string Wrap( const std::string& content, const std::string& prefix ) const;

    //@}

private:
    //! @name Member data
    //@{
    MIL_Config        startupConfig_;

    WinArguments    winArguments_ ;

    SIM_NetworkLogger*                pNetworkLogger_;    //<! Error dispatchers
    MT_FileLogger*                  logger_;

    static bool                        bCrashWithCoreDump_;
    static bool                        bUserInterrupt_;
    SIM_Dispatcher*                    pDispatcher_;
    int                                maxConnections_;
    //@}

    //! @name UI members
    //@{
    HWND                           hWnd_ ;
    HINSTANCE                       hInstance_ ;
    NOTIFYICONDATA                   TrayIcon_;
    unsigned int                   nIconIndex_;
    std::auto_ptr< boost::thread > guiThread_ ;
    std::auto_ptr< boost::thread > dispatcherThread_ ;
    //@}


    void RunGUI();
    void RunDispatcher();
    void AnimateIcon() ;
    void StartIconAnimation();
    void StopIconAnimation();

    static LRESULT CALLBACK    MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#include "SIM_App.inl"

#endif // __SIM_App_h_
