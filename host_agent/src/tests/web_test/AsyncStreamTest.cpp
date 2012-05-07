// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "web_test.h"

#include <host/Pool.h>
#include <web/AsyncStream.h>

#include <boost/phoenix/bind.hpp>
#include <iostream>

using namespace web;
using host::Pool;

namespace
{
    struct Fixture
    {
        Fixture()
            : pool( 1 )
            , stream( async.Get() )
        {
            // NOTHING
        }
        Pool pool;
        AsyncStream async;
        std::istream& stream;
    };
}

BOOST_FIXTURE_TEST_CASE( async_primitives, Fixture )
{
    const std::string line = "abcd\n";
    async.Write( line.c_str(), line.size() );
    Pool::Future ft = pool.Post( boost::bind( &AsyncStream::Close, &async ) );
    BOOST_CHECK_EQUAL( stream.get(), 'a' );
    BOOST_CHECK_EQUAL( stream.get(), 'b' );
    std::string read;
    std::getline( stream, read );
    BOOST_CHECK_EQUAL( read, "cd" );
    ft.wait();
}
