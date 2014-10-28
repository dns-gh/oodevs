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
#include "clients_gui/FireOption.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/SoundEvent.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationPart_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/PopulationFlow_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace gui;

namespace
{
    const kernel::Entity_ABC& GetTarget( const sword::StartUnitFire& message,
        const tools::Resolver_ABC< Agent_ABC >& agentResolver,
        const tools::Resolver_ABC< PopulationPart_ABC >& populationResolver )
    {
        if( message.target().has_unit() )
            return agentResolver.Get( message.target().unit().id() );
        if( message.target().has_crowd_element() )
            return populationResolver.Get( message.target().crowd_element().id() );
        throw MASA_EXCEPTION( "DirectFire on position..." );
    }
    geometry::Point2f GetPosition( const kernel::Entity_ABC& entity )
    {
        return entity.Get< Positions >().GetPosition();
    }
    geometry::Point2f GetPosition( const kernel::Entity_ABC& target, const kernel::Entity_ABC& firer )
    {
        return target.GetTypeName() == PopulationFlow_ABC::typeName_
            ? static_cast< const PopulationFlow_ABC& >( target ).GetNearestPosition( GetPosition( firer ) )
            : GetPosition( target );
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
DirectFire::DirectFire( const sword::StartUnitFire& message, kernel::Controller& controller, const kernel::Profile_ABC& profile,
        const tools::Resolver_ABC< Agent_ABC >& agentResolver, const tools::Resolver_ABC< PopulationPart_ABC >& populationResolver, unsigned long entityId )
    : Fire_ABC( agentResolver.Get( message.firing_unit().id() ) )
    , controller_( controller )
    , profile_( profile )
    , target_( GetTarget( message, agentResolver, populationResolver ) )
    , isTarget_( target_.GetId() == entityId )
    , position_( isTarget_ ? GetPosition( origin_ ) : GetPosition( target_, origin_ ) )
{
    controller_.Update( gui::SoundEvent( &origin_, "directfire", gui::SoundEvent::eStart ) );
}

// -----------------------------------------------------------------------------
// Name: DirectFire destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
DirectFire::~DirectFire()
{
    controller_.Update( gui::SoundEvent( &origin_, "directfire", gui::SoundEvent::eStop ) );
}

namespace
{
    void DrawArrow( const geometry::Point2f& from, const geometry::Point2f& to, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools )
    {
        if( viewport.IsVisible( geometry::Rectangle2f( from, to ) ) )
            tools.DrawArrow( from, to );
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFire::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void DirectFire::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& view ) const
{
    auto color = GetDirectFireColor( view, origin_ );
    if( isTarget_ )
        DrawArrow( ComputePosition( origin_ ), where, viewport, view );
    else
        DrawArrow( where, ComputePosition( target_ ), viewport, view );
}

// -----------------------------------------------------------------------------
// Name: DirectFire::ComputePosition
// Created: MCO 2013-12-06
// -----------------------------------------------------------------------------
geometry::Point2f DirectFire::ComputePosition( const kernel::Entity_ABC& entity ) const
{
    return profile_.IsVisible( entity ) ? GetPosition( entity ) : position_;
}
