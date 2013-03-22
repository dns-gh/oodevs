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
#pragma warning( push, 0 )
#include <boost/thread/thread.hpp>
#include <boost/exception_ptr.hpp>
#include <boost/phoenix.hpp>
#pragma warning( pop )

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

BOOST_FIXTURE_TEST_CASE( CreateDatabaseTwice_Test, Fixture )
{
    geostore::GeoStoreManager manager( terrain, indexer );
    geostore::GeoStoreManager( terrain, indexer );
}

namespace
{
    void Watch( boost::exception_ptr& error, const boost::function< void() >& f )
    {
        try
        {
            f();
        }
        catch( std::exception& e )
        {
            error = boost::copy_exception( e );
        }
        catch( ... )
        {
            error = boost::current_exception();
        }
    }
}

BOOST_FIXTURE_TEST_CASE( ConcurrentlyCreateDatabaseTwice_Test, Fixture )
{
    boost::exception_ptr error;
    auto f = boost::bind( &Watch, boost::ref( error ),
        boost::phoenix::construct< geostore::GeoStoreManager >( terrain, boost::cref( indexer ) ) );
    boost::thread_group group;
    group.create_thread( f );
    boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
    group.create_thread( f );
    group.join_all();
    BOOST_CHECK_MESSAGE( ! error, boost::diagnostic_information( error ) );
    BOOST_CHECK( database.Exists() );
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
