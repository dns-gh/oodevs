// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AltitudeModifierAttribute.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( Controller& controller )
    : controller_( controller )
    , height_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute destructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::~AltitudeModifierAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::Display
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Altitude modifier:" ), height_ * Units::meters );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::DisplayInTooltip
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::DisplayInSummary
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::DisplayInSummary( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::DoUpdate
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::DoUpdate
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::UpdateData
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
template< typename T >
void AltitudeModifierAttribute::UpdateData( const T& message )
{
    if( message.has_altitude_modifier() )
    {
        height_ = message.altitude_modifier().height();
        controller_.Update( *static_cast< AltitudeModifierAttribute_ABC* >( this ) );
    }
}
