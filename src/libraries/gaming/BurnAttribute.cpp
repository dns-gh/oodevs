// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "BurnAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: BurnAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
BurnAttribute::BurnAttribute( Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
BurnAttribute::~BurnAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void BurnAttribute::UpdateData( const T& message )
{
    if( message.has_burn()  )
    {
        currentHeat_ = message.burn().current_heat();
        currentCombustionEnergy_ = message.burn().combustion_energy();
        controller_.Update( *(BurnAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void BurnAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void BurnAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void BurnAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Burn" ) )
        .Display( tools::translate( "Object", "Heat:" ), currentHeat_ )
        .Display( tools::translate( "Object", "Combustion energy:" ), currentCombustionEnergy_ );
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void BurnAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
