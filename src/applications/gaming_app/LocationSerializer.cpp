// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LocationSerializer.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "protocol/Protocol.h"

using namespace Common;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LocationSerializer constructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
LocationSerializer::LocationSerializer( const CoordinateConverter_ABC& converter )
    : converter_   ( converter )
    , localisation_( NULL )
    //, pCoords_     ( 0 )
    , ownsCoords_  ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer constructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
LocationSerializer::LocationSerializer( const CoordinateConverter_ABC& converter, MsgLocation& localisation )
    : converter_   ( converter )
    //, localisation_( &localisation )
    //, pCoords_     ( 0 )
    , ownsCoords_  ( true )
{
    // NOTHING
    localisation_ = new MsgLocation();
    localisation_->CopyFrom( localisation );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer destructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
LocationSerializer::~LocationSerializer()
{
//    if( ownsCoords_ )
//        delete[] pCoords_;
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::Serialize
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::Serialize( const Location_ABC& location )
{
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::Serialize
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::Serialize( const Location_ABC& location, MsgLocation& localisation )
{
    *localisation_ = localisation;
    Serialize( location );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::SetPoints
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::SetPoints( const T_PointVector& points )
{
    if( ! localisation_ )
        throw std::runtime_error( "localisation not set" );
    const unsigned nNbrPoints = points.size();

//    if( nNbrPoints )
//    {
//        if( ownsCoords_ )
//            delete[] pCoords_;
//        pCoords_ = new MsgCoordLatLong[ nNbrPoints ];
//        localisation_->mutable_coordinates().mutable_elem() = pCoords_;

        for( unsigned int i = 0; i < nNbrPoints; ++i )
            converter_.ConvertToGeo( points[i], *localisation_->mutable_coordinates()->add_elem() );
//    }
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitLines
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::VisitLines( const T_PointVector& points )
{
    SetPoints( points );
    localisation_->set_type( MsgLocation_Geometry_line );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitPolygon
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::VisitPolygon( const T_PointVector& points )
{
    SetPoints( points );
    localisation_->set_type( MsgLocation_Geometry_polygon );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitPath
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
void LocationSerializer::VisitPath( const geometry::Point2f& , const T_PointVector& points )
{
    VisitLines( points );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitCircle
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::VisitCircle( const geometry::Point2f& center, float radius )
{
    T_PointVector points;
    points.push_back( center );
    points.push_back( geometry::Point2f( center.X(), center.Y() + radius ) ); // $$$$ AGE 2006-08-09: may go out of extent !
    SetPoints( points );
    localisation_->set_type( MsgLocation_Geometry_circle );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::VisitPoint( const geometry::Point2f& point )
{
    T_PointVector points( 1, point );
    SetPoints( points );
    localisation_->set_type( MsgLocation_Geometry_point );
}
