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
#include "simulation_terrain/TER_Localisation.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PolygonParameter::MIL_PolygonParameter( const sword::MsgPolygon& asn )
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
bool MIL_PolygonParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::ePolygon
        || type == MIL_ParameterType_ABC::eLocationComposite;
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

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_PolygonParameter::ToElement( sword::MsgMissionParameter_Value& elem ) const
{
    NET_ASN_Tools::WritePolygon( *pPolygon_, *elem.mutable_area() );
    return true;
}
