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
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_PionMission )

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion, const sword::UnitOrder& asn )
    : MIL_Mission_ABC( type, pion.GetKnowledge(), asn.parameters(), pion.GetRole< PHY_RoleInterface_Location >().GetPosition() )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( const MIL_MissionType_ABC& type, MIL_AgentPion& pion, const boost::shared_ptr< MIL_Mission_ABC > parent )
    : MIL_Mission_ABC( type, pion.GetKnowledge(), parent )
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
    : MIL_Mission_ABC( type, pion.GetKnowledge() )
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
    : MIL_Mission_ABC( pion.GetKnowledge(), rhs )
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::CreateCopy
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > MIL_PionMission::CreateCopy( MIL_AgentPion& target ) const
{
    return boost::shared_ptr< MIL_Mission_ABC >( new MIL_PionMission( target, *this ) );
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
void MIL_PionMission::Start( boost::shared_ptr< MIL_Mission_ABC > self )
{
    assert( !bDIABehaviorActivated_ );
    pion_.GetDecision().StartMissionBehavior( self );
    bDIABehaviorActivated_ = true;
    Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::Stop
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionMission::Stop( boost::shared_ptr< MIL_Mission_ABC > self )
{
    if( bDIABehaviorActivated_ )
    {
        pion_.GetDecision().StopMissionBehavior( self );
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
    client::UnitOrder asn;
    asn().mutable_tasker()->set_id( pion.GetID() );
    asn().mutable_type()->set_id( 0 );
    asn().mutable_parameters();
    NET_ASN_Tools::WriteGDH( MIL_AgentServer::GetWorkspace().GetRealTime(), *asn().mutable_start_time() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::Send
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionMission::Send() const
{
    client::UnitOrder asn;
    asn().mutable_tasker()->set_id( pion_.GetID() );
    asn().mutable_type()->set_id( GetType().GetID() );
    Serialize( *asn().mutable_parameters() );
    NET_ASN_Tools::WriteGDH( MIL_AgentServer::GetWorkspace().GetRealTime(), *asn().mutable_start_time() );
    asn.Send( NET_Publisher_ABC::Publisher() );
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

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_PionMission* mission, const unsigned int /*version*/ )
{
    const MIL_AgentPion* const pion = &mission->pion_;
    unsigned int id = mission->type_.GetID();
    archive << pion
            << id;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_PionMission* mission, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion = 0;
    unsigned int id = 0;
    archive >> pion
            >> id;
    const MIL_MissionType_ABC* type = MIL_PionMissionType::Find( id );
    assert( type );
    ::new( mission )MIL_PionMission( *type, *pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::load
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
void MIL_PionMission::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Mission_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::save
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
void MIL_PionMission::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_Mission_ABC >( *this );
}
