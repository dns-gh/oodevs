// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "web_test.h"

#include <web/MimeReader.h>
#include <runtime/Pool.h>

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <iostream>
#include <sstream>

using namespace web;
using runtime::Pool;

namespace
{
    struct Fixture
    {
        Fixture()
            : reader()
            , pool( 1, 1 )
        {
            // NOTHING
        }
        MimeReader reader;
        Pool pool;
    };
}

BOOST_FIXTURE_TEST_CASE( reader_without_headers_is_invalid, Fixture )
{
    BOOST_CHECK_EQUAL( reader.IsValid(), false );
}

BOOST_FIXTURE_TEST_CASE( reader_with_header_is_valid, Fixture )
{
    reader.PutHeader( "Content-Type", "multipart/form-data; boundary=limit" );
    BOOST_CHECK_EQUAL( reader.IsValid(), true );
}

namespace
{
std::string SinkStream( io::Reader_ABC& src )
{
    char buffer[64];
    std::string reply;
    for(;;)
    {
        const int len = src.Read( &buffer[0], sizeof buffer );
        if( len <= 0 )
            break;
        reply.append( &buffer[0], len );
    }
    return reply;
}

void CheckStream( io::Reader_ABC& src, size_t& count, const std::string& expected )
{
    const std::string actual = SinkStream( src );
    BOOST_CHECK_EQUAL( expected, actual );
    ++count;
}
}

BOOST_FIXTURE_TEST_CASE( reader_parse_single_part, Fixture )
{
    const std::string limit = "0123456789abcdefedcba9876543210";
    const std::string headers = "Content-Disposition: form-data; filename=\"wut\"; name=\"my_name\"";
    const std::string data = "some data...";
    std::stringstream buffer;
    buffer << "--" + limit + " \t \r\n"
           << headers + "\r\n"
           << "\r\n"
           << data + "\r\n"
           << "--" + limit + "--\r\n";
    reader.PutHeader( "Content-Type", "multipart/form-data; boundary=" + limit );
    BOOST_CHECK_EQUAL( reader.IsValid(), true );

    size_t count = 0;
    reader.Register( "my_name", boost::bind( &CheckStream, _1, boost::ref( count ), data ) );
    StreamReader src( buffer );
    reader.Parse( pool, src );
    BOOST_CHECK_EQUAL( count, size_t( 1 ) );
}

BOOST_FIXTURE_TEST_CASE( reader_parse_truncated_mime, Fixture )
{
    const std::string limit = "0123456789abcdefedcba9876543210";
    std::stringstream buffer;
    buffer << "--" + limit + " \t \r\n"
           << "Content-Disposition: form-data; filename=\"wut\"; name=\"my_name\"\r\n"
           << "\r\n"
           << "some data...\r\n"
           << "--" + limit.substr( 0, limit.size() - 1 ) + "--\r\n";
    reader.PutHeader( "Content-Type", "multipart/form-data; boundary=" + limit );
    BOOST_CHECK_EQUAL( reader.IsValid(), true );
    reader.Register( "my_name", []( io::Reader_ABC& src ){ SinkStream( src ); } );
    StreamReader src( buffer );
    reader.Parse( pool, src );
}

BOOST_FIXTURE_TEST_CASE( reader_parse_evil_delimiter, Fixture )
{
    const std::string limit = "0123456789abcdefedcba9876543210";
    std::stringstream buffer;
    buffer << "--" + limit + " \t \r\n"
           << "Content-Disposition: form-data; filename=\"wut\"; name=\"my_name\"\r\n"
           << "\r\n"
           << "some data...\r\n"
           << "--" + limit.substr( 0, limit.size()>>2 ) + "--\r\n";
    reader.PutHeader( "Content-Type", "multipart/form-data; boundary=" + limit );
    BOOST_CHECK_EQUAL( reader.IsValid(), true );
    reader.Register( "my_name", []( io::Reader_ABC& src ){ SinkStream( src ); } );
    StreamReader src( buffer );
    reader.Parse( pool, src );
}

BOOST_FIXTURE_TEST_CASE( reader_parse_multi_part, Fixture )
{
    const std::string limit = "0123456789abcdefedcba9876543210";
    const std::string textHeader = "Content-Disposition: form-data; filename=\"wut\"; name=\"text\"";
    const std::string text = "some data...";
    const std::string binaryHeader = "Content-Disposition: form-data; filename=\"a.bin\"; name=\"binary\"";
    std::vector< char > binary = boost::assign::list_of( 1 )( 0 ).repeat( 8000 , 127 )( 42 );
    std::stringstream buffer;
    buffer << "--" + limit + " \t \r\n"
           << textHeader + "\r\n"
           << "\r\n"
           << text + "\r\n"
           << "--" + limit + "\r\n"
           << binaryHeader + "\r\n"
           << "\r\n"
           << std::string( &binary[0], binary.size() ) + "\r\n"
           << "--" + limit + "--\r\n";
    reader.PutHeader( "Content-Type", "multipart/form-data; boundary=" + limit );
    BOOST_CHECK_EQUAL( reader.IsValid(), true );

    size_t count = 0;
    reader.Register( "text", boost::bind( &CheckStream, _1, boost::ref( count ), text ) );
    reader.Register( "binary", boost::bind( &CheckStream, _1, boost::ref( count ), std::string( &binary[0], binary.size() ) ) );
    StreamReader src( buffer );
    reader.Parse( pool, src );
    BOOST_CHECK_EQUAL( count, size_t( 2 ) );
}

namespace
{
void ReadLittle( io::Reader_ABC& src, size_t& count, const char* data, size_t size )
{
    std::vector< char > buffer( size );
    size_t fill = 0;
    while( fill < size )
    {
        const int len = src.Read( &buffer[fill], size - fill );
        if( len <= 0 )
            break;
        fill += len;
    }
    BOOST_CHECK_EQUAL( std::string( &buffer[0], buffer.size() ), std::string( data, fill ) );
    ++count;
}
}

BOOST_FIXTURE_TEST_CASE( reader_skip_invalid_mime_handlers, Fixture )
{
    const std::string limit = "0123456789abcdefedcba9876543210";
    const std::string headers = "Content-Disposition: form-data; filename=\"wut\"; name=\"my_name\"";
    std::vector< char > binary = boost::assign::list_of( 1 )( 0 ).repeat( 8000 , 127 )( 42 );
    std::stringstream buffer;
    buffer << "--" + limit + " \t \r\n"
           << headers + "\r\n"
           << "\r\n"
           << std::string( &binary[0], binary.size() ) + "\r\n"
           << "--" + limit + "--\r\n";
    reader.PutHeader( "Content-Type", "multipart/form-data; boundary=" + limit );
    BOOST_CHECK_EQUAL( reader.IsValid(), true );

    size_t count = 0;
    reader.Register( "my_name", boost::bind( &ReadLittle, _1, boost::ref( count ), &binary[0], 1024 ) );
    reader.Register( "zebulon", boost::bind( &ReadLittle, _1, boost::ref( count ), &binary[0], 0 ) );
    StreamReader src( buffer );
    reader.Parse( pool, src );
    BOOST_CHECK_EQUAL( count, size_t( 1 ) );
}

BOOST_FIXTURE_TEST_CASE( reader_skip_unregistered_parts, Fixture )
{
    const std::string limit = "0123456789abcdefedcba9876543210";
    const std::string headers = "Content-Disposition: form-data; filename=\"wut\"; name=\"my_name\"";
    std::stringstream buffer;
    buffer << "--" + limit + " \t \r\n"
           << headers + "\r\n"
           << "\r\n"
           << "blah\r\n"
           << "--" + limit + "--\r\n";
    reader.PutHeader( "Content-Type", "multipart/form-data; boundary=" + limit );
    BOOST_CHECK_EQUAL( reader.IsValid(), true );
    StreamReader src( buffer );
    reader.Parse( pool, src );
}
