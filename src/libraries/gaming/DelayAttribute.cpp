// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DelayAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Controller.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DelayAttribute constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::DelayAttribute( kernel::Controller& controller )
    : controller_ ( controller )
    , delay_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute destructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::~DelayAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::Display
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Delay time:" ), delay_ / 3600. * Units::hours );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::DisplayInTooltip
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::DisplayInSummary
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::DoUpdate
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::DoUpdate
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::DoUpdate( const MsgsSimToClient::MsgObjectCreation& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::UpdateData
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
template< typename T >
void DelayAttribute::UpdateData( const T& message )
{
    if( message.has_effect_delay()  )
    {
        delay_ = static_cast< unsigned int >( message.effect_delay().value() );
        controller_.Update( *(DelayAttribute_ABC*)this );
    }
}
