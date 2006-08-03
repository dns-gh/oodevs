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
#include "Displayer_ABC.h"
#include "Units.h"
#include "DataDictionary.h"

// -----------------------------------------------------------------------------
// Name: MedicalStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalStates::MedicalStates( Controller& controller, const Resolver_ABC< EquipmentType >& resolver, const Resolver_ABC< Agent_ABC >& agentResolver, DataDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
    , agentResolver_( agentResolver )
{
    dico.Register( "Chaine médicale/Chaine activée", bChainEnabled_ );
    dico.Register( "Chaine médicale/Priorités", priorities_ );
    dico.Register( "Chaine médicale/Priorités tactiques", tacticalPriorities_ );
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
// Name: MedicalStates::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void MedicalStates::DoUpdate( const ASN1T_MsgLogSanteEtat& message )
{
    if( message.m.chaine_activeePresent )
        bChainEnabled_ = message.chaine_activee;

    if( message.m.prioritesPresent )
    {
        priorities_.resize( message.priorites.n );
        for( uint i = 0; i < message.priorites.n; ++i )
            priorities_[i] = E_HumanWound( message.priorites.elem[i] );
    }
    if( message.m.priorites_tactiquesPresent )
    {
        tacticalPriorities_.resize( message.priorites_tactiques.n );
        for( uint i = 0; i < message.priorites_tactiques.n; ++i )
            tacticalPriorities_[i] = & agentResolver_.Get( message.priorites_tactiques.elem[i] );
    }

    if( message.m.disponibilites_ambulances_ramassagePresent )
    {
        dispoRamassageAmbulances_.resize( message.disponibilites_ambulances_ramassage.n );
        for( uint i = 0; i < message.disponibilites_ambulances_ramassage.n; ++i )
            dispoRamassageAmbulances_[i] = Availability( resolver_, message.disponibilites_ambulances_ramassage.elem[i] );
    }
    if( message.m.disponibilites_ambulances_relevePresent )
    {
        dispoReleveAmbulances_.resize( message.disponibilites_ambulances_releve.n );
        for( uint i = 0; i < message.disponibilites_ambulances_releve.n; ++i )
            dispoReleveAmbulances_[i] = Availability( resolver_, message.disponibilites_ambulances_releve.elem[i] );
    }
    if( message.m.disponibilites_medecinsPresent )
    {
        dispoDoctors_.resize( message.disponibilites_medecins.n );
        for( uint i = 0; i < message.disponibilites_medecins.n; ++i )
            dispoDoctors_[i] = Availability( resolver_, message.disponibilites_medecins.elem[i] );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MedicalStates::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void MedicalStates::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Etat chaine santé" )
                .Display( "Etat chaine", bChainEnabled_ ? "Activée" : "Désactivée" )
                .Display( "Priorités", priorities_ )
                .Display( "Priorités tactiques", tacticalPriorities_ );
}
