// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Availability.h"
#include "Displayer_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Availability constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
Availability::Availability()
    : type_( 0 )
    , total_( 0 )
    , available_( 0 )
    , atWork_( 0 )
    , atRest_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Availability::Availability
// Created: MMC 2013-01-23
// -----------------------------------------------------------------------------
Availability::Availability( const kernel::EquipmentType* type, unsigned int total, unsigned int available, unsigned int atWork, unsigned int atRest )
    : type_( type )
    , total_( total )
    , available_( available )
    , atWork_( atWork )
    , atRest_( atRest )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Availability destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
Availability::~Availability()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Availability::Display
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Availability::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( 0, type_ )
             .Display( 0, total_)
             .Display( 0, available_ )
             .Display( 0, atWork_ )
             .Display( 0, atRest_ );
}

// -----------------------------------------------------------------------------
// Name: Availability::operator+=
// Created: MMC 2013-01-23
// -----------------------------------------------------------------------------
void Availability::operator+=( const Availability& other )
{ 
    total_ += other.total_;
    available_ += other.available_;
    atWork_+= other.atWork_;
    atRest_+= other.atRest_;
}
