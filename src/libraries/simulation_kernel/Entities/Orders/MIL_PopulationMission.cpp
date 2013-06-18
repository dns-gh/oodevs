// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationMission.h"
#include "MIL_MissionType_ABC.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Entities/Orders/MIL_PopulationMissionType.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_PopulationMission )

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PopulationMission::MIL_PopulationMission( const MIL_MissionType_ABC& type, MIL_Population& population, const sword::CrowdOrder& asn )
    : MIL_Mission_ABC       ( type, population.GetKnowledge(), asn.parameters() )
    , population_           ( population )
    , bDIABehaviorActivated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission constructor
// Created: ABR 2012-02-13
// -----------------------------------------------------------------------------
MIL_PopulationMission::MIL_PopulationMission( const MIL_MissionType_ABC& type, MIL_Population& population )
    : MIL_Mission_ABC       ( type, population.GetKnowledge() )
    , population_           ( population )
    , bDIABehaviorActivated_( false )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission destructor
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
MIL_PopulationMission::~MIL_PopulationMission()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::IsFragOrderAvailable
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_PopulationMission::IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const
{
    return population_.GetType().GetModel().IsFragOrderAvailableForMission( GetType(), fragOrderType );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::Start
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationMission::Start( boost::shared_ptr< MIL_Mission_ABC > self )
{
    assert( !bDIABehaviorActivated_ );

    population_.GetDecision().StartMissionBehavior( self );
    bDIABehaviorActivated_ = true;
    Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::Stop
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationMission::Stop( boost::shared_ptr< MIL_Mission_ABC > self )
{
    if( bDIABehaviorActivated_ )
    {
        population_.GetDecision().StopMissionBehavior( self );
        bDIABehaviorActivated_ = false;
    }
    SendNoMission( population_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void MIL_PopulationMission::SendNoMission( const MIL_Population& population )
{
    client::CrowdOrder asn;
    asn().mutable_tasker()->set_id( population.GetID() );
    asn().mutable_type()->set_id( 0 );
    NET_ASN_Tools::WriteGDH( MIL_Time_ABC::GetTime().GetRealTime(), *asn().mutable_start_time() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::Send
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationMission::Send() const
{
    client::CrowdOrder asn;
    asn().mutable_tasker()->set_id( population_.GetID() );
    asn().mutable_type()->set_id( GetType().GetID() );
    Serialize( *asn().mutable_parameters() );
    NET_ASN_Tools::WriteGDH( MIL_Time_ABC::GetTime().GetRealTime(), *asn().mutable_start_time() );
    asn().set_name( GetName() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::GetOwnerId
// Created: ABR 2012-02-13
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationMission::GetOwnerId() const
{
    return population_.GetID();
}

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_PopulationMission* mission, const unsigned int /*version*/ )
{
    const MIL_Population* const population = &mission->population_;
    unsigned int id = mission->type_.GetID();
    archive << population
            << id;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_PopulationMission* mission, const unsigned int /*version*/ )
{
    MIL_Population* population = 0;
    unsigned int id = 0;
    archive >> population
            >> id;
    const MIL_MissionType_ABC* type = MIL_PopulationMissionType::Find( id );
    assert( type );
    ::new( mission )MIL_PopulationMission( *type, *population );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::load
// Created: ABR 2012-02-13
// -----------------------------------------------------------------------------
void MIL_PopulationMission::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Mission_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::save
// Created: ABR 2012-02-13
// -----------------------------------------------------------------------------
void MIL_PopulationMission::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_Mission_ABC >( *this );
}
