// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "WikiXmlConverter.h"
#include <boost/regex.hpp>
#include <boost/bind.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <queue>

namespace
{
typedef std::pair< std::string, std::string > XmlToWikiTag;
const XmlToWikiTag xmlToWikiTags[] =
{
    XmlToWikiTag( "bold",       "\"\"" ),
    XmlToWikiTag( "italic",     "''" ),
    XmlToWikiTag( "underlined", "__" ),
    XmlToWikiTag( "link",       "$$" ),
};

const std::string& ConvertWikiTagToXmlTag( const std::string& wikiTag )
{
    for( size_t i = 0; i != sizeof( xmlToWikiTags )/sizeof( *xmlToWikiTags); ++i )
        if( wikiTag.compare( xmlToWikiTags[i].second ) == 0 )
            return xmlToWikiTags[i].first;
    throw MASA_EXCEPTION( "Used wiki tag is invalid." );
}

const std::string& ConvertXmlToWikiTag( const std::string& xmlTag )
{
    for( size_t i = 0; i != sizeof( xmlToWikiTags )/sizeof( *xmlToWikiTags); ++i )
        if( xmlTag.compare( xmlToWikiTags[i].first ) == 0 )
            return xmlToWikiTags[i].second;
    throw MASA_EXCEPTION( "Used xml tag is invalid." );
}

// -----------------------------------------------------------------------------
// XML TO WIKI CONVERSION
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ReadXmlData
// Created: NPT 2013-01-23
// -----------------------------------------------------------------------------
void ReadXmlLine( const std::string& tag, xml::xistream& xis, std::string& text )
{
    if( tag == "text" )
    {
        std::string xmlValue;
        xis >> xml::optional >> xmlValue;
        text += xmlValue;
    }
    else
    {
        text += ConvertXmlToWikiTag( tag );
        xis >> xml::list( boost::bind( &ReadXmlLine, _2, _3, boost::ref( text )));
        text += ConvertXmlToWikiTag( tag );
    }
}

// -----------------------------------------------------------------------------
// Name: ReadXmlList
// Created: NPT 2013-01-23
// -----------------------------------------------------------------------------
void ReadXmlList( const std::string& tag, xml::xistream& xis, std::string& text, int level )
{
    if( tag == "ul" )
    {
        xis >> xml::list( boost::bind( &ReadXmlList, _2, _3,
            boost::ref( text ), level + 1 ));
    }
    else if( tag == "li" )
    {
        text += std::string( level, ' ') + "* ";
        xis >> xml::optional >> xml::start( "line" )
            >> xml::list( boost::bind( &ReadXmlLine, _2, _3, boost::ref( text )))
            >> xml::end;
        text += "\n";
    }
}

// -----------------------------------------------------------------------------
// Name: FromXmlTagsToWiki
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void FromXmlTagsToWiki( const std::string& tag, xml::xistream& xis, std::string& text )
{
    if( tag == "line" )
        xis >> xml::list( boost::bind( &ReadXmlLine, _2, _3, boost::ref( text )));
    else if( tag == "ul" )
        xis >> xml::list( boost::bind( &ReadXmlList, _2, _3,
            boost::ref( text ), 1 ));
    text += "\n";
}

// -----------------------------------------------------------------------------
// WIKI TO XML CONVERSION
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: MakeStringXmlItem
// Created: NPT 2013-01-22
// -----------------------------------------------------------------------------

const boost::regex reStyles = boost::regex( "(.*?)(\"\"|''|__|\\$\\$)(.*)" );

void MakeStringXmlItem( xml::xostream& output, std::string line )
{
    if( line.empty() )
        return;
    xml::xosubstream xos( output );
    xos.start( "line" );
    std::set< std::string > openTags;
    while ( !line.empty() )
    {
        boost::smatch match;
        if( boost::regex_search( line, match, reStyles ) )
        {
            if( match[ 1 ].length() > 0 )
                xos << xml::content( "text", match[ 1 ] );
            auto ret = openTags.insert( match[ 2 ] );
            if( ret.second )
                xos << xml::start( ConvertWikiTagToXmlTag( *ret.first ) );
            else
            {
                xos << xml::end;
                openTags.erase( ret.first );
            }
            line = match[ 3 ];
        }
        else
        {
            xos << xml::content( "text", line );
            break;
        }
    }
}

}  // namespace

// -----------------------------------------------------------------------------
// Name: FromXmlToWiki
// Created: NPT 2013-02-05
// -----------------------------------------------------------------------------
void FromXmlToWiki( xml::xistream& xis, std::string& text )
{
    xis >> xml::list( boost::bind( &FromXmlTagsToWiki, _2, _3, boost::ref( text ) ) );
}

// -----------------------------------------------------------------------------
// Name: FromWikiToXml
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------

const boost::regex reList = boost::regex( "^(\\s+)\\* (.*)$" );

void FromWikiToXml( xml::xostream& output, const std::string& text )
{
    std::vector< std::string > stringList;
    std::string result;
    boost::split( stringList, text, boost::algorithm::is_any_of( "\n" ) );

    //list search
    std::queue< std::pair< std::size_t, std::string > > tokensVector;
    for( auto it = stringList.begin(); it != stringList.end(); ++it )
    {
        std::string val = *it;
        boost::smatch match;
        if( boost::regex_search( val, match, reList ) ) //check if the current line is a list
            tokensVector.push( std::make_pair< std::size_t, std::string >( match[ 1 ].length(), match[ 2 ] ) ); //< space before the *, text after the star >
        else
            tokensVector.push( std::make_pair< std::size_t, std::string >( 0, val ) );//< 0 space beause text, text >
    }

    //create xml file
    xml::xosubstream xos( output );
    size_t previousLength = 0;
    while( !tokensVector.empty() )
    {
        std::pair< std::size_t, std::string > line = tokensVector.front();
        tokensVector.pop();
        int ulOffset = static_cast< int >( line.first - previousLength );
        for( int i = 0; i < ulOffset ; ++i )
            xos << xml::start( "ul" );
        for( int i = ulOffset; i < 0 ; ++i )
            xos << xml::end;
        if( line.first )
            xos << xml::start( "li" );
        MakeStringXmlItem( xos, line.second );
        if( line.first )
            xos << xml::end;
        previousLength = line.first;
    }
}
