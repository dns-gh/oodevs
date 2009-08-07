// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DirectionParameter.h"

#include "MIL_ParameterType_Direction.h"

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_DirectionParameter::MIL_DirectionParameter( const ASN1T_Heading& heading )
: heading_( heading )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_DirectionParameter::~MIL_DirectionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_DirectionParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_Direction*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter::ToDirection
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_DirectionParameter::ToDirection( ASN1T_Heading& asn ) const
{
    asn = heading_;
    return true;
}
