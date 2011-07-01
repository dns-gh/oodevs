// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LodgingAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LodgingAttribute constructor
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute( Controller& controller, const tools::Resolver_ABC< Automat_ABC >& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , capacity_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute destructor
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::~LodgingAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::UpdateData
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
template< typename T >
void LodgingAttribute::UpdateData( const T& message )
{
    if( message.has_lodging()  )
    {
        capacity_ = message.lodging().capacity();
        controller_.Update( *(LodgingAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::DoUpdate
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::DoUpdate
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::Display
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Camp" ) )
        .Display( tools::translate( "Object", "Lodging capacity:" ), capacity_ );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::DisplayInSummary
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::DisplayInTooltip
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Capacity:" ), capacity_ );
}