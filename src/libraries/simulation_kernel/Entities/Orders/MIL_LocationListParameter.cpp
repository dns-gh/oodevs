// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LocationListParameter.h"

#include "simulation_orders/MIL_ParameterType_LocationList.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_LocationListParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_LocationListParameter::MIL_LocationListParameter( const ASN1T_LocationList& asn )
{
    NET_ASN_Tools::ReadLocationList( asn, locationList_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationListParameter constructor
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
MIL_LocationListParameter::MIL_LocationListParameter( const std::vector< boost::shared_ptr< TER_Localisation > >& locationList )
: locationList_( locationList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationListParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_LocationListParameter::~MIL_LocationListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationListParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_LocationListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_LocationList*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationListParameter::ToLocationList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_LocationListParameter::ToLocationList( ASN1T_LocationList& asn ) const
{
    NET_ASN_Tools::WriteLocationList( locationList_, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationListParameter::ToLocationList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_LocationListParameter::ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& value ) const
{
    value = locationList_;
    return true;
}
