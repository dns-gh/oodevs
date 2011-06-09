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
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_StringParameter )

// -----------------------------------------------------------------------------
// Name: MIL_StringParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_StringParameter::MIL_StringParameter()
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
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_StringParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eString;
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

// -----------------------------------------------------------------------------
// Name: MIL_StringParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_StringParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.set_acharstr( value_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_StringParameter::serialize
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_StringParameter::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< MIL_BaseParameter >( *this )
         & value_;
}
