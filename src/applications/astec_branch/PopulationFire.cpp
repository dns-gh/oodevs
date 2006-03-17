// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationFire.h"
#include "Population.h"

// -----------------------------------------------------------------------------
// Name: PopulationFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
PopulationFire::PopulationFire( const ASN1T_MsgStartPopulationFire& message, const Resolver_ABC< Population >& resolver )
    : Fire_ABC( resolver.Get( message.oid_src ) )
    , id_( message.oid_tir )
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
void PopulationFire::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    // $$$$ AGE 2006-03-17: 
}
