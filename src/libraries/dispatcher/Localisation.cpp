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
    : nType_ ( sword::Location::ellipse )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Localisation constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Localisation::Localisation( const sword::Location& msg )
    : nType_ ( msg.type() )
{
    points_.reserve( msg.coordinates().elem_size() );
    std::copy( msg.coordinates().elem().begin(), msg.coordinates().elem().end(), std::back_inserter( points_ ) );
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
void Localisation::Update( const sword::Location& msg )
{
    nType_ = msg.type();
    points_.resize( 0 );
    points_.reserve( msg.coordinates().elem_size() );
    std::copy( msg.coordinates().elem().begin(), msg.coordinates().elem().end(), std::back_inserter( points_ ) );
}

// -----------------------------------------------------------------------------
// Name: Localisation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Localisation::Send( sword::Location& msg ) const
{
    msg.set_type( nType_ );
    for( T_PositionVector::const_iterator it = points_.begin(); it != points_.end(); ++it )
        *msg.mutable_coordinates()->add_elem() = *it;
}

// -----------------------------------------------------------------------------
// Name: Localisation::Send
// Created: NLD 2010-12-27
// -----------------------------------------------------------------------------
bool Localisation::IsEmpty() const
{
    return points_.empty();
}