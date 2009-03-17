// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __XmlChecks_h_
#define __XmlChecks_h_

#include <string>
#include <boost/tokenizer.hpp>

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

inline void BOOST_CHECK_XML_EQUAL( const std::string& expectedXml, const std::string& actualXml )
{
    typedef boost::tokenizer< boost::char_separator< char > > T_Tokenizer;
    boost::char_separator< char > sep( "<>" );
    const std::string formattedExpected = xml::format( expectedXml );
    const std::string formattedActual = xml::format( actualXml );
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

#endif // __XmlChecks_h_
