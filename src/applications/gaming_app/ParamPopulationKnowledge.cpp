// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPopulationKnowledge.h"
#include "gaming/PopulationKnowledge_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "gaming/AgentKnowledgeConverter_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::ParamPopulationKnowledge( QObject* parent, const QString& name, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityParameter< PopulationKnowledge_ABC >( parent, name )
    , converter_( converter )
    , agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::~ParamPopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_knowledgePopulation;
    EntityParameter< PopulationKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.knowledgePopulation );
    asn.null_value = asn.value.u.knowledgePopulation ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::NotifyContextMenu( const Population_ABC& entity, ContextMenu& menu )
{
    const PopulationKnowledge_ABC* knowledge = converter_.Find( entity, agent_ );
    if( knowledge )
        EntityParameter< PopulationKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );

}
