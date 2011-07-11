// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UndergroundAttribute.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute constructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundAttribute::UndergroundAttribute( Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute destructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundAttribute::~UndergroundAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::GetNetwork
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
const std::string& UndergroundAttribute::GetNetwork() const
{
    return network_;
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::Display
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Underground network:" ), network_ );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::DisplayInTooltip
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::DisplayInSummary
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::DisplayInSummary( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::DoUpdate
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::DoUpdate
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::UpdateData
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
template< typename T >
void UndergroundAttribute::UpdateData( const T& message )
{
    if( message.has_underground() )
    {
        network_ = message.underground().network();
        controller_.Update( *static_cast< UndergroundAttribute_ABC* >( this ) );
    }
}
