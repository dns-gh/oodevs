// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamObjectKnowledgeList.h"
#include "ParamObjectKnowledge.h"
#include "actions/ObjectKnowledgeList.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::ParamObjectKnowledgeList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller, kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent )
    : EntityListParameter< kernel::ObjectKnowledge_ABC >( parent, parameter, actions, controller )
    , parameter_( parameter )
    , converter_( converter )
    , agent_( agent )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::~ParamObjectKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::AddToMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tools::translate( "ParamObjectKnowledgeList", "Add object knowledge" ), menu );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter< ObjectKnowledge_ABC >* ParamObjectKnowledgeList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< kernel::ObjectKnowledge_ABC >* ParamObjectKnowledgeList::CreateElement( const kernel::ObjectKnowledge_ABC& potential )
{
    return new ParamObjectKnowledge( this, kernel::OrderParameter( tools::translate( "ParamObjectKnowledgeList", "Object knowledge %1" ).arg( ++count_ ).ascii(), "objectknowledge", false ), converter_, agent_, potential, controller_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    const kernel::Team_ABC& team = static_cast< const kernel::Team_ABC& >( agent_.Get< kernel::CommunicationHierarchies >().GetTop() );
    const kernel::ObjectKnowledge_ABC* knowledge = converter_.Find( entity, team );
    if( knowledge )
        EntityListParameter< kernel::ObjectKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::ObjectKnowledgeList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}
