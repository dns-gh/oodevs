// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "web_test.h"

#include <web/AsyncStream.h>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

using namespace web;

namespace
{
    struct Fixture
    {
        Fixture()
        {
            // NOTHING
        }
        AsyncStream async;
    };

    void ReadLine( std::string& dst, std::istream& src )
    {
        std::getline( src, dst );
    }

    void PeekGetLoop( std::istream& src, const std::string& line )
    {
        BOOST_FOREACH( char c, line )
        {
            BOOST_CHECK_EQUAL( src.peek(), c );
            BOOST_CHECK_EQUAL( src.get(),  c );
        }
    }
}

BOOST_FIXTURE_TEST_CASE( empty_async_joins, Fixture )
{
}

BOOST_FIXTURE_TEST_CASE( async_writes_and_joins, Fixture )
{
    char tmp[256];
    memset( tmp, 0, sizeof tmp );
    async.Write( tmp, sizeof tmp );
}

BOOST_FIXTURE_TEST_CASE( async_writes_closes_and_joins, Fixture )
{
    char tmp[256];
    memset( tmp, 0, sizeof tmp );
    async.Write( tmp, sizeof tmp );
    async.CloseWrite();
}

BOOST_FIXTURE_TEST_CASE( async_write_read, Fixture )
{
    const std::string line = "abcd\n";
    async.Write( line.data(), line.size() );
    async.CloseWrite();
    std::string reply;
    async.Read( boost::bind( &ReadLine, boost::ref( reply ), _1 ) );
    BOOST_CHECK_EQUAL( reply + "\n", line );
}

BOOST_FIXTURE_TEST_CASE( async_write_read_incomplete, Fixture )
{
    const std::string line = "abcd\n";
    async.Write( line.data(), line.size() );
    async.Write( line.data(), line.size() );
    async.CloseWrite();
    std::string reply;
    async.Read( boost::bind( &ReadLine, boost::ref( reply ), _1 ) );
    BOOST_CHECK_EQUAL( reply + "\n", line );
}

BOOST_FIXTURE_TEST_CASE( async_write_peek_get, Fixture )
{
    const std::string line = "abcd\n";
    async.Write( line.data(), line.size() );
    async.CloseWrite();
    async.Read( boost::bind( &PeekGetLoop, _1, line ) );
}
