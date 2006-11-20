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
MaintenanceStates::MaintenanceStates( Controller& controller, const Resolver_ABC< EquipmentType >& resolver, const Resolver_ABC< Agent_ABC >& agentResolver, PropertiesDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
    , agentResolver_( agentResolver )
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
    dico.Register( *this, tools::translate( "Logistique", "Chaine maintenance/Chaine activée" ), bChainEnabled_ );
    dico.Register( *this, tools::translate( "Logistique", "Chaine maintenance/Priorités" ), priorities_ );
    dico.Register( *this, tools::translate( "Logistique", "Chaine maintenance/Priorités tactiques" ), tacticalPriorities_ );
    dico.Register( *this, tools::translate( "Logistique", "Chaine maintenance/Régime de travail" ), nWorkRate_ );
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
        nWorkRate_ = message.regime_travail + 1; // $$$$ AGE 2006-06-27: 

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
    displayer.Group( tools::translate( "Logistique", "Etat chaine maintenance" ) )
                .Display( tools::translate( "Logistique", "Etat chaine" ), bChainEnabled_ ?
                        tools::translate( "Logistique", "Activée" ) : tools::translate( "Logistique", "Désactivée" ) )
                .Display( tools::translate( "Logistique", "Régime de travail" ), tools::translate( "Logistique", "R%1" ).arg( nWorkRate_ ) ) // $$$$ AGE 2006-04-03: 
                .Display( tools::translate( "Logistique", "Priorités" ), priorities_ )
                .Display( tools::translate( "Logistique", "Priorités tactiques" ), tacticalPriorities_ );
}
