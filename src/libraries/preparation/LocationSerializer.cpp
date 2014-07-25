// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LocationSerializer.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LocationSerializer constructor
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
LocationSerializer::LocationSerializer( const CoordinateConverter_ABC& converter )
    : converter_   ( converter )
    , xos_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer destructor
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
LocationSerializer::~LocationSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::Serialize
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void LocationSerializer::Serialize( const Location_ABC& location, xml::xostream& xos )
{
    xos_ = &xos;
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitLines
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void LocationSerializer::VisitLines( const T_PointVector& points )
{
    *xos_ << xml::attribute( "type", "ligne" );
    SetPoints( points );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitLines
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void LocationSerializer::VisitRectangle( const T_PointVector& /*points*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitPolygon
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void LocationSerializer::VisitPolygon( const T_PointVector& points )
{
    *xos_ << xml::attribute( "type", "polygone" );
    T_PointVector copy( points );
    copy.pop_back();
    SetPoints( copy );
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
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void LocationSerializer::VisitCircle( const geometry::Point2f& center, float radius )
{
    *xos_ << xml::attribute( "type", "cercle" );
    T_PointVector points;
    points.push_back( center );
    points.push_back( geometry::Point2f( center.X(), center.Y() + radius ) ); // $$$$ AGE 2006-08-09: may go out of extent !
    SetPoints( points );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitCurve
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void LocationSerializer::VisitCurve( const T_PointVector& points )
{
    *xos_ << xml::attribute( "type", "curve" );
    SetPoints( points );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitText
// Created: LGY 2014-07-21
// -----------------------------------------------------------------------------
void LocationSerializer::VisitText( const QString& /*text*/, const QFont& /*font*/, const geometry::Point2f& /*point*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::VisitPoint
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void LocationSerializer::VisitPoint( const geometry::Point2f& point )
{
    *xos_ << xml::attribute( "type", "point" );
    T_PointVector points( 1, point );
    SetPoints( points );
}

// -----------------------------------------------------------------------------
// Name: LocationSerializer::SetPoints
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationSerializer::SetPoints( const T_PointVector& points )
{
    *xos_ << xml::start( "points" );
    for( unsigned i = 0; i < points.size(); ++i )
    {
        const std::string coord = converter_.ConvertToMgrs( points[i] );
        *xos_ << xml::content( "point", coord.c_str() );
    }
    *xos_ << xml::end;
}