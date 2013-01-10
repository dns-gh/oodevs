// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_test_pch.h"

#include <geostore/Database.h>
#include <geostore/GeoStoreManager.h>
#include <geostore/SpatialIndexer.h>

using namespace geostore;
using namespace geometry;

namespace bfs = boost::filesystem;

namespace
{
    class DummyIndexer : public SpatialIndexer
    {
        virtual void GetListWithinCircle( const geometry::Point2f& , float , std::vector< const kernel::UrbanObject_ABC* >& result ) const
        {
            result.clear();
        }
    };
}

BOOST_AUTO_TEST_CASE( Database_Test )
{
    bfs::path terrainPath( BOOST_RESOLVE( "terrain" ) );

    // Destroy existing database...
    bfs::remove( terrainPath / "Graphics" / "geostore.sqlite" );

    DummyIndexer index;
    GeoStoreManager manager( terrainPath, index );
    BOOST_CHECK_EQUAL( manager.GetDatabase().tables_.size(), 11u );
}

BOOST_AUTO_TEST_CASE( UrbanBlockAutoGeneration_Test )
{
    DummyIndexer index;
    GeoStoreManager manager( BOOST_RESOLVE( "terrain" ), index );

    std::vector< Point2f > pts;
    pts.push_back( Point2f( 55744.945f, 58097.934f ) );
    pts.push_back( Point2f( 56851.191f, 58429.145f ) );
    pts.push_back( Point2f( 57403.211f, 57607.742f ) );
    pts.push_back( Point2f( 55941.465f, 57011.563f ) );

    geometry::Polygon2f extent( pts );
    std::vector< geometry::Polygon2f > blocks;
    manager.CreateUrbanBlocksOnCities( extent, 10., blocks );

    BOOST_CHECK_EQUAL( blocks.size(), 17u );
}
