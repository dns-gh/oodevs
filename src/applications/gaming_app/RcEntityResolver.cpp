// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "RcEntityResolver.h"
#include "gaming/Agent.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "gaming/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/InternalLinks.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: RcEntityResolver constructor
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
RcEntityResolver::RcEntityResolver(Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver destructor
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
RcEntityResolver::~RcEntityResolver()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const Agent_ABC& element )
{
    tools::Resolver< Agent_ABC >::Register( element.GetId(), const_cast< Agent_ABC& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const Agent_ABC& element )
{
    tools::Resolver< Agent_ABC >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const ObjectKnowledge_ABC& element )
{
    tools::Resolver< ObjectKnowledge_ABC >::Register( element.GetId(), const_cast< ObjectKnowledge_ABC& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const ObjectKnowledge_ABC& element )
{
    tools::Resolver< ObjectKnowledge_ABC >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const AgentKnowledge_ABC& element )
{
    tools::Resolver< AgentKnowledge_ABC >::Register( element.GetId(), const_cast< AgentKnowledge_ABC& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const AgentKnowledge_ABC& element )
{
    tools::Resolver< AgentKnowledge_ABC >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const PopulationKnowledge_ABC& element )
{
    tools::Resolver< PopulationKnowledge_ABC >::Register( element.GetId(), const_cast< PopulationKnowledge_ABC& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const PopulationKnowledge_ABC& element )
{
    tools::Resolver< PopulationKnowledge_ABC >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::CreateLink
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
QString RcEntityResolver::CreateLink( const QString& type, unsigned long id ) const
{
    if( type == Agent::typeName_ )
        return CreateLink< Agent_ABC >( id );
    else if( type == ObjectKnowledge_ABC::typeName_ )
        return CreateLink< ObjectKnowledge_ABC >( id );
    else if( type == AgentKnowledge_ABC::typeName_ )
        return CreateLink< AgentKnowledge_ABC >( id );
    else if( type == PopulationKnowledge_ABC::typeName_ )
        return CreateLink< PopulationKnowledge_ABC >( id );
    return QString::number( id );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::CreateLink
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
template< typename T >
QString RcEntityResolver::CreateLink( unsigned long id ) const
{
    const T* element = tools::Resolver< T >::Find( id );
    if( element )
        return InternalLinks::CreateLink( *element, element->GetName() );
    return tools::translate( "RcEntityResolver", "[Unknown]" ); // can now happen with the replayer
}
