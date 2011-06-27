// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "dispatcher_test_pch.h"
#include "dispatcher/RotatingLog.h"
#include "dispatcher/Log_ABC.h"
#include "dispatcher/LogFactory_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <ostream>
#include <memory>
#include <deque>

using namespace dispatcher;

namespace
{
    MOCK_BASE_CLASS( MockLog, dispatcher::Log_ABC )
    {
        MOCK_DESTRUCTOR( MockLog, destructor )
        MOCK_METHOD( Write, 1 )
    };

    MOCK_BASE_CLASS( MockLogFactory, dispatcher::LogFactory_ABC )
    {
        MOCK_METHOD( CreateLog, 1 )
    };
}

BOOST_AUTO_TEST_CASE( max_files_set_to_zero_disables_log )
{
    MockLogFactory factory;
    RotatingLog log( factory, "filename", 0, 42 );
    log.Write( "some text" );
}

BOOST_AUTO_TEST_CASE( rotating_log_first_log_is_sent_to_log )
{
    MockLogFactory factory;
    MockLog* pLog = new MockLog();
    RotatingLog log( factory, "filename", 12, 42 );
    MOCK_EXPECT( factory, CreateLog ).once().returns( pLog );
    MOCK_EXPECT( pLog, Write ).once().with( "some text" );
    log.Write( "some text" );
    MOCK_EXPECT( pLog, destructor );
}

BOOST_AUTO_TEST_CASE( rotating_log_switches_to_next_log_when_max_entries_is_reached )
{
    MockLogFactory factory;
    MockLog* pLog = new MockLog();
    const unsigned int size = 3;
    RotatingLog log( factory, "filename", 2, size );
    MOCK_EXPECT( factory, CreateLog ).once().with( "filename" ).returns( pLog );
    MOCK_EXPECT( pLog, Write ).exactly( size ).with( "some text" );
    log.Write( "some text" );
    log.Write( "some text" );
    log.Write( "some text" );
    mock::verify();
    mock::sequence s;
    MOCK_EXPECT( pLog, destructor ).in( s );
    pLog = new MockLog();
    MOCK_EXPECT( factory, CreateLog ).in( s ).with( "filename.2" ).once().returns( pLog );
    MOCK_EXPECT( pLog, Write ).once().with( "some text" );
    log.Write( "some text" );
    MOCK_EXPECT( pLog, destructor );
}

BOOST_AUTO_TEST_CASE( rotating_log_goes_back_to_the_first_one_when_max_files_is_reached )
{
    MockLogFactory factory;
    MockLog* pLog = new MockLog();
    const unsigned int size = 3;
    RotatingLog log( factory, "filename", 2, size );
    MOCK_EXPECT( factory, CreateLog ).once().with( "filename" ).returns( pLog );
    MOCK_EXPECT( pLog, Write ).exactly( size ).with( "some text" );
    log.Write( "some text" );
    log.Write( "some text" );
    log.Write( "some text" );
    mock::verify();
    MOCK_EXPECT( pLog, destructor );
    pLog = new MockLog();
    MOCK_EXPECT( factory, CreateLog ).once().with( "filename.2" ).returns( pLog );
    MOCK_EXPECT( pLog, Write ).exactly( size ).with( "some text" );
    log.Write( "some text" );
    log.Write( "some text" );
    log.Write( "some text" );
    mock::verify();
    MOCK_EXPECT( pLog, destructor );
    pLog = new MockLog();
    MOCK_EXPECT( factory, CreateLog ).once().with( "filename" ).returns( pLog );
    MOCK_EXPECT( pLog, Write ).once().with( "some text" );
    log.Write( "some text" );
    MOCK_EXPECT( pLog, destructor );
}
