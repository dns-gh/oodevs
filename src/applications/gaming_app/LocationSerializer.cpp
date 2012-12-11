// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LocationSerializer.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Location_ABC.h"

using namespace sword;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LocationSerializer constructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
LocationSerializer::LocationSerializer( const CoordinateConverter_ABC& converter )
    : converter_ ( converter )
    , location_  ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer constructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
LocationSerializer::LocationSerializer( const CoordinateConverter_ABC& converter, Location& localisation )
    : converter_ ( converter )
    , location_  ( &localisation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer destructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
LocationSerializer::~LocationSerializer()
{
    // NOTHING
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
void LocationSerializer::Serialize( const Location_ABC& location, Location& localisation )
{
    location_ = &localisation;
    Serialize( location );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::SetPoints
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::SetPoints( const T_PointVector& points )
{
    if( ! location_ )
        throw MASA_EXCEPTION( "localisation not set" );
    location_->mutable_coordinates()->Clear();
    for( T_PointVector::const_iterator it = points.begin(); it != points.end(); ++it )
        converter_.ConvertToGeo( *it, *location_->mutable_coordinates()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitLines
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::VisitLines( const T_PointVector& points )
{
    SetPoints( points );
    location_->set_type( Location_Geometry_line );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void LocationSerializer::VisitRectangle( const T_PointVector& points )
{
    SetPoints( points );
    location_->set_type( Location_Geometry_rectangle );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitPolygon
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::VisitPolygon( const T_PointVector& points )
{
    SetPoints( points );
    location_->set_type( Location_Geometry_polygon );
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
    location_->set_type( Location_Geometry_circle );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::VisitPoint( const geometry::Point2f& point )
{
    T_PointVector points( 1, point );
    SetPoints( points );
    location_->set_type( Location_Geometry_point );
}
