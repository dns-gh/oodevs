// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamPopulationKnowledge.h"
#include "PopulationKnowledge.h"
#include "Population.h"
#include "Agent_ABC.h"
#include "AgentKnowledgeConverter_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::ParamPopulationKnowledge( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityParameter< PopulationKnowledge >( pParent, id, label, menu )
    , converter_( converter )
    , agent_( dynamic_cast< const Agent_ABC& >( agent ) )
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
void ParamPopulationKnowledge::NotifyContextMenu( const Population& entity, QPopupMenu& menu )
{
    const PopulationKnowledge* knowledge = converter_.Find( entity, agent_.GetKnowledgeGroup() );
    if( knowledge )
        EntityParameter< PopulationKnowledge >::NotifyContextMenu( *knowledge, menu );

}
