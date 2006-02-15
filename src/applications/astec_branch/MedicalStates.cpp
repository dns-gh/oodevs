// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "MedicalStates.h"
#include "Controller.h"


// -----------------------------------------------------------------------------
// Name: MedicalStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalStates::MedicalStates( Controller& controller )
    : controller_( controller )
{

}

// -----------------------------------------------------------------------------
// Name: MedicalStates destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalStates::~MedicalStates()
{

}

// -----------------------------------------------------------------------------
// Name: MedicalStates::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void MedicalStates::DoUpdate( const ASN1T_MsgLogSanteEtat& message )
{
    if( message.m.chaine_activeePresent )
        bChainEnabled_ = message.chaine_activee;

    if( message.m.temps_de_bordeePresent )
        nTempsBordee_ = 8 + 4 * message.temps_de_bordee;

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

    if( message.m.disponibilites_ambulances_ramassagePresent )
    {
        dispoRamassageAmbulances_.resize( message.disponibilites_ambulances_ramassage.n );
        for( uint i = 0; i < message.disponibilites_ambulances_ramassage.n; ++i )
            dispoRamassageAmbulances_[i] = std::make_pair( message.disponibilites_ambulances_ramassage.elem[i].type_equipement, message.disponibilites_ambulances_ramassage.elem[i].pourcentage_disponibilite );
    }
    if( message.m.disponibilites_ambulances_relevePresent )
    {
        dispoReleveAmbulances_.resize( message.disponibilites_ambulances_releve.n );
        for( uint i = 0; i < message.disponibilites_ambulances_releve.n; ++i )
            dispoReleveAmbulances_[i] = std::make_pair( message.disponibilites_ambulances_releve.elem[i].type_equipement, message.disponibilites_ambulances_releve.elem[i].pourcentage_disponibilite );
    }
    if( message.m.disponibilites_medecinsPresent )
    {
        dispoDoctors_.resize( message.disponibilites_medecins.n );
        for( uint i = 0; i < message.disponibilites_medecins.n; ++i )
            dispoDoctors_[i] = std::make_pair( message.disponibilites_medecins.elem[i].type_equipement, message.disponibilites_medecins.elem[i].pourcentage_disponibilite );
    }
    controller_.Update( *this );
}
