// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamObjectKnowledge.h"
#include "actions/ObjectKnowledge.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Team_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledge::ParamObjectKnowledge( QObject* parent, const kernel::OrderParameter& parameter, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, kernel::Controller& controller )
    : EntityParameter< kernel::ObjectKnowledge_ABC >( parent, parameter, controller )
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
ParamObjectKnowledge::ParamObjectKnowledge( QObject* parent, const kernel::OrderParameter& parameter, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, const kernel::ObjectKnowledge_ABC& potential, kernel::Controller& controller )
    : EntityParameter< kernel::ObjectKnowledge_ABC >( parent, parameter, potential, controller )
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
void ParamObjectKnowledge::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    const kernel::Team_ABC& team = static_cast< const kernel::Team_ABC& >( agent_.Get< kernel::CommunicationHierarchies >().GetTop() );
    const kernel::ObjectKnowledge_ABC* knowledge = converter_.Find( entity, team );
    if( knowledge )
        EntityParameter< kernel::ObjectKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< kernel::ObjectKnowledge_ABC > > param( new actions::parameters::ObjectKnowledge( parameter_, controller_ ) );
    EntityParameter< kernel::ObjectKnowledge_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

