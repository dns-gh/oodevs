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
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/SoundEvent.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationPart_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/PopulationFlow_ABC.h"
#include "clients_kernel/Positions.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DirectFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
DirectFire::DirectFire( const sword::StartUnitFire& message, kernel::Controller& controller, const tools::Resolver_ABC< Agent_ABC >& agentResolver, const tools::Resolver_ABC< PopulationPart_ABC >& populationResolver, unsigned long entityId )
    : Fire_ABC( agentResolver.Get( message.firing_unit().id() ) )
    , id_( message.fire().id() )
    , isTarget_( false )
    , controller_( controller )
{
    const kernel::Entity_ABC* target = 0;

    if( message.target().has_unit() )
        target = agentResolver.Find( message.target().unit().id() );
    else if( message.target().has_crowd_element() )
        target = populationResolver.Find( message.target().crowd_element().id() );
    if( !target )
        throw MASA_EXCEPTION( "DirectFire on position..." );

    isTarget_ = target->GetId() == entityId;

    const Positions* position = 0;
    if( isTarget_ )
        position = agentResolver.Get( message.firing_unit().id() ).Retrieve< Positions >();
    else
    {
        if( message.target().has_crowd() && target->GetTypeName() == PopulationFlow_ABC::typeName_ )
        {
            const PopulationFlow_ABC* populationFlow = static_cast< const PopulationFlow_ABC* >( target );
            const Entity_ABC* firer = agentResolver.Find( message.firing_unit().id() );
            const Positions* firerPosition = ( firer ) ? firer->Retrieve< Positions >() : 0;
            if( firerPosition )
                position_ = populationFlow->GetNearestPosition( firerPosition->GetPosition() );
        }
        else
            position = target->Retrieve< Positions >();
    }

    if( position )
        position_ = position->GetPosition();

    controller_.Update( gui::SoundEvent( &GetOrigin(), "directfire", gui::SoundEvent::eStart ) );
}

// -----------------------------------------------------------------------------
// Name: DirectFire destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
DirectFire::~DirectFire()
{
    controller_.Update( gui::SoundEvent( &GetOrigin(), "directfire", gui::SoundEvent::eStop ) );
}

// -----------------------------------------------------------------------------
// Name: DirectFire::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void DirectFire::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( !position_.IsZero() && viewport.IsVisible( geometry::Rectangle2f( where, position_ ) ))
    {
        if( isTarget_ )
            tools.DrawArrow( position_, where );
        else
            tools.DrawArrow( where, position_ );
    }
}
