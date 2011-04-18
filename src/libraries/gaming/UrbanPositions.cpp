// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanPositions.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"
#include <urban/TerrainObject_ABC.h>

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( const urban::TerrainObject_ABC& object, const sword::Location& message, const kernel::CoordinateConverter_ABC& converter, const std::string& name )
    : object_( object )
    , name_  ( name )
{
    for( int i = 0; i < message.coordinates().elem_size(); ++i )
        polygon_.Add( converter.ConvertToXY( message.coordinates().elem( i ) ) );
    boundingBox_ = polygon_.BoundingBox();
    barycenter_ = polygon_.Barycenter();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions destructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
UrbanPositions::~UrbanPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Barycenter
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
geometry::Point2f UrbanPositions::Barycenter() const
{
    return barycenter_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::BoundingBox
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
geometry::Rectangle2f UrbanPositions::BoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsInside
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
bool UrbanPositions::IsInside( const geometry::Point2f& point ) const
{
    return polygon_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Draw
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    tools.DrawDecoratedPolygon( polygon_, object_.GetDecoration(), name_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Vertices
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
const std::vector< geometry::Point2f >& UrbanPositions::Vertices() const
{
    return polygon_.Vertices();
}
