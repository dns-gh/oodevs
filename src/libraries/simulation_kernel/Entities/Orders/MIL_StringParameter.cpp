// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_StringParameter.h"
#include "simulation_orders/MIL_ParameterType_String.h"

// -----------------------------------------------------------------------------
// Name: MIL_StringParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_StringParameter::MIL_StringParameter( const ASN1VisibleString & asn )
    : value_( asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_StringParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_StringParameter::MIL_StringParameter( const std::string& message )
    : value_( message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_StringParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_StringParameter::~MIL_StringParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_StringParameter::IsOfType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_StringParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_String*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_StringParameter::ToString
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_StringParameter::ToString( ASN1VisibleString& asn ) const
{
    asn = value_.c_str();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_StringParameter::ToString
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_StringParameter::ToString( std::string& value ) const
{
    value = value_;
    return true;
}
