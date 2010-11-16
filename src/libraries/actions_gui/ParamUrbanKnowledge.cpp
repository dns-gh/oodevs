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
#include "clients_kernel/UrbanKnowledge_ABC.h"
#include "clients_kernel/UrbanKnowledgeConverter_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/TerrainObjectProxy.h"
#include <urban/TerrainObject_ABC.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamUrbanKnowledge constructor
// Created: MGD 2010-11-16
// -----------------------------------------------------------------------------
ParamUrbanKnowledge::ParamUrbanKnowledge( QObject* parent, const kernel::OrderParameter& parameter, kernel::UrbanKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, kernel::Controller& controller )
: EntityParameter< kernel::UrbanKnowledge_ABC >( parent, parameter, controller )
, parameter_( parameter )
, converter_( converter )
, agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanKnowledge constructor
// Created: MGD 2010-11-16
// -----------------------------------------------------------------------------
ParamUrbanKnowledge::ParamUrbanKnowledge( QObject* parent, const kernel::OrderParameter& parameter, kernel::UrbanKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, const kernel::UrbanKnowledge_ABC& potential, kernel::Controller& controller )
: EntityParameter< kernel::UrbanKnowledge_ABC >( parent, parameter, potential, controller )
, parameter_( parameter )
, converter_( converter )
, agent_( agent )
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
void ParamUrbanKnowledge::NotifyContextMenu( const ::gui::TerrainObjectProxy& entity, kernel::ContextMenu& menu )
{
    const kernel::Team_ABC& team = static_cast< const kernel::Team_ABC& >( agent_.Get< kernel::CommunicationHierarchies >().GetTop() );
    const kernel::UrbanKnowledge_ABC* knowledge = converter_.Find( entity, team );
    if( knowledge )
        EntityParameter< kernel::UrbanKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanKnowledge::CommitTo
// Created: MGD 2010-11-16
// -----------------------------------------------------------------------------
void ParamUrbanKnowledge::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< kernel::UrbanKnowledge_ABC > > param( new actions::parameters::UrbanBlock( parameter_, controller_ ) );
    EntityParameter< kernel::UrbanKnowledge_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

