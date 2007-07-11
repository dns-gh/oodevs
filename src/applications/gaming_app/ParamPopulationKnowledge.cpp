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
#include "clients_kernel/Population_ABC.h"
#include "gaming/ActionParameterPopulationKnowledge.h"
#include "gaming/PopulationKnowledge_ABC.h"
#include "gaming/AgentKnowledgeConverter_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::ParamPopulationKnowledge( QObject* parent, const OrderParameter& parameter, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityParameter< PopulationKnowledge_ABC >( parent, parameter )
    , parameter_( parameter )
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
    const PopulationKnowledge_ABC* knowledge = converter_.Find( entity, agent_ );
    if( knowledge )
        EntityParameter< PopulationKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameterEntity< PopulationKnowledge_ABC > > param( new ActionParameterPopulationKnowledge( parameter_ ) );
    EntityParameter< PopulationKnowledge_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}
