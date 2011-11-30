// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "CrossingSiteAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttribute::CrossingSiteAttribute( Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttribute::~CrossingSiteAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void CrossingSiteAttribute::UpdateData( const T& message )
{
    if( message.has_crossing_site()  )
    {
        width_ = message.crossing_site().width();
        depth_ = message.crossing_site().depth();
        speed_ = message.crossing_site().flow_rate();
        needsConstruction_ = message.crossing_site().banks_require_fitting() != 0;
        controller_.Update( *(CrossingSiteAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Crossing site" ) )
                .Display( tools::translate( "Object", "Width:" ), width_ * Units::meters )
                .Display( tools::translate( "Object", "Depth:" ), depth_ * Units::meters )
                .Display( tools::translate( "Object", "Stream speed:" ), speed_ * Units::metersPerSecond )
                .Display( tools::translate( "Object", "Build river banks:" ), needsConstruction_ );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
