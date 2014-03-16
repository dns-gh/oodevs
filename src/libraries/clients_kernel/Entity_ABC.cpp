// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Entity_ABC.h"
#include "ExtensionVisitor_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityBase_ABC::AddExtension
// Created: LDC 2012-08-20
// -----------------------------------------------------------------------------
void EntityBase_ABC::AddExtension( Extension_ABC& ext )
{
    GetInterfaces().Register( ext );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC constructor
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
Entity_ABC::Entity_ABC()
    : aggregated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC destructor
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
Entity_ABC::~Entity_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::GetTypeName
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
const std::string& Entity_ABC::GetTypeName() const
{
    static const std::string entity( "entity" );
    return entity;
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::GetTooltip
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
QString Entity_ABC::GetTooltip() const
{
    return QString( "%1 [%2]" ).arg( GetName() ).arg( GetId() );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::AddExtension
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
void Entity_ABC::AddExtension( Extension_ABC& ext )
{
    EntityBase_ABC::AddExtension( ext );
    extensions_.push_back( &ext );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::Apply
// Created: BAX 2013-01-22
// -----------------------------------------------------------------------------
void Entity_ABC::Apply( ExtensionVisitor_ABC& visitor ) const
{
    for( auto it = extensions_.begin(); it != extensions_.end(); ++it )
        visitor.Visit( **it );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::OverFly
// Created: ABR 2011-10-27
// -----------------------------------------------------------------------------
void Entity_ABC::OverFly( ActionController& /*controller*/ ) const
{
    // NOTHING
}

void Entity_ABC::Aggregate( bool aggregate )
{
    aggregated_ = aggregate;
}

bool Entity_ABC::IsAggregated() const
{
    return aggregated_;
}
