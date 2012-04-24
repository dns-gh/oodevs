// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MaintenanceStates.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/DictionaryUpdated.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MaintenanceStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MaintenanceStates::MaintenanceStates( kernel::Entity_ABC& entity, Controller& controller,const tools::Resolver_ABC< kernel::EquipmentType >& resolver,
                                      const tools::Resolver_ABC< Automat_ABC >& automatResolver, PropertiesDictionary& dico )
    : entity_         ( entity )
    , controller_     ( controller )
    , resolver_       ( resolver )
    , automatResolver_( automatResolver )
    , bChainEnabled_  ( false )
    , nWorkRate_      ( 0 )
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
    dico.Register( entity_, tools::translate( "MaintenanceStates", "Maintenance system/System enabled" ), bChainEnabled_ );
    dico.Register( entity_, tools::translate( "MaintenanceStates", "Maintenance system/Priorities" ), priorities_ );
    dico.Register( entity_, tools::translate( "MaintenanceStates", "Maintenance system/Tactical priorities" ), tacticalPriorities_ );
    dico.Register( entity_, tools::translate( "MaintenanceStates", "Maintenance system/Working scheme" ), nWorkRate_ );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void MaintenanceStates::DoUpdate( const sword::LogMaintenanceState& message )
{
    if( message.has_chain()  )
        bChainEnabled_ = message.chain() != 0;

    if( message.has_work_rate()  )
        nWorkRate_ = message.work_rate() + 1; // $$$$ AGE 2006-06-27:

    if( message.has_priorities() )
    {
        priorities_.resize( message.priorities().elem_size() );
        for( int i = 0; i < message.priorities().elem_size(); ++i )
            priorities_[i] = & resolver_.Get( message.priorities().elem( i ).id() );
    }
    if( message.has_tactical_priorities() )
    {
        tacticalPriorities_.resize( message.tactical_priorities().elem_size() );
        for( int i = 0; i < message.tactical_priorities().elem_size(); ++i )
            tacticalPriorities_[i] = & automatResolver_.Get( message.tactical_priorities().elem( i ).id() );
    }
    if( message.has_haulers() )
    {
        dispoHaulers_.resize( message.haulers().elem_size() );
        for( int i = 0; i < message.haulers().elem_size(); ++i )
            dispoHaulers_[i] = kernel::Availability( resolver_, message.haulers().elem( i ) );
    }
    if( message.has_repairers() )
    {
        dispoRepairers_.resize( message.repairers().elem_size() );
        for( int i = 0; i < message.repairers().elem_size(); ++i )
            dispoRepairers_[i] = kernel::Availability( resolver_, message.repairers().elem( i ) );
    }

    if( message.has_priorities() || message.has_work_rate() || message.has_tactical_priorities() || message.has_chain() )
        controller_.Update( kernel::DictionaryUpdated( entity_, tools::translate( "MaintenanceStates", "Maintenance system" ) ) );
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
                .Display( tools::translate( "MaintenanceStates", "Working scheme" ), tools::translate( "MaintenanceStates", "R%L1" ).arg( nWorkRate_ ) )
                .Display( tools::translate( "MaintenanceStates", "Priorities" ), priorities_ )
                .Display( tools::translate( "MaintenanceStates", "Tactical priorities" ), tacticalPriorities_ );
}

// -----------------------------------------------------------------------------
// Name: std::vector< const kernel::EquipmentType* > MaintenanceStates::GetPriorities
// Created: LDC 2012-04-24
// -----------------------------------------------------------------------------
const std::vector< const kernel::EquipmentType* >& MaintenanceStates::GetPriorities() const
{
    return priorities_;
}

// -----------------------------------------------------------------------------
// Name: std::vector< kernel::Availability >& MaintenanceStates::GetDispoHaulers
// Created: LDC 2012-04-24
// -----------------------------------------------------------------------------
const std::vector< kernel::Availability >& MaintenanceStates::GetDispoHaulers() const
{
    return dispoHaulers_;
}

// -----------------------------------------------------------------------------
// Name: std::vector< kernel::Availability >& MaintenanceStates::GetDispoRepairers
// Created: LDC 2012-04-24
// -----------------------------------------------------------------------------
const std::vector< kernel::Availability >& MaintenanceStates::GetDispoRepairers() const
{
    return dispoRepairers_;
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::HasPriority
// Created: LDC 2012-04-24
// -----------------------------------------------------------------------------
bool MaintenanceStates::HasPriority( const kernel::EquipmentType* type ) const
{
    return std::find( priorities_.begin(), priorities_.end(), type ) != priorities_.end();
}