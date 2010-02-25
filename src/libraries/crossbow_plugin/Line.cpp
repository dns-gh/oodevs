// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "Line.h"
#include "Point.h"
#include "protocol/simulationsenders.h"

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: Line constructor
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
crossbow::Line::Line()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line constructor
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Line::Line( const Common::MsgCoordLatLongList& message )
    : PointCollection( message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line constructor
// Created: JCR 2007-11-06
// -----------------------------------------------------------------------------
crossbow::Line::Line( IGeometryPtr geometry )
    : PointCollection( geometry )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line destructor
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Line::~Line()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line::UpdateGeometry
// Created: JCR 2007-08-31
// -----------------------------------------------------------------------------
void crossbow::Line::Serialize( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const
{
    if( geometry == NULL )
        geometry.CreateInstance( CLSID_Polyline );
    PointCollection::Serialize( geometry, spatialReference );
}

// -----------------------------------------------------------------------------
// Name: Line::Serialize
// Created: JCR 2009-04-27
// -----------------------------------------------------------------------------
void crossbow::Line::Serialize( std::ostream& geometry ) const
{
  const int srid = 0;
    geometry << "st_linestring("
             << "'linestring"; // point collection already provides '(' and ')'
    PointCollection::Serialize( geometry );
    geometry << "'," << srid
             << ")";
}

// -----------------------------------------------------------------------------
// Name: Line::Serialize
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Line::Serialize( Common::MsgLocation& message ) const
{
    message.set_type( Common::MsgLocation_Geometry_line );
    PointCollection::Serialize( message );
}
