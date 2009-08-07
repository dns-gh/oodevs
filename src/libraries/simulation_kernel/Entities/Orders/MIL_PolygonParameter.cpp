// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PolygonParameter.h"
#include "MIL_ParameterType_Polygon.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PolygonParameter::MIL_PolygonParameter( const ASN1T_Polygon& asn )
: pPolygon_( new TER_Localisation() )
{
    NET_ASN_Tools::ReadPolygon( asn, *pPolygon_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PolygonParameter::~MIL_PolygonParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PolygonParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_Polygon*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter::ToPolygon
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PolygonParameter::ToPolygon( ASN1T_Polygon& asn ) const
{
    NET_ASN_Tools::WritePolygon( *pPolygon_, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter::ToPolygon
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PolygonParameter::ToPolygon( boost::shared_ptr< TER_Localisation >& value ) const
{
    value = pPolygon_;
    return true;
}
