// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_FragOrder.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Missions_Data.h"
#include "ADN_ConsistencyChecker.h"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <queue>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

namespace bfs = boost::filesystem;

ADN_Missions_FragOrder::ADN_Missions_FragOrder()
    : id_( ADN_Missions_Data::idManager_.GetNextId() )
    , isAvailableWithoutMission_( false )
{
    // NOTHING
}

ADN_Missions_FragOrder::ADN_Missions_FragOrder( unsigned int id )
    : id_                       ( id )
    , isAvailableWithoutMission_( false )
{
    ADN_Missions_Data::idManager_.Lock( id );
}

ADN_Missions_FragOrder::~ADN_Missions_FragOrder()
{
    // NOTHING
}

std::string ADN_Missions_FragOrder::GetItemName()
{
    return strName_.GetData();
}

ADN_Missions_FragOrder* ADN_Missions_FragOrder::CreateCopy()
{
    ADN_Missions_FragOrder* newFragOrder = new ADN_Missions_FragOrder();
    newFragOrder->strName_ = strName_.GetData();
    newFragOrder->missionSheetPath_ = missionSheetPath_.GetData();
    newFragOrder->parameters_.reserve( parameters_.size() );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        ADN_Missions_Parameter* newParam = (*it)->CreateCopy();
        newFragOrder->parameters_.AddItem( newParam );
    }
    return newFragOrder;
}

void ADN_Missions_FragOrder::ReadArchive( xml::xistream& input, const std::string& missionDir )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "dia-type", diaType_ )
          >> xml::optional >> xml::attribute( "available-without-mission", isAvailableWithoutMission_ )
          >> xml::list( "parameter", *this, &ADN_Missions_FragOrder::ReadParameter );
    ReadMissionSheet( missionDir );
}

void ADN_Missions_FragOrder::ReadParameter( xml::xistream& input )
{
    std::auto_ptr< ADN_Missions_Parameter > spNew( new ADN_Missions_Parameter() );
    spNew->ReadArchive( input );
    parameters_.AddItem( spNew.release() );
}

namespace
{
    QString BuildDiaFragOrderType( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        if( list.front() == "Pion" || list.front() == "Automate" || list.front() == "Population" )
            list[0].append( "_" );
        for( int i = 1; i < list.size() - 1; ++i )
            if( list[i].length() > 1 && list[i] == list[i].upper() )
                list[i].append( "_" );
        return QString( "Rep_OrderConduite_%1" ).arg( list.join( "" ) );
    }
}

void ADN_Missions_FragOrder::WriteArchive( xml::xostream& output )
{
    if( diaType_.GetData().empty() )
        diaType_ = BuildDiaFragOrderType( strName_.GetData().c_str() ).toStdString();

    output << xml::start( "fragorder" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "dia-type", diaType_ )
            << xml::attribute( "id", id_ )
            << xml::attribute( "available-without-mission", isAvailableWithoutMission_ );

    for( unsigned int i = 0; i < parameters_.size(); ++i )
        parameters_[i]->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::CheckDataConsistency
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::CheckMissionDataConsistency( ADN_ConsistencyChecker& checker )
{
    CheckFieldDataConsistency( descriptionContext_.GetData(), checker );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
        CheckFieldDataConsistency( (*it)->description_.GetData(), checker );
    CheckFieldDataConsistency( descriptionBehavior_.GetData(), checker );
    CheckFieldDataConsistency( descriptionSpecific_.GetData(), checker );
    CheckFieldDataConsistency( descriptionComment_.GetData(), checker );
    CheckFieldDataConsistency( descriptionMissionEnd_.GetData(), checker );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::CheckFieldDataConsistency
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::CheckFieldDataConsistency( std::string fieldData, ADN_ConsistencyChecker& checker )
{
    boost::smatch match;
    bool tagOpen = false;
    while( boost::regex_search( fieldData, match, boost::regex( "(.*?)(\\$\\$)(.*)" ) ) )
    {
        if( tagOpen )
            if( !IsFileInAttachmentList( match[ 1 ].str() ) )
                checker.AddError( eMissionAttachmentInvalid, strName_.GetData(), eMissions, eNbrEntityTypes, match[ 1 ].str() );
        tagOpen = !tagOpen;
        fieldData = match[ 3 ];
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::ReadMissionSheetDescriptionsParameters
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::ReadMissionSheetParametersDescriptions( xml::xistream& xis )
{
    std::string parameterName;
    std::string parameterData;
    xis >> xml::attribute( "name", parameterName )
        >> xml::list( *this, &ADN_Missions_FragOrder::FromXmlToWiki, parameterData );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
        if( (*it)->strName_ == parameterName )
            (*it)->description_ = parameterData;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::ReadMissionSheetAttachments
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::ReadMissionSheetAttachments( xml::xistream& xis )
{
    attachments_.AddItem( new ADN_Missions_Attachment( xis.attribute< std::string >( "name" ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::WriteMissionSheetParametersDescriptions
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::WriteMissionSheetParametersDescriptions( xml::xostream& xos )
{
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        xos << xml::start( "parameter" )
            << xml::attribute( "name", (*it)->strName_ );
        FromWikiToXml( xos, (*it)->description_.GetData() );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::ReadMissionSheetAttachments
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::WriteMissionSheetAttachments( xml::xostream& xos )
{
    for( IT_MissionAttachment_Vector it = attachments_.begin(); it != attachments_.end(); ++it )
        xos << xml::start( "attachment" )
        << xml::attribute( "name", (*it)->strName_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::FromXmlToWiki
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::FromXmlToWiki( const std::string& tag, xml::xistream& xis, std::string& text )
{
    int level = 0;
    if( tag == "line" )
        xis >> xml::list( *this, &ADN_Missions_FragOrder::ReadXmlLine, text );
    else if( tag == "ul" )
    {
        xis >> xml::list( *this, &ADN_Missions_FragOrder::ReadXmlList, text, ++level );
        --level;
    }
    text += "\n";
}

namespace
{
    std::string ConvertWikiTagToXmlTag( const std::string& wikiTag )
    {
        if( wikiTag == "\"\"" )
            return "bold";
        if( wikiTag == "''")
            return "italic";
        if( wikiTag == "__" )
            return "underlined";
        if( wikiTag == "$$")
            return "link";
        throw MASA_EXCEPTION( "Used wiki tag is invalid." );
    }

    std::string ConvertXmlToWikiTag( const std::string& xmlTag )
    {
        if( xmlTag =="bold" )
            return "\"\"" ;
        if( xmlTag == "italic")
            return "''";
        if( xmlTag =="underlined" )
            return  "__" ;
        if( xmlTag == "link" )
            return "$$";
        throw MASA_EXCEPTION( "Used xml tag is invalid." );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::IsFileInAttachmentList
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
bool ADN_Missions_FragOrder::IsFileInAttachmentList( const std::string& fileName )
{
    for( IT_MissionAttachment_Vector it = attachments_.begin(); it != attachments_.end(); ++it )
        if(fileName == (*it)->strName_.GetData() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::MakeStringXmlItem
// Created: NPT 2013-01-22
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::MakeStringXmlItem( xml::xostream& xos, std::size_t length, std::string line )
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
// Name: ADN_Missions_FragOrder::ReadXmlData
// Created: NPT 2013-01-23
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::ReadXmlLine( const std::string& tag, xml::xistream& xis, std::string& text )
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
        xis >> xml::list( *this, &ADN_Missions_FragOrder::ReadXmlLine, text );
        text += ConvertXmlToWikiTag( tag );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::ReadXmlList
// Created: NPT 2013-01-23
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::ReadXmlList( const std::string& tag, xml::xistream& xis, std::string& text, int& level )
{
    if( tag == "ul" )
    {
        xis >> xml::list( *this, &ADN_Missions_FragOrder::ReadXmlList, text, ++level );
        --level;
    }
    else if( tag == "li" )
    {
        text += std::string( level, ' ') + "* ";
        xis >> xml::start( "line" )
            >> xml::list( *this, &ADN_Missions_FragOrder::ReadXmlLine, text )
            >> xml::end;
        text += "\n";
    }
}

namespace
{
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
                vector.pop_back();
        }
    private:
        xml::xostream& xos_;
        std::vector< std::auto_ptr< UL > > vector;
    };
}
// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::FromWikiToXml
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::FromWikiToXml( xml::xostream& xos, const std::string& text )
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
            tokensVector.push( std::make_pair< std::size_t, std::string >( match[ 1 ].length(), match[ 2 ] ) ); //< space before the "* ", text after the star >
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

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::ReadMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::ReadMissionSheet( const std::string& missionDir )
{
    const std::string fileName = std::string( missionDir +  "/" + strName_.GetData() );
    if( bfs::is_directory( missionDir ) && bfs::is_regular_file( fileName + ".xml" ) )
    {
        missionSheetPath_ = fileName + ".html";
        xml::xifstream xis( fileName + ".xml" );
        std::string descriptionContext;
        std::string descriptionBehavior;
        std::string descriptionSpecific;
        std::string descriptionComment;
        std::string descriptionMissionEnd;
        xis >> xml::start( "mission-sheet" )
            >> xml::optional >> xml::start( "context" )
            >> xml::list( *this, &ADN_Missions_FragOrder::FromXmlToWiki, descriptionContext )
            >> xml::end
            >> xml::optional >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &ADN_Missions_FragOrder::ReadMissionSheetParametersDescriptions )
            >> xml::end
            >> xml::start( "behavior" )
            >> xml::list( *this, &ADN_Missions_FragOrder::FromXmlToWiki, descriptionBehavior )
            >> xml::end
            >> xml::optional >> xml::start( "specific-cases" )
            >> xml::list( *this, &ADN_Missions_FragOrder::FromXmlToWiki, descriptionSpecific )
            >> xml::end
            >> xml::optional >> xml::start( "comments" )
            >> xml::list( *this, &ADN_Missions_FragOrder::FromXmlToWiki, descriptionComment )
            >> xml::end
            >> xml::optional >> xml::start( "mission-end" )
            >> xml::list( *this, &ADN_Missions_FragOrder::FromXmlToWiki, descriptionMissionEnd )
            >> xml::end
            >> xml::optional >> xml::start( "attachments" )
            >> xml::list( "attachment", *this, &ADN_Missions_FragOrder::ReadMissionSheetAttachments )
            >> xml::end
            >> xml::end;
        descriptionContext_ = descriptionContext;
        descriptionBehavior_ = descriptionBehavior;
        descriptionSpecific_ = descriptionSpecific;
        descriptionComment_ = descriptionComment;
        descriptionMissionEnd_ = descriptionMissionEnd;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::RemoveMissionSheet
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::RemoveDifferentNamedMissionSheet( const std::string& missionDir )
{
    //xml file
    const std::string newXmlPath = std::string( missionDir + "/" + strName_.GetData() + ".xml" );
    const std::string oldXmlPath = std::string( QString( missionDir.c_str() ) + "/" + QFileInfo( missionSheetPath_.GetData().c_str() ).baseName() + ".xml" );
    if( !missionSheetPath_.GetData().empty() && newXmlPath != oldXmlPath )
        bfs::remove( oldXmlPath );

    //html file
    const std::string newHtmlPath = std::string( missionDir + "/" + strName_.GetData() + ".html" );
    const std::string oldHtmlPath = std::string( QString( missionDir.c_str() ) + "/" + QFileInfo( missionSheetPath_.GetData().c_str() ).baseName() + ".html" );
    if( !missionSheetPath_.GetData().empty() && newHtmlPath != oldHtmlPath )
        bfs::remove( oldHtmlPath );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::WriteMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::WriteMissionSheet( const std::string& missionDir )
{
    std::string fileName = std::string( missionDir + "/" + strName_.GetData() );
    if( !bfs::is_directory( missionDir + "/obsolete" ) )
        bfs::create_directories( missionDir + "/obsolete" );
    xml::xofstream xos( fileName + ".xml" );
    xos << xml::start( "mission-sheet" )
        << xml::attribute( "name", strName_.GetData() )
        << xml::start( "context" );
    FromWikiToXml( xos, descriptionContext_.GetData() );
    xos << xml::end
        << xml::start( "parameters" );
    WriteMissionSheetParametersDescriptions( xos );
    xos << xml::end
        << xml::start( "behavior" );
    FromWikiToXml( xos,descriptionBehavior_.GetData() );
    xos << xml::end
        << xml::start( "specific-cases" );
    FromWikiToXml( xos, descriptionSpecific_.GetData() );
    xos << xml::end
        << xml::start( "comments" );
    FromWikiToXml( xos, descriptionComment_.GetData() );
    xos << xml::end
        << xml::start( "mission-end" );
    FromWikiToXml( xos, descriptionMissionEnd_.GetData() );
    xos << xml::end
        << xml::start( "attachments" );
    WriteMissionSheetAttachments( xos );
    xos << xml::end
        << xml::end;

    std::string xslFile = ADN_Workspace::GetWorkspace().GetProject().GetMissionSheetXslFile();
    assert( bfs::exists( xslFile ) );
    xml::xifstream xisXML( fileName + ".xml" );
    xsl::xstringtransform xst( xslFile );
    xst << xisXML;
    std::fstream fileStream( fileName + ".html", std::ios::out | std::ios::trunc );
    fileStream << xst.str();
    fileStream.close();
    missionSheetPath_ = fileName + ".html";
}
