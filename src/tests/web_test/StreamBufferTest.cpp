// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "web_test.h"

#include "web/StreamBuffer.h"

using namespace web;

namespace
{
    struct Fixture
    {
        Fixture()
            : src   ( stream )
            , buffer( src )
        {
            // NOTHING
        }

        std::stringstream stream;
        StreamReader src;
        StreamBuffer buffer;
    };
}

BOOST_FIXTURE_TEST_CASE( buffer_get_line, Fixture )
{
    const std::string line = "some data";
    stream << line << "\n";
    const std::string value = buffer.GetLine();
    BOOST_CHECK_EQUAL( value, line );
    BOOST_CHECK( buffer.Eof() );
}

BOOST_FIXTURE_TEST_CASE( buffer_read_data, Fixture )
{
    const std::string line = "some data\n";
    stream << line;
    char dst[256];
    const size_t read = buffer.Read( dst, 4 );
    BOOST_CHECK_EQUAL( read, size_t( 4 ) );
    const size_t next = buffer.Read( dst + read, sizeof dst - read );
    BOOST_CHECK_EQUAL( read + next, line.size() );
    BOOST_CHECK_EQUAL( std::string( dst, read + next ), line );
    BOOST_CHECK( buffer.Eof() );
}

BOOST_FIXTURE_TEST_CASE( buffer_skip_peek_get, Fixture )
{
    const std::string prefix = "abcd";
    stream << prefix << "\n";
    BOOST_CHECK_EQUAL( buffer.Peek(), 'a' );
    char* dst;
    const size_t size = buffer.Peek( &dst );
    BOOST_CHECK_EQUAL( size, prefix.size() + 1 );
    buffer.Skip( 1 );
    BOOST_CHECK_EQUAL( buffer.Peek(), 'b' );
    BOOST_CHECK_EQUAL( buffer.Peek(), 'b' );
    BOOST_CHECK_EQUAL( buffer.Get(),  'b' );
    buffer.Skip( 1 );
    BOOST_CHECK_EQUAL( buffer.Get(),  'd' );
    buffer.Skip( 1 );
    BOOST_CHECK( buffer.Eof() );
}

BOOST_FIXTURE_TEST_CASE( buffer_get_line_after_skip, Fixture )
{
    const std::string prefix = "abcd";
    stream << prefix << "\n";
    buffer.Skip( 2 );
    BOOST_CHECK_EQUAL( buffer.GetLine(), "cd" );
    BOOST_CHECK( buffer.Eof() );
}

BOOST_FIXTURE_TEST_CASE( buffer_read_after_skip, Fixture )
{
    const std::string prefix = "abcd";
    stream << prefix << "\n";
    buffer.Skip( 2 );
    char dst[256];
    const size_t read = buffer.Read( dst, sizeof dst );
    BOOST_CHECK_EQUAL( std::string( dst, read ), "cd\n" );
    BOOST_CHECK( buffer.Eof() );
}

BOOST_FIXTURE_TEST_CASE( buffer_skip_overread, Fixture )
{
    const std::string prefix = "abcd";
    stream << prefix << "\n";
    buffer.Skip( prefix.size() + 1 );
    BOOST_CHECK( buffer.Eof() );
    BOOST_CHECK_THROW( buffer.Peek(), std::runtime_error );
    BOOST_CHECK_THROW( buffer.Get(), std::runtime_error );
    BOOST_CHECK_EQUAL( buffer.GetLine(), "" );
    char dst[256];
    const size_t read = buffer.Read( dst, sizeof dst );
    BOOST_CHECK_EQUAL( read, size_t( 0 ) );
    char* ptr;
    const size_t peek = buffer.Peek( &ptr );
    BOOST_CHECK_EQUAL( peek, size_t( 0 ) );
}
