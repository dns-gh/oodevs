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
#include "clients_kernel/Units.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MedicalStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalStates::MedicalStates( Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const tools::Resolver_ABC< Automat_ABC >& automatResolver, PropertiesDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
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
void MedicalStates::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "MedicalStates", "Medical system/System enabled" ), bChainEnabled_ );
    dico.Register( *this, tools::translate( "MedicalStates", "Medical system/Priorities" ), priorities_ );
    dico.Register( *this, tools::translate( "MedicalStates", "Medical system/Tactical priorities" ), tacticalPriorities_ );
}

// -----------------------------------------------------------------------------
// Name: MedicalStates::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void MedicalStates::DoUpdate( const MsgsSimToClient::MsgLogMedicalState& message )
{
    if( message.has_chaine_activee()  )
        bChainEnabled_ = message.chaine_activee() != 0;

    if( message.has_priorites()  )
    {
        priorities_.resize( message.priorites().elem_size() );
        for( int i = 0; i < message.priorites().elem_size(); ++i )
            priorities_[i] = E_HumanWound( message.priorites().elem( i ) );
    }
    if( message.has_tactical_priorities()  )
    {
        tacticalPriorities_.resize( message.tactical_priorities().elem_size() );
        for( int i = 0; i < message.tactical_priorities().elem_size(); ++i )
            tacticalPriorities_[i] = & automatResolver_.Get( message.tactical_priorities().elem( i ).id() );
    }

    if( message.has_disponibilites_ambulances_ramassage()  )
    {
        dispoRamassageAmbulances_.resize( message.disponibilites_ambulances_ramassage().elem_size() );
        for( int i = 0; i < message.disponibilites_ambulances_ramassage().elem_size(); ++i )
            dispoRamassageAmbulances_[i] = Availability( resolver_, message.disponibilites_ambulances_ramassage().elem( i ) );
    }
    if( message.has_disponibilites_ambulances_releve()  )
    {
        dispoReleveAmbulances_.resize( message.disponibilites_ambulances_releve().elem_size() );
        for( int i = 0; i < message.disponibilites_ambulances_releve().elem_size(); ++i )
            dispoReleveAmbulances_[i] = Availability( resolver_, message.disponibilites_ambulances_releve().elem( i ) );
    }
    if( message.has_disponibilites_medecins()  )
    {
        dispoDoctors_.resize( message.disponibilites_medecins().elem_size() );
        for( int i = 0; i < message.disponibilites_medecins().elem_size(); ++i )
            dispoDoctors_[i] = Availability( resolver_, message.disponibilites_medecins().elem( i ) );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MedicalStates::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void MedicalStates::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "MedicalStates", "Medical system state" ) )
                .Display( tools::translate( "MedicalStates", "System status" ), bChainEnabled_ ?
                    tools::translate( "MedicalStates", "Enabled" ) : tools::translate( "MedicalStates", "Disabled" ) )
                .Display( tools::translate( "MedicalStates", "Priorities" ), priorities_ )
                .Display( tools::translate( "MedicalStates", "Tactical priorities" ), tacticalPriorities_ );
}
