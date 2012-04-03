// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DirectFire.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Positions.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DirectFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
DirectFire::DirectFire( const sword::StartUnitFire& message, const tools::Resolver_ABC< Agent_ABC >& agentResolver, const tools::Resolver_ABC< Population_ABC >& populationResolver, unsigned long entityId )
    : Fire_ABC( agentResolver.Get( message.firing_unit().id() ) )
    , id_( message.fire().id() )
    , isTarget_( false )
{
    const kernel::Entity_ABC* target;

    if( message.target().has_unit() )
        target = & agentResolver.Get( message.target().unit().id() );
    else if( message.target().has_crowd() )
        target = & populationResolver.Get( message.target().crowd().id() );
    else
        throw std::runtime_error( "DirectFire on position..." );

    isTarget_ = target->GetId() == entityId;
    if( isTarget_ )
        position_ = agentResolver.Get( message.firing_unit().id() ).Retrieve< Positions >();
    else
        position_ = target->Retrieve< Positions >();
}

// -----------------------------------------------------------------------------
// Name: DirectFire destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
DirectFire::~DirectFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFire::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void DirectFire::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( position_ )
    {
        const geometry::Point2f pos = position_->GetPosition();
        if( viewport.IsVisible( geometry::Rectangle2f( where, pos ) ) )
        {
            if( isTarget_ )
                tools.DrawArrow( pos, where );
            else
                tools.DrawArrow( where, pos );
        }
    }
}
