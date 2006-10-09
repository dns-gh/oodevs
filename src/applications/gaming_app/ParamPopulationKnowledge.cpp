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
#include "gaming/PopulationKnowledge.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "gaming/AgentKnowledgeConverter_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::ParamPopulationKnowledge( QWidget* pParent, ASN1T_OID& id, const QString& label, const QString& menu, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityParameter< PopulationKnowledge >( pParent, id, label, menu )
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
// Name: ParamPopulationKnowledge::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::NotifyContextMenu( const Population_ABC& entity, ContextMenu& menu )
{
    const PopulationKnowledge* knowledge = converter_.Find( entity, agent_ );
    if( knowledge )
        EntityParameter< PopulationKnowledge >::NotifyContextMenu( *knowledge, menu );

}
