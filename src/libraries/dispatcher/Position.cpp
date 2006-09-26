// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
