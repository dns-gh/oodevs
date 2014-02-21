// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MedicalStates.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MedicalStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalStates::MedicalStates( kernel::Entity_ABC& entity, Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const tools::Resolver_ABC< Automat_ABC >& automatResolver, gui::PropertiesDictionary& dico )
    : entity_         ( entity )
    , controller_     ( controller )
    , resolver_       ( resolver )
    , automatResolver_( automatResolver )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: MedicalStates destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalStates::~MedicalStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalStates::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void MedicalStates::CreateDictionary( gui::PropertiesDictionary& dico ) const
{
    dico.Register( entity_, tools::translate( "MedicalStates", "Medical system/System enabled" ), bChainEnabled_ );
    dico.Register( entity_, tools::translate( "MedicalStates", "Medical system/Priorities" ), priorities_ );
    dico.Register( entity_, tools::translate( "MedicalStates", "Medical system/Tactical priorities" ), tacticalPriorities_ );
}

// -----------------------------------------------------------------------------
// Name: MedicalStates::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void MedicalStates::DoUpdate( const sword::LogMedicalState& message )
{
    if( message.has_chain() )
        bChainEnabled_ = message.chain() != 0;

    priorities_.clear();
    for( int i = 0; i < message.priorities().elem_size(); ++i )
        priorities_.push_back( E_HumanWound( message.priorities().elem( i ) ) );

    tacticalPriorities_.clear();
    for( int i = 0; i < message.tactical_priorities().elem_size(); ++i )
        tacticalPriorities_.push_back( &automatResolver_.Get( message.tactical_priorities().elem( i ).id() ) );

    dispoRamassageAmbulances_.clear();
    for( int i = 0; i < message.collection_ambulances().elem_size(); ++i )
        dispoRamassageAmbulances_.push_back( kernel::Availability( entity_, resolver_, message.collection_ambulances().elem( i ) ) );

    dispoReleveAmbulances_.clear();
    for( int i = 0; i < message.evacuation_ambulances().elem_size(); ++i )
        dispoReleveAmbulances_.push_back( kernel::Availability( entity_, resolver_, message.evacuation_ambulances().elem( i ) ) );

    dispoDoctors_.clear();
    for( int i = 0; i < message.doctors().elem_size(); ++i )
        dispoDoctors_.push_back( kernel::Availability( entity_, resolver_, message.doctors().elem( i ) ) );

    controller_.Update( gui::DictionaryUpdated( entity_, tools::translate( "MedicalStates", "Medical system" ) ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MedicalStates::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void MedicalStates::Display( Displayer_ABC& displayer ) const
{
    auto& group = displayer.Group( "" );
    if( bChainEnabled_ )
        group.Display( tools::translate( "MedicalStates", "System status" ), tools::translate( "MedicalStates", "Enabled" ) );
    else
        group.Display( tools::translate( "MedicalStates", "System status" ), tools::translate( "MedicalStates", "Disabled" ) );
    group.Display( tools::translate( "MedicalStates", "Priorities" ), priorities_ )
        .Display( tools::translate( "MedicalStates", "Tactical priorities" ), tacticalPriorities_ );
}
