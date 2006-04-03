// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "MaintenanceStates.h"
#include "Controller.h"
#include "Displayer_ABC.h"
#include "Equipment.h"
#include "Units.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MaintenanceStates::MaintenanceStates( Controller& controller, const Resolver_ABC< EquipmentType >& resolver, const Resolver_ABC< Agent >& agentResolver )
    : controller_( controller )
    , resolver_( resolver )
    , agentResolver_( agentResolver )
{
    // NOTHING
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
// Name: MaintenanceStates::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void MaintenanceStates::DoUpdate( const ASN1T_MsgLogMaintenanceEtat& message )
{
    if( message.m.chaine_activeePresent )
        bChainEnabled_ = message.chaine_activee;
    if( message.m.regime_travailPresent )
        nWorkRate_ = message.regime_travail;

    if( message.m.prioritesPresent )
    {
        priorities_.resize( message.priorites.n );
        for( uint i = 0; i < message.priorites.n; ++i )
            priorities_[i] = & resolver_.Get( message.priorites.elem[i] );
    }   
    if( message.m.priorites_tactiquesPresent )
    {
        tacticalPriorities_.resize( message.priorites_tactiques.n );
        for( uint i = 0; i < message.priorites_tactiques.n; ++i )
            tacticalPriorities_[i] = & agentResolver_.Get( message.priorites_tactiques.elem[i] );
    }
    if( message.m.disponibilites_remorqueursPresent )
    {
        dispoHaulers_.resize( message.disponibilites_remorqueurs.n );
        for( uint i = 0; i < message.disponibilites_remorqueurs.n; ++i )
            dispoHaulers_[i] = Availability( resolver_, message.disponibilites_remorqueurs.elem[i] );
    }
    if( message.m.disponibilites_reparateursPresent )
    {
        dispoRepairers_.resize( message.disponibilites_reparateurs.n );
        for( uint i = 0; i < message.disponibilites_reparateurs.n; ++i )
            dispoRepairers_[i] = Availability( resolver_, message.disponibilites_reparateurs.elem[i] );
    }

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void MaintenanceStates::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Etat chaine maintenance" )
                .Display( "Etat chaine", bChainEnabled_ ? "Activée" : "Désactivée" )
                .Display( "Régime de travail", QString( "R%1" ).arg( nWorkRate_ ) ) // $$$$ AGE 2006-04-03: 
                .Display( "Priorités", priorities_ )
                .Display( "Priorités tactiques", tacticalPriorities_ );
}
