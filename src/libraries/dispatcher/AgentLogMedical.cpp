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
#include "Automat.h"
#include "Agent.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogMedical constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogMedical::AgentLogMedical( Model& model, const Agent& agent, const ASN1T_MsgLogMedicalState& asnMsg )
    : agent_                           ( agent )
    , model_                           ( model )
    , bSystemEnabled_                  ( false )
    , priorities_                      ()
    , tacticalPriorities_              ()
    , evacuationAmbulancesAvailability_()
    , collectionAmbulancesAvailability_()
    , doctorsAvailability_             ()
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentLogMedical destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
AgentLogMedical::~AgentLogMedical()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AgentLogMedical::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMedical::Update( const ASN1T_MsgLogMedicalState& asnMsg )
{
    if( asnMsg.m.chaine_activeePresent )
        bSystemEnabled_ = asnMsg.chaine_activee;

    if( asnMsg.m.disponibilites_ambulances_ramassagePresent )
    {
        collectionAmbulancesAvailability_.Clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_ambulances_ramassage.n; ++i )
            collectionAmbulancesAvailability_.Create( model_, i, asnMsg.disponibilites_ambulances_ramassage.elem[ i ] );
    }

    if( asnMsg.m.disponibilites_ambulances_relevePresent )
    {
        evacuationAmbulancesAvailability_.Clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_ambulances_releve.n; ++i )
            evacuationAmbulancesAvailability_.Create( model_, i, asnMsg.disponibilites_ambulances_releve.elem[ i ] );
    }

    if( asnMsg.m.disponibilites_medecinsPresent )
    {
        doctorsAvailability_.Clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_medecins.n; ++i )
            doctorsAvailability_.Create( model_, i, asnMsg.disponibilites_medecins.elem[ i ] );
    }

    if( asnMsg.m.priorites_tactiquesPresent )
    {
        tacticalPriorities_.Clear();
        for( unsigned int i = 0; i < asnMsg.priorites_tactiques.n; ++i )
            tacticalPriorities_.Register( model_.GetAutomats().Get( asnMsg.priorites_tactiques.elem[ i ] ) );
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
    AsnMsgSimToClientLogMedicalState asn;

    asn().oid_pion = agent_.GetID();

    asn().m.chaine_activeePresent                      = 1;
    asn().m.disponibilites_ambulances_ramassagePresent = 1;
    asn().m.disponibilites_ambulances_relevePresent    = 1;
    asn().m.disponibilites_medecinsPresent             = 1;
    asn().m.priorites_tactiquesPresent                 = 1;
    asn().m.prioritesPresent                           = 1;

    asn().chaine_activee = bSystemEnabled_;

    evacuationAmbulancesAvailability_.Send< ASN1T__SeqOfLogMedicalEquipmentAvailability, ASN1T_LogMedicalEquipmentAvailability >( asn().disponibilites_ambulances_releve    );
    collectionAmbulancesAvailability_.Send< ASN1T__SeqOfLogMedicalEquipmentAvailability, ASN1T_LogMedicalEquipmentAvailability >( asn().disponibilites_ambulances_ramassage );
    doctorsAvailability_             .Send< ASN1T__SeqOfLogMedicalEquipmentAvailability, ASN1T_LogMedicalEquipmentAvailability >( asn().disponibilites_medecins             );

    tacticalPriorities_.Send< ASN1T_AutomatList, ASN1T_Automat >( asn().priorites_tactiques );
    SendContainerValues< ASN1T_LogMedicalPriorities, ASN1T_EnumHumanWound, T_HumanWoundVector >( priorities_, asn().priorites );

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
