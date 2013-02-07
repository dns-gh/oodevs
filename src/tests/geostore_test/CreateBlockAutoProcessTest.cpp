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

}  // namespace

BOOST_AUTO_TEST_CASE( CreateBlockAutoProcessTest_EmptyRun )
{
    tools::TemporaryDirectory dir( "geostoretest-", temp_directory );
    boost::filesystem::create_directories( dir.path() / "Graphics" ); 

    DummyPointProjector proj;
    DummySpatialIndexer index;
    Database db( dir.path(), proj );

    std::vector< geometry::Point2f > points;
    points.push_back( geometry::Point2f( 0, 0 ));
    points.push_back( geometry::Point2f( 1, 0 ));
    points.push_back( geometry::Point2f( 1, 1 ));
    points.push_back( geometry::Point2f( 0, 1 ));
    geometry::Polygon2f area( points );
    std::vector< geometry::Polygon2f > result;
    CreateBlockAutoProcess process( db.GetTables(), index, proj, 10 );
    process.Run( area, result );
    BOOST_CHECK_EQUAL( 0u, result.size() );
}

