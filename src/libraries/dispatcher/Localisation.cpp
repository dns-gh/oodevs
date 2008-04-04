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

using namespace dispatcher;


// -----------------------------------------------------------------------------
// Name: Localisation constructor
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
Localisation::Localisation()
    : nType_ ( EnumLocationType::ellipse )
    , points_()
{
}

// -----------------------------------------------------------------------------
// Name: Localisation constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Localisation::Localisation( const ASN1T_Location& asn )
    : nType_ ( asn.type )
    , points_( )
{
    points_.reserve( asn.coordinates.n );
    std::copy( asn.coordinates.elem, asn.coordinates.elem + asn.coordinates.n, std::back_inserter( points_ ) );
}

// -----------------------------------------------------------------------------
// Name: Localisation destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Localisation::~Localisation()
{

}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Localisation::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Localisation::Update( const ASN1T_Location& asn )
{
    nType_ = asn.type;
    points_.resize( 0 );
    std::copy( asn.coordinates.elem, asn.coordinates.elem + asn.coordinates.n, std::back_inserter( points_ ) );
}

// -----------------------------------------------------------------------------
// Name: Localisation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Localisation::Send( ASN1T_Location& asn ) const
{
    asn.type              = nType_;
    asn.coordinates.n    = points_.size();
    asn.coordinates.elem = points_.size() ? const_cast< ASN1T_CoordLatLong* >( & points_.front() ) : 0;
}
