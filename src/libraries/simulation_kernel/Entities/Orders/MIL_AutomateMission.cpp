// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomateMission.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Model_ABC.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_AutomateMission::MIL_AutomateMission( const MIL_MissionType_ABC& type, MIL_Automate& automate, const ASN1T_MsgAutomatOrder& asn )
    : MIL_Mission_ABC          ( type, automate.GetKnowledge(), asn.parametres, automate.GetPionPC().GetRole< PHY_RoleInterface_Location >().GetPosition() )
    , automate_                ( automate )
    , bDIAMrtBehaviorActivated_( false )
    , bDIACdtBehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission constructor
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
MIL_AutomateMission::MIL_AutomateMission( const MIL_MissionType_ABC& type, MIL_Automate& automate )
    : MIL_Mission_ABC          ( type, automate.GetKnowledge() )
    , automate_                ( automate )
    , bDIAMrtBehaviorActivated_( false )
    , bDIACdtBehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_AutomateMission::MIL_AutomateMission( const MIL_MissionType_ABC& type, MIL_Automate& automate, const MIL_AutomateMission& parent )
    : MIL_Mission_ABC          ( type, automate.GetKnowledge(), parent )
    , automate_                ( automate )
    , bDIAMrtBehaviorActivated_( false )
    , bDIACdtBehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_AutomateMission::MIL_AutomateMission( MIL_Automate& automate, const MIL_AutomateMission& rhs )
    : MIL_Mission_ABC          ( automate.GetKnowledge(), rhs )
    , automate_                ( automate)
    , bDIAMrtBehaviorActivated_( false )
    , bDIACdtBehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission destructor
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
MIL_AutomateMission::~MIL_AutomateMission()
{
    Stop();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::CreateCopy
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_AutomateMission& MIL_AutomateMission::CreateCopy( MIL_Automate& target ) const
{
    return *new MIL_AutomateMission( target, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::IsFragOrderAvailable
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_AutomateMission::IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const
{
    return automate_.GetType().GetModel().IsFragOrderAvailableForMission( GetType(), fragOrderType );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::Start
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_AutomateMission::Start()
{
    assert( !bDIAMrtBehaviorActivated_ );

    automate_.GetDecision().StartMissionMrtBehavior( *this );
    bDIAMrtBehaviorActivated_ = true;
    Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::Stop
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_AutomateMission::Stop()
{
    if( bDIAMrtBehaviorActivated_ )
        automate_.GetDecision().StopMissionMrtBehavior( *this );
    if( bDIACdtBehaviorActivated_ )
        automate_.GetDecision().StopMissionConduiteBehavior( *this );

    SendNoMission( automate_ );
    
    bDIACdtBehaviorActivated_ = false;
    bDIAMrtBehaviorActivated_ = false;    
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::GoToCdt
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateMission::GoToCdt()
{
    assert(  bDIAMrtBehaviorActivated_ );
    assert( !bDIACdtBehaviorActivated_ );

    automate_.GetDecision().StopMissionMrtBehavior( *this );
    bDIAMrtBehaviorActivated_ = false;

    automate_.GetDecision().StartMissionConduiteBehavior( *this );
    bDIACdtBehaviorActivated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void MIL_AutomateMission::SendNoMission( const MIL_Automate& automate )
{
    NET_ASN_MsgAutomatOrder asn;

    asn().oid          = automate.GetID();
    asn().mission      = 0;
    asn().parametres.n = 0;
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::Send
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_AutomateMission::Send() const
{
    NET_ASN_MsgAutomatOrder asn;

    asn().oid       = automate_.GetID();
    asn().mission   = GetType().GetID();

    MIL_Mission_ABC::Serialize( asn().parametres );

    asn.Send();

    MIL_Mission_ABC::CleanAfterSerialization( asn().parametres );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::AffectDirection
// Created: SBO 2008-01-15
// -----------------------------------------------------------------------------
void MIL_AutomateMission::AffectDirection( const MT_Vector2D& direction )
{
    MIL_Mission_ABC::AffectDirection( direction );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::GetAutomate
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateMission::GetAutomate() const
{
    return automate_;
}
