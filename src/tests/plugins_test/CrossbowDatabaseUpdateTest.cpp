// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "../dispatcher_test/MockModel.h"
#include "../dispatcher_test/MockAgent.h"
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
#include "dispatcher/EntitySymbols_ABC.h"
#include "tools/SessionConfig.h"
#include "protocol/protocol.h"
#include <gdal/ogr_geometry.h>
#include <boost/lexical_cast.hpp>

using namespace plugins;

namespace
{
    const std::string host = "sword-test.dmz.masagroup.net";
}

//// -----------------------------------------------------------------------------
//// Name: BOOST_AUTO_TEST_CASE
//// Created: JCR 2009-02-10
//// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( crossbow_test_clear_table )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "feature", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        workspace.InitializeConnectionReference( "row", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        BOOST_CHECK_NO_THROW( workspace.GetDatabase( "feature" ).ClearTable( "UnitForces", "session_id=2000" ) );
//        BOOST_CHECK_NO_THROW( workspace.GetDatabase( "row" ).ClearTable( "Formations", "session_id=2000" ) );
//    }
//}
//
//namespace
//{
//    void CreateShapeFeature( crossbow::Workspace_ABC& workspace, int public_id, const crossbow::Shape_ABC& shape, const std::string& tablename )
//    {
//        std::auto_ptr< crossbow::Table_ABC > table( workspace.GetDatabase( "feature" ).OpenTable( tablename ) );
//        crossbow::Row_ABC& row = table->CreateRow();
//        row.SetField( "public_oid", crossbow::FieldVariant( public_id ) );
//        row.SetField( "session_id", crossbow::FieldVariant( 2000 ) );
//        row.SetGeometry( shape );
//        table->InsertRow( row );
//    }
//
//    void CreateFeature( crossbow::Workspace_ABC& workspace, int public_id, int parent_id, const crossbow::Point& point )
//    {
//        std::auto_ptr< crossbow::Table_ABC > table( workspace.GetDatabase( "feature" ).OpenTable( "UnitForces" ) );
//
//        table->BeginTransaction();
//        crossbow::Row_ABC& row = table->CreateRow();
//        row.SetField( "public_oid", crossbow::FieldVariant( public_id ) );
//        row.SetField( "parent_oid", crossbow::FieldVariant( parent_id ) );
//        row.SetField( "name"      , crossbow::FieldVariant( std::string( "test" ) ) );
//        row.SetField( "type"      , crossbow::FieldVariant( 99 /*Police*/ ) );
//        // row.SetField( "direction" , crossbow::FieldVariant( 360. ) );
//        row.SetField( "speed"     , crossbow::FieldVariant( 2 ) );
//        row.SetField( "session_id", crossbow::FieldVariant( 2000 ) );
//        row.SetField( "symbol_id",  crossbow::FieldVariant( std::string( "SFGPUEPOP------" ) ) );
//        row.SetGeometry( point );
//        table->InsertRow( row );
//        table->EndTransaction();
//    }
//
//    void UpdateFeature( crossbow::Workspace_ABC& workspace, int public_id, const crossbow::Point& point )
//    {
//        std::auto_ptr< crossbow::Table_ABC > table( workspace.GetDatabase( "feature" ).OpenTable( "UnitForces" ) );
//
//        table->BeginTransaction();
//        crossbow::Row_ABC* row = table->Find( "public_oid=" + boost::lexical_cast<std::string>( public_id ) + " AND session_id=2000" );
//
//        BOOST_CHECK( row != NULL );
//        // row->SetField( "direction", crossbow::FieldVariant( 180. )  );
//        row->SetField( "speed", crossbow::FieldVariant( 25 )  );
//        row->SetGeometry( point );
//        table->UpdateRow( *row );
//        table->EndTransaction();
//    }
//
//    void CreateRow( crossbow::Workspace_ABC& workspace, int public_id, int parent_id )
//    {
//        std::auto_ptr< crossbow::Table_ABC > table( workspace.GetDatabase( "row" ).OpenTable( "Formations" ) );
//        crossbow::Row_ABC& row = table->CreateRow();
//        row.SetField( "public_oid", crossbow::FieldVariant( public_id ) );
//        row.SetField( "parent_oid", crossbow::FieldVariant( parent_id ) );
//        row.SetField( "name", crossbow::FieldVariant( std::string( "formation test" ) ) );
//        row.SetField( "type", crossbow::FieldVariant( -1 ) );
//        row.SetField( "engaged", crossbow::FieldVariant( 0 ) );
//        row.SetField( "session_id", crossbow::FieldVariant( 2000 ) );
//        row.SetField( "symbol_id",  crossbow::FieldVariant( std::string( "-F---------H---" ) ) );
//        table->InsertRow( row );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( crossbow_test_insert_row_geometry )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "feature", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10045, 34, crossbow::Point( OGRPoint( 49.592909, 0.131836 ) ) ) );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( crossbow_test_insert_multiple_row_geometry )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "feature", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10045, 34, crossbow::Point( OGRPoint( 49.592909, 0.131836 ) ) ) );
//        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10046, 34, crossbow::Point( OGRPoint( 49.59290, 0.13183 ) ) ) );
//        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10047, 34, crossbow::Point( OGRPoint( 49.5929, 0.1318 ) ) ) );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( crossbow_test_insert_geometry )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "feature", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10047, 34, crossbow::Point() ) );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( crossbow_test_insert_data_formation )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "row", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10060, 0 ) );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( crossbow_test_insert_multiple_data_formation )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "row", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10067, 0 ) );
//        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10068, 0 ) );
//        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10069, 0 ) );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( crossbow_test_insert_multiple_data_type )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "feature", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        workspace.InitializeConnectionReference( "row", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10067, 0 ) );
//        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10068, 0 ) );
//        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10069, 34, crossbow::Point( OGRPoint( 49.59290, 0.13183 ) ) ) );
//        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10070, 34, crossbow::Point() ) );
//        BOOST_CHECK_NO_THROW( CreateRow( workspace, 10071, 0 ) );
//        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10074, 34, crossbow::Point() ) );
//        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10076, 34, crossbow::Point() ) );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( crossbow_test_insert_and_update_geometry )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "feature", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        BOOST_CHECK_NO_THROW( workspace.GetDatabase( "feature" ).ClearTable( "UnitForces", "session_id=2000" ) );
//        BOOST_CHECK_NO_THROW( CreateFeature( workspace, 10070, 34, crossbow::Point() ) );
//        BOOST_CHECK_NO_THROW( UpdateFeature( workspace, 10070, crossbow::Point( OGRPoint( 49.59290, 0.13183 ) ) ) );
//    }
//}
//
//namespace
//{
//    MOCK_BASE_CLASS( MockSession, crossbow::WorkingSession_ABC )
//    {
//        MOCK_METHOD( GetId, 0 )
//    };
//
//    MOCK_BASE_CLASS( MockSymbol, dispatcher::EntitySymbols_ABC )
//    {
//        MOCK_METHOD( BuildSymbol, 1 )
//    };
//}
//
//
//BOOST_AUTO_TEST_CASE( crossbow_test_insert_and_update_geometry_thru_updater )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "flat", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//        workspace.InitializeConnectionReference( "geometry", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//
//        MockSession session;
//        MOCK_EXPECT( session, GetId ).returns( 2000 );
//
//        tools::Resolver< dispatcher::Agent_ABC > agents;
//        MockAgent agent( 10071 );
//        MOCK_EXPECT( agent, GetId ).returns( 10071 );
//        MockSymbol* symbol = new MockSymbol();
//        agent.Attach( *static_cast< dispatcher::EntitySymbols_ABC* >( symbol ) );
//        MOCK_EXPECT( *symbol, BuildSymbol ).returns( "**************" );
//        BOOST_CHECK_EQUAL( symbol, &agent.Get< dispatcher::EntitySymbols_ABC >() );
//        agents.Register( agent.GetId(), agent );
//
//        MockModel model;
//        MOCK_EXPECT( model, Agents ).returns( boost::ref( agents ) );
//
//        crossbow::DatabaseUpdater updater( workspace, model, session );
//        {
//            sword::UnitCreation msg;
//            msg.mutable_unit()->set_id( 10071 );
//            msg.mutable_type()->set_id( 99 );
//            msg.set_name( "unit test" );
//            msg.mutable_automat()->set_id( 34 );
//            updater.Update( msg );
//        }
//        {
//            sword::UnitAttributes msg;
//            msg.mutable_unit()->set_id( 10071 );
//            msg.set_speed( 42 );
//            msg.mutable_position()->set_latitude( 49.59290 );
//            msg.mutable_position()->set_longitude( 0.13183 );
//            updater.Update( msg );
//        }
//    }
//}
//
//namespace
//{
//    void UpdateCoordinates( sword::CoordLatLong& coord, double x, double y )
//    {
//        coord.set_longitude( x );
//        coord.set_latitude( y );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( crossbow_test_insert_line_geometry )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "feature", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//
//        MockSession session;
//        MOCK_EXPECT( session, GetId ).returns( 2000 );
//        {
//            sword::CoordLatLongList list;
//            UpdateCoordinates( *list.add_elem(), 33.123, 0.123 );
//            UpdateCoordinates( *list.add_elem(), 32.123, 0.223 );
//            UpdateCoordinates( *list.add_elem(), 31.123, 0.423 );
//            CreateShapeFeature( workspace, 42, crossbow::Line( list ), "objects" );
//        }
//    }
//}
//
//BOOST_AUTO_TEST_CASE( crossbow_test_insert_area_geometry )
//{
//    crossbow::OGR_Workspace workspace;
//    {
//        workspace.InitializeConnectionReference( "feature", "postgres://sword:sword@" + host + ":5432/sword_crossbow_db.sword" );
//
//        MockSession session;
//        MOCK_EXPECT( session, GetId ).returns( 2000 );
//        {
//            sword::CoordLatLongList list;
//            UpdateCoordinates( *list.add_elem(), 33.123, 0.123 );
//            UpdateCoordinates( *list.add_elem(), 32.123, 0.223 );
//            UpdateCoordinates( *list.add_elem(), 31.123, 0.423 );
//            UpdateCoordinates( *list.add_elem(), 33.123, 0.123 );
//            CreateShapeFeature( workspace, 42, crossbow::Area( list ), "objects" );
//        }
//    }
//}
