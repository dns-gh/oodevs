// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "AgentLogMaintenance.h"

#include "Model.h"
#include "Automat.h"
#include "Agent.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogMaintenance::AgentLogMaintenance( Model& model, const Agent& agent, const ASN1T_MsgLogMaintenanceEtat& asnMsg )
    : agent_                ( agent )
    , model_                ( model )
    , bSystemEnabled_       ( false )
    , priorities_           ()
    , tacticalPriorities_   ()
    , haulersAvailability_  ()
    , repairersAvailability_()
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
AgentLogMaintenance::~AgentLogMaintenance()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMaintenance::Update( const ASN1T_MsgLogMaintenanceEtat& asnMsg )
{
    if( asnMsg.m.chaine_activeePresent )
        bSystemEnabled_ = asnMsg.chaine_activee;

    if( asnMsg.m.disponibilites_remorqueursPresent )
    {
        haulersAvailability_.Clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_remorqueurs.n; ++i )
            haulersAvailability_.Create( model_, i, asnMsg.disponibilites_remorqueurs.elem[ i ] );
    }

    if( asnMsg.m.disponibilites_reparateursPresent )
    {
        repairersAvailability_.Clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_reparateurs.n; ++i )
            repairersAvailability_.Create( model_, i, asnMsg.disponibilites_reparateurs.elem[ i ] );
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
// Name: AgentLogMaintenance::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMaintenance::Send( Publisher_ABC& publisher ) const
{
    AsnMsgInClientLogMaintenanceEtat asn;

    asn().oid_pion = agent_.GetID();

    asn().m.chaine_activeePresent             = 1;
    asn().m.disponibilites_remorqueursPresent = 1;
    asn().m.disponibilites_reparateursPresent = 1;
    asn().m.priorites_tactiquesPresent        = 1;
    asn().m.prioritesPresent                  = 1;

    asn().chaine_activee = bSystemEnabled_;

    repairersAvailability_.Send< ASN1T__SeqOfMaintenanceDisponibiliteMoyens, ASN1T_MaintenanceDisponibiliteMoyens >( asn().disponibilites_reparateurs );
    haulersAvailability_  .Send< ASN1T__SeqOfMaintenanceDisponibiliteMoyens, ASN1T_MaintenanceDisponibiliteMoyens >( asn().disponibilites_remorqueurs );

    tacticalPriorities_.Send< ASN1T_ListAutomate, ASN1T_Automate >( asn().priorites_tactiques );
    SendContainerValues< ASN1T_MaintenancePriorites, ASN1T_TypeEquipement, T_EquipmentTypeVector >( priorities_, asn().priorites );

    asn.Send( publisher );

    if( asn().disponibilites_reparateurs.n > 0 )
        delete [] asn().disponibilites_reparateurs.elem;
    if( asn().disponibilites_remorqueurs.n > 0 )
        delete [] asn().disponibilites_remorqueurs.elem;
    if( asn().priorites.n > 0 )
        delete [] asn().priorites.elem;
    if( asn().priorites_tactiques.n > 0 )
        delete [] asn().priorites_tactiques.elem;
}
