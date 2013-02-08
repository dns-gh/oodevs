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
#include <boost/regex.hpp>
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
            >> xml::optional >> xml::attribute( "usage", usage_ )
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
                >> xml::optional >> xml::start( "context" );
                FromXmlToWiki( xis, descriptionContext );
        xis     >> xml::end
                >> xml::optional >> xml::start( "parameters" )
                >> xml::list( "parameter", *this, &ADN_Missions_Mission::ReadMissionSheetParametersDescriptions )
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
    missionSheetPath_ = fileName + ".html";
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::RemoveMissionSheet
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::RemoveDifferentNamedMissionSheet( const std::string& missionDir )
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
// Name: ADN_Missions_Mission::WriteMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::WriteMissionSheet( const std::string& missionDir )
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
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::IsEmptyParameterList
// Created: NPT 2013-02-04
// -----------------------------------------------------------------------------
bool ADN_Missions_Mission::IsEmptyParameterList()
{
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
        if( !(*it)->description_.GetData().empty() )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::IsEmptyMissionSheet
// Created: NPT 2013-02-04
// -----------------------------------------------------------------------------
bool ADN_Missions_Mission::IsEmptyMissionSheet()
{
    return  descriptionContext_.GetData().empty()
            && descriptionBehavior_.GetData().empty()
            && descriptionMissionEnd_.GetData().empty()
            && descriptionSpecific_.GetData().empty()
            && descriptionComment_.GetData().empty()
            && IsEmptyParameterList();
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
