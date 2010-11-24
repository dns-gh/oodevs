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
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_RealParameter::MIL_RealParameter( float value )
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
bool MIL_RealParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eNumeric || MIL_ParameterType_ABC::eInteger;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter::ToNumeric
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_RealParameter::ToNumeric( float& value ) const
{
    value = value_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_RealParameter::ToElement( Common::MsgMissionParameter_Value& elem ) const
{
    elem.set_areal( value_ );
    return true;
}
