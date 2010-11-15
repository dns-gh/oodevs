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
#include "simulation_orders/MIL_ParameterType_Point.h"
#include "simulation_orders/MIL_ParameterType_LocationComposite.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/protocol.h"
#include "tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PointParameter::MIL_PointParameter( const Common::MsgPoint& asn )
    : pPoint_( new MT_Vector2D() )
{
    if( asn.location().type() != Common::MsgLocation_Geometry_point )
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
bool MIL_PointParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return dynamic_cast< const MIL_ParameterType_Point* >( &type ) != 0
        || dynamic_cast< const MIL_ParameterType_LocationComposite* >( &type ) != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter::ToPoint
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PointParameter::ToPoint( Common::MsgPoint& asn ) const
{
    NET_ASN_Tools::WritePoint( *pPoint_, asn );
    return true;
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
bool MIL_PointParameter::ToElement( Common::MsgMissionParameter_Value& elem ) const
{
    ToPoint( *elem.mutable_point() );
    return true;
}
