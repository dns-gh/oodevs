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
#include "dispatcher/Config.h"

#include <iostream>

using namespace plugins;
using namespace plugins::crossbow;

#define CROSSBOW_TEST_ESRI_CONNECTION
#ifdef CROSSBOW_TEST_ESRI_CONNECTION
# undef CROSSBOW_TEST_ESRI_CONNECTION

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_db_connection )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version", 
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--config", "GDAL_DRIVER_PATH", "D:/dev/projects/csword/branches/3.1/run/vc80/",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        dispatcher::Config config;
        workspace.Initialize( "population", config, "sde://sword:sword@sword-test.dmz.masagroup.net:6789/sword_crossbow_db.sword" );
        crossbow::Database_ABC& db = workspace.GetDatabase( "population" );
    }
    std::cout << "crossbow::Worskpace loaded." << std::endl;
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( crossbow_test_db_connection_throw )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version", 
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--config", "GDAL_DRIVER_PATH", "D:/dev/projects/csword/branches/3.1/run/vc80/",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        dispatcher::Config config;
        workspace.Initialize( "population", config, "sde://sword:sword@sword-test.dmz.masagroup.net:6789/sword_crossbow_db.sword" );
        BOOST_CHECK_THROW( 
            workspace.Initialize( "population-shared", config, "sde://sword:sword@sword-test.dmz.masagroup.net:6789/sword_crossbow_db.sword" ), 
            std::runtime_error 
        );
        crossbow::Database_ABC& db = workspace.GetDatabase( "population" );
    }
    std::cout << "crossbow::Worskpace loaded." << std::endl;
}

BOOST_AUTO_TEST_CASE( crossbow_test_db_multiple_connection )
{
    int gdal_argc = 0;
    char *gdal_argv[] = { "--version", 
                          "--config", "ARCGISHOME", "C:/Program Files/ArcGIS/Bin",
                          "--config", "GDAL_DRIVER_PATH", "D:/dev/projects/csword/branches/3.1/run/vc80/",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        dispatcher::Config config;
        workspace.Initialize( "population", config, "sde://sword:sword@sword-test.dmz.masagroup.net:6789/sword_crossbow_db.sword" );
        crossbow::Database_ABC& db = workspace.GetDatabase( "population" );
        crossbow::Database_ABC& db1 = workspace.GetDatabase( "population" );
        crossbow::Database_ABC& db2 = workspace.GetDatabase( "population" );
        crossbow::Database_ABC& db3 = workspace.GetDatabase( "population" );
        BOOST_CHECK( &db1 == &db2 && &db == &db1 && &db2 == &db3 );
    }
    std::cout << "crossbow::Worskpace loaded." << std::endl;
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
                          "--config", "GDAL_DRIVER_PATH", "D:/dev/projects/csword/branches/3.1/run/vc80/",
                          "--debug", "on" };
    crossbow::OGR_Workspace workspace( gdal_argc, gdal_argv );
    {
        dispatcher::Config config;
        workspace.Initialize( "population", config, "sde://sword:sword@sword-test.dmz.masagroup.net:6789/sword_crossbow_db.sword" );
        crossbow::Database_ABC& db = workspace.GetDatabase( "population" );
        std::auto_ptr< crossbow::Table_ABC > table( db.OpenTable( "unitforces" ) );
        BOOST_CHECK( table.get() != 0 );
    }
    std::cout << "crossbow::Worskpace loaded." << std::endl;
}


#endif

