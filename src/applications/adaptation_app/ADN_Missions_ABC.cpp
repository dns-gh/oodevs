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
#include "clients_gui/WikiXmlConverter.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>
#include <queue>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::Mission
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
ADN_Missions_ABC::ADN_Missions_ABC()
    : id_( ADN_Missions_Data::idManager_.GetNextId() )
    ,needSheetSaving_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_ABC::ADN_Missions_ABC( unsigned int id )
    : id_( id )
    , needSheetSaving_ ( false )
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
void ADN_Missions_ABC::CheckMissionDataConsistency( ADN_ConsistencyChecker& checker, E_MissionType type )
{
    CheckFieldDataConsistency( descriptionContext_.GetData(), checker, type );
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
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
void ADN_Missions_ABC::CheckFieldDataConsistency( std::string fieldData, ADN_ConsistencyChecker& checker, E_MissionType type )
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
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
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
    attachments_.AddItem( new ADN_Missions_Attachment( xis.attribute< tools::Path >( "name" ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::WriteMissionSheetParametersDescriptions
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::WriteMissionSheetParametersDescriptions( xml::xostream& xos )
{
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        xos << xml::start( "parameter" )
            << xml::attribute( "name", (*it)->strName_ )
            << xml::attribute( "optional", (*it)->isOptional_ );
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
    for( auto it = attachments_.begin(); it != attachments_.end(); ++it )
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
    for( auto it = attachments_.begin(); it != attachments_.end(); ++it )
        if( fileName == (*it)->strName_.GetData() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::ReadMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ReadMissionSheet( const tools::Path& missionDir )
{
    const tools::Path fileName = missionDir / tools::Path::FromUTF8( strName_.GetData() );
    if( !missionDir.IsDirectory() )
        missionDir.CreateDirectories();

    if( ( fileName + ".xml" ).IsRegularFile() )
    {
        tools::Xifstream xis( fileName + ".xml" );
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
        tools::Path namePath = missionDir / "Images" / tools::Path::FromUTF8( QString( strName_.GetData().c_str() ).replace( "\'", " " ).toStdString() );
        ParseImagesInImageDirectory( namePath );
    }
    missionSheetPath_ = fileName.ToUTF8() + ".html";
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::RemoveMissionSheet
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::RenameDifferentNamedMissionSheet( const tools::Path& missionDir )
{
    const tools::Path newPath = missionDir / tools::Path::FromUTF8( strName_.GetData() );
    const tools::Path oldPath = missionDir / tools::Path::FromUTF8( missionSheetPath_.GetData() ).BaseName();
    if( !missionSheetPath_.GetData().empty() && newPath != oldPath )
    {
        if( ( oldPath + ".xml" ).Exists() && ( oldPath + ".html" ).Exists() )
        {
            ( oldPath + ".xml" ).Rename( newPath + ".xml" );
            ( oldPath + ".html" ).Rename( newPath + ".html" );
            missionSheetPath_ = newPath.ToUTF8() + ".html";
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::WriteMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
bool ADN_Missions_ABC::WriteMissionSheet( const tools::Path& missionDir, const tools::Path& fileName, int type )
{
    bool result = false;
    tools::Path filePath = missionDir / fileName;
    if( !( missionDir / "obsolete" ).IsDirectory() )
        ( missionDir / "obsolete" ).CreateDirectories();
    if( !IsEmptyMissionSheet() || fileName == ADN_Missions_Data::missionSheetTemporaryFile_ )
    {
        //mission sheet xml creation
        tools::Xofstream xos( filePath + ".xml" );
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
        tools::Xifstream xisXML( filePath + ".xml" );
        xsl::xstringtransform xst( ( tools::Path::TemporaryPath() / ADN_Missions_Data::xslTemporaryFile_ ).ToUTF8() );
        QString directoryName = ( fileName == ADN_Missions_Data::missionSheetTemporaryFile_ )? strName_.GetData().c_str() : fileName.ToUTF8().c_str();
        tools::Path namePath =  tools::Path::FromUTF8( directoryName.replace( "\'", " " ).toStdString() );
        if( fileName == ADN_Missions_Data::missionSheetTemporaryFile_ )
        {
            tools::Path parameterValue = ( tools::Path::TemporaryPath() / ADN_Missions_Data::imageTemporaryPath_ + boost::lexical_cast< std::string >( type ).c_str() ) / namePath + "/";
            xst.parameter( "imageDirectory", parameterValue.Normalize().ToBoost().string() );
        }
        else
        {
            tools::Path parameterValue = tools::Path( "Images" ) / namePath + "/";
            xst.parameter( "imageDirectory", parameterValue.Normalize().ToBoost().string() );
        }
        xst << xisXML;
        tools::Ofstream fileStream( filePath + ".html", std::ios::out | std::ios::trunc );
        fileStream << xst.str();
        fileStream.close();
        if( missionSheetPath_.GetData().empty() )
            missionSheetPath_ = filePath.ToUTF8() + ".html";
        result = true;
    }
    if( fileName.ToUTF8() == strName_.GetData() )
    {
        needSheetSaving_ = false;
        tools::Path tempXML = missionDir / ADN_Missions_Data::missionSheetTemporaryFile_ + ".xml";
        tools::Path tempHTML = missionDir / ADN_Missions_Data::missionSheetTemporaryFile_ + ".html";
        if( tempXML.Exists() || tempHTML.Exists() )
        {
            tempXML.Remove();
            tempHTML.Remove();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::IsEmptyParameterList
// Created: NPT 2013-02-04
// -----------------------------------------------------------------------------
bool ADN_Missions_ABC::IsEmptyParameterList()
{
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
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
// Name: ADN_Missions_ABC::NeedsSaving
// Created: NPT 2013-03-01
// -----------------------------------------------------------------------------
bool ADN_Missions_ABC::NeedsSaving()
{
    return needSheetSaving_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::SetNeedsSaving
// Created: NPT 2013-03-01
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::SetNeedsSaving( bool saving )
{
    needSheetSaving_ = saving;
}

namespace
{
    bool IsAnImageExtension( const tools::Path& extension )
    {
        return extension == ".bmp"
            || extension == ".jpg"
            || extension == ".jpeg"
            || extension == ".gif"
            || extension == ".png";
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::ParseImagesInImageDirectory
// Created: NPT 2013-05-29
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ParseImagesInImageDirectory( const tools::Path& imageDir )
{
    if( !imageDir.Exists() )
        return;

    for( auto it = imageDir.begin(); it != imageDir.end(); ++it )
    {
        const tools::Path& child = *it;
        if( child.IsRegularFile() && IsAnImageExtension( child.Extension() ) && !IsFileInAttachmentList( child.FileName().ToUTF8() ) )
            attachments_.AddItem( new ADN_Missions_Attachment( child.FileName() ) );
    }
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
void ADN_Missions_ABC::ReadArchive( xml::xistream& input, const tools::Path& )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "dia-type", diaType_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::WriteArchive
// Created: NPT 2013-02-18
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::WriteArchive( xml::xostream& output, E_MissionType )
{
    output << xml::attribute( "name", strName_ )
           << xml::attribute( "dia-type", diaType_ )
           << xml::attribute( "id", id_ );
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