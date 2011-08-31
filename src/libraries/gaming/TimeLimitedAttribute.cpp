// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TimeLimitedAttribute.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( Controller& controller )
    : controller_  ( controller )
    , activityTime_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
TimeLimitedAttribute::~TimeLimitedAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Mine parameters" ) )
             .Display( tools::translate( "Object", "Activity time:" ), activityTime_ / 3600. * Units::hours );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::DisplayInSummary
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::DisplayInSummary( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: template< typename T >     void TimeLimitedAttribute::UpdateData
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
template< typename T >
void TimeLimitedAttribute::UpdateData( const T& message )
{
    if( message.has_life_time() )
    {
        activityTime_ = static_cast< unsigned int >( message.life_time().value() );
        controller_.Update( *static_cast< TimeLimitedAttribute_ABC* >( this ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::DoUpdate
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::DoUpdate
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}
