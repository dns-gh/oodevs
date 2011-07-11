// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "crossbow_plugin/OGR_Workspace.h"
#include "crossbow_plugin/DatabaseFactory.h"
#include "crossbow_plugin/Database_ABC.h"
#include "crossbow_plugin/Table_ABC.h"

using namespace plugins;

namespace
{
    const std::string host = "sword-test.dmz.masagroup.net";
}

BOOST_AUTO_TEST_CASE( crossbow_test_db_connection )
{
    crossbow::OGR_Workspace workspace;
    {
        workspace.InitializeConnectionReference( "sword", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
        BOOST_CHECK_NO_THROW( workspace.GetDatabase( "sword" ) );
    }
}

BOOST_AUTO_TEST_CASE( crossbow_test_db_connection_multiple_name_reference )
{
    crossbow::OGR_Workspace workspace;
    {
        workspace.InitializeConnectionReference( "ref1", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
        workspace.InitializeConnectionReference( "ref2", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
        crossbow::Database_ABC& rhs = workspace.GetDatabase( "ref1" );
        crossbow::Database_ABC& lhs = workspace.GetDatabase( "ref2" );
        BOOST_CHECK_EQUAL( &lhs, &rhs );
    }
}

BOOST_AUTO_TEST_CASE( crossbow_test_db_unitialized_connection_throw )
{
    crossbow::OGR_Workspace workspace;
    {
        BOOST_CHECK_THROW( workspace.GetDatabase( "population" ), std::runtime_error );
        BOOST_CHECK_THROW( workspace.GetDatabase( "population-shared" ), std::runtime_error );
    }
}

BOOST_AUTO_TEST_CASE( crossbow_test_db_multiple_connection )
{
    crossbow::OGR_Workspace workspace;
    {
        workspace.InitializeConnectionReference( "sword", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
        crossbow::Database_ABC& db = workspace.GetDatabase( "sword" );
        crossbow::Database_ABC& db1 = workspace.GetDatabase( "sword" );
        crossbow::Database_ABC& db2 = workspace.GetDatabase( "sword" );
        crossbow::Database_ABC& db3 = workspace.GetDatabase( "sword" );
        BOOST_CHECK_EQUAL( &db, &db1 );
        BOOST_CHECK_EQUAL( &db1, &db2 );
        BOOST_CHECK_EQUAL( &db2, &db3 );
    }
}

#define CHECK_TABLE( N ) \
    BOOST_CHECK( std::auto_ptr< crossbow::Table_ABC >( db.OpenTable( N ) ).get() );

BOOST_AUTO_TEST_CASE( crossbow_test_db_connection_check_tables )
{
    crossbow::OGR_Workspace workspace;
    {
        workspace.InitializeConnectionReference( "sword", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
        {
            crossbow::Database_ABC& db = workspace.GetDatabase( "sword" );
            CHECK_TABLE( "create_order" );
            CHECK_TABLE( "create_object" );
            CHECK_TABLE( "unitforces" );
            CHECK_TABLE( "objects" );
            CHECK_TABLE( "objectparameters" );
            CHECK_TABLE( "knowledgeobjects" );
            CHECK_TABLE( "knowledgeunits" );
            CHECK_TABLE( "tacticallines" );
            CHECK_TABLE( "formations" );
            CHECK_TABLE( "reports" );
            CHECK_TABLE( "exercises" );
            CHECK_TABLE( "sessions" );
            CHECK_TABLE( "boundarylimits" );
            CHECK_TABLE( "simulationproperties" );
            CHECK_TABLE( "simulationclock" );
        }
    }
}
