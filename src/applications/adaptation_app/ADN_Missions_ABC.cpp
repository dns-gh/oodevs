// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_ABC.h"

#include "ADN_Missions_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_ConsistencyChecker.h"
#include <tools/Loader_ABC.h>
#include <tools/EncodingConverter.h>
#include <boost/regex.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include "clients_gui/WikiXmlConverter.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <queue>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

namespace bfs = boost::filesystem;


// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::Mission
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
ADN_Missions_ABC::ADN_Missions_ABC()
    : id_( ADN_Missions_Data::idManager_.GetNextId() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_ABC::ADN_Missions_ABC( unsigned int id )
    : id_( id )
{
    ADN_Missions_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::~Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_ABC::~ADN_Missions_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::GetItemName
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
std::string ADN_Missions_ABC::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::ReadParameter
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ReadParameter( xml::xistream& input )
{
    std::auto_ptr< ADN_Missions_Parameter > spNew( new ADN_Missions_Parameter() );
    spNew->ReadArchive( input );
    parameters_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::CheckDataConsistency
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::CheckMissionDataConsistency( ADN_ConsistencyChecker& checker, E_EntityType type )
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
// Name: ADN_Missions_ABC::CheckFieldDataConsistency
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::CheckFieldDataConsistency( std::string fieldData, ADN_ConsistencyChecker& checker, E_EntityType type )
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
// Name: ADN_Missions_ABC::ReadMissionSheetDescriptionsParameters
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ReadMissionSheetParametersDescriptions( xml::xistream& xis )
{
    std::string parameterName;
    std::string parameterData;
    xml::xisubstream sub( xis );
    FromXmlToWiki( sub, parameterData );
    xis >> xml::attribute( "name", parameterName );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
        if( (*it)->strName_ == parameterName )
        {
            (*it)->description_ = parameterData;
            break;
        }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::ReadMissionSheetAttachments
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ReadMissionSheetAttachments( xml::xistream& xis )
{
    attachments_.AddItem( new ADN_Missions_Attachment( xis.attribute< std::string >( "name" ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::WriteMissionSheetParametersDescriptions
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::WriteMissionSheetParametersDescriptions( xml::xostream& xos )
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
// Name: ADN_Missions_ABC::ReadMissionSheetAttachments
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::WriteMissionSheetAttachments( xml::xostream& xos )
{
    for( IT_MissionAttachment_Vector it = attachments_.begin(); it != attachments_.end(); ++it )
        xos << xml::start( "attachment" )
        << xml::attribute( "name", (*it)->strName_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::IsFileInAttachmentList
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
bool ADN_Missions_ABC::IsFileInAttachmentList( const std::string& fileName )
{
    for( IT_MissionAttachment_Vector it = attachments_.begin(); it != attachments_.end(); ++it )
        if(fileName == (*it)->strName_.GetData() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::ReadMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ReadMissionSheet( const std::string& missionDir )
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
            >> xml::optional >> xml::start( "context" );
        FromXmlToWiki( xis, descriptionContext );
        xis     >> xml::end
            >> xml::optional >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &ADN_Missions_ABC::ReadMissionSheetParametersDescriptions )
            >> xml::end
            >> xml::optional >> xml::start( "behavior" );
        FromXmlToWiki( xis, descriptionBehavior );
        xis     >> xml::end
            >> xml::optional >> xml::start( "specific-cases" );
        FromXmlToWiki( xis, descriptionSpecific );
        xis     >> xml::end
            >> xml::optional >> xml::start( "comments" );
        FromXmlToWiki( xis, descriptionComment );
        xis     >> xml::end
            >> xml::optional >> xml::start( "mission-end" );
        FromXmlToWiki( xis, descriptionMissionEnd );
        xis     >> xml::end
            >> xml::optional >> xml::start( "attachments" )
            >> xml::list( "attachment", *this, &ADN_Missions_ABC::ReadMissionSheetAttachments )
            >> xml::end
            >> xml::end;
        descriptionContext_ = descriptionContext;
        descriptionBehavior_ = descriptionBehavior;
        descriptionSpecific_ = descriptionSpecific;
        descriptionComment_ = descriptionComment;
        descriptionMissionEnd_ = descriptionMissionEnd;
    }
    missionSheetPath_ = fileName + ".html";
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::RemoveMissionSheet
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::RemoveDifferentNamedMissionSheet( const std::string& missionDir )
{
    const std::string newPath = std::string( missionDir + "/" + strName_.GetData() );
    const std::string oldPath = std::string( missionDir + "/" + QFileInfo( missionSheetPath_.GetData().c_str() ).completeBaseName().toStdString() );
    if( !missionSheetPath_.GetData().empty() && newPath != oldPath )
    {
        bfs::remove( oldPath + ".xml" );
        bfs::remove( oldPath + ".html" );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::WriteMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::WriteMissionSheet( const std::string& missionDir )
{
    std::string fileName = std::string( missionDir + "/" + strName_.GetData() );
    if( !bfs::is_directory( missionDir + "/obsolete" ) )
        bfs::create_directories( missionDir + "/obsolete" );
    if( !IsEmptyMissionSheet() )
    {
        //mission sheet xml creation
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

        //mission sheet html creation
        xml::xifstream xisXML( fileName + ".xml" );
        xsl::xstringtransform xst( QDir::tempPath().toStdString() + "/_adnTempXslt.xsl" );
        xst << xisXML;
        std::fstream fileStream( tools::FromUtf8ToLocalCharset( fileName + ".html" ), std::ios::out | std::ios::trunc );
        fileStream << xst.str();
        fileStream.close();
        if( missionSheetPath_.GetData().empty() )
            missionSheetPath_ = fileName + ".html";
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::IsEmptyParameterList
// Created: NPT 2013-02-04
// -----------------------------------------------------------------------------
bool ADN_Missions_ABC::IsEmptyParameterList()
{
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
        if( !(*it)->description_.GetData().empty() )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::IsEmptyMissionSheet
// Created: NPT 2013-02-04
// -----------------------------------------------------------------------------
bool ADN_Missions_ABC::IsEmptyMissionSheet()
{
    return  descriptionContext_.GetData().empty()
        && descriptionBehavior_.GetData().empty()
        && descriptionMissionEnd_.GetData().empty()
        && descriptionSpecific_.GetData().empty()
        && descriptionComment_.GetData().empty()
        && IsEmptyParameterList();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::AddContextParameter
// Created: ABR 2013-01-07
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::AddContextParameter( E_ContextParameters contextType, E_MissionParameterType parameterType, bool optional, int minOccurs /* = 1 */, int maxOccurs /* = 1 */ )
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
// Name: ADN_Missions_ABC::ReadArchive
// Created: NPT 2013-02-14
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ReadArchive( xml::xistream& /*input*/, const std::string& /*missionDir*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::FillContextParameters
// Created: ABR 2013-01-07
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::FillContextParameters( E_EntityType entityType )
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