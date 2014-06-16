// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_test_pch.h"
#include "geostore/Database.h"
#include "geostore/SpatialIndexer.h"
#include "geostore/CreateBlockAutoProcess.h"
#include <terrain/DummyPointProjector.h>
#include <terrain/TerrainObject.h>
#include <tools/TemporaryDirectory.h>
#include <boost/assign/list_of.hpp>
#include <boost/make_shared.hpp>

using namespace geostore;

namespace
{
    struct Fixture : protected SpatialIndexer
    {
        Fixture()
            : dir( "geostoretest-", temp_directory )
            , db ( dir.Path() / "geostore.sqlite", dir.Path(), proj )
        {}
        void AddLayer( const std::string& name, GeometryType geomType, float top, float left, float bottom, float right )
        {
            std::vector< geometry::Point2d > points = boost::assign::list_of
                ( geometry::Point2d( left, top ) )( geometry::Point2d( left, bottom ) )
                ( geometry::Point2d( right, bottom ) )( geometry::Point2d( right, top ) );
            auto feature = boost::make_shared< TerrainObject >( points );
            std::vector< boost::shared_ptr< TerrainObject > > features;
            features.push_back( feature );
            db.AddLayer( name, geomType, features );
        }
        tools::TemporaryDirectory dir;
        DummyPointProjector proj;
        Database db;
        std::vector< geometry::Polygon2f > result;
    private:
        virtual void GetListWithinCircle( const geometry::Point2f& /*center*/,
            float /*radius*/, std::vector< const kernel::UrbanObject_ABC* >& /*result*/ ) const
        {}
    };

    geometry::Polygon2f MakeRect( float top, float left, float bottom, float right )
    {
        const std::vector< geometry::Point2f > points = boost::assign::list_of
            ( geometry::Point2f( left, top ) )( geometry::Point2f( left, bottom ) )
            ( geometry::Point2f( right, bottom ) )( geometry::Point2f( right, top ) );
        return geometry::Polygon2f( points );
    }
}

BOOST_FIXTURE_TEST_CASE( CreateBlockAutoProcessTest_EmptyRun, Fixture )
{
    CreateBlockAutoProcess process( db.GetTables(), *this, proj, 10 );
    process.Run( MakeRect( 1, 0, 0, 1 ), result );
    BOOST_CHECK_EQUAL( 0u, result.size() );
}

BOOST_FIXTURE_TEST_CASE( CreateBlockAutoProcessTest_WithoutUrbanButWithBuilding, Fixture )
{
    AddLayer( "building", geostore::Polygon, 3, 1, 1, 3 );
    CreateBlockAutoProcess process( db.GetTables(), *this, proj, 10 );
    process.Run( MakeRect( 2, 0, 0, 2 ), result );
    BOOST_CHECK_EQUAL( 1u, result.size() );
}
