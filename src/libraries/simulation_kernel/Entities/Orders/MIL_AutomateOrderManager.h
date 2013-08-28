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

#include "MIL_OrderManager_ABC.h"
#include "MIL_AutomateMRT.h"

namespace sword
{
    class AutomatOrder;
    class FragOrder;
}

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
             MIL_AutomateOrderManager( MissionController_ABC& controller, MIL_Automate& automate );
    virtual ~MIL_AutomateOrderManager();

    // @name Events
    //@{
    virtual uint32_t OnReceiveFragOrder( const sword::FragOrder& asn );
            uint32_t OnReceiveMission  ( const sword::AutomatOrder& asn );
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
    boost::shared_ptr< MIL_Mission_ABC > MRT_CreatePionMission( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType );
    void                    MRT_Validate         ();
    boost::shared_ptr< MIL_Mission_ABC > CDT_CreatePionMission( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType );
    boost::shared_ptr< MIL_Mission_ABC > CreatePionMissionBM( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType );
    boost::shared_ptr< MIL_Mission_ABC > CreatePionMissionVersPionBM( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType );
    void                    CDT_GivePionMission  ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    void                    CDT_GivePionMissionVersPion  ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    boost::shared_ptr< MIL_Mission_ABC > CreateAutomateMission( MIL_Automate& automate, const MIL_MissionType_ABC& missionType );
    void                    GiveAutomateMission  ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    void                    GiveMissionToNewAutomat( MIL_Automate& automate, boost::shared_ptr< MIL_Mission_ABC > mission );
    //@}

private:
    //! @name Operations
    //@{
    virtual void StopAllMissions();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< boost::shared_ptr< MIL_Mission_ABC > > T_MissionSet;
    //@}

private:
    MIL_Automate&   automate_;
    MIL_AutomateMRT mrt_;
    T_MissionSet    preparedMissions_;
};

#endif // __MIL_AutomateOrderManager_h_
