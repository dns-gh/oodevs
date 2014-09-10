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
#include "protocol/Protocol.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_BoolParameter )

// -----------------------------------------------------------------------------
// Name: MIL_BoolParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_BoolParameter::MIL_BoolParameter()
    : value_( false )
{
    // NOTHING
}

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
bool MIL_BoolParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eBool;
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

// -----------------------------------------------------------------------------
// Name: MIL_BoolParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_BoolParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.set_booleanvalue( value_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_BoolParameter::load
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void MIL_BoolParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> value_;
}

// -----------------------------------------------------------------------------
// Name: MIL_BoolParameter::save
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void MIL_BoolParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << value_;
}
