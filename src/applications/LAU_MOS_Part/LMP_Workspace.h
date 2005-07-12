//*****************************************************************************
//
// $Created : AML 03-04-02 $
//  $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/LMP_Workspace.h $
//   $Author: Aml $
//  $Modtime: 25/07/03 12:09 $
// $Revision: 9 $
// $Workfile: LMP_Workspace.h $
//
//*****************************************************************************

#ifndef __LMP_Workspace_h_
#define __LMP_Workspace_h_

#include "LMP.h"
#include "MOS_LAU.h"

class LMP_App;

//*****************************************************************************
// Object : PUT THE COMMENTS ON THE CLASS HERE
// Created: AML 03-04-02
//*****************************************************************************
class LMP_Workspace
{
    MT_COPYNOTALLOWED( LMP_Workspace );

public:
    explicit LMP_Workspace( LMP_App& app );
    virtual ~LMP_Workspace();
    
    //-----------------------------------------------------------------------------
    /** @name Main methods */
    //-----------------------------------------------------------------------------
    //@{
    void Reconnect( const std::string& strHostName = "localhost" );
    void Execute( void );
    //@}
    
    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{
    //@}

    //-----------------------------------------------------------------------------
    /** @name Message To Send (tous envoyé au launcher)*/
    //-----------------------------------------------------------------------------
    //@{
    bool32 SendMsg_RequestListMission       ();
    bool32 SendMsg_LaunchSim                ( const uint32 nExerciceID, const uint32 nScenarioID, const std::string& strCommandLine );
    bool32 SendMsg_RequestListSim           ();
    bool32 SendMsg_StopSim                  ( const uint32 nExerciceID );
    bool32 SendMsg_RequestListCheckpoint    ( const uint32 nExerciceID );
    bool32 SendMsg_LaunchSimFromCheckpoint  ( const uint32 nExerciceID, const uint32 nCheckpointID, const std::string& strCommandLine );

    //@}

private:
    LMP_App&            app_;       // (not owned)
    MOS_LAU*            pLau_;      // (owned)
    DIN::DIN_Engine*    pEngine_;   // (owned)

    uint32              nNextExerciceID_;
};


#ifdef USE_INLINE
#   include "LMP_Workspace.inl"
#endif

#endif // __LMP_Workspace_h_
