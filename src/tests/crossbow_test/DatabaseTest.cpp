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
#include "tools/ExerciseConfig.h"

#include <iostream>

using namespace plugins;
using namespace plugins::crossbow;

#define CROSSBOW_TEST_ESRI_CONNECTION
#ifdef CROSSBOW_TEST_ESRI_CONNECTION
# undef CROSSBOW_TEST_ESRI_CONNECTION

namespace
{
    std::string GetHost()
    {
        static std::string host = "localhost";
        return host;
    }
}


// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_db_connection )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "population", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );
        BOOST_CHECK_NO_THROW( workspace.GetDatabase( "population" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_db_connection_with_no_parameters )
{
    crossbow::OGR_Workspace workspace;
    {
        workspace.InitializeConnectionReference( "population", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );
        BOOST_CHECK_NO_THROW( workspace.GetDatabase( "population" ) );
    }
}


// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_db_connection_multiple_name_reference )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "population", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );
        workspace.InitializeConnectionReference( "population-shared", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        crossbow::Database_ABC& rhs = workspace.GetDatabase( "population" );
        crossbow::Database_ABC& lhs = workspace.GetDatabase( "population-shared" );
        BOOST_CHECK_EQUAL( &lhs, &rhs );
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_db_unitialized_connection_throw )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        BOOST_CHECK_THROW( workspace.GetDatabase( "population" ), std::runtime_error );
        BOOST_CHECK_THROW( workspace.GetDatabase( "population-shared" ), std::runtime_error );
    }
}

BOOST_AUTO_TEST_CASE( crossbow_test_db_multiple_connection )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "population", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        crossbow::Database_ABC& db = workspace.GetDatabase( "population" );
        crossbow::Database_ABC& db1 = workspace.GetDatabase( "population" );
        crossbow::Database_ABC& db2 = workspace.GetDatabase( "population" );
        crossbow::Database_ABC& db3 = workspace.GetDatabase( "population" );
        BOOST_CHECK_EQUAL( &db, &db1 );
        BOOST_CHECK_EQUAL( &db1, &db2 );
        BOOST_CHECK_EQUAL( &db2, &db3 );
    }
}


// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_db_connection_and_open_table )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "population", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );

        crossbow::Database_ABC& db = workspace.GetDatabase( "population" );
        std::auto_ptr< crossbow::Table_ABC > table( db.OpenTable( "unitforces" ) );
        BOOST_CHECK( table.get() != 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_db_connection_release_and_open_table )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "population", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );
        {
            crossbow::Database_ABC& db = workspace.GetDatabase( "population" );
            std::auto_ptr< crossbow::Table_ABC > table( db.OpenTable( "unitforces" ) );
            BOOST_CHECK( table.get() != 0 );
        }
        workspace.Release( "population" );
        {
            crossbow::Database_ABC& db = workspace.GetDatabase( "population" );
            std::auto_ptr< crossbow::Table_ABC > table( db.OpenTable( "unitforces" ) );
            BOOST_CHECK( table.get() != 0 );
        }
    }
}

#define CHECK_TABLE( N ) \
{ \
    std::auto_ptr< crossbow::Table_ABC > table( db.OpenTable( N ) ); \
    BOOST_CHECK( table.get() != 0 ); \
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_db_connection_check_tables )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version",
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin"
                          "--debug", "on" };

    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        workspace.InitializeConnectionReference( "geometry", "sde://sword:sword@" + GetHost() + ":6789/sword_crossbow_db.sword" );
        workspace.InitializeConnectionReference( "flat", "postgres://sword:sword@" + GetHost() + ":5432/sword_crossbow_db.sword" );
        {
            crossbow::Database_ABC& db = workspace.GetDatabase( "geometry" );
            CHECK_TABLE( "unitforces" );
            CHECK_TABLE( "orderparameters_point" );
            CHECK_TABLE( "orderparameters_line" );
            CHECK_TABLE( "orderparameters_area" );
            CHECK_TABLE( "tacticallines" );
            CHECK_TABLE( "tacticalobject_point" );
            CHECK_TABLE( "tacticalobject_line" );
            CHECK_TABLE( "tacticalobject_area" );
            CHECK_TABLE( "knowledgeunits" );
            CHECK_TABLE( "knowledgeobjects_point" );
            CHECK_TABLE( "knowledgeobjects_line" );
            CHECK_TABLE( "knowledgeobjects_area" );
        }

        {
            crossbow::Database_ABC& db = workspace.GetDatabase( "flat" );
            CHECK_TABLE( "formations" );
            CHECK_TABLE( "orders" );
            CHECK_TABLE( "orderparameters" );
            CHECK_TABLE( "reports" );
            CHECK_TABLE( "exercises" );
            CHECK_TABLE( "sessions" );
            CHECK_TABLE( "knowledgeobjects" );
        }
    }
}

#endif
