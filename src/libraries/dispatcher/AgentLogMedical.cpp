// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AgentLogMedical.h"
#include "Model_ABC.h"
#include "Automat_ABC.h"
#include "Agent_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogMedical constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogMedical::AgentLogMedical( const Model_ABC& model, const kernel::Agent_ABC& agent, const MsgsSimToClient::MsgLogMedicalState& asnMsg )
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
void AgentLogMedical::Update( const MsgsSimToClient::MsgLogMedicalState& asnMsg )
{
    if( asnMsg.has_chaine_activee()  )
        bSystemEnabled_ = asnMsg.chaine_activee() != 0;

    if( asnMsg.has_disponibilites_ambulances_ramassage()  )
    {
        collectionAmbulancesAvailability_.clear();
        for( int i = 0; i < asnMsg.disponibilites_ambulances_ramassage().elem_size(); ++i )
            collectionAmbulancesAvailability_.push_back( T_Availability( asnMsg.disponibilites_ambulances_ramassage().elem( i ) ) );
    }

    if( asnMsg.has_disponibilites_ambulances_releve()  )
    {
        evacuationAmbulancesAvailability_.clear();
        for( int i = 0; i < asnMsg.disponibilites_ambulances_releve().elem_size(); ++i )
            evacuationAmbulancesAvailability_.push_back( T_Availability( asnMsg.disponibilites_ambulances_releve().elem( i ) ) );
    }

    if( asnMsg.has_disponibilites_medecins()  )
    {
        doctorsAvailability_.clear();
        for( int i = 0; i < asnMsg.disponibilites_medecins().elem_size(); ++i )
            doctorsAvailability_.push_back( T_Availability( asnMsg.disponibilites_medecins().elem( i ) ) );
    }

    if( asnMsg.has_priorites_tactiques()  )
    {
        tacticalPriorities_.clear();
        for( int i = 0; i < asnMsg.priorites_tactiques().elem_size(); ++i )
            tacticalPriorities_.push_back( &model_.Automats().Get( asnMsg.priorites_tactiques().elem( i ).oid() ) );
    }

    if( asnMsg.has_priorites()  )
    {
        priorities_.clear();
        for( int i = 0; i < asnMsg.priorites().elem_size(); ++i )
            priorities_.push_back( asnMsg.priorites().elem( i ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentLogMedical::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMedical::Send( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalState asn;
    asn().set_oid_pion ( agent_.GetId() );
    asn().set_chaine_activee ( bSystemEnabled_ );

    {
        for( std::vector< T_Availability >::const_iterator it = evacuationAmbulancesAvailability_.begin(); it != evacuationAmbulancesAvailability_.end(); ++it )
            it->Send( *asn().mutable_disponibilites_ambulances_releve()->add_elem() );
    }
    {
        for( std::vector< T_Availability >::const_iterator it = collectionAmbulancesAvailability_.begin(); it != collectionAmbulancesAvailability_.end(); ++it )
            it->Send( *asn().mutable_disponibilites_ambulances_ramassage()->add_elem() );
    }
    {
        for( std::vector< T_Availability >::const_iterator it = doctorsAvailability_.begin(); it != doctorsAvailability_.end(); ++it )
            it->Send( *asn().mutable_disponibilites_medecins()->add_elem() );
    }
    {
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = tacticalPriorities_.begin(); it != tacticalPriorities_.end(); ++it )
            asn().mutable_priorites_tactiques()->add_elem()->set_oid( (*it)->GetId() );
    }
    {
        for( std::vector< Common::EnumHumanWound >::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
            asn().mutable_priorites()->add_elem( *it );
    }

    asn.Send( publisher );

    if( asn().disponibilites_ambulances_releve().elem_size() > 0 )
        asn().mutable_disponibilites_ambulances_releve()->Clear();
    if( asn().disponibilites_ambulances_ramassage().elem_size() > 0 )
        asn().mutable_disponibilites_ambulances_ramassage()->Clear();
    if( asn().disponibilites_medecins().elem_size() > 0 )
        asn().mutable_disponibilites_medecins()->Clear();
    if( asn().priorites().elem_size() > 0 )
        asn().mutable_priorites()->Clear();
    if( asn().priorites_tactiques().elem_size() > 0 )
        asn().mutable_priorites_tactiques()->Clear();
}
