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

// -----------------------------------------------------------------------------
// Name: MaintenanceStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MaintenanceStates::MaintenanceStates( Controller& controller )
    : controller_( controller )
{

}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MaintenanceStates::~MaintenanceStates()
{

}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void MaintenanceStates::Update( const ASN1T_MsgLogMaintenanceEtat& message )
{
    if( message.m.chaine_activeePresent )
        bChainEnabled_ = message.chaine_activee;
    if( message.m.temps_de_bordeePresent )
        nTempsBordee_ = 8 + message.temps_de_bordee * 4;

    if( message.m.prioritesPresent )
    {
        priorities_.resize( message.priorites.n );
        for( uint i = 0; i < message.priorites.n; ++i )
            priorities_[i] = message.priorites.elem[i];
    }   
    if( message.m.priorites_tactiquesPresent )
    {
        tacticalPriorities_.resize( message.priorites_tactiques.n );
        for( uint i = 0; i < message.priorites_tactiques.n; ++i )
            tacticalPriorities_[i] = message.priorites_tactiques.elem[i];
    }
    if( message.m.disponibilites_remorqueursPresent )
    {
        dispoHaulers_.resize( message.disponibilites_remorqueurs.n );
        for( uint i = 0; i < message.disponibilites_remorqueurs.n; ++i )
            dispoHaulers_[i] = std::make_pair( message.disponibilites_remorqueurs.elem[i].type_equipement, message.disponibilites_remorqueurs.elem[i].pourcentage_disponibilite );
    }
    if( message.m.disponibilites_reparateursPresent )
    {
        dispoRepairers_.resize( message.disponibilites_reparateurs.n );
        for( uint i = 0; i < message.disponibilites_reparateurs.n; ++i )
            dispoRepairers_[i] = std::make_pair( message.disponibilites_reparateurs.elem[i].type_equipement, message.disponibilites_reparateurs.elem[i].pourcentage_disponibilite );
    }

    controller_.Update( *this );
}
