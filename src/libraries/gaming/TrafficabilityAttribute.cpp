// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TrafficabilityAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute( Controller& controller )
    : controller_( controller )
    , max_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute destructor
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
TrafficabilityAttribute::~TrafficabilityAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::GetMaxValue
// Created: CMA 2011-09-07
// -----------------------------------------------------------------------------
double TrafficabilityAttribute::GetMaxValue() const
{
    return max_;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::Display
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Trafficability parameters" ) )
             .Display( tools::translate( "Object", "Trafficability:" ), max_ );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::DisplayInTooltip
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::DisplayInSummary
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::DisplayInSummary( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::UpdateData
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
template< typename T >
void TrafficabilityAttribute::UpdateData( const T& message )
{
    if( message.has_trafficability() )
    {
        max_ = static_cast< double >( message.trafficability().value() );
        controller_.Update( *static_cast< TrafficabilityAttribute_ABC* >( this ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::DoUpdate
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::DoUpdate
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}
