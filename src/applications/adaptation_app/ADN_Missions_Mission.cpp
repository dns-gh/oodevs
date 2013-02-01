// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_Mission.h"
#include "ADN_Missions_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_ConsistencyChecker.h"
#include <tools/Loader_ABC.h>
#include <tools/EncodingConverter.h>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <queue>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>


namespace bfs = boost::filesystem;

// =============================================================================
// Missions
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::Mission
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
ADN_Missions_Mission::ADN_Missions_Mission()
    : id_( ADN_Missions_Data::idManager_.GetNextId() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Mission::ADN_Missions_Mission( unsigned int id )
    : id_( id )
{
    ADN_Missions_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::~Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Mission::~ADN_Missions_Mission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::GetItemName
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
std::string ADN_Missions_Mission::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::CreateCopy
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Mission* ADN_Missions_Mission::CreateCopy()
{
    ADN_Missions_Mission* newMission     = new ADN_Missions_Mission();
    newMission->strName_                 = strName_.GetData();
    newMission->diaBehavior_             = diaBehavior_.GetData();
    newMission->cdtDiaBehavior_          = cdtDiaBehavior_.GetData();
    newMission->mrtDiaBehavior_          = mrtDiaBehavior_.GetData();
    newMission->missionSheetPath_        = missionSheetPath_.GetData();
    newMission->symbol_                  = symbol_.GetData();
    newMission->parameters_.reserve( parameters_.size() );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        ADN_Missions_Parameter* newParam = (*it)->CreateCopy();
        newMission->parameters_.AddItem( newParam );
    }
    return newMission;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::ReadArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadArchive( xml::xistream& input, ADN_Drawings_Data& drawings, const std::string& missionDir )
{
    std::string missionSheetDesc, symbol;
    input >> xml::attribute( "name", strName_ )
        >> xml::attribute( "dia-type", diaType_ )
        >> xml::optional >> xml::attribute( "symbol", symbol )
        >> xml::optional >> xml::attribute( "dia-behavior", diaBehavior_ )
        >> xml::optional >> xml::attribute( "cdt-dia-behavior", cdtDiaBehavior_ )
        >> xml::optional >> xml::attribute( "mrt-dia-behavior", mrtDiaBehavior_ )
        >> xml::optional >> xml::attribute( "package", strPackage_ )
        >> xml::optional >> xml::attribute( "package", strPackage_ )
        >> xml::optional >> xml::start( "description" )
            >> xml::optional >> xml::attribute( "doctrine", doctrine_ )
            >> xml::optional >> xml::attribute( "doctrine", usage_ )
        >> xml ::end
        >> xml::list( "parameter", boost::bind( &ADN_Missions_Mission::ReadParameter, this , _1 ) );
    const std::string code = symbol.empty() ? " - " : symbol;
    symbol_.SetVector( drawings.GetCategoryDrawings( "tasks" ) );
    symbol_.SetData( drawings.GetDrawing( code ) );
    ReadMissionSheet( missionDir );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::ReadParameter
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadParameter( xml::xistream& input )
{
    std::auto_ptr< ADN_Missions_Parameter > spNew( new ADN_Missions_Parameter() );
    spNew->ReadArchive( input );
    parameters_.AddItem( spNew.release() );
}

namespace
{
    QString BuildDiaMissionType( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        for( int i = 0; i < list.size() - 1; ++i )
            if( list[i].length() > 1 && ( list[i] == list[i].upper() || list[i].lower() == "test" ) )
                list[i].append( "_" );
        return list.join( "" );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::WriteArchive( xml::xostream& output, const std::string& type )
{
    output << xml::start( "mission" );
    bool isAutomat = type == "automats";
    const QString typeName = type == "units" ? "Pion" : (isAutomat ? "Automate" : "Population");
    const QString diaName  = BuildDiaMissionType( strName_.GetData().c_str() );
    if( diaType_.GetData().empty() )
        diaType_ = QString( "T_Mission_%1_%2" ).arg( typeName ).arg( diaName ).toStdString();

    output << xml::attribute( "name", strName_ )
           << xml::attribute( "dia-type", diaType_ )
           << xml::attribute( "id", id_ );

    const std::string code = ( symbol_.GetData() ) ? symbol_.GetData()->GetCode() : "";
    if( code != "" && code != " - " )
        output << xml::attribute( "symbol", code );

    if( ! strPackage_.GetData().empty() )
        output << xml::attribute( "package", strPackage_);

    if( !isAutomat )
    {
        if( diaBehavior_.GetData().empty() )
            diaBehavior_ = QString( "MIS_%1_%2" ).arg( typeName ).arg( diaName ).toStdString();
        output << xml::attribute( "dia-behavior", diaBehavior_ );
    }
    else
    {
        if( cdtDiaBehavior_.GetData().empty() )
            cdtDiaBehavior_ = QString( "MIS_%1_CDT_%2" ).arg( typeName ).arg( diaName ).toStdString();
        if( mrtDiaBehavior_.GetData().empty() )
            mrtDiaBehavior_ = QString( "MIS_%1_MRT_%2" ).arg( typeName ).arg( diaName ).toStdString();
        output << xml::attribute( "mrt-dia-behavior", mrtDiaBehavior_ )
               << xml::attribute( "cdt-dia-behavior", cdtDiaBehavior_ );
    }
    for( unsigned int i = 0; i < parameters_.size(); ++i )
        parameters_[i]->WriteArchive( output );

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::CheckDataConsistency
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::CheckMissionDataConsistency( ADN_ConsistencyChecker& checker, E_EntityType type )
{
   CheckFieldDataConsistency( descriptionContext_.GetData(), checker, type );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
        CheckFieldDataConsistency( (*it)->description_.GetData(), checker, type );
    CheckFieldDataConsistency( descriptionBehavior_.GetData(), checker, type );
    CheckFieldDataConsistency( descriptionSpecific_.GetData(), checker, type );
    CheckFieldDataConsistency( descriptionComment_.GetData(), checker, type );
    CheckFieldDataConsistency( descriptionMissionEnd_.GetData(), checker, type );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::CheckFieldDataConsistency
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::CheckFieldDataConsistency( std::string fieldData, ADN_ConsistencyChecker& checker, E_EntityType type )
{
    boost::smatch match;
    while( boost::regex_search( fieldData, match, boost::regex( "\\$\\$(.*?)\\$\\$(.*)" ) ) )
    {
        if( !IsFileInAttachmentList( match[ 1 ].str() ) )
            checker.AddError( eMissionAttachmentInvalid, strName_.GetData(), eMissions, type , match[ 1 ].str() );
        fieldData = match[ 2 ];
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::ReadMissionSheetDescriptionsParameters
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadMissionSheetParametersDescriptions( xml::xistream& xis )
{
    std::string parameterName;
    std::string parameterData;
    xis >> xml::attribute( "name", parameterName )
            >> xml::list( *this, &ADN_Missions_Mission::FromXmlToWiki, parameterData );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
        if( (*it)->strName_ == parameterName )
            (*it)->description_ = parameterData;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::ReadMissionSheetAttachments
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadMissionSheetAttachments( xml::xistream& xis )
{
    attachments_.AddItem( new ADN_Missions_Attachment( xis.attribute< std::string >( "name" ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::WriteMissionSheetParametersDescriptions
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::WriteMissionSheetParametersDescriptions( xml::xostream& xos )
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
// Name: ADN_Missions_Mission::ReadMissionSheetAttachments
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::WriteMissionSheetAttachments( xml::xostream& xos )
{
    for( IT_MissionAttachment_Vector it = attachments_.begin(); it != attachments_.end(); ++it )
        xos << xml::start( "attachment" )
            << xml::attribute( "name", (*it)->strName_ )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::FromXmlToWiki
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::FromXmlToWiki( const std::string& tag, xml::xistream& xis, std::string& text )
{
    int level = 0;
    if( tag == "line" )
        xis >> xml::list( *this, &ADN_Missions_Mission::ReadXmlLine, text );
    else if( tag == "ul" )
    {
        int sublevel = level + 1;
        xis >> xml::list( *this, &ADN_Missions_Mission::ReadXmlList, text, sublevel );
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
// Name: ADN_Missions_Mission::IsFileInAttachmentList
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
bool ADN_Missions_Mission::IsFileInAttachmentList( const std::string& fileName )
{
    for( IT_MissionAttachment_Vector it = attachments_.begin(); it != attachments_.end(); ++it )
        if(fileName == (*it)->strName_.GetData() )
            return true;
    return false;
}


// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::MakeStringXmlItem
// Created: NPT 2013-01-22
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::MakeStringXmlItem( xml::xostream& xos, std::size_t length, std::string line )
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
                    if( match[ 2 ] == "$$" )
                        if( !IsFileInAttachmentList( match[ 1 ].str() ) )
                        {
                            //return an error
                        }
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
// Name: ADN_Missions_Mission::ReadXmlData
// Created: NPT 2013-01-23
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadXmlLine( const std::string& tag, xml::xistream& xis, std::string& text )
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
        xis >> xml::list( *this, &ADN_Missions_Mission::ReadXmlLine, text );
        text += ConvertXmlToWikiTag( tag );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::ReadXmlList
// Created: NPT 2013-01-23
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadXmlList( const std::string& tag, xml::xistream& xis, std::string& text, int level )
{
    if( tag == "ul" )
    {
        int sublevel = level + 1;
        xis >> xml::list( *this, &ADN_Missions_Mission::ReadXmlList, text, sublevel );
    }
    else if( tag == "li" )
    {
        text += std::string( level, ' ') + "* ";
        xis >> xml::start( "line" )
                >> xml::list( *this, &ADN_Missions_Mission::ReadXmlLine, text )
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
// Name: ADN_Missions_Mission::FromWikiToXml
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::FromWikiToXml( xml::xostream& xos, const std::string& text )
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

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::ReadMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadMissionSheet( const std::string& missionDir )
{
    const std::string fileName = std::string( missionDir +  "/" + strName_.GetData() );
    if( !bfs::is_directory( missionDir ) )
        bfs::create_directories( missionDir );
    
    if( bfs::is_regular_file( fileName + ".xml" ) )
    {
        xml::xifstream xis( fileName + ".xml" );
        std::string descriptionContext;
        std::string descriptionBehavior;
        std::string descriptionSpecific;
        std::string descriptionComment;
        std::string descriptionMissionEnd;
        xis >> xml::start( "mission-sheet" )
            >> xml::optional >> xml::start( "context" )
            >> xml::list( *this, &ADN_Missions_Mission::FromXmlToWiki, descriptionContext )
            >> xml::end
            >> xml::optional >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &ADN_Missions_Mission::ReadMissionSheetParametersDescriptions )
            >> xml::end
            >> xml::optional >> xml::start( "behavior" )
            >> xml::list( *this, &ADN_Missions_Mission::FromXmlToWiki, descriptionBehavior )
            >> xml::end
            >> xml::optional >> xml::start( "specific-cases" )
            >> xml::list( *this, &ADN_Missions_Mission::FromXmlToWiki, descriptionSpecific )
            >> xml::end
            >> xml::optional >> xml::start( "comments" )
            >> xml::list( *this, &ADN_Missions_Mission::FromXmlToWiki, descriptionComment )
            >> xml::end
            >> xml::optional >> xml::start( "mission-end" )
            >> xml::list( *this, &ADN_Missions_Mission::FromXmlToWiki, descriptionMissionEnd )
            >> xml::end
            >> xml::optional >> xml::start( "attachments" )
            >> xml::list( "attachment", *this, &ADN_Missions_Mission::ReadMissionSheetAttachments )
            >> xml::end
            >> xml::end;
        descriptionContext_ = descriptionContext;
        descriptionBehavior_ = descriptionBehavior;
        descriptionSpecific_ = descriptionSpecific;
        descriptionComment_ = descriptionComment;
        descriptionMissionEnd_ = descriptionMissionEnd;
    }
    else
    {
        descriptionContext_ = usage_;
        descriptionBehavior_ = doctrine_;
    }
    if( !bfs::exists( fileName + ".html" ) )
    {
        std::fstream fileStream( fileName + ".html", std::ios::out | std::ios::trunc );
        fileStream.close();
    }
    missionSheetPath_ = fileName + ".html";
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::RemoveMissionSheet
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::RemoveDifferentNamedMissionSheet( const std::string& missionDir )
{
    //xml file
    const std::string newPath = std::string( missionDir + "/" + strName_.GetData() );
    const std::string oldPath = std::string( missionDir + "/" + QFileInfo( missionSheetPath_.GetData().c_str() ).completeBaseName().toStdString() );
    if( !missionSheetPath_.GetData().empty() && newPath != oldPath )
    {
        bfs::remove( oldPath + ".xml" );
        bfs::remove( oldPath + ".html" );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::WriteMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::WriteMissionSheet( const std::string& missionDir )
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

    xml::xifstream xisXML( fileName + ".xml" );
    xsl::xstringtransform xst( QDir::tempPath().toStdString() + "/_adnTempXslt.xsl" );
    xst << xisXML;
    std::fstream fileStream( tools::FromUtf8ToLocalCharset( fileName + ".html" ), std::ios::out | std::ios::trunc );
    fileStream << xst.str();
    fileStream.close();
    missionSheetPath_ = fileName + ".html";
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::AddContextParameter
// Created: ABR 2013-01-07
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::AddContextParameter( E_ContextParameters contextType, E_MissionParameterType parameterType, bool optional, int minOccurs /* = 1 */, int maxOccurs /* = 1 */ )
{
    std::auto_ptr< ADN_Missions_Parameter > spNew( new ADN_Missions_Parameter() );

    spNew->strName_ = ADN_Tr::ConvertFromContextParameters( contextType, ENT_Tr_ABC::eToTr );
    spNew->diaName_ = ADN_Tr::ConvertFromContextParameters( contextType, ENT_Tr_ABC::eToSim );
    spNew->type_ = parameterType;
    spNew->isOptional_ = optional;
    spNew->isContext_ = true;

    if( minOccurs != 1 )
        spNew->minOccurs_ = minOccurs;
    if( maxOccurs != 1 )
        spNew->maxOccurs_ = maxOccurs;

    parameters_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::FillContextParameters
// Created: ABR 2013-01-07
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::FillContextParameters( E_EntityType entityType )
{
    switch( entityType )
    {
    case eEntityType_Pawn:
        AddContextParameter( eContextParameters_Heading,    eMissionParameterTypeDirection, false );
        AddContextParameter( eContextParameters_Limas,      eMissionParameterTypePhaseLine, true, 1, std::numeric_limits< int >::max() );
        AddContextParameter( eContextParameters_Limit1,     eMissionParameterTypeLimit,     true );
        AddContextParameter( eContextParameters_Limit2,     eMissionParameterTypeLimit,     true );
        break;
    case eEntityType_Automat:
        AddContextParameter( eContextParameters_Heading,    eMissionParameterTypeDirection, false );
        AddContextParameter( eContextParameters_Limas,      eMissionParameterTypePhaseLine, true, 1, std::numeric_limits< int >::max() );
        AddContextParameter( eContextParameters_Limit1,     eMissionParameterTypeLimit,     false );
        AddContextParameter( eContextParameters_Limit2,     eMissionParameterTypeLimit,     false );
        break;
    default:
        break;
    }
}
