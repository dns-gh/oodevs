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
#include "simulation_orders/MIL_ParameterType_GDH.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_DateTimeParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_DateTimeParameter::MIL_DateTimeParameter( const ASN1T_DateTime & asn )
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
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_DateTimeParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_GDH*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_DateTimeParameter::ToGDH
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_DateTimeParameter::ToGDH( ASN1T_DateTime& asn ) const
{
    NET_ASN_Tools::WriteGDH( dateTime_, asn );
    return true;
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
