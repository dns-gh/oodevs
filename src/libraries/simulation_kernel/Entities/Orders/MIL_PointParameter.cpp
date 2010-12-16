// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PointParameter.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PointParameter::MIL_PointParameter( const sword::Point& asn )
    : pPoint_( new MT_Vector2D() )
{
    if( asn.location().type() != sword::Location_Geometry_point )
        throw std::runtime_error( "Unexpected type passed for point" );
    if( asn.location().coordinates().elem_size() > 1 )
        throw std::runtime_error( "Too many points" );
    if( asn.location().coordinates().elem_size() == 0 )
        throw std::runtime_error( "No point" );
    MIL_Tools::ConvertCoordMosToSim( asn.location().coordinates().elem(0), *pPoint_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter constructor
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
MIL_PointParameter::MIL_PointParameter( const MT_Vector2D& point )
    : pPoint_( new MT_Vector2D( point ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PointParameter::~MIL_PointParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PointParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::ePoint
        || type == MIL_ParameterType_ABC::eLocationComposite;
}

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter::ToPoint
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PointParameter::ToPoint( boost::shared_ptr< MT_Vector2D >& result ) const
{
    result = pPoint_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter::ToElement
// Created: MGD 2010-11-05
// -----------------------------------------------------------------------------
bool MIL_PointParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    NET_ASN_Tools::WritePoint( *pPoint_, *elem.mutable_point() );
    return true;
}
