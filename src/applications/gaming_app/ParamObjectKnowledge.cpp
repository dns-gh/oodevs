// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamObjectKnowledge.h"
#include "actions/ObjectKnowledge.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledge::ParamObjectKnowledge( QObject* parent, const OrderParameter& parameter, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityParameter< ObjectKnowledge_ABC >( parent, parameter )
    , parameter_( parameter )
    , converter_( converter )
    , agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge constructor
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
ParamObjectKnowledge::ParamObjectKnowledge( QObject* parent, const OrderParameter& parameter, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& agent, const ObjectKnowledge_ABC& potential )
    : EntityParameter< ObjectKnowledge_ABC >( parent, parameter, potential )
    , parameter_( parameter )
    , converter_( converter )
    , agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledge::~ParamObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( agent_.Get< CommunicationHierarchies >().GetTop() );
    const ObjectKnowledge_ABC* knowledge = converter_.Find( entity, team );
    if( knowledge )
        EntityParameter< ObjectKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< ObjectKnowledge_ABC > > param( new actions::parameters::ObjectKnowledge( parameter_ ) );
    EntityParameter< ObjectKnowledge_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

