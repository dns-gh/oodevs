// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AgentLogMedical.h"
#include "Model_ABC.h"
#include "Automat_ABC.h"
#include "Agent_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "MedicalEquipmentAvailability.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogMedical constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogMedical::AgentLogMedical( const Model_ABC& model, const kernel::Agent_ABC& agent, const sword::LogMedicalState& asnMsg )
    : model_         ( model )
    , agent_         ( agent )
    , bSystemEnabled_( false )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentLogMedical destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
AgentLogMedical::~AgentLogMedical()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentLogMedical::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMedical::Update( const sword::LogMedicalState& asnMsg )
{
    if( asnMsg.has_chain()  )
        bSystemEnabled_ = asnMsg.chain() != 0;

    collectionAmbulancesAvailability_.clear();
    for( int i = 0; i < asnMsg.collection_ambulances().elem_size(); ++i )
        collectionAmbulancesAvailability_.push_back( MedicalEquipmentAvailability( asnMsg.collection_ambulances().elem( i ) ) );

    evacuationAmbulancesAvailability_.clear();
    for( int i = 0; i < asnMsg.evacuation_ambulances().elem_size(); ++i )
        evacuationAmbulancesAvailability_.push_back( MedicalEquipmentAvailability( asnMsg.evacuation_ambulances().elem( i ) ) );

    doctorsAvailability_.clear();
    for( int i = 0; i < asnMsg.doctors().elem_size(); ++i )
        doctorsAvailability_.push_back( MedicalEquipmentAvailability( asnMsg.doctors().elem( i ) ) );

    tacticalPriorities_.clear();
    for( int i = 0; i < asnMsg.tactical_priorities().elem_size(); ++i )
        tacticalPriorities_.push_back( asnMsg.tactical_priorities().elem( i ) );

    priorities_.clear();
    for( int i = 0; i < asnMsg.priorities().elem_size(); ++i )
        priorities_.push_back( asnMsg.priorities().elem( i ) );
}

// -----------------------------------------------------------------------------
// Name: AgentLogMedical::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMedical::Send( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalState asn;
    asn().mutable_unit()->set_id( agent_.GetId() );
    asn().set_chain ( bSystemEnabled_ );
    for( auto it = evacuationAmbulancesAvailability_.begin(); it != evacuationAmbulancesAvailability_.end(); ++it )
        it->Send( *asn().mutable_evacuation_ambulances()->add_elem() );
    for( auto it = collectionAmbulancesAvailability_.begin(); it != collectionAmbulancesAvailability_.end(); ++it )
        it->Send( *asn().mutable_collection_ambulances()->add_elem() );
    for( auto it = doctorsAvailability_.begin(); it != doctorsAvailability_.end(); ++it )
        it->Send( *asn().mutable_doctors()->add_elem() );
    for( auto it = tacticalPriorities_.begin(); it != tacticalPriorities_.end(); ++it )
        asn().mutable_tactical_priorities()->add_elem()->set_id( it->id() );
    for( auto it = priorities_.begin(); it != priorities_.end(); ++it )
        asn().mutable_priorities()->add_elem( *it );
    asn.Send( publisher );
}
