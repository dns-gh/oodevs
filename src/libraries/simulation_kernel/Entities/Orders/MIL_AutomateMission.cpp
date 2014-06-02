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

#include "ActionManager.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Model_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "MIL_Time_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

#include <boost/make_shared.hpp>
#include <boost/optional/optional.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_AutomateMission::MIL_AutomateMission( const MIL_MissionType_ABC& type,
                                          MIL_Automate& automate,
                                          uint32_t id,
                                          const boost::shared_ptr< MIL_Mission_ABC >& parent )
    : MIL_Mission_ABC          ( type, automate.GetKnowledge(), id, parent )
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
MIL_AutomateMission::MIL_AutomateMission( const MIL_MissionType_ABC& type,
                                          MIL_Automate& automate,
                                          uint32_t id,
                                          const sword::MissionParameters& parameters )
    : MIL_Mission_ABC          ( type, automate.GetKnowledge(), id, parameters, automate.GetPosition() )
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
MIL_AutomateMission::MIL_AutomateMission( MIL_Automate& automate,
                                          const MIL_AutomateMission& rhs,
                                          uint32_t id )
    : MIL_Mission_ABC          ( rhs, automate.GetKnowledge(), id )
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::CreateCopy
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > MIL_AutomateMission::CreateCopy( MIL_Automate& target, uint32_t id ) const
{
    return boost::make_shared< MIL_AutomateMission >( target, *this, id );
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
void MIL_AutomateMission::Start( boost::shared_ptr< MIL_Mission_ABC > self, ActionManager& actions )
{
    assert( !bDIAMrtBehaviorActivated_ );

    static_cast< DEC_AutomateDecision& >( automate_.GetDecision() ).StartMissionMrtBehavior( self );
    bDIAMrtBehaviorActivated_ = true;
    Send( actions );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::Stop
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_AutomateMission::Stop( boost::shared_ptr< MIL_Mission_ABC > self )
{
    if( bDIAMrtBehaviorActivated_ )
        static_cast< DEC_AutomateDecision& >( automate_.GetDecision() ).StopMissionMrtBehavior( self );
    if( bDIACdtBehaviorActivated_ )
        static_cast< DEC_AutomateDecision& >( automate_.GetDecision() ).StopMissionConduiteBehavior( self );

    SendNoMission( automate_ );

    bDIACdtBehaviorActivated_ = false;
    bDIAMrtBehaviorActivated_ = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::GoToCdt
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateMission::GoToCdt( boost::shared_ptr< MIL_Mission_ABC > self )
{
    assert(  bDIAMrtBehaviorActivated_ );
    assert( !bDIACdtBehaviorActivated_ );

    static_cast< DEC_AutomateDecision& >( automate_.GetDecision() ).StopMissionMrtBehavior( self );
    bDIAMrtBehaviorActivated_ = false;

    static_cast< DEC_AutomateDecision& >( automate_.GetDecision() ).StartMissionConduiteBehavior( self );
    bDIACdtBehaviorActivated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void MIL_AutomateMission::SendNoMission( const MIL_Automate& automate )
{
    client::AutomatOrder asn;
    asn().mutable_tasker()->set_id( automate.GetID() );
    asn().mutable_type()->set_id( 0 );
    asn().mutable_parameters();
    NET_ASN_Tools::WriteGDH( MIL_Time_ABC::GetTime().GetRealTime(), *asn().mutable_start_time() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::Send
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_AutomateMission::Send( ActionManager& actions ) const
{
    client::AutomatOrder asn;
    asn().mutable_tasker()->set_id( automate_.GetID() );
    asn().mutable_type()->set_id( GetType().GetID() );
    MIL_Mission_ABC::Serialize( *asn().mutable_parameters() );
    NET_ASN_Tools::WriteGDH( MIL_Time_ABC::GetTime().GetRealTime(), *asn().mutable_start_time() );
    asn().set_name( GetName() );
    asn().set_id( GetId() );
    asn.Send( NET_Publisher_ABC::Publisher() );
    const auto action = actions.Register( asn() );
    if( action.created )
        actions.Send( action.id, 0, "" );
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
