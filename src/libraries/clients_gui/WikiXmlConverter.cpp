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
#include <boost/filesystem.hpp>
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

class UL : private boost::noncopyable
{
public:
    UL( xml::xostream& xos )
        : xos_( xos )
    {
        xos << xml::start( "ul" );
    }
    ~UL()
    {
        xos_ << xml::end;
    }
private:
    xml::xostream& xos_;
};

class ULFormatter : private boost::noncopyable
{
public:
    ULFormatter( xml::xostream& xos )
        : xos_( xos )
    {
        // NOTHING
    }
    ~ULFormatter()
    {
        // NOTHING
    }

    void FormatUL( int n )
    {
        for( int i = 0; i < n ; ++i )
            vector.emplace_back( new UL( xos_ ) );
        for( int i = n; i < 0 ; ++i )
        {
            delete vector.back();
            vector.pop_back();
        }
    }
private:
    xml::xostream& xos_;
    std::vector< UL* > vector;
};

// =============================================================================
/** @class  WikiXmlConverter
    @brief  WikiXmlConverter
*/
// Created: NPT 2013-02-05
// =============================================================================
class WikiXmlConverter : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             WikiXmlConverter();
    virtual ~WikiXmlConverter();
    //@}

    //! @name Operations
    //@{
    void FromWikiToXml( xml::xostream& xos, const std::string& text );
    void FromXmlToWiki( xml::xistream& xis, std::string& text );
    //@}

private:
    //! @name Helpers
    //@{
    void FromXmlTagsToWiki( const std::string& tag, xml::xistream& xis, std::string& text );
    void MakeStringXmlItem( xml::xostream& xos, std::size_t length, std::string line );
    void ReadXmlLine( const std::string& tag, xml::xistream& xis, std::string& text );
    void ReadXmlList( const std::string& tag, xml::xistream& xis, std::string& text, int level );
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

// -----------------------------------------------------------------------------
// Name: WikiXmlConverter constructor
// Created: NPT 2013-02-05
// -----------------------------------------------------------------------------
WikiXmlConverter::WikiXmlConverter()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WikiXmlConverter destructor
// Created: NPT 2013-02-05
// -----------------------------------------------------------------------------
WikiXmlConverter::~WikiXmlConverter()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// XML TO WIKI CONVERSION
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: WikiXmlConverter::ReadXmlData
// Created: NPT 2013-01-23
// -----------------------------------------------------------------------------
void WikiXmlConverter::ReadXmlLine( const std::string& tag, xml::xistream& xis, std::string& text )
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
        xis >> xml::list( *this, &WikiXmlConverter::ReadXmlLine, text );
        text += ConvertXmlToWikiTag( tag );
    }
}

// -----------------------------------------------------------------------------
// Name: WikiXmlConverter::ReadXmlList
// Created: NPT 2013-01-23
// -----------------------------------------------------------------------------
void WikiXmlConverter::ReadXmlList( const std::string& tag, xml::xistream& xis, std::string& text, int level )
{
    if( tag == "ul" )
    {
        int sublevel = level + 1;
        xis >> xml::list( *this, &WikiXmlConverter::ReadXmlList, text, sublevel );
    }
    else if( tag == "li" )
    {
        text += std::string( level, ' ') + "* ";
        xis >> xml::start( "line" )
            >> xml::list( *this, &WikiXmlConverter::ReadXmlLine, text )
            >> xml::end;
        text += "\n";
    }
}

// -----------------------------------------------------------------------------
// Name: WikiXmlConverter::FromXmlTagsToWiki
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void WikiXmlConverter::FromXmlTagsToWiki( const std::string& tag, xml::xistream& xis, std::string& text )
{
    int level = 0;
    if( tag == "line" )
        xis >> xml::list( *this, &WikiXmlConverter::ReadXmlLine, text );
    else if( tag == "ul" )
    {
        int sublevel = level + 1;
        xis >> xml::list( *this, &WikiXmlConverter::ReadXmlList, text, sublevel );
    }
    text += "\n";
}

// -----------------------------------------------------------------------------
// Name: WikiXmlConverter::FromXmlToWiki
// Created: NPT 2013-02-05
// -----------------------------------------------------------------------------
void WikiXmlConverter::FromXmlToWiki( xml::xistream& xis, std::string& text )
{
    xis >> xml::list( boost::bind( &WikiXmlConverter::FromXmlTagsToWiki, this, _2, _3, boost::ref( text ) ) );
}

// -----------------------------------------------------------------------------
// WIKI TO XML CONVERSION
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: WikiXmlConverter::MakeStringXmlItem
// Created: NPT 2013-01-22
// -----------------------------------------------------------------------------
void WikiXmlConverter::MakeStringXmlItem( xml::xostream& xos, std::size_t length, std::string line )
{

    if( length > 0 )
        xos.start( "li" );
    if( line.size() > 0 )
    {
        xos.start( "line" );
        std::set< std::string > openTags;
        while ( line.size() > 0 )
        {
            boost::smatch match;
            if( boost::regex_search( line, match, boost::regex( "(.*?)(\"\"|''|__|\\$\\$)(.*)" ) ) )
            {
                if( match[ 1 ].length() > 0 )
                    xos << xml::start( "text" ) << match[ 1 ] << xml::end;
                auto ret = openTags.insert( match[ 2 ] );
                if( ret.second )
                    xos.start( ConvertWikiTagToXmlTag( *ret.first ) );
                else
                {
                    xos.end();
                    openTags.erase( ret.first );
                }
                line = match[ 3 ];
            }
            else
            {
                xos << xml::start( "text" ) << line << xml::end;
                break;
            }
        }
        while( !openTags.empty() )
        {
            openTags.erase( openTags.begin() );
            xos.end();
        }
        xos.end();
    }
    if( length > 0 )
        xos.end();
}

// -----------------------------------------------------------------------------
// Name: WikiXmlConverter::FromWikiToXml
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void WikiXmlConverter::FromWikiToXml( xml::xostream& xos, const std::string& text )
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
        if( boost::regex_search( val, match, boost::regex( "^(\\s+)\\* (.*)$" ) ) ) //check if the current line is a list
            tokensVector.push( std::make_pair< std::size_t, std::string >( match[ 1 ].length(), match[ 2 ] ) ); //< space before the *, text after the star >
        else
            tokensVector.push( std::make_pair< std::size_t, std::string >( 0, val ) );//< 0 space beause text, text >
    }

    //create xml file
    size_t previousLength = 0;
    ULFormatter formatter( xos );
    while( !tokensVector.empty() )
    {
        std::pair< std::size_t, std::string > line = tokensVector.front();
        tokensVector.pop();
        formatter.FormatUL( static_cast< int >( line.first - previousLength ) );
        MakeStringXmlItem( xos, line.first, line.second );
        previousLength = line.first;
    }
}

}  // namespace

void FromWikiToXml( xml::xostream& xos, const std::string& text )
{
    WikiXmlConverter converter;
    converter.FromWikiToXml( xos, text );
}

void FromXmlToWiki( xml::xistream& xis, std::string& text )
{
    WikiXmlConverter converter;
    converter.FromXmlToWiki( xis, text );
}
