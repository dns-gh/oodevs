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
#include "protocol/Protocol.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_RealParameter )

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_RealParameter::MIL_RealParameter()
{
    // NOTHING
}

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
    return type == MIL_ParameterType_ABC::eNumeric || MIL_ParameterType_ABC::eInteger || MIL_ParameterType_ABC::eEnumeration;
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
// Name: MIL_RealParameter::ToId
// Created: LMT 2010-10-17
// -----------------------------------------------------------------------------
bool MIL_RealParameter::ToId( int& value ) const
{
    value = static_cast< int >( value_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_RealParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.set_areal( value_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter::load
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_RealParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> value_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealParameter::save
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_RealParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << value_;
}
