// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanGeometryAttribute.h"
#include "MT_Tools/MT_Vector2D.h"
#include "simulation_terrain/TER_World.h"

// -----------------------------------------------------------------------------
// Name: UrbanGeometryAttribute constructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
UrbanGeometryAttribute::UrbanGeometryAttribute( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "footprint" )
            >> xml::list( "point", *this, &UrbanGeometryAttribute::ReadPoint )
        >> xml::end;
    // TODO???
    // EliminateRedundantVertices()
    // ChokeSpikes()
}

// -----------------------------------------------------------------------------
// Name: UrbanGeometryAttribute destructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
UrbanGeometryAttribute::~UrbanGeometryAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanGeometryAttribute::ReadPoint
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void UrbanGeometryAttribute::ReadPoint( xml::xistream& xis )
{
    // TODO à simplifier quand on sera passé en double
    MT_Vector2D coord;
    TER_World::GetWorld().MosToSimMgrsCoord( xis.attribute< std::string >( "location" ), coord );
    vertices_.push_back( geometry::Point2f( static_cast< float >( coord.rX_ ), static_cast< float >( coord.rY_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: std::vector< MT_Vector2D >& UrbanGeometryAttribute::Vertices
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
const std::vector< geometry::Point2f >& UrbanGeometryAttribute::Vertices() const
{
    return vertices_;
}

// -----------------------------------------------------------------------------
// Name: UrbanGeometryAttribute::SetGeometry
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void UrbanGeometryAttribute::SetGeometry( const std::vector< geometry::Point2f >& vertices )
{
    vertices_ = vertices;
}

// -----------------------------------------------------------------------------
// Name: UrbanGeometryAttribute::ComputeArea
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
float UrbanGeometryAttribute::ComputeArea() const
{
    geometry::Polygon2f polygon( vertices_ );
    return polygon.ComputeArea();
}

// -----------------------------------------------------------------------------
// Name: UrbanGeometryAttribute::BoundingBox
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
geometry::Rectangle2f UrbanGeometryAttribute::BoundingBox() const
{
    geometry::Polygon2f polygon( vertices_ );
    return polygon.BoundingBox();
}

