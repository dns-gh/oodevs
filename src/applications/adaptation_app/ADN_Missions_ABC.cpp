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
#include "clients_gui/WikiXmlConverter.h"
#include "clients_kernel/Language.h"
#include "ENT/ENT_Tr.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <xeuseuleu/xsl.hpp>

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::Mission
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
ADN_Missions_ABC::ADN_Missions_ABC()
    : id_( ADN_Missions_Data::idManager_.GetNextId() )
    , needSheetSaving_( false )
    , type_( eNbrMissionTypes )
{
    assert( false ); // $$$$ ABR 2013-08-23: useless constructor, needed by ADN_Wizard...
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC constructor
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
ADN_Missions_ABC::ADN_Missions_ABC( E_MissionType type )
    : id_( ADN_Missions_Data::idManager_.GetNextId() )
    , type_( type )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_ABC::ADN_Missions_ABC( E_MissionType type, unsigned int id )
    : id_( id )
    , needSheetSaving_ ( false )
    , type_( type )
{
    Initialize();
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
// Name: ADN_Missions_ABC::Initialize
// Created: ABR 2013-08-28
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::Initialize()
{
    std::string context = ENT_Tr::ConvertFromMissionType( type_, ENT_Tr_ABC::eToSim );
    if( type_ != eMissionType_FragOrder )
        context += "-missions";
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eMissions, context ) );
    boost::shared_ptr< kernel::Context > missionSheetContext = ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissionSheetContext( type_ );
    missionSheetPath_.SetContext( missionSheetContext );
    descriptionContext_.SetContext( missionSheetContext );
    descriptionBehavior_.SetContext( missionSheetContext );
    descriptionSpecific_.SetContext( missionSheetContext );
    descriptionComment_.SetContext( missionSheetContext );
    descriptionMissionEnd_.SetContext( missionSheetContext );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::ReadParameter
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ReadParameter( xml::xistream& input )
{
    std::auto_ptr< ADN_Missions_Parameter > spNew( new ADN_Missions_Parameter( type_ ) );
    spNew->ReadArchive( input );
    parameters_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::CheckDataConsistency
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::CheckMissionDataConsistency( ADN_ConsistencyChecker& checker, const std::string& language )
{
    CheckFieldDataConsistency( descriptionContext_.GetTranslation()->Value( language ), checker );
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        CheckFieldDataConsistency( (*it)->description_.GetTranslation()->Value( language ), checker );
    CheckFieldDataConsistency( descriptionBehavior_.GetTranslation()->Value( language ), checker );
    CheckFieldDataConsistency( descriptionSpecific_.GetTranslation()->Value( language ), checker );
    CheckFieldDataConsistency( descriptionComment_.GetTranslation()->Value( language ), checker );
    CheckFieldDataConsistency( descriptionMissionEnd_.GetTranslation()->Value( language ), checker );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::CheckFieldDataConsistency
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::CheckFieldDataConsistency( const std::string& fieldData, ADN_ConsistencyChecker& checker )
{
    std::string str = fieldData;
    boost::smatch match;
    while( boost::regex_search( str, match, boost::regex( "\\$\\$(.*?)\\$\\$(.*)" ) ) )
    {
        if( !IsFileInAttachmentList( match[ 1 ].str() ) )
            checker.AddError( eMissionAttachmentInvalid, strName_.GetData(), eMissions, type_, match[ 1 ].str() );
        str = match[ 2 ];
    }
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

namespace
{
    void ReadMissionSheetField( xml::xistream& xis, const std::string& xmltag, ADN_Type_LocalizedString& localizedField, const std::string& language )
    {
        std::string field;
        kernel::E_TranslationType type;
        xis >> xml::optional >> xml::start( xmltag )
            >> type;
        FromXmlToWiki( xis, field );
        xis >> xml::end;
        localizedField.SetValue( language, field );
        localizedField.SetType( language, type );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::ReadMissionSheetDescriptionsParameters
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ReadMissionSheetParametersDescriptions( xml::xistream& xis, const std::string& language )
{
    kernel::E_TranslationType type;
    std::string parameterData;
    std::string parameterName;
    xis >> xml::attribute( "name", parameterName )
        >> type;
    FromXmlToWiki( xis, parameterData );
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        if( (*it)->strName_ == parameterName )
        {
            (*it)->description_.SetValue( language, parameterData );
            (*it)->description_.SetType( language, type );
            break;
        }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::ReadMissionSheetAttachments
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ReadMissionSheetAttachments( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name" );
    auto it = std::find_if( attachments_.begin(), attachments_.end(), ADN_Tools::NameCmp( name ) );
    if( it == attachments_.end() )
        attachments_.AddItem( new ADN_Missions_Attachment( tools::Path::FromUTF8( name ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::ReadMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::ReadMissionSheet( const tools::Path& missionDir, const std::string& language )
{
    tools::Path fileName = missionDir / tools::Path::FromUTF8( strName_.GetValue( language ) );

    missionDir.CreateDirectories();

    if( ( fileName + ".xml" ).IsRegularFile() )
    {
        tools::Xifstream xis( fileName + ".xml" );

        xis >> xml::start( "mission-sheet" )
                >> xml::optional >> xml::start( "parameters" )
                    >> xml::list( "parameter", boost::bind( &ADN_Missions_ABC::ReadMissionSheetParametersDescriptions, this, _1, boost::cref( language ) ) )
                >> xml::end
                >> xml::optional >> xml::start( "attachments" )
                    >> xml::list( "attachment", *this, &ADN_Missions_ABC::ReadMissionSheetAttachments )
                >> xml::end;
        ReadMissionSheetField( xis, "context", descriptionContext_, language );
        ReadMissionSheetField( xis, "behavior", descriptionBehavior_, language );
        ReadMissionSheetField( xis, "specific-cases", descriptionSpecific_, language );
        ReadMissionSheetField( xis, "comments", descriptionComment_, language );
        ReadMissionSheetField( xis, "mission-end", descriptionMissionEnd_, language );
        xis >> xml::end; //! mission-sheet

        const tools::Path namePath = missionDir / "Images" / tools::Path::FromUTF8( QString( strName_.GetKey().c_str() ).replace( "\'", " " ).toStdString() );
        ParseImagesInImageDirectory( namePath );
    }
    missionSheetPath_.SetValue( language, fileName.ToUTF8() + ".html" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::RemoveMissionSheet
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::RenameDifferentNamedMissionSheet( const tools::Path& missionDir, const std::string& language )
{
    const std::string& newValue = strName_.GetValue( language );
    const std::string& oldValue = tools::Path::FromUTF8( missionSheetPath_.GetTranslation()->Value( language ) ).BaseName().ToUTF8();
    if( !oldValue.empty() && newValue != oldValue )
    {
        const tools::Path newPath = missionDir / tools::Path::FromUTF8( newValue );
        const tools::Path oldPath = missionDir / tools::Path::FromUTF8( oldValue );
        if( ( oldPath + ".xml" ).Exists() && ( oldPath + ".html" ).Exists() )
        {
            ( oldPath + ".xml" ).Rename( newPath + ".xml" );
            ( oldPath + ".html" ).Rename( newPath + ".html" );
            missionSheetPath_.SetValue( language, newPath.ToUTF8() + ".html" );
        }
    }
}

namespace
{
    void LocalizedFromWikiToXml( xml::xostream& xos, const ADN_Type_LocalizedString& localizedField, const std::string& language, bool isMergedXml )
    {
        xos << localizedField.GetType( language );
        FromWikiToXml( xos, isMergedXml && localizedField.GetType( language ) == kernel::eTranslationType_Unfinished ? localizedField.GetKey() : localizedField.GetTranslation()->Value( language ) );
    }
    void WriteMissionSheetField( xml::xostream& xos, const std::string& xmltag, const ADN_Type_LocalizedString& localizedField, const std::string& language, bool isMergedXml )
    {
        xos << xml::start( xmltag );
        LocalizedFromWikiToXml( xos, localizedField, language, isMergedXml );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::WriteMissionSheetParametersDescriptions
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::WriteMissionSheetParametersDescriptions( xml::xostream& xos, const std::string& language, bool isMergedXml )
{
    xos << xml::start( "parameters" );
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        xos << xml::start( "parameter" )
            << xml::attribute( "name", ( *it )->strName_ )
            << xml::attribute( "optional", ( *it )->isOptional_ );
        LocalizedFromWikiToXml( xos, ( *it )->description_, language, isMergedXml );
        xos << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::WriteMissionSheetAttachments
// Created: NPT 2013-01-21
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::WriteMissionSheetAttachments( xml::xostream& xos )
{
    xos << xml::start( "attachments" );
    for( auto it = attachments_.begin(); it != attachments_.end(); ++it )
        xos << xml::start( "attachment" )
        << xml::attribute( "name", (*it)->strName_ )
        << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::InternalWriteMissionSheet
// Created: ABR 2013-09-26
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::InternalWriteMissionSheet( xml::xostream& xos, const std::string& language, bool isMergedXml )
{
    xos << xml::start( "mission-sheet" )
        << xml::attribute( "name", strName_.GetValue( language ) );
    WriteMissionSheetField( xos, "context", descriptionContext_, language, isMergedXml );
    WriteMissionSheetParametersDescriptions( xos, language, isMergedXml );
    WriteMissionSheetField( xos, "behavior", descriptionBehavior_, language, isMergedXml );
    WriteMissionSheetField( xos, "specific-cases", descriptionSpecific_, language, isMergedXml );
    WriteMissionSheetField( xos, "comments", descriptionComment_, language, isMergedXml );
    WriteMissionSheetField( xos, "mission-end", descriptionMissionEnd_, language, isMergedXml );
    WriteMissionSheetAttachments( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::WriteMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::WriteMissionSheet( const tools::Path& missionDir, const std::string& language )
{
    tools::Path filePath = missionDir / tools::Path::FromUTF8( strName_.GetValue( language ) );
    ( missionDir / ADN_Missions_Data::obsoletePath_ ).CreateDirectories();

    //mission sheet xml creation
    tools::Xofstream xos( filePath + ".xml" );
    InternalWriteMissionSheet( xos, language, false );

    //mission sheet html creation
    xsl::xstringtransform xst( ( tools::Path::TemporaryPath() / ADN_Missions_Data::xslTemporaryFile_ ).ToUTF8() );

    std::string directoryName = strName_.GetKey();
    boost::replace_all( directoryName, "\'", " " );
    const tools::Path relativePath = kernel::Language::IsDefault( language ) ? "./" : "./../..";

    const tools::Path imageDirectory = relativePath / ADN_Missions_Data::imagePath_ / tools::Path::FromUTF8( directoryName ) + "/";
    xst.parameter( "imageDirectory", imageDirectory.Normalize().ToUTF8() );

    const tools::Path cssFile = relativePath / ".." / ADN_Missions_Data::cssFile_;
    xst.parameter( "cssFile", cssFile.Normalize().ToUTF8() );

    if( kernel::Language::IsDefault( language ) )
    {
        tools::Xifstream xisXML( filePath + ".xml" );
        xst << xisXML;
    }
    else
    {
        xml::xobufferstream mergedXml;
        InternalWriteMissionSheet( mergedXml, languageId, true );
        xst << mergedXml;
    }

    tools::Ofstream fileStream( filePath + ".html", std::ios::out | std::ios::trunc );
    fileStream << xst.str();
    fileStream.close();
    if( missionSheetPath_.GetTranslation()->Value( language ).empty() )
        missionSheetPath_.SetValue( language, filePath.ToUTF8() + ".html" );
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
    std::auto_ptr< ADN_Missions_Parameter > spNew( new ADN_Missions_Parameter( type_ ) );

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
void ADN_Missions_ABC::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "dia-type", diaType_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::WriteArchive
// Created: NPT 2013-02-18
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::WriteArchive( xml::xostream& output )
{
    output << xml::attribute( "name", strName_ )
           << xml::attribute( "dia-type", diaType_ )
           << xml::attribute( "id", id_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::FillContextParameters
// Created: ABR 2013-01-07
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::FillContextParameters()
{
    switch( type_ )
    {
    case eMissionType_Pawn:
        AddContextParameter( eContextParameters_Heading,    eMissionParameterTypeDirection, false );
        AddContextParameter( eContextParameters_Limas,      eMissionParameterTypePhaseLine, true, 1, std::numeric_limits< int >::max() );
        AddContextParameter( eContextParameters_Limit1,     eMissionParameterTypeLimit,     true );
        AddContextParameter( eContextParameters_Limit2,     eMissionParameterTypeLimit,     true );
        break;
    case eMissionType_Automat:
        AddContextParameter( eContextParameters_Heading,    eMissionParameterTypeDirection, false );
        AddContextParameter( eContextParameters_Limas,      eMissionParameterTypePhaseLine, true, 1, std::numeric_limits< int >::max() );
        AddContextParameter( eContextParameters_Limit1,     eMissionParameterTypeLimit,     false );
        AddContextParameter( eContextParameters_Limit2,     eMissionParameterTypeLimit,     false );
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::CheckValidity
// Created: ABR 2013-09-26
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::CheckValidity()
{
    ADN_RefWithLocalizedName::CheckValidity();
    CheckTypeValidity( descriptionContext_ );
    CheckTypeValidity( descriptionBehavior_ );
    CheckTypeValidity( descriptionSpecific_ );
    CheckTypeValidity( descriptionComment_ );
    CheckTypeValidity( descriptionMissionEnd_ );
    parameters_.CheckValidity();
}
