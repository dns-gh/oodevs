// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Localisation.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Localisation constructor
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
Localisation::Localisation()
: nType_ ( Common::MsgLocation::ellipse )
    , points_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Localisation constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Localisation::Localisation( const Common::MsgLocation& asn )
    : nType_ ( asn.type() )
    , points_( )
{
    points_.reserve( asn.coordinates().elem_size() );
    std::copy( asn.coordinates().elem().begin(), asn.coordinates().elem().end(), std::back_inserter( points_ ) );
}

// -----------------------------------------------------------------------------
// Name: Localisation destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Localisation::~Localisation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Localisation::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Localisation::Update( const Common::MsgLocation& asn )
{
    nType_ = asn.type();
    points_.resize( 0 );
    points_.reserve( asn.coordinates().elem_size() );
    std::copy( asn.coordinates().elem().begin(), asn.coordinates().elem().end(), std::back_inserter( points_ ) );
}

// -----------------------------------------------------------------------------
// Name: Localisation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Localisation::Send( Common::MsgLocation& asn ) const
{
    asn.set_type( nType_ );
    for( T_PositionVector::const_iterator it = points_.begin(); it != points_.end(); ++it )
        *asn.mutable_coordinates()->add_elem() = *it;
}
