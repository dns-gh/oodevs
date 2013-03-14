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

namespace
{
    struct SpatialIndexer : geostore::SpatialIndexer
    {
        virtual void GetListWithinCircle( const geometry::Point2f& /*center*/,
            float /*radius*/, std::vector< const kernel::UrbanObject_ABC* >& /*result*/ ) const
        {}
    };

    struct Fixture
    {
        Fixture()
            : terrain ( BOOST_RESOLVE( "terrain" ) )
            , database( terrain / "Graphics/geostore.sqlite" )
        {
            database.Remove();
        }
        ~Fixture()
        {
            database.Remove();
        }
        tools::Path terrain;
        tools::Path database;
        SpatialIndexer indexer;
    };
}

BOOST_FIXTURE_TEST_CASE( CreateDatabase_Test, Fixture )
{
    geostore::GeoStoreManager manager( terrain, indexer );
    BOOST_CHECK_EQUAL( manager.GetDatabase().GetTables().size(), 11u );
}

BOOST_FIXTURE_TEST_CASE( LoadDatabase_Test, Fixture )
{
    geostore::GeoStoreManager( terrain, indexer );
    const std::time_t t = database.LastWriteTime();
    geostore::GeoStoreManager( terrain, indexer );
    BOOST_CHECK_MESSAGE( t == database.LastWriteTime(),
        "file time differs, likely because the database has been re-created instead of loaded" );
}

BOOST_FIXTURE_TEST_CASE( UrbanBlockAutoGeneration_Test, Fixture )
{
    geostore::GeoStoreManager manager( BOOST_RESOLVE( "terrain" ), indexer );
    std::vector< geometry::Point2f > pts;
    pts.push_back( geometry::Point2f( 55744.945f, 58097.934f ) );
    pts.push_back( geometry::Point2f( 56851.191f, 58429.145f ) );
    pts.push_back( geometry::Point2f( 57403.211f, 57607.742f ) );
    pts.push_back( geometry::Point2f( 55941.465f, 57011.563f ) );
    geometry::Polygon2f extent( pts );
    std::vector< geometry::Polygon2f > blocks;
    manager.CreateUrbanBlocksOnCities( extent, 10., blocks );
    BOOST_CHECK_EQUAL( blocks.size(), 17u );
}

BOOST_AUTO_TEST_CASE( LoadObsoleteDatabase_Test )
{
    const tools::Path terrain = BOOST_RESOLVE( "obsolete" );
    ( terrain / "Graphics/geostore.old" ).Copy( terrain / "Graphics/geostore.sqlite", tools::Path::OverwriteIfExists );
    SpatialIndexer indexer;
    geostore::GeoStoreManager( terrain, indexer );
    ( terrain / "Graphics/geostore.sqlite" ).Remove();
}
