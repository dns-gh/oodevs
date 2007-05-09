// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Position.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Position::Position()
    : strPosition_( )
{
}

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Position::Position( const ASN1T_CoordUTM& asn )
    : strPosition_( (const char*)asn.data, 15 )
{
}

// -----------------------------------------------------------------------------
// Name: Position destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Position::~Position()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Position::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Position::Update( const ASN1T_CoordUTM& asn )
{
    strPosition_ = ( (const char*)asn.data, 15 );
}

// -----------------------------------------------------------------------------
// Name: Position::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Position::Send( ASN1T_CoordUTM& asn ) const
{
    asn = strPosition_.c_str();
}
