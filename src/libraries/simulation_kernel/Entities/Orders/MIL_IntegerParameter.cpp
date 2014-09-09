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
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_IntegerParameter )

// -----------------------------------------------------------------------------
// Name: MIL_IntegerParameter constructor
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
MIL_IntegerParameter::MIL_IntegerParameter()
    : value_( 0 )
{
    // NOTHING
}

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

// -----------------------------------------------------------------------------
// Name: MIL_IntegerParameter::load
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_IntegerParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> value_;
}

// -----------------------------------------------------------------------------
// Name: MIL_IntegerParameter::save
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_IntegerParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << value_;
}
