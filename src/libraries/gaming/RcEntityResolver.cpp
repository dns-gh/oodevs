// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "RcEntityResolver.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "ObjectKnowledge.h"
#include "AgentKnowledge.h"
#include "PopulationKnowledge.h"
#include "InternalLinks.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: RcEntityResolver constructor
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
RcEntityResolver::RcEntityResolver( kernel::Controllers& controllers )
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
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const kernel::Agent_ABC& element )
{
    Resolver< Agent_ABC >::Register( element.GetId(), const_cast< Agent_ABC& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const kernel::Agent_ABC& element )
{
    Resolver< Agent_ABC >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const ObjectKnowledge& element )
{
    Resolver< ObjectKnowledge >::Register( element.GetId(), const_cast< ObjectKnowledge& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const ObjectKnowledge& element )
{
    Resolver< ObjectKnowledge >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const AgentKnowledge& element )
{
    Resolver< AgentKnowledge >::Register( element.GetId(), const_cast< AgentKnowledge& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const AgentKnowledge& element )
{
    Resolver< AgentKnowledge >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const PopulationKnowledge& element )
{
    Resolver< PopulationKnowledge >::Register( element.GetId(), const_cast< PopulationKnowledge& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const PopulationKnowledge& element )
{
    Resolver< PopulationKnowledge >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::CreateLink
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
QString RcEntityResolver::CreateLink( const QString& type, unsigned long id ) const
{
    if( type == InternalLinks::agent_ )
        return CreateLink< Agent_ABC >( id );
    else if( type == InternalLinks::objectKnowledge_ )
        return CreateLink< ObjectKnowledge >( id );
    else if( type == InternalLinks::agentKnowledge_ )
        return CreateLink< AgentKnowledge >( id );
    else if( type == InternalLinks::populationKnowledge_ )
        return CreateLink< PopulationKnowledge >( id );
    return QString::number( id );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::CreateLink
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
template< typename T >
QString RcEntityResolver::CreateLink( unsigned long id ) const
{
    const T& element = Resolver< T >::Get( id );
    return InternalLinks::CreateLink( element, element.GetName() );
}
