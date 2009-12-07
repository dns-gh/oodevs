// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_AutomateOrderManager_h_
#define __MIL_AutomateOrderManager_h_

#include "MIL.h"

#include "MIL_OrderManager_ABC.h"
#include "MIL_AutomateMRT.h"
#include "Network/NET_ASN_Messages.h"

class MIL_AutomateMission;
class MIL_MissionType_ABC;
class MIL_Automate;
class MIL_AgentPion;
class MIL_MissionType_ABC;
class MIL_PionMission;

//=============================================================================
// Created: NLD 2003-01-10
//=============================================================================
class MIL_AutomateOrderManager : public MIL_OrderManager_ABC
{

public:
    explicit MIL_AutomateOrderManager( MIL_Automate& automate );
    virtual ~MIL_AutomateOrderManager();

    // @name Events 
    //@{
    virtual void OnReceiveFragOrder( const ASN1T_MsgFragOrder&     asn );
            void OnReceiveMission  ( const ASN1T_MsgAutomatOrder& asn );
            void OnReceiveMission  ( const MIL_MissionType_ABC& type );
    //@}

    //! @name Operations
    //@{
    const MIL_Fuseau& GetFuseau         () const;
          bool        IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const;
    //@}

    //! @name Operations
    //@{
    void                    MRT_SetFuseauForPion ( MIL_AgentPion& pion, MIL_Fuseau& fuseau );
    const MIL_Fuseau*       MRT_GetFuseauForPion ( MIL_AgentPion& pion ) const;
    MIL_PionMission*        MRT_CreatePionMission( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType );
    void                    MRT_Validate         ();
    MIL_PionMission*        CDT_CreatePionMission( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType );
    void                    CDT_GivePionMission  ( MIL_Mission_ABC& mission );

    MIL_AutomateMission*    CreateAutomateMission( MIL_Automate& automate, const MIL_MissionType_ABC& missionType );
    void                    GiveAutomateMission  ( MIL_AutomateMission& mission );
    //@}

private:
    //! @name Operations
    //@{
    virtual void StopAllMissions();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_Mission_ABC* > T_MissionSet;
    typedef T_MissionSet::const_iterator CIT_MissionSet;
    //@}
   
private:
    MIL_Automate&   automate_;
    MIL_AutomateMRT mrt_;
    T_MissionSet    preparedMissions_;
};

#endif // __MIL_AutomateOrderManager_h_
