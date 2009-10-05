// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PointListParameter.h"
#include "simulation_orders/MIL_ParameterType_PointList.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_PointListParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PointListParameter::MIL_PointListParameter( const ASN1T_PointList& asn )
{
    pointList_.resize( asn.n );
    for( unsigned int i = 0; i < asn.n; ++i )
    {
        ASN1T_Point& point = asn.elem[i];
        if( point.type != EnumLocationType::point )
            throw std::runtime_error( "Unexpected type passed for point" );
        if( point.coordinates.n != 1 )
            throw std::runtime_error( "Too many points" );
        pointList_[i].reset( new MT_Vector2D() );
        MIL_Tools::ConvertCoordMosToSim( point.coordinates.elem[0], *pointList_[i] );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PointListParameter constructor
// Created: LDC 2009-07-08
// -----------------------------------------------------------------------------
MIL_PointListParameter::MIL_PointListParameter( const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList )
: pointList_( pointList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PointListParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PointListParameter::~MIL_PointListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PointListParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PointListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_PointList*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_PointListParameter::ToPointList
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PointListParameter::ToPointList( ASN1T_PointList& asn ) const
{
    asn.n = pointList_.size();
    if( !pointList_.empty() )
    {
        ASN1T_Location* pCoord = new ASN1T_Location[ pointList_.size() ]; //$$$ RAM
        asn.elem = pCoord;

        for( unsigned int i = 0; i < pointList_.size(); ++i )
            NET_ASN_Tools::WritePoint( *pointList_[i], pCoord[i] );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PointListParameter::ToPointList
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PointListParameter::ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& value ) const
{
    value = pointList_;
    return true;
}
