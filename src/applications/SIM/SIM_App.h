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

#include "MIL/Tools/MIL_Config.h"

class SIM_NetworkLogger;

//=============================================================================
// Created: NLD 2002-08-07
//=============================================================================
class SIM_App
{
    MT_COPYNOTALLOWED( SIM_App );

public:
     SIM_App( int nArgc, char* pArgv[] );
    ~SIM_App();
    
    int Execute();

    static bool CrashWithCoreDump();

private:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Run       (); 
    void Cleanup   ();
    int Test       ();
    bool ParseCmdArgs( int nArgc, char* pArgv[], MIL_Config& startupConfig );

    std::string Wrap( const std::string& content, const std::string& prefix ) const;
    bool IsAlreadyWrapped( const std::string& content ) const;
    //@}
    
private:
    //! @name Member data
    //@{
    MIL_Config  startupConfig_;
    
    // Error dispatchers
    SIM_NetworkLogger* pNetworkLogger_;

    static bool bCrashWithCoreDump_;
    bool bTestMode_;
    //@}
};

#include "SIM_App.inl"

#endif // __SIM_App_h_
