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
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"

namespace
{
    std::vector< geometry::Point2f > Convert( const sword::Location& location, const kernel::CoordinateConverter_ABC& converter )
    {
        std::vector< geometry::Point2f > positions;
        for( int i = 0; i < location.coordinates().elem_size(); ++i )
            positions.push_back( converter.ConvertToXY( location.coordinates().elem( i ) ) );
        if( positions.empty() )
            throw MASA_EXCEPTION( "UrbanPositions cannot be created with empty location" );
        return positions;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( const sword::Location& location, const kernel::CoordinateConverter_ABC& converter, const kernel::UrbanObject_ABC& object,
                                gui::Tesselator& tesselator )
    : gui::UrbanPositions( eUrbanLevelBlock, object, Convert( location, converter ), tesselator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions destructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
UrbanPositions::~UrbanPositions()
{
    // NOTHING
}
