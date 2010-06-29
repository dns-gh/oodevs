// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanBlockDetectionMap.h"
#include "clients_kernel/DetectionMap.h"
#include <urban/TerrainObject_ABC.h>

// -----------------------------------------------------------------------------
// Name: UrbanBlockDetectionMap constructor
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
UrbanBlockDetectionMap::UrbanBlockDetectionMap( const kernel::DetectionMap& map )
    : map_( map )
{
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDetectionMap destructor
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
UrbanBlockDetectionMap::~UrbanBlockDetectionMap()
{
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDetectionMap::AddUrbanBlock
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
void UrbanBlockDetectionMap::AddUrbanBlock( urban::TerrainObject_ABC& object )
{
    float cellsize = map_.GetCellSize();
    const geometry::Polygon2f* footprint = object.GetFootprint();
    geometry::Rectangle2f boundingBox = footprint->BoundingBox();
    unsigned imin = boundingBox.Left()/cellsize;
    unsigned jmin = boundingBox.Bottom()/cellsize;
    unsigned imax = boundingBox.Right()/cellsize;
    unsigned jmax = boundingBox.Top()/cellsize;

    for( unsigned j = jmin; j < jmax; ++j )
    {
        for( unsigned i = imin; i < imax; ++i )
        {
            geometry::Point2f cellCenter( i * cellsize + cellsize/2, j * cellsize + cellsize/2 );
            if( footprint->IsInside( cellCenter ) )
                urbanBlockEnvironment_[ std::pair< int, int >( i, j ) ].data_ = &object;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDetectionMap::GetUrbanBlock
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
const urban::TerrainObject_ABC* UrbanBlockDetectionMap::GetUrbanBlock( const geometry::Point2f& point ) const
{
    float cellsize = map_.GetCellSize();
    unsigned nCellX = point.X() / cellsize;
    unsigned nCellY = point.Y() / cellsize;
    std::map<std::pair<int, int>, UrbanBlockEnvironment>::const_iterator it = urbanBlockEnvironment_.find( std::pair<int, int>( nCellX, nCellY ) );
    if( it != urbanBlockEnvironment_.end() )
        return urbanBlockEnvironment_.find( std::pair<int, int>( nCellX, nCellY ) )->second.data_;
    return 0;
}