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
#include <host/Pool.h>

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <iostream>
#include <sstream>

using namespace web;
using host::Pool;

namespace
{
    struct Fixture
    {
        Fixture()
            : reader()
            , pool( 1 )
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
    void CheckStream( std::istream& src, const std::string& expected )
    {
        char buffer[64];
        std::string actual;
        while( !src.eof() )
        {
            src.read( &buffer[0], sizeof buffer );
            actual.append( &buffer[0], static_cast< size_t >( src.gcount() ) );
        }
        BOOST_CHECK_EQUAL( expected, actual );
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

    std::istream& part = reader.Register( "my_name" );
    Pool::Future future = pool.Post( boost::bind( &MimeReader::Parse, &reader, boost::ref( buffer ) ) );

    CheckStream( part, data );
    future.wait();
    BOOST_CHECK( !future.has_exception() );
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

    std::istream& itext = reader.Register( "text" );
    std::istream& ibinary = reader.Register( "binary" );
    Pool::Future future = pool.Post( boost::bind( &MimeReader::Parse, &reader, boost::ref( buffer ) ) );

    CheckStream( itext, text );
    CheckStream( ibinary, std::string( &binary[0], binary.size() ) );
    future.wait();
    BOOST_CHECK( !future.has_exception() );
}
