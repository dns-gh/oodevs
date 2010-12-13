// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_EnumerationParameter.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_EnumerationParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_EnumerationParameter::MIL_EnumerationParameter( int value )
: value_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_EnumerationParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_EnumerationParameter::~MIL_EnumerationParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_EnumerationParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_EnumerationParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eEnumeration;
}

// -----------------------------------------------------------------------------
// Name: MIL_EnumerationParameter::ToId
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_EnumerationParameter::ToId( int& value ) const
{
    value = value_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_EnumerationParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_EnumerationParameter::ToElement( sword::MsgMissionParameter_Value& elem ) const
{
    elem.set_enumeration( value_ );
    return true;
}

