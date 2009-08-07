// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PionMission.h"
#include "MIL_AutomateMission.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Model_ABC.h"
#include "Network/NET_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion, const ASN1T_MsgUnitOrder& asn )
    : MIL_Mission_ABC       ( type, pion.GetKnowledge(), asn.parametres, pion.GetRole< PHY_RolePion_Location >().GetPosition() )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion, const MIL_AutomateMission& parent )
    : MIL_Mission_ABC       ( type, pion.GetKnowledge(), parent )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion )
    : MIL_Mission_ABC       ( type, pion.GetKnowledge() )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( MIL_AgentPion& pion, const MIL_PionMission& rhs )
    : MIL_Mission_ABC       ( pion.GetKnowledge(), rhs )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission destructor
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
MIL_PionMission::~MIL_PionMission()
{
    Stop();
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::CreateCopy
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PionMission& MIL_PionMission::CreateCopy( MIL_AgentPion& target ) const
{
    return *new MIL_PionMission( target, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::IsFragOrderAvailable
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_PionMission::IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const
{
    return pion_.GetType().GetModel().IsFragOrderAvailableForMission( GetType(), fragOrderType );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::Start
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionMission::Start()
{
    assert( !bDIABehaviorActivated_ );

    pion_.GetDecision().StartMissionBehavior( *this );
    bDIABehaviorActivated_ = true;
    Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::Stop
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionMission::Stop()
{
    if( bDIABehaviorActivated_ )
    {
        pion_.GetDecision().StopMissionBehavior( *this );
        bDIABehaviorActivated_ = false;
    }
    SendNoMission( pion_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void MIL_PionMission::SendNoMission( const MIL_AgentPion& pion )
{
    NET_ASN_MsgUnitOrder asn;
    asn().oid          = pion.GetID();
    asn().mission      = 0;
    asn().parametres.n = 0;
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::Send
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionMission::Send() const
{
    NET_ASN_MsgUnitOrder asn;
    asn().oid       = pion_.GetID();
    asn().mission   = GetType().GetID();
    Serialize( asn().parametres );
    asn.Send();
    CleanAfterSerialization( asn().parametres );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::AffectDirection
// Created: SBO 2008-01-15
// -----------------------------------------------------------------------------
void MIL_PionMission::AffectDirection( const MT_Vector2D& direction )
{
    MIL_Mission_ABC::AffectDirection( direction );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::GetPion
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_PionMission::GetPion() const
{
    return pion_;
}
