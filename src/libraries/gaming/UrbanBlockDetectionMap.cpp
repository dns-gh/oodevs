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
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/Positions.h"

// -----------------------------------------------------------------------------
// Name: UrbanBlockDetectionMap constructor
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
UrbanBlockDetectionMap::UrbanBlockDetectionMap( const kernel::DetectionMap& map )
    : map_( map )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDetectionMap destructor
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
UrbanBlockDetectionMap::~UrbanBlockDetectionMap()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDetectionMap::AddUrbanBlock
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
void UrbanBlockDetectionMap::AddUrbanBlock( gui::TerrainObjectProxy& object )
{
    float cellsize = map_.GetCellSize();
    if( kernel::Positions* positions = object.Retrieve< kernel::Positions >() )
    {
        geometry::Rectangle2f boundingBox = positions->GetBoundingBox();
        const unsigned int imin = static_cast< unsigned int >( boundingBox.Left() / cellsize );
        const unsigned int jmin = static_cast< unsigned int >( boundingBox.Bottom() / cellsize );
        const unsigned int imax = static_cast< unsigned int >( boundingBox.Right() / cellsize );
        const unsigned int jmax = static_cast< unsigned int >( boundingBox.Top() / cellsize );
        for( unsigned int j = jmin; j < jmax; ++j )
            for( unsigned int i = imin; i < imax; ++i )
            {
                geometry::Point2f cellCenter( i * cellsize + cellsize / 2, j * cellsize + cellsize / 2 );
                if( object.GetFootprint()->IsInside( cellCenter ) )
                    urbanBlockEnvironment_[ std::pair< int, int >( i, j ) ].data_ = &object;
            }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDetectionMap::GetUrbanBlock
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
const kernel::Object_ABC* UrbanBlockDetectionMap::GetUrbanBlock( const geometry::Point2f& point ) const
{
    float cellsize = map_.GetCellSize();
    const unsigned int nCellX = static_cast< unsigned int >( point.X() / cellsize );
    const unsigned int nCellY = static_cast< unsigned int >( point.Y() / cellsize );
    std::map< std::pair< int, int >, UrbanBlockEnvironment>::const_iterator it = urbanBlockEnvironment_.find( std::pair< int, int >( nCellX, nCellY ) );
    if( it != urbanBlockEnvironment_.end() )
        return urbanBlockEnvironment_.find( std::pair< int, int >( nCellX, nCellY ) )->second.data_;
    return 0;
}
