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
#include <tools/Loader_ABC.h>
#include <tools/EncodingConverter.h>
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
    while( boost::regex_search( fieldData, match, boost::regex( "\\$\\$(.*?)\\$\\$(.*)" ) ) )
    {
        if( !IsFileInAttachmentList( match[ 1 ].str() ) )
            checker.AddError( eMissionAttachmentInvalid, strName_.GetData(), eMissions, eNbrEntityTypes , match[ 1 ].str() );
        fieldData = match[ 2 ];
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
// Name: ADN_Missions_FragOrder::ReadMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::ReadMissionSheet( const std::string& missionDir )
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
        xis >> xml::end
            >> xml::optional >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &ADN_Missions_FragOrder::ReadMissionSheetParametersDescriptions )
            >> xml::end
            >> xml::start( "behavior" );
            FromXmlToWiki( xis, descriptionBehavior );
        xis >> xml::end
            >> xml::optional >> xml::start( "specific-cases" );
            FromXmlToWiki( xis, descriptionSpecific );
        xis >> xml::end
            >> xml::optional >> xml::start( "comments" );
            FromXmlToWiki( xis, descriptionComment );
        xis >> xml::end
            >> xml::optional >> xml::start( "mission-end" );
            FromXmlToWiki( xis, descriptionMissionEnd );
        xis >> xml::end
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
    missionSheetPath_ = fileName + ".html";
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::RemoveMissionSheet
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_FragOrder::RemoveDifferentNamedMissionSheet( const std::string& missionDir )
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

    xml::xifstream xisXML( fileName + ".xml" );
    xsl::xstringtransform xst( QDir::tempPath().toStdString() + "/_adnTempXslt.xsl" );
    xst << xisXML;
    std::fstream fileStream( tools::FromUtf8ToLocalCharset( fileName + ".html" ), std::ios::out | std::ios::trunc );
    fileStream << xst.str();
    fileStream.close();
    if( missionSheetPath_.GetData().empty() )
        missionSheetPath_ = fileName + ".html";
}
