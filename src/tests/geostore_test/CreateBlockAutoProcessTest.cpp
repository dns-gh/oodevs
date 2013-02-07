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
#include "geostore/GeoTable.h"
#include "geostore/SpatialIndexer.h"
#include "geostore/CreateBlockAutoProcess.h"
#include <terrain/DummyPointProjector.h>
#include <terrain/TerrainObject.h>
#include <tools/TemporaryDirectory.h>
#include <boost/filesystem.hpp>

using namespace geostore;

namespace
{

class DummySpatialIndexer : public SpatialIndexer
{
public:
    virtual void GetListWithinCircle( const geometry::Point2f& /*center*/,
        float /*radius*/, std::vector< const kernel::UrbanObject_ABC* >& /*result*/ ) const
    {
    }
};

typedef std::vector< geometry::Point2f > T_Points;
T_Points MakeRect( float top, float left, float bottom, float right )
{
    T_Points points;
    points.push_back( geometry::Point2f( left, top ));
    points.push_back( geometry::Point2f( left, bottom ));
    points.push_back( geometry::Point2f( right, bottom ));
    points.push_back( geometry::Point2f( right, top ));
    return points;
}

void AddLayer( const std::string& name, int geomType, float top, float left,
        float bottom, float right, Database& db )
{
    std::vector< geometry::Point2d > points;
    points.push_back( geometry::Point2d( left, top ));
    points.push_back( geometry::Point2d( left, bottom ));
    points.push_back( geometry::Point2d( right, bottom ));
    points.push_back( geometry::Point2d( right, top ));
    TerrainObject feature( points );

    std::vector< TerrainObject* > features;
    features.push_back( &feature );
    db.AddLayer( name, geomType, features );
}

}  // namespace

BOOST_AUTO_TEST_CASE( CreateBlockAutoProcessTest_EmptyRun )
{
    tools::TemporaryDirectory dir( "geostoretest-", temp_directory );

    DummyPointProjector proj;
    DummySpatialIndexer index;
    Database db( dir.path() / "geostore.sqlite", dir.path(), proj );

    geometry::Polygon2f area( MakeRect( 1, 0, 0, 1 ));
    std::vector< geometry::Polygon2f > result;
    CreateBlockAutoProcess process( db.GetTables(), index, proj, 10 );
    process.Run( area, result );
    BOOST_CHECK_EQUAL( 0u, result.size() );
}

BOOST_AUTO_TEST_CASE( CreateBlockAutoProcessTest_NoUrban )
{
    // Test a generation with no urban layer but a building one, this
    // used to crash.
    tools::TemporaryDirectory dir( "geostoretest-", temp_directory );

    DummyPointProjector proj;
    DummySpatialIndexer index;
    Database db( dir.path() / "geostore.sqlite", dir.path(), proj );
    AddLayer( "building", 1, 3, 1, 1, 3, db );

    geometry::Polygon2f area( MakeRect( 2, 0, 0, 2 ));
    std::vector< geometry::Polygon2f > result;
    CreateBlockAutoProcess process( db.GetTables(), index, proj, 10 );
    process.Run( area, result );
    BOOST_CHECK_EQUAL( 1u, result.size() );
}

