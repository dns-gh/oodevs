// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DetectionMap.h"
#include "pathfind/InputBinaryStream.h"
#include "WorldParameters.h"

#include "boost/static_assert.hpp"

BOOST_STATIC_ASSERT( sizeof( DetectionMap::Environment ) == 1 );

namespace
{
    struct ArchiveCell {
        short h;
        unsigned char dh;
        unsigned char e;
    };

    BOOST_STATIC_ASSERT( sizeof( ArchiveCell ) == 4 );
}

// -----------------------------------------------------------------------------
// Name: DetectionMap constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
DetectionMap::DetectionMap()
    : map_( 0 )
    , environment_( 0 )
    , cellsize_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionMap destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
DetectionMap::~DetectionMap()
{
    delete map_;
    delete[] environment_;
}

// -----------------------------------------------------------------------------
// Name: DetectionMap::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void DetectionMap::Load( const std::string& scipioXml )
{
    delete map_;
    delete[] environment_;

    WorldParameters parameters( scipioXml );
    map_ = new ElevationMap( parameters.detection_ );

    InputBinaryStream archive( parameters.detection_ );
    double rcs; unsigned uDummy;
    archive >> rcs >> uDummy >> uDummy;
    cellsize_ = static_cast< float >( rcs );

    environment_ = new Environment[ Width() * Height() ];
    ArchiveCell archiveCell;
    for( unsigned j = 0; j < Width(); ++j )
        for( unsigned i = 0; i < Height(); ++i )
        {
            archive >> archiveCell;
            Environment* env = environment_ + i * Width() + j;
            if( archiveCell.e & 0x01 )
                env->data_ = Environment::forest_;
            else if( archiveCell.e & 0x02 )
                env->data_ = Environment::town_;
            else
                env->data_ = 0;
        }
}

// -----------------------------------------------------------------------------
// Name: DetectionMap::EnvironmentData
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
const DetectionMap::Environment* DetectionMap::EnvironmentData( unsigned x, unsigned y ) const
{
    if( ! environment_ )
        return 0;
    return environment_ + y * Width() + x;
}

// -----------------------------------------------------------------------------
// Name: DetectionMap::EnvironmentAt
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
DetectionMap::Environment DetectionMap::EnvironmentAt( const geometry::Point2f& point ) const
{
    const std::pair< unsigned, unsigned > p = Unmap( point );
    return *EnvironmentData( p.first, p.second );
}

// -----------------------------------------------------------------------------
// Name: DetectionMap::GetCellSize
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
float DetectionMap::GetCellSize() const
{
    return cellsize_;
}
