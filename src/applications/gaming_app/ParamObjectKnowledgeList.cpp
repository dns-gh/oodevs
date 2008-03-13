// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamObjectKnowledgeList.h"
#include "ParamObjectKnowledge.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "gaming/ActionParameterObjectKnowledgeList.h"
#include "gaming/ObjectKnowledge_ABC.h"
#include "gaming/ObjectKnowledgeConverter_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::ParamObjectKnowledgeList( QObject* parent, const OrderParameter& parameter, kernel::ActionController& controller, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent )
    : EntityListParameter< ObjectKnowledge_ABC >( parent, parameter, controller )
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
    MakeMenu( tr( "Add object knowledge" ), menu );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter< ObjectKnowledge_ABC >* ParamObjectKnowledgeList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< ObjectKnowledge_ABC >* ParamObjectKnowledgeList::CreateElement( const ObjectKnowledge_ABC& potential )
{
    return new ParamObjectKnowledge( this, OrderParameter( tr( "Object knowledge %1" ).arg( ++count_ ).ascii(), "objectknowledge", false ), converter_, agent_, potential );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( agent_.Get< CommunicationHierarchies >().GetTop() );
    const ObjectKnowledge_ABC* knowledge = converter_.Find( entity, team );
    if( knowledge )
        EntityListParameter< ObjectKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterObjectKnowledgeList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}
