// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MaintenanceStates.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MaintenanceStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MaintenanceStates::MaintenanceStates( Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const tools::Resolver_ABC< Automat_ABC >& automatResolver, PropertiesDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
    , automatResolver_( automatResolver )
    , bChainEnabled_( false )
    , nWorkRate_( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MaintenanceStates::~MaintenanceStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void MaintenanceStates::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "MaintenanceStates", "Maintenance system/System enabled" ), bChainEnabled_ );
    dico.Register( *this, tools::translate( "MaintenanceStates", "Maintenance system/Priorities" ), priorities_ );
    dico.Register( *this, tools::translate( "MaintenanceStates", "Maintenance system/Tactical priorities" ), tacticalPriorities_ );
    dico.Register( *this, tools::translate( "MaintenanceStates", "Maintenance system/Working scheme" ), nWorkRate_ );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void MaintenanceStates::DoUpdate( const MsgsSimToClient::MsgLogMaintenanceState& message )
{
    if( message.has_chaine_activee()  )
        bChainEnabled_ = message.chaine_activee() != 0;
    if( message.has_regime_travail()  )
        nWorkRate_ = message.regime_travail() + 1; // $$$$ AGE 2006-06-27:

    if( message.has_priorites()  )
    {
        priorities_.resize( message.priorites().elem_size() );
        for( int i = 0; i < message.priorites().elem_size(); ++i )
            priorities_[i] = & resolver_.Get( message.priorites().elem( i ).id() );
    }
    if( message.has_priorites_tactiques()  )
    {
        tacticalPriorities_.resize( message.priorites_tactiques().elem_size() );
        for( int i = 0; i < message.priorites_tactiques().elem_size(); ++i )
            tacticalPriorities_[i] = & automatResolver_.Get( message.priorites_tactiques().elem( i ).id() );
    }
    if( message.has_disponibilites_remorqueurs()  )
    {
        dispoHaulers_.resize( message.disponibilites_remorqueurs().elem_size() );
        for( int i = 0; i < message.disponibilites_remorqueurs().elem_size(); ++i )
            dispoHaulers_[i] = Availability( resolver_, message.disponibilites_remorqueurs().elem( i ) );
    }
    if( message.has_disponibilites_reparateurs()  )
    {
        dispoRepairers_.resize( message.disponibilites_reparateurs().elem_size() );
        for( int i = 0; i < message.disponibilites_reparateurs().elem_size(); ++i )
            dispoRepairers_[i] = Availability( resolver_, message.disponibilites_reparateurs().elem( i ) );
    }

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void MaintenanceStates::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "MaintenanceStates", "Maintenance system state" ) )
                .Display( tools::translate( "MaintenanceStates", "System status" ), bChainEnabled_ ?
                        tools::translate( "MaintenanceStates", "Enabled" ) : tools::translate( "MaintenanceStates", "Disabled" ) )
                .Display( tools::translate( "MaintenanceStates", "Working scheme" ), tools::translate( "MaintenanceStates", "R%1" ).arg( nWorkRate_ ) )
                .Display( tools::translate( "MaintenanceStates", "Priorities" ), priorities_ )
                .Display( tools::translate( "MaintenanceStates", "Tactical priorities" ), tacticalPriorities_ );
}
