// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/RotatingLog.h"
#include "tools/Log_ABC.h"
#include "tools/LogFactory_ABC.h"

using namespace tools;

namespace
{
    MOCK_BASE_CLASS( MockLog, tools::Log_ABC )
    {
        MOCK_DESTRUCTOR( MockLog, destructor )
        MOCK_METHOD( Write, 1 )
    };

    MOCK_BASE_CLASS( MockLogFactory, tools::LogFactory_ABC )
    {
        MOCK_METHOD( CreateLog, 2 )
    };
}

BOOST_AUTO_TEST_CASE( max_files_set_to_zero_disables_log )
{
    MockLogFactory factory;
    RotatingLog log( factory, "filename.log", 0, 42 );
    log.Write( "some text" );
}

BOOST_AUTO_TEST_CASE( max_entries_set_to_zero_disables_rotation )
{
    MockLogFactory factory;
    MockLog* pLog = new MockLog();
    RotatingLog log( factory, "filename.log", 1, 0 );
    MOCK_EXPECT( factory.CreateLog ).once().returns( pLog );
    MOCK_EXPECT( pLog->Write ).exactly( 3 ).with( "some text" ).returns( 0 );
    log.Write( "some text" );
    log.Write( "some text" );
    log.Write( "some text" );
    MOCK_EXPECT( pLog->destructor );
}

BOOST_AUTO_TEST_CASE( rotating_log_first_log_is_sent_to_log )
{
    MockLogFactory factory;
    MockLog* pLog = new MockLog();
    RotatingLog log( factory, "filename.log", 12, 42 );
    MOCK_EXPECT( factory.CreateLog ).once().returns( pLog );
    MOCK_EXPECT( pLog->Write ).once().with( "some text" ).returns( 0 );
    log.Write( "some text" );
    MOCK_EXPECT( pLog->destructor );
}

BOOST_AUTO_TEST_CASE( rotating_log_switches_to_next_log_when_max_entries_is_reached )
{
    MockLogFactory factory;
    MockLog* pLog = new MockLog();
    const unsigned int size = 3;
    RotatingLog log( factory, "filename.log", 2, size );
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.log", mock::any ).returns( pLog );
    MOCK_EXPECT( pLog->Write ).once().with( "some text" ).returns( size );
    mock::sequence s;
    MOCK_EXPECT( pLog->destructor ).in( s );
    pLog = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).in( s ).with( "filename.1.log", mock::any ).once().returns( pLog );
    MOCK_EXPECT( pLog->Write ).once().with( "some text" ).returns( 0 );
    log.Write( "some text" );
    log.Write( "some text" );
    MOCK_EXPECT( pLog->destructor );
}

BOOST_AUTO_TEST_CASE( rotating_log_goes_back_to_the_first_one_when_max_files_is_reached )
{
    MockLogFactory factory;
    MockLog* pLog = new MockLog();
    const unsigned int size = 3;
    RotatingLog log( factory, "filename.log", 2, size );
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.log", mock::any ).returns( pLog );
    MOCK_EXPECT( pLog->Write ).once().with( "some text" ).returns( size );
    MOCK_EXPECT( pLog->destructor );
    pLog = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.1.log", mock::any ).returns( pLog );
    MOCK_EXPECT( pLog->Write ).once().with( "some text" ).returns( size );
    MOCK_EXPECT( pLog->destructor );
    pLog = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.log", mock::any ).returns( pLog );
    MOCK_EXPECT( pLog->Write ).once().with( "some text" ).returns( 0 );
    log.Write( "some text" );
    log.Write( "some text" );
    log.Write( "some text" );
    MOCK_EXPECT( pLog->destructor );
}

namespace
{
    struct Fixture
    {
        Fixture()
        {
            std::ofstream( "filename.log" );
        }
        ~Fixture()
        {
            std::remove( "filename.log" );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( rotating_log_overwrites_the_oldest_log, Fixture )
{
    MockLogFactory factory;
    MockLog* pLog = new MockLog();
    RotatingLog log( factory, "filename.log", 2, 42 );
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.1.log", mock::any ).returns( pLog );
    MOCK_EXPECT( pLog->Write ).once().with( "some text" ).returns( 0 );
    log.Write( "some text" );
    MOCK_EXPECT( pLog->destructor );
}
