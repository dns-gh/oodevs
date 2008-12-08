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
#include "ClientPublisher_ABC.h"
#include "Automat.h"
#include "Agent.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogMaintenance::AgentLogMaintenance( const Model& model, const kernel::Agent_ABC& agent, const ASN1T_MsgLogMaintenanceState& asnMsg )
    : model_         ( model )
    , agent_         ( agent )
    , bSystemEnabled_( false )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
AgentLogMaintenance::~AgentLogMaintenance()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMaintenance::Update( const ASN1T_MsgLogMaintenanceState& asnMsg )
{
    if( asnMsg.m.chaine_activeePresent )
        bSystemEnabled_ = asnMsg.chaine_activee != 0;

    if( asnMsg.m.disponibilites_remorqueursPresent )
    {
        haulersAvailability_.clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_remorqueurs.n; ++i )
            haulersAvailability_.push_back( T_Availability( asnMsg.disponibilites_remorqueurs.elem[ i ] ) );
    }

    if( asnMsg.m.disponibilites_reparateursPresent )
    {
        repairersAvailability_.clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_reparateurs.n; ++i )
            repairersAvailability_.push_back( T_Availability( asnMsg.disponibilites_reparateurs.elem[ i ] ) );
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
// Name: AgentLogMaintenance::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMaintenance::Send( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceState asn;

    asn().oid_pion = agent_.GetId();

    asn().m.chaine_activeePresent             = 1;
    asn().m.disponibilites_remorqueursPresent = 1;
    asn().m.disponibilites_reparateursPresent = 1;
    asn().m.priorites_tactiquesPresent        = 1;
    asn().m.prioritesPresent                  = 1;

    asn().chaine_activee = bSystemEnabled_;

    {
        asn().disponibilites_reparateurs.n = repairersAvailability_.size();
        asn().disponibilites_reparateurs.elem = asn().disponibilites_reparateurs.n > 0 ? new ASN1T_LogMaintenanceEquipmentAvailability[ asn().disponibilites_reparateurs.n ] : 0;
        unsigned int i = 0;
        for( std::vector< T_Availability >::const_iterator it = repairersAvailability_.begin(); it != repairersAvailability_.end(); ++it )
            it->Send( asn().disponibilites_reparateurs.elem[i++] );
    }
    {
        asn().disponibilites_remorqueurs.n = haulersAvailability_.size();
        asn().disponibilites_remorqueurs.elem = asn().disponibilites_remorqueurs.n > 0 ? new ASN1T_LogMaintenanceEquipmentAvailability[ asn().disponibilites_remorqueurs.n ] : 0;
        unsigned int i = 0;
        for( std::vector< T_Availability >::const_iterator it = haulersAvailability_.begin(); it != haulersAvailability_.end(); ++it )
            it->Send( asn().disponibilites_remorqueurs.elem[i++] );
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
        asn().priorites.elem = asn().priorites.n > 0 ? new ASN1T_EquipmentType[ asn().priorites.n ] : 0;
        unsigned int i = 0;
        for( std::vector< ASN1T_EquipmentType >::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
            asn().priorites.elem[i++] = *it;
    }
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
