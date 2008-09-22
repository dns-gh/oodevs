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
#include "Model.h"
#include "ClientPublisher_ABC.h"
#include "Automat.h"
#include "Agent.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogMedical constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogMedical::AgentLogMedical( const Model& model, const kernel::Agent_ABC& agent, const ASN1T_MsgLogMedicalState& asnMsg )
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
void AgentLogMedical::Update( const ASN1T_MsgLogMedicalState& asnMsg )
{
    if( asnMsg.m.chaine_activeePresent )
        bSystemEnabled_ = asnMsg.chaine_activee != 0;

    if( asnMsg.m.disponibilites_ambulances_ramassagePresent )
    {
        collectionAmbulancesAvailability_.clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_ambulances_ramassage.n; ++i )
            collectionAmbulancesAvailability_.push_back( T_Availability( asnMsg.disponibilites_ambulances_ramassage.elem[ i ] ) );
    }

    if( asnMsg.m.disponibilites_ambulances_relevePresent )
    {
        evacuationAmbulancesAvailability_.clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_ambulances_releve.n; ++i )
            evacuationAmbulancesAvailability_.push_back( T_Availability( asnMsg.disponibilites_ambulances_releve.elem[ i ] ) );
    }

    if( asnMsg.m.disponibilites_medecinsPresent )
    {
        doctorsAvailability_.clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_medecins.n; ++i )
            doctorsAvailability_.push_back( T_Availability( asnMsg.disponibilites_medecins.elem[ i ] ) );
    }

    if( asnMsg.m.priorites_tactiquesPresent )
    {
        tacticalPriorities_.clear();
        for( unsigned int i = 0; i < asnMsg.priorites_tactiques.n; ++i )
            tacticalPriorities_.push_back( &model_.automats_.Get( asnMsg.priorites_tactiques.elem[ i ] ) );
    }

    if( asnMsg.m.prioritesPresent )
    {
        priorities_.clear();
        for( unsigned int i = 0; i < asnMsg.priorites.n; ++i )
            priorities_.push_back( asnMsg.priorites.elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentLogMedical::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMedical::Send( ClientPublisher_ABC& publisher ) const
{
    client::LogMedicalState asn;

    asn().oid_pion = agent_.GetId();

    asn().m.chaine_activeePresent                      = 1;
    asn().m.disponibilites_ambulances_ramassagePresent = 1;
    asn().m.disponibilites_ambulances_relevePresent    = 1;
    asn().m.disponibilites_medecinsPresent             = 1;
    asn().m.priorites_tactiquesPresent                 = 1;
    asn().m.prioritesPresent                           = 1;

    asn().chaine_activee = bSystemEnabled_;

    {
        asn().disponibilites_ambulances_releve.n = evacuationAmbulancesAvailability_.size();
        asn().disponibilites_ambulances_releve.elem = asn().disponibilites_ambulances_releve.n > 0 ? new ASN1T_LogMedicalEquipmentAvailability[ asn().disponibilites_ambulances_releve.n ] : 0;
        unsigned int i = 0;
        for( std::vector< T_Availability >::const_iterator it = evacuationAmbulancesAvailability_.begin(); it != evacuationAmbulancesAvailability_.end(); ++it )
            it->Send( asn().disponibilites_ambulances_releve.elem[i++] );
    }
    {
        asn().disponibilites_ambulances_ramassage.n = collectionAmbulancesAvailability_.size();
        asn().disponibilites_ambulances_ramassage.elem = asn().disponibilites_ambulances_ramassage.n > 0 ? new ASN1T_LogMedicalEquipmentAvailability[ asn().disponibilites_ambulances_ramassage.n ] : 0;
        unsigned int i = 0;
        for( std::vector< T_Availability >::const_iterator it = collectionAmbulancesAvailability_.begin(); it != collectionAmbulancesAvailability_.end(); ++it )
            it->Send( asn().disponibilites_ambulances_ramassage.elem[i++] );
    }
    {
        asn().disponibilites_medecins.n = doctorsAvailability_.size();
        asn().disponibilites_medecins.elem = asn().disponibilites_medecins.n > 0 ? new ASN1T_LogMedicalEquipmentAvailability[ asn().disponibilites_medecins.n ] : 0;
        unsigned int i = 0;
        for( std::vector< T_Availability >::const_iterator it = doctorsAvailability_.begin(); it != doctorsAvailability_.end(); ++it )
            it->Send( asn().disponibilites_medecins.elem[i++] );
    }
    {
        asn().priorites_tactiques.n = tacticalPriorities_.size();
        asn().priorites_tactiques.elem = asn().priorites_tactiques.n > 0 ? new ASN1T_Automat[ asn().priorites_tactiques.n ] : 0;
        unsigned int i = 0;
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = tacticalPriorities_.begin(); it != tacticalPriorities_.end(); ++it, ++i )
            asn().priorites_tactiques.elem[i] = (*it)->GetId();
    }
    {
        asn().priorites.n = priorities_.size();
        asn().priorites.elem = asn().priorites.n > 0 ? new ASN1T_EnumHumanWound[ asn().priorites.n ] : 0;
        unsigned int i = 0;
        for( std::vector< ASN1T_EnumHumanWound >::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
            asn().priorites.elem[i++] = *it;
    }

    asn.Send( publisher );

    if( asn().disponibilites_ambulances_releve.n > 0 )
        delete [] asn().disponibilites_ambulances_releve.elem;
    if( asn().disponibilites_ambulances_ramassage.n > 0 )
        delete [] asn().disponibilites_ambulances_ramassage.elem;
    if( asn().disponibilites_medecins.n > 0 )
        delete [] asn().disponibilites_medecins.elem;
    if( asn().priorites.n > 0 )
        delete [] asn().priorites.elem;
    if( asn().priorites_tactiques.n > 0 )
        delete [] asn().priorites_tactiques.elem;
}
