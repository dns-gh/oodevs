// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FloodModel.h"
#include <core/EventListener_ABC.h>
#include <core/Facade.h>
#include <core/Model.h>
#include <core/MakeModel.h>

using namespace sword;

namespace
{
    const std::string event = "flood event";
}

// -----------------------------------------------------------------------------
// Name: FloodModel constructor
// Created: LGY 2012-06-13
// -----------------------------------------------------------------------------
FloodModel::FloodModel( core::Facade& facade )
    : facade_    ( facade )
    , identifier_( 0 )
{
    facade.Register( event, *this );
}

// -----------------------------------------------------------------------------
// Name: FloodModel destructor
// Created: LGY 2012-06-13
// -----------------------------------------------------------------------------
FloodModel::~FloodModel()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: FloodModel::GenerateFlood
// Created: LGY 2012-07-30
// -----------------------------------------------------------------------------
void FloodModel::GenerateFlood( const geometry::Point2d& center, T_Polygons& deepAreas, T_Polygons& lowAreas, int depth, int refDist ) const
{
    GenerateFlood( geometry::Point2f( static_cast< float >( center.X() ), static_cast< float >( center.Y() ) ), deepAreas, lowAreas, depth, refDist );
}

// -----------------------------------------------------------------------------
// Name: FloodModel::GenerateFlood
// Created: LGY 2012-07-30
// -----------------------------------------------------------------------------
void FloodModel::GenerateFlood( const geometry::Point2f& center, T_Polygons& deepAreas, T_Polygons& lowAreas, int depth, int refDist ) const
{
    T_FloodEvent& event = floods_[ ++identifier_ ];
    event.deepAreas_ = &deepAreas;
    event.lowAreas_ = &lowAreas;
    facade_.PostCommand( "flood command", core::MakeModel( "center", core::MakeModel( "x", center.X() )
                                                                                    ( "y", center.Y() ) )
                                                         ( "depth", depth )
                                                         ( "radius", refDist )
                                                         ( "identifier", identifier_ ) );
}

// -----------------------------------------------------------------------------
// Name: FloodModel::Notify
// Created: LGY 2012-07-31
// -----------------------------------------------------------------------------
void FloodModel::Notify( const core::Model& event )
{
    unsigned int identifier = event[ "identifier" ];
    const T_FloodEvent& flood = floods_[ identifier ];

    Fill( flood.deepAreas_, event[ "deep-areas" ] );
    Fill( flood.lowAreas_, event[ "low-areas" ] );

    floods_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: FloodModel::Fill
// Created: LGY 2012-08-01
// -----------------------------------------------------------------------------
void FloodModel::Fill( T_Polygons* polygons, const core::Model& areas )
{
    for( std::size_t i = 0; i < areas.GetSize(); ++i )
    {
        polygons->push_back( new geometry::Polygon2f() );
        const core::Model& area = areas.GetElement( i );
        for( std::size_t j = 0; j < area.GetSize(); ++j )
        {
            const core::Model& point = area.GetElement( j );
            polygons->back()->Add( geometry::Point2f( point[ "x" ], point[ "y" ] ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FloodModel::serialize
// Created: LGY 2012-08-01
// -----------------------------------------------------------------------------
template< typename Archive >
void FloodModel::serialize( Archive& archile, const unsigned int )
{
    archive & boost::serialization::base_object< FloodModel_ABC >( *this )
            & identifier_;
}
