// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DateTimeParameter.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_DateTimeParameter )

// -----------------------------------------------------------------------------
// Name: MIL_DateTimeParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_DateTimeParameter::MIL_DateTimeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DateTimeParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_DateTimeParameter::MIL_DateTimeParameter( const sword::DateTime& asn )
{
    NET_ASN_Tools::ReadGDH( asn, dateTime_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_DateTimeParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_DateTimeParameter::~MIL_DateTimeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DateTimeParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_DateTimeParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eDateTime;
}

// -----------------------------------------------------------------------------
// Name: MIL_DateTimeParameter::ToGDH
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_DateTimeParameter::ToGDH( float& value ) const
{
    value = static_cast< float >( dateTime_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_DateTimeParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_DateTimeParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    NET_ASN_Tools::WriteGDH( dateTime_, *elem.mutable_datetime() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_DateTimeParameter::serialize
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_DateTimeParameter::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< MIL_BaseParameter >( *this )
         & dateTime_;
}
