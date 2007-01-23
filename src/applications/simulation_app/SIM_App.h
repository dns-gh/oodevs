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

#include "SIM_Config.h"

class SIM_NetworkLogger;
class SIM_Dispatcher;

//=============================================================================
// Created: NLD 2002-08-07
//=============================================================================
class SIM_App
{
    MT_COPYNOTALLOWED( SIM_App );

public:
     SIM_App( int argc, char** argv );
    ~SIM_App();
    
    int Execute();

    static bool CrashWithCoreDump();

private:
    //! @name Tools
    //@{
    void Initialize();
    void Run       (); 
    void Cleanup   ();
    int  Test      ();

    std::string Wrap                ( const std::string& content, const std::string& prefix ) const;
    bool        IsAlreadyWrapped    ( const std::string& content ) const;

    static bool ConsoleEventHandler( int nEvent );
    //@}
    
private:
    //! @name Member data
    //@{
    SIM_Config startupConfig_;
    
    // Error dispatchers
    SIM_NetworkLogger* pNetworkLogger_;

    static bool     bCrashWithCoreDump_;
    static bool     bUserInterrupt_;
    SIM_Dispatcher* pDispatcher_;
    //@}
};

#include "SIM_App.inl"

#endif // __SIM_App_h_
