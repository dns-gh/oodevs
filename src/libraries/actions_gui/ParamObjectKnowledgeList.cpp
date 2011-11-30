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
#include "actions/ParameterList.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::ParamObjectKnowledgeList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller,
                                                    kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent )
    : EntityListParameter< kernel::ObjectKnowledge_ABC >( parent, parameter, actions, controller )
    , converter_( converter )
    , agent_    ( agent )
    , count_    ( 0u )
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
    MakeMenu( tools::translate( "ListParameter", "%1: add item" ).arg( GetName() ), menu );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter< ObjectKnowledge_ABC >* ParamObjectKnowledgeList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< kernel::ObjectKnowledge_ABC >* ParamObjectKnowledgeList::CreateElement( const kernel::ObjectKnowledge_ABC& potential )
{
    return new ParamObjectKnowledge( this, kernel::OrderParameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).arg( potential.GetName() ).ascii(), "objectknowledge", false ), converter_, agent_, potential, controller_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    if( !entity.GetType().IsUrban() )
    {
        const kernel::Hierarchies* hierarchies = agent_.Retrieve< kernel::CommunicationHierarchies >();
        if( ! hierarchies )
            hierarchies = agent_.Retrieve< kernel::TacticalHierarchies >();
        const kernel::Team_ABC& team = static_cast< const kernel::Team_ABC& >( hierarchies->GetTop() );
        const kernel::ObjectKnowledge_ABC* knowledge = converter_.Find( entity, team );
        if( knowledge )
            EntityListParameter< kernel::ObjectKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
    }
}
