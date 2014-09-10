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

#include "ActionManager.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "MIL_AgentServer.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional/optional.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_PionMission )

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( const MIL_MissionType_ABC& type,
                                  MIL_AgentPion& pion,
                                  uint32_t id,
                                  const boost::shared_ptr< MIL_Mission_ABC >& parent )
    : MIL_Mission_ABC       ( type, pion.GetKnowledge(), id, parent )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( const MIL_MissionType_ABC& type,
                                  MIL_AgentPion& pion,
                                  uint32_t id,
                                  const sword::MissionParameters& parameters )
    : MIL_Mission_ABC       ( type, pion.GetKnowledge(), id, parameters,
                              pion.GetRole< PHY_RoleInterface_Location >().GetPosition() )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( MIL_AgentPion& pion,
                                 const MIL_PionMission& rhs,
                                 uint32_t id )
    : MIL_Mission_ABC       ( rhs, pion.GetKnowledge(), id )
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
boost::shared_ptr< MIL_Mission_ABC > MIL_PionMission::CreateCopy( MIL_AgentPion& target, uint32_t id ) const
{
    return boost::make_shared< MIL_PionMission >( target, *this, id );
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
void MIL_PionMission::Start( boost::shared_ptr< MIL_Mission_ABC > self, ActionManager& actions )
{
    assert( !bDIABehaviorActivated_ );
    pion_.GetDecision().StartMissionBehavior( self );
    bDIABehaviorActivated_ = true;
    Send( actions );
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
    if( pion.IsMarkedForDestruction() )
        return;
    client::UnitOrder asn;
    asn().mutable_tasker()->set_id( pion.GetID() );
    asn().mutable_type()->set_id( 0 );
    asn().mutable_parameters();
    NET_ASN_Tools::WriteGDH( MIL_Time_ABC::GetTime().GetRealTime(), *asn().mutable_start_time() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::Send
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionMission::Send( ActionManager& actions ) const
{
    if( pion_.IsMarkedForDestruction() )
        return;
    client::UnitOrder asn;
    asn().mutable_tasker()->set_id( pion_.GetID() );
    asn().mutable_type()->set_id( GetType().GetID() );
    Serialize( *asn().mutable_parameters() );
    NET_ASN_Tools::WriteGDH( MIL_Time_ABC::GetTime().GetRealTime(), *asn().mutable_start_time() );
    asn().set_name( GetName() );
    asn().set_id( GetId() );
    asn().set_parent( GetParentId() );
    asn.Send( NET_Publisher_ABC::Publisher() );
    const auto action = actions.Register( asn() );
    if( action.created )
        actions.Send( action.id, 0, "" );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::GetPion
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_PionMission::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::GetOwnerId
// Created: ABR 2012-02-13
// -----------------------------------------------------------------------------
unsigned int MIL_PionMission::GetOwnerId() const
{
    return pion_.GetID();
}

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_PionMission* mission, const unsigned int /*version*/ )
{
    const MIL_AgentPion* const pion = &mission->pion_;
    unsigned int idType = mission->type_.GetID();
    uint32_t id = mission->GetId();
    archive << pion;
    archive << idType;
    archive << id;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_PionMission* mission, const unsigned int /*version*/ )
{
    try
    {
        MIL_AgentPion* pion = 0;
        unsigned int idType = 0;
        uint32_t id = 0;
        archive >> pion;
        archive >> idType;
        archive >> id;
        const MIL_MissionType_ABC* type = MIL_PionMissionType::Find( idType );
        if( !type )
            throw MASA_EXCEPTION( "unknown unit mission type: "
                    + boost::lexical_cast< std::string >( idType ));
        ::new( mission ) MIL_PionMission( *type, *pion, id, boost::shared_ptr< MIL_Mission_ABC >() );
    }
    catch( const std::exception& e )
    {
        throw MASA_EXCEPTION( "could not load unit mission: " + tools::GetExceptionMsg( e ));
    }
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
