// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

unsigned short PORT = 55000;

namespace
{
    std::string data_directory;

    void set_data_directory( int argc, char* argv[] )
    {
        while( argc-- )
        {
            const std::string argument = argv[argc];
            const std::string::size_type n = argument.find( '=' );
            if( n != std::string::npos && argument.substr( 0, n ) == "--data_directory" )
                data_directory = argument.substr( n+1 );
            if( n != std::string::npos && argument.substr( 0, n ) == "--port_number" )
                PORT = boost::lexical_cast< unsigned short >( argument.substr( n+1 ) );
        }
    }
}

::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    set_data_directory( argc, argv );
    return 0;
}

std::string BOOST_RESOLVE( const std::string& filename )
{
    if( data_directory.empty() )
        return filename;
    return data_directory + '/' + filename;
}

namespace xml
{
    inline std::string& remove_prolog( std::string& in )
    {
        const std::string::size_type position = in.find( "?>" );
        if( position != std::string::npos )
            in.erase( 0, position + 2 );
        return in;
    }

    inline std::string& remove_crlf( std::string& in )
    {
        std::string::size_type position;
        while( (position = in.find_first_of( "\r\n" )) != std::string::npos )
            in.erase( position, 1 );
        return in;
    }

    inline bool is_inside_quotes( const std::string& in, std::string::size_type position )
    {
        if( in[position] == '\"' )
            return false;
        return std::count( in.begin(), in.begin() + position, '\'' ) % 2 == 1;
    }

    inline std::string& replace( std::string& in, const std::string& from, const std::string& to )
    {
        std::string::size_type position = 0;
        while( (position = in.find( from, position )) != std::string::npos )
            if( is_inside_quotes( in, position ) )
                ++position;
            else
                in.replace( position, from.length(), to );
        return in;
    }

    inline std::string format( const std::string& in )
    {
        std::string result = in;
        remove_prolog( result );
        remove_crlf( result );
        replace( result, "\"", "'" );
        replace( result, "  ", " " );
        replace( result, " <", "<" );
        replace( result, "< ", "<" );
        replace( result, "> ", ">" );
        replace( result, " =", "=" );
        replace( result, "= ", "=" );
        replace( result, " /", "/" );
        return result;
    }
}

void BOOST_CHECK_XML_EQUAL( const std::string& expected, const std::string& actual )
{
    typedef boost::tokenizer< boost::char_separator< char > > T_Tokenizer;
    boost::char_separator< char > sep( "<>" );
    const std::string formattedExpected = xml::format( expected );
    const std::string formattedActual = xml::format( actual );
    T_Tokenizer expectedTokens( formattedExpected, sep );
    T_Tokenizer actualTokens( formattedActual, sep );
    T_Tokenizer::iterator itExpected = expectedTokens.begin();
    T_Tokenizer::iterator itActual = actualTokens.begin();
    while( itExpected != expectedTokens.end() && itActual != actualTokens.end() )
    {
        const std::string expected = "<" + *itExpected + ">";
        const std::string actual = "<" + *itActual + ">";
        BOOST_CHECK_EQUAL( expected, actual );
        ++itExpected;
        ++itActual;
    }
    BOOST_CHECK( itExpected == expectedTokens.end() );
    BOOST_CHECK( itActual == actualTokens.end() );
}
