// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "SupplyRouteAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyRouteAttribute::SupplyRouteAttribute( Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyRouteAttribute::~SupplyRouteAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void SupplyRouteAttribute::UpdateData( const T& message )
{
    if( message.has_supply_route()  )
    {
        nLogRouteFlow_      = message.supply_route().flow_rate();
        nLogRouteLength_    = message.supply_route().length();
        nLogRouteWidth_     = message.supply_route().width();
        nLogRouteMaxWeight_ = message.supply_route().max_weight();
        bLogRouteEquipped_  = message.supply_route().equipped() != 0;
        controller_.Update( *(SupplyRouteAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Logistic route" ) )
                .Display( tools::translate( "Object", "Equipped:" ), bLogRouteEquipped_ )
                .Display( tools::translate( "Object", "Flow:" ), nLogRouteFlow_ * Units::vehiclesPerHour )
                .Display( tools::translate( "Object", "Width:" ), nLogRouteWidth_ * Units::meters )
                .Display( tools::translate( "Object", "Length:" ), nLogRouteLength_ * Units::meters )
                .Display( tools::translate( "Object", "Maximum weight:" ), nLogRouteMaxWeight_ * Units::tons );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
