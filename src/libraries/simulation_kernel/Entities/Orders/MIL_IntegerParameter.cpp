// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_IntegerParameter.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_IntegerParameter constructor
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
MIL_IntegerParameter::MIL_IntegerParameter( int value )
    : value_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_IntegerParameter destructor
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
MIL_IntegerParameter::~MIL_IntegerParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_IntegerParameter::IsOfType
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
bool MIL_IntegerParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eInteger;
}

// -----------------------------------------------------------------------------
// Name: MIL_IntegerParameter::ToId
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
bool MIL_IntegerParameter::ToId( int& value ) const
{
    value = value_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_IntegerParameter::ToElement
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
bool MIL_IntegerParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.set_intvalue( value_ );
    return true;
}
