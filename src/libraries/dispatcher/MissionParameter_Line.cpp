// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Line.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Line constructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_Line::MissionParameter_Line( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , location_( *asn.value.u.line )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Line destructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_Line::~MissionParameter_Line()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Line::Send
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_Line::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value   = bNullValue_;
    asn.value.t      = T_MissionParameter_value_line;
    asn.value.u.line = new ASN1T_Line();
    location_.Send( *asn.value.u.line );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Line::AsnDelete
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_Line::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.line;
}
