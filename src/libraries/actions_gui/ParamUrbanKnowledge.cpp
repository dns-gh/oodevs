// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamUrbanKnowledge.h"
#include "actions/UrbanBlock.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"
#include "clients_kernel/UrbanKnowledgeConverter_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Team_ABC.h"

using namespace actions::gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamUrbanKnowledge constructor
// Created: MGD 2010-11-16
// -----------------------------------------------------------------------------
ParamUrbanKnowledge::ParamUrbanKnowledge( QObject* parent, const OrderParameter& parameter, UrbanKnowledgeConverter_ABC& converter, const Entity_ABC& agent, Controller& controller )
    : EntityParameter< UrbanKnowledge_ABC >( parent, parameter, controller )
    , parameter_( parameter )
    , converter_( converter )
    , agent_    ( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanKnowledge constructor
// Created: MGD 2010-11-16
// -----------------------------------------------------------------------------
ParamUrbanKnowledge::ParamUrbanKnowledge( QObject* parent, const OrderParameter& parameter, UrbanKnowledgeConverter_ABC& converter, const Entity_ABC& agent, const UrbanKnowledge_ABC& potential, Controller& controller )
    : EntityParameter< UrbanKnowledge_ABC >( parent, parameter, potential, controller )
    , parameter_( parameter )
    , converter_( converter )
    , agent_    ( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanKnowledge destructor
// Created: MGD 2010-11-16
// -----------------------------------------------------------------------------
ParamUrbanKnowledge::~ParamUrbanKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanKnowledge::NotifyContextMenu
// Created: MGD 2010-11-16
// -----------------------------------------------------------------------------
void ParamUrbanKnowledge::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( agent_.Get< CommunicationHierarchies >().GetTop() );
    const UrbanKnowledge_ABC* knowledge = converter_.Find( entity, team );
    if( knowledge )
        EntityParameter< UrbanKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanKnowledge::CommitTo
// Created: MGD 2010-11-16
// -----------------------------------------------------------------------------
void ParamUrbanKnowledge::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< UrbanKnowledge_ABC > > param( new actions::parameters::UrbanBlock( parameter_, controller_ ) );
    EntityParameter< UrbanKnowledge_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

