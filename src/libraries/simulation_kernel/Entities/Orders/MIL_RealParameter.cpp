// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_RealParameter.h"
#include "simulation_orders/MIL_ParameterType_Numeric.h"

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_RealParameter::MIL_RealParameter( double value )
: value_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_RealParameter::~MIL_RealParameter()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_RealParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_RealParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_Numeric*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter::ToNumeric
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_RealParameter::ToNumeric( ASN1REAL& value ) const
{
    value = value_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter::ToNumeric
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_RealParameter::ToNumeric( float& value ) const
{
    value = static_cast< float >( value_ );
    return true;
}
