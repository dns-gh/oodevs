// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_test_pch.h"
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>
#include <boost/tokenizer.hpp>

namespace xml
{
    std::string& remove_prolog( std::string& in )
    {
        const std::string::size_type position = in.find( "?>" );
        if( position != std::string::npos )
            in.erase( 0, position + 2 );
        return in;
    }

    std::string& remove_crlf( std::string& in )
    {
        std::string::size_type position;
        while( (position = in.find_first_of( "\r\n" )) != std::string::npos )
            in.erase( position, 1 );
        return in;
    }

    bool is_inside_quotes( const std::string& in, std::string::size_type position )
    {
        if( in[position] == '\"' )
            return false;
        return std::count( in.begin(), in.begin() + position, '\'' ) % 2 == 1;
    }

    std::string& replace( std::string& in, const std::string& from, const std::string& to )
    {
        std::string::size_type position = 0;
        while( (position = in.find( from, position )) != std::string::npos )
            if( is_inside_quotes( in, position ) )
                ++position;
            else
                in.replace( position, from.length(), to );
        return in;
    }

    std::string format( const std::string& in )
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

// -----------------------------------------------------------------------------
// Name: Transformat_old_to_new_schema
// Created: MGD 2010-11-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Transformat_old_to_new_schema )
{
    const std::string input(
        "<?xml version='1.0' encoding='UTF-8' standalone='no' ?>"
        "<actions>"
            "<action id='1' name='TEST' target='1' time='2009-05-06T05:04:14' type='mission'>"
                "<parameter name='Preparer terrain' type='Bool' value='false'/>"
                "<parameter name='Automate a escorter' type='Automate' value='42'/>"
                "<parameter name='Population' type='PopulationKnowledge' value='42'/>"
                "<parameter name='Ammunitions' type='DotationType' value='4'/>"
                "<parameter name='target' type='UrbanBlock' value='18'/>"
                "<parameter name='Obstacles' type='genobject'>"
                "<parameter name='Obstacles (element 1)' type='obstacle' value='abattis'>"
                    "<parameter name='Obstacle type' type='obstacletype' value='0'/>"
                        "<parameter name='Location' type='location'>"
                            "<location type='polygon'>"
                                "<point coordinates='29TNF3205456973'/>"
                                "<point coordinates='29TNF3200957006'/>"
                                "<point coordinates='29TNF3212256989'/>"
                                "<point coordinates='29TNF3205456973'/>"
                            "</location>"
                        "</parameter>"
                    "</parameter>"
                "</parameter>"
                "<parameter name='Agent' type='AgentBM' value='42'/>"
                "<parameter name='Automate' type='AutomateBM' value='42'/>"
                "<parameter name='objectives' type='PointListBM'>"
                    "<parameter name='Location 1' type='location'>"
                        "<location type='point'>"
                            "<point coordinates='31TCM1182987823'/>"
                        "</location>"
                    "</parameter>"
                "</parameter>"
            "</action>"
        "</actions>" );
    const std::string expected(
        "<actions>"
            "<action id='1' name='TEST' target='1' time='2009-05-06T05:04:14' type='mission'>"
                "<parameter name='Preparer terrain' type='boolean' value='false'/>"
                "<parameter name='Automate a escorter' type='automat' value='42'/>"
                "<parameter name='Population' type='crowdKnowledge' value='42'/>"
                "<parameter name='Ammunitions' type='resourceType' value='4'/>"
                "<parameter name='target' type='urbanKnowledge' value='18'/>"
                "<parameter name='Obstacles' type='plannedwork'>"
                "<parameter name='Obstacles (element 1)' type='plannedwork' value='abattis'>"
                    "<parameter name='Obstacle type' type='obstacletype' value='0'/>"
                        "<parameter name='Location' type='location'>"
                            "<location type='polygon'>"
                            "<point coordinates='29TNF3205456973'/>"
                            "<point coordinates='29TNF3200957006'/>"
                            "<point coordinates='29TNF3212256989'/>"
                            "<point coordinates='29TNF3205456973'/>"
                            "</location>"
                        "</parameter>"
                    "</parameter>"
                "</parameter>"
                "<parameter name='Agent' type='Agent' value='42'/>"
                "<parameter name='Automate' type='automat' value='42'/>"
                "<parameter name='objectives' type='Point'>"
                    "<parameter name='Location 1' type='location'>"
                        "<location type='point'>"
                            "<point coordinates='31TCM1182987823'/>"
                        "</location>"
                    "</parameter>"
            "</parameter>"
            "</action>"
        "</actions>" );
    xml::xistringstream xis( input );
    xsl::xstringtransform xst( "resources/schemas/4.2.0/xsl/ordCompatibility.xsl" );
    xst << xis;
    BOOST_CHECK_XML_EQUAL( expected, xst.str() );
}
