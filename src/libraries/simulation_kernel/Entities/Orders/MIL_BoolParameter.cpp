// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_BoolParameter.h"
#include "simulation_orders/MIL_ParameterType_Bool.h"

// -----------------------------------------------------------------------------
// Name: MIL_BoolParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_BoolParameter::MIL_BoolParameter( bool value )
: value_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_BoolParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_BoolParameter::~MIL_BoolParameter()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_BoolParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_BoolParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_Bool*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_BoolParameter::ToBool
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_BoolParameter::ToBool( bool& value ) const
{
    value = value_;
    return true;
}
