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
#include "gaming/AgentKnowledges.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/KnowledgeConverter_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_gui/InternalLinks.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: RcEntityResolver constructor
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
RcEntityResolver::RcEntityResolver( Controllers& controllers, const kernel::KnowledgeConverter_ABC& converter )
    : controllers_( controllers )
    , converter_( converter )
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
// Created: MGD 2011-01-05
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const Automat_ABC& element )
{
    tools::Resolver< Automat_ABC >::Register( element.GetId(), const_cast< Automat_ABC& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: MGD 2011-01-05
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const Automat_ABC& element )
{
    tools::Resolver< Automat_ABC >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const Population_ABC& element )
{
    tools::Resolver< Population_ABC >::Register( element.GetId(), const_cast< Population_ABC& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const Population_ABC& element )
{
    tools::Resolver< Population_ABC >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: LGY 2011-05-02
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const Object_ABC& element )
{
    tools::Resolver< Object_ABC >::Register( element.GetId(), const_cast< Object_ABC& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: LGY 2011-05-02
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const Object_ABC& element )
{
    tools::Resolver< Object_ABC >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyCreated
// Created: JSR 2012-06-01
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyCreated( const kernel::UrbanObject_ABC& element )
{
    tools::Resolver< UrbanObject_ABC >::Register( element.GetId(), const_cast< UrbanObject_ABC& >( element ) );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::NotifyDeleted
// Created: JSR 2012-06-01
// -----------------------------------------------------------------------------
void RcEntityResolver::NotifyDeleted( const kernel::UrbanObject_ABC& element )
{
    tools::Resolver< UrbanObject_ABC >::Remove( element.GetId() );
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::CreateLink
// Created: SBO 2006-09-18
// -----------------------------------------------------------------------------
QString RcEntityResolver::CreateLink( const kernel::Entity_ABC* entity, const std::string& type, unsigned long id ) const
{
    if( type == Agent_ABC::typeName_ )
        return CreateLink< Agent_ABC >( id );
    else if( type == AgentKnowledge_ABC::typeName_ )
    {
        if( entity )
            if( const auto k = converter_.FindAgent( id, *entity ) )
                return InternalLinks::CreateLink( *k, k->GetName() );
        return CreateLink< Agent_ABC >( id );
    }
    else if( type == Automat_ABC::typeName_ )
        return CreateLink< Automat_ABC >( id );
    else if( type == Object_ABC::typeName_ )
        return CreateLink< Object_ABC >( id );
    else if( type == UrbanObject_ABC::typeName_ )
        return CreateLink< UrbanObject_ABC >( id );
    else if( type == Population_ABC::typeName_ )
        return CreateLink< Population_ABC >( id );
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
