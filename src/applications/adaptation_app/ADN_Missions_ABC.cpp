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
#include "ADN_Languages_Data.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Languages.h"
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
    , type_( eNbrMissionType )
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
    std::string context = ENT_Tr::ConvertFromMissionType( type_, ENT_Tr::eToSim );
    if( type_ != eMissionType_FragOrder )
        context += "-missions";
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eMissions, context ) );
    missionSheetPath_.SetContext( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissionSheetPathContext() );
    boost::shared_ptr< kernel::Context > missionSheetContext = ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissionSheetContext( type_ );
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
// Name: ADN_Missions_ABC::CheckMissionDataConsistency
// Created: LDC 2014-04-03
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::CheckMissionDataConsistency( ADN_ConsistencyChecker& checker )
{
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->CheckDatabaseValidity( checker );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::CheckDataConsistency
// Created: NPT 2013-01-24
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::CheckMissionDataConsistency( ADN_ConsistencyChecker& checker, const std::string& language )
{
    CheckFieldDataConsistency( descriptionContext_.GetValue( language ), checker );
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        CheckFieldDataConsistency( (*it)->description_.GetValue( language ), checker );
    CheckFieldDataConsistency( descriptionBehavior_.GetValue( language ), checker );
    CheckFieldDataConsistency( descriptionSpecific_.GetValue( language ), checker );
    CheckFieldDataConsistency( descriptionComment_.GetValue( language ), checker );
    CheckFieldDataConsistency( descriptionMissionEnd_.GetValue( language ), checker );
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
    void FromXmlToWikiNoThrow( xml::xistream& xis, std::string& text )
    {
        try
        {
            FromXmlToWiki( xis, text );
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( "xml to wiki conversion failed: " << e.what() );
        }
    }

    void ReadMissionSheetField( xml::xistream& xis, const std::string& xmltag, ADN_Type_LocalizedString& localizedField, const std::string& language )
    {
        std::string field;
        kernel::E_TranslationType type;
        xis >> xml::optional >> xml::start( xmltag )
            >> type;
        FromXmlToWikiNoThrow( xis, field );
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
    FromXmlToWikiNoThrow( xis, parameterData );
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        if( (*it)->strName_.GetValue( language ) == parameterName )
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
    const std::string& oldValue = tools::Path::FromUTF8( missionSheetPath_.GetValue( language ) ).BaseName().ToUTF8();
    if( newValue != oldValue )
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
        FromWikiToXml( xos, ADN_Workspace::GetWorkspace().GetLanguages().GetData().IsMaster( language ) ||
                            isMergedXml && localizedField.GetType( language ) == kernel::eTranslationType_Unfinished
                            ? localizedField.GetKey()
                            : localizedField.GetTranslation()->Value( language ) );
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
void ADN_Missions_ABC::WriteMissionSheetParametersDescriptions( xml::xostream& xos, const std::string& language, bool isMergedXml ) const
{
    xos << xml::start( "parameters" );
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        xos << xml::start( "parameter" )
            << xml::attribute( "name", ( *it )->strName_.GetValue( language ) )
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
void ADN_Missions_ABC::WriteMissionSheetAttachments( xml::xostream& xos ) const
{
    xos << xml::start( "attachments" );
    for( auto it = attachments_.begin(); it != attachments_.end(); ++it )
        xos << xml::start( "attachment" )
                << xml::attribute( "name", **it )
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::InternalWriteMissionSheet
// Created: ABR 2013-09-26
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::InternalWriteMissionSheet( xml::xostream& xos, const std::string& language, bool isMergedXml ) const
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
void ADN_Missions_ABC::WriteMissionSheet( const tools::Path& missionDir, const std::string& language ) const
{
    tools::Path filePath = missionDir / tools::Path::FromUTF8( strName_.GetValue( language ) );
    ( missionDir / ADN_Missions_Data::obsoletePath_ ).CreateDirectories();

    //mission sheet xml creation
    tools::Xofstream xos( filePath + ".xml" );
    InternalWriteMissionSheet( xos, language, false );

    //mission sheet html creation
    xsl::xstringtransform xst( ( tools::Path::TemporaryPath() / ADN_Missions_Data::xslTemporaryFile_ ).ToUTF8() );

    std::string directoryName = strName_.GetKey();
    const tools::Path relativePath = ADN_Workspace::GetWorkspace().GetLanguages().GetData().IsMaster( language ) ? "./" : "./../..";

    const tools::Path imageDirectory = relativePath / ADN_Missions_Data::imagePath_ / tools::Path::FromUTF8( directoryName ) + "/";
    directoryName = imageDirectory.Normalize().ToUTF8();
    boost::replace_all( directoryName, "'", "&#39;" );
    boost::replace_all( directoryName, "\"", "&#34;" );
    boost::replace_all( directoryName, "&", "&#38;" );
    xst.parameter( "imageDirectory", directoryName );

    const tools::Path cssFile = relativePath / ".." / ADN_Missions_Data::cssFile_;
    xst.parameter( "cssFile", cssFile.Normalize().ToUTF8() );

    if( ADN_Workspace::GetWorkspace().GetLanguages().GetData().IsMaster( language ) )
    {
        tools::Xifstream xisXML( filePath + ".xml" );
        xst << xisXML;
    }
    else
    {
        xml::xobufferstream mergedXml;
        InternalWriteMissionSheet( mergedXml, language, true );
        xst << mergedXml;
    }

    tools::Ofstream fileStream( filePath + ".html", std::ios::out | std::ios::trunc );
    fileStream << xst.str();
    fileStream.close();
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

    spNew->strName_ = ADN_Tr::ConvertFromContextParameters( contextType, ENT_Tr::eToTr );
    spNew->diaName_ = ADN_Tr::ConvertFromContextParameters( contextType, ENT_Tr::eToSim );
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
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "dia-type", diaType_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::WriteArchive
// Created: NPT 2013-02-18
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::WriteArchive( xml::xostream& output ) const
{
    output << xml::attribute( "name", *this )
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
    CheckTypeValidity( parameters_ );
}

namespace
{
    QString BuildDiaType( const QString& name, bool isFragOrder )
    {
        QStringList list = QStringList::split( ' ', name );
        if( isFragOrder )
        {
            if( list.front() == "Pion" || list.front() == "Automate" || list.front() == "Population" )
                list[0].append( "_" );
            for( int i = 1; i < list.size() - 1; ++i )
                if( list[i].length() > 1 && list[i] == list[i].upper() )
                    list[i].append( "_" );
            return QString( "Rep_OrderConduite_%1" ).arg( list.join( "" ) );
        }
        for( int i = 0; i < list.size() - 1; ++i )
            if( list[i].length() > 1 && ( list[i] == list[i].upper() || list[i].lower() == "test" ) )
                list[i].append( "_" );
        return list.join( "" );
    }
    bool IsPresent( const std::string& text, const kernel::LocalizedString& localizedText, const tools::LanguagesVector& languages )
    {
        if( text == localizedText.Key() )
            return true;
        for( auto it = languages.begin(); it != languages.end(); ++it )
            if( text == localizedText.Value( it->GetCode() ) )
                return true;
        return false;
    }
    void RemoveIfDeprecated( const std::string& htmlFile, const kernel::LocalizedString& actualPaths, const tools::LanguagesVector& languages )
    {
        if( !IsPresent( htmlFile, actualPaths, languages ) )
        {
            tools::Path file = tools::Path::FromUTF8( htmlFile );
            file.Remove();
            file.ReplaceExtension( ".xml" );
            file.Remove();
        }
    }
    void RemoveDeprecatedMissionSheets( const tools::LanguagesVector& languages, const kernel::LocalizedString& oldPaths, const kernel::LocalizedString& actualPaths )
    {
        tools::Path newMissionSheetPath = tools::Path::FromUTF8( actualPaths.Key() ).Parent();
        if( oldPaths.Key().find( newMissionSheetPath.ToUTF8() ) == std::string::npos )
            return; // performing a save as, we don't want to delete original mission sheets
        RemoveIfDeprecated( oldPaths.Key(), actualPaths, languages );
        for( auto it = languages.begin(); it != languages.end(); ++it )
            RemoveIfDeprecated( oldPaths.Value( it->GetCode() ), actualPaths, languages );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_ABC::FixConsistency
// Created: ABR 2013-10-10
// -----------------------------------------------------------------------------
void ADN_Missions_ABC::FixConsistency()
{
    if( diaType_.GetData().empty() )
        diaType_ = BuildDiaType( strName_.GetData().c_str(), type_ == eMissionType_FragOrder ).toStdString();

    kernel::LocalizedString copy = *missionSheetPath_.GetTranslation();
    const tools::LanguagesVector& languages = ADN_Workspace::GetWorkspace().GetLanguages().GetData().GetActiveLanguages();
    const tools::Path keyFilePath = ADN_Missions_Data::GetMissionSheetsPath( type_ ) / tools::Path::FromUTF8( strName_.GetKey() );
    missionSheetPath_.SetKey( keyFilePath.ToUTF8() + ".html" );
    for( auto it = languages.begin(); it != languages.end(); ++it )
    {
        const tools::Path filePath = ADN_Missions_Data::GetLocalizedMissionSheetsPath( it->GetCode(), ADN_Missions_Data::GetMissionSheetsPath( type_ ) ) / tools::Path::FromUTF8( strName_.GetValue( it->GetCode() ) );
        missionSheetPath_.SetValue( it->GetCode(), filePath.ToUTF8() + ".html" );
    }
    RemoveDeprecatedMissionSheets( languages, copy, *missionSheetPath_.GetTranslation() );
}
