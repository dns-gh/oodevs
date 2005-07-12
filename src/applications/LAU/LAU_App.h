//*****************************************************************************
//
// Name     : LAU_App.h
//
// $Created : AML 03-03-27 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_App.h $
// $Author: Nld $
// $Modtime: 20/04/04 14:54 $
// $Revision: 13 $
// $Workfile: LAU_App.h $
//
//*****************************************************************************

#ifndef __LAU_App_h_
#define __LAU_App_h_

#include "LAU.h"

class LAU_Workspace;

//*****************************************************************************
// Created: AML 03-03-28
//*****************************************************************************
class LAU_App
{
    MT_COPYNOTALLOWED( LAU_App );


public:
     explicit   LAU_App( int argc = 0, char** argv = NULL );
                ~LAU_App();

    bool32      Initialize();
    bool32      Run       (); //!< return false if the application must be closed
    bool32      RunOneFrame(); //!< return false if the application must be closed

    void        Stop      ();

    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{
    const LAU_Workspace& GetWorkspace() const;
          LAU_Workspace& GetWorkspace();

    bool32               IsKillStuckedSim( void ) const;
    //@}

public:
    //-------------------------------------------------------------------------
    /** @name Singleton implementation */
    //-------------------------------------------------------------------------
    //@{
    static LAU_App* Instance();
    //@}
    
private:
    LAU_Workspace*          pWorkSpace_;
    bool32                  bRunning_;
    int                     nSleepTime_;            // SleepTime by frame
    bool32                  bKillStuckedSim_;       // if true and if a sim doesn't respond to ping, kill the sim (and report to thales-launcher)

    static LAU_App*         static_pInstance;
};



#ifdef USE_INLINE
#   include "LAU_App.inl"
#endif


#endif // __LAU_App_h_
