// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationFire.h"
#include "clients_kernel/Population_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
PopulationFire::PopulationFire( const ASN1T_MsgStartPopulationFire& message, const tools::Resolver_ABC< Population_ABC >& resolver )
    : Fire_ABC( resolver.Get( message.firer_oid ) )
    , id_( message.fire_oid )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFire destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
PopulationFire::~PopulationFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFire::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void PopulationFire::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& , const GlTools_ABC& ) const
{
    // $$$$ AGE 2007-02-23: 
}
