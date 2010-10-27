// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "crossbow_test_pch.h"
#include "crossbow_plugin/OGR_Workspace.h"
#include "crossbow_plugin/DatabaseFactory.h"
#include "crossbow_plugin/Database_ABC.h"
#include "crossbow_plugin/Table_ABC.h"
#include "crossbow_plugin/Row_ABC.h"
#include "crossbow_plugin/Point.h"
#include "crossbow_plugin/Area.h"
#include "crossbow_plugin/Line.h"
#include "crossbow_plugin/WorkingSession.h"
#include "crossbow_plugin/DatabaseUpdater.h"
#include "dispatcher/Model.h"
#include "tools/SessionConfig.h"

#include <gdal/ogr_geometry.h>
#include <iostream>
#include <boost/lexical_cast.hpp>

#pragma warning( push, 0 )
#pragma warning( disable: 4505 )
#include <turtle/mock.hpp>
#pragma warning( pop )

using namespace plugins;
using namespace plugins::crossbow;

namespace
{
    std::string GetHost()
    {
        static std::string host = "localhost";
        return host;
    }
}

#define CROSSBOW_TEST_ESRI_CONNECTION
#ifdef CROSSBOW_TEST_ESRI_CONNECTION
# undef CROSSBOW_TEST_ESRI_CONNECTION

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_clear_table )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "feature", "sde://sword:sword@" + GetHost() + ":5432/sword_crossbow_db.sword" );
        workspace.InitializeConnectionReference( "row", "postgres://sword:sword@" + GetHost() + ":5432/sword_crossbow_db.sword" );
        BOOST_CHECK_NO_THROW( workspace.GetDatabase( "feature" ).ClearTable( "UnitForces", "session_id=2000" ) );
        BOOST_CHECK_NO_THROW( workspace.GetDatabase( "row" ).ClearTable( "Formations", "session_id=2000" ) );
    }
}

namespace
{
    void CreateShapeFeature( Workspace_ABC& workspace, int public_id, const Shape_ABC& shape, const std::string& tablename )
    {
        std::auto_ptr< Table_ABC > table( workspace.GetDatabase( "feature" ).OpenTable( tablename ) );
        Row_ABC& row = table->CreateRow();
        row.SetField( "public_oid", FieldVariant( public_id ) );
        row.SetField( "session_id", FieldVariant( 2000 ) );
        row.SetGeometry( shape );
        table->InsertRow( row );
    }

    void CreateFeature( Workspace_ABC& workspace, int public_id, int parent_id, const Point& point )
    {
        std::auto_ptr< Table_ABC > table( workspace.GetDatabase( "feature" ).OpenTable( "UnitForces" ) );
        
        table->BeginTransaction();
        Row_ABC& row = table->CreateRow();
        row.SetField( "public_oid", FieldVariant( public_id ) );
        row.SetField( "parent_oid", FieldVariant( parent_id ) );
        row.SetField( "name"      , FieldVariant( std::string( "test" ) ) );
        row.SetField( "type"      , FieldVariant( 99 /*Police*/ ) );
        // row.SetField( "direction" , FieldVariant( 360. ) );
        row.SetField( "speed"     , FieldVariant( 2 ) );
        row.SetField( "session_id", FieldVariant( 2000 ) );
        row.SetField( "symbol_id",  FieldVariant( std::string( "SFGPUEPOP------" ) ) );
        row.SetGeometry( point );
        table->InsertRow( row );
        table->EndTransaction();
    }

    void UpdateFeature( Workspace_ABC& workspace, int public_id, const Point& point )
    {
        std::auto_ptr< Table_ABC > table( workspace.GetDatabase( "feature" ).OpenTable( "UnitForces" ) );
        
        table->BeginTransaction();
        Row_ABC* row = table->Find( "public_oid=" + boost::lexical_cast<std::string>( public_id ) + " AND session_id=2000" );

        BOOST_CHECK( row != NULL );
        // row->SetField( "direction", FieldVariant( 180. )  );
        row->SetField( "speed", FieldVariant( 25 )  );
        row->SetGeometry( point );
        table->UpdateRow( *row );
        table->EndTransaction();
    }

    void CreateRow( Workspace_ABC& workspace, int public_id, int parent_id )
    {
        std::auto_ptr< Table_ABC > table( workspace.GetDatabase( "row" ).OpenTable( "Formations" ) );
        Row_ABC& row = table->CreateRow();
        row.SetField( "public_oid", FieldVariant( public_id ) );
        row.SetField( "parent_oid", FieldVariant( parent_id ) );
        row.SetField( "name", FieldVariant( std::string( "formation test" ) ) );
        row.SetField( "type", FieldVariant( -1 ) );
        row.SetField( "engaged", FieldVariant( 0 ) );
        row.SetField( "session_id", FieldVariant( 2000 ) );
        row.SetField( "symbol_id",  FieldVariant( std::string( "-F---------H---" ) ) );
        table->InsertRow( row );
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_insert_row_geometry )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "feature", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10045, 34, Point( OGRPoint( 49.592909, 0.131836 ) ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_insert_multiple_row_geometry )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "feature", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10045, 34, Point( OGRPoint( 49.592909, 0.131836 ) ) ) );
        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10046, 34, Point( OGRPoint( 49.59290, 0.13183 ) ) ) );
        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10047, 34, Point( OGRPoint( 49.5929, 0.1318 ) ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_insert_geometry )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "feature", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );
        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10047, 34, Point() ) );
    }
}


// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_insert_data_formation )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "row", "postgres://sword:sword@" + GetHost() + ":5432/sword_crossbow_db.sword" );

        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10060, 0 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_insert_multiple_data_formation )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--config", "GDAL_DRIVER_PATH", "D:/dev/projects/csword/branches/3.1/run/vc80/",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "row", "postgres://sword:sword@" + GetHost() + ":5432/sword_crossbow_db.sword" );

        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10067, 0 ) );
        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10068, 0 ) );
        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10069, 0 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_insert_multiple_data_type )
{
    int gdal_argc = 1;
    char *gdal_argv[] = { "--quiet",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--config", "GDAL_DRIVER_PATH", "D:/dev/projects/csword/branches/3.1/run/vc80/",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "feature", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );
        workspace.InitializeConnectionReference( "row", "postgres://sword:sword@" + GetHost() + ":5432/sword_crossbow_db.sword" );

        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10067, 0 ) );
        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10068, 0 ) );
        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10069, 34, Point( OGRPoint( 49.59290, 0.13183 ) ) ) );
        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10070, 34, Point() ) );
        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10071, 0 ) );
        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10074, 34, Point() ) );
        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10076, 34, Point() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_insert_and_update_geometry )
{
    int gdal_argc = 2;
    char *gdal_argv[] = { "--debug", "on"
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--config", "GDAL_DRIVER_PATH", "D:/dev/projects/csword/branches/3.1/run/vc80/",
                          "--version" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "feature", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        BOOST_CHECK_NO_THROW( workspace.GetDatabase( "feature" ).ClearTable( "UnitForces", "session_id=2000" ) );

        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10070, 34, Point() ) );
        BOOST_CHECK_NO_THROW( UpdateFeature( workspace, 10070, Point( OGRPoint( 49.59290, 0.13183 ) ) ) );
    }
}

namespace
{
    class MockConfig : public tools::SessionConfig
    {
    public:
        MockConfig()
        {
            int argc = 3;
            char *argv[] = { "--root=C:/Documents and Settings/jcr/Mes documents/SWORD",
                             "--root-dir=C:/Documents and Settings/jcr/Mes documents/SWORD",
                             "--exercise=lehavre" };
            Parse( argc, argv );
        }

        virtual std::string GetSessionDir() const
        {
            return tools::ExerciseConfig::GetSessionDir( "default" );
        }
    };

    MOCK_BASE_CLASS( MockSession, crossbow::WorkingSession_ABC )
    {
        MOCK_METHOD( GetId, 0 )
    };

}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_insert_and_update_geometry_thru_updater )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        MockConfig config;

        workspace.InitializeConnectionReference( "flat", "postgres://sword:sword@" + GetHost() + ":5432/sword_crossbow_db.sword" );
        workspace.InitializeConnectionReference( "geometry", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        dispatcher::Model model( config );
        MockSession session;
        MOCK_EXPECT( session, GetId ).returns( 2000 );

        crossbow::DatabaseUpdater updater( workspace, model, session );

        {
            std::string name( "test-sde" );
            ASN1T_MsgUnitCreation msg;
            msg.oid = 10071;
            msg.type_pion = 99;
            msg.nom = (ASN1VisibleString)name.c_str();
            msg.oid_automate = 34;
            updater.Update( msg );
        }
        {
            ASN1T_MsgUnitAttributes msg;
            msg.oid = 10071;
            msg.m.vitessePresent = 1;
            msg.vitesse = 42;
            msg.m.positionPresent = 1;
            msg.position.latitude = 49.59290;
            msg.position.longitude = 0.13183;
            updater.Update( msg );
        }
    }
}

namespace
{
    void UpdateCoordinates( ASN1T_CoordLatLong& coord, double x, double y )
    {
        coord.longitude = x;
        coord.latitude = y;
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_insert_line_geometry )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        MockConfig config;

        workspace.InitializeConnectionReference( "feature", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        MockSession session;
        MOCK_EXPECT( session, GetId ).returns( 2000 );
        {
            std::vector< ASN1T_CoordLatLong > coordinates ( 3 );
            UpdateCoordinates( coordinates[ 0 ], 33.123, 0.123 );
            UpdateCoordinates( coordinates[ 1 ], 32.123, 0.223 );
            UpdateCoordinates( coordinates[ 2 ], 31.123, 0.423 );
            ASN1T_CoordLatLongList asnCoordinates;
            asnCoordinates.n = coordinates.size();
            asnCoordinates.elem = &(*coordinates.begin());
            CreateShapeFeature( workspace, 42, crossbow::Line( asnCoordinates ), "tacticalobject_line_test" );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_insert_area_geometry )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        MockConfig config;

        workspace.InitializeConnectionReference( "feature", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        MockSession session;
        MOCK_EXPECT( session, GetId ).returns( 2000 );
        {
            std::vector< ASN1T_CoordLatLong > coordinates ( 4 );
            UpdateCoordinates( coordinates[ 0 ], 33.123, 0.123 );
            UpdateCoordinates( coordinates[ 1 ], 32.123, 0.223 );
            UpdateCoordinates( coordinates[ 2 ], 31.123, 0.423 );
            UpdateCoordinates( coordinates[ 3 ], 33.123, 0.123 );
            ASN1T_CoordLatLongList asnCoordinates;
            asnCoordinates.n = coordinates.size();
            asnCoordinates.elem = &(*coordinates.begin());
            CreateShapeFeature( workspace, 42, crossbow::Area( asnCoordinates ), "tacticalobject_area_test" );
        }
    }
}

#endif