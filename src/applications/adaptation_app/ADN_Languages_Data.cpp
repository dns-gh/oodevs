// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Languages_Data.h"
#include "ADN_GeneralConfig.h"
#include "ADN_Tools.h"
#include "ADN_Missions_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Workspace.h"
#include "ADN_WorkspaceElement.h"
#include "clients_kernel/Context.h"
#include "clients_kernel/LanguageController.h"
#include "clients_kernel/LocalizedString.h"
#include "tools/Languages.h"

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data constructor
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
ADN_Languages_Data::ADN_Languages_Data()
    : ADN_Data_ABC( eLanguages )
    , allLanguages_( ADN_Workspace::GetWorkspace().GetConfig().GetLanguages() )
{
    tools::Language::SetCurrent( "" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data destructor
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
ADN_Languages_Data::~ADN_Languages_Data()
{
    tools::Language::SetCurrent( "" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::FilesNeeded
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szLanguages_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::ReadArchive
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "languages" )
            >> xml::list( "language", *this, &ADN_Languages_Data::ReadLanguage )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::ReadLanguage
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_Data::ReadLanguage( xml::xistream& input )
{
    const tools::Language& language = allLanguages_.Get( input.attribute( "code", "" ) );
    if( input.attribute( "master", false ) )
        InternalSetMaster( language.GetCode() );
    else
        activeLanguages_.push_back( language );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::WriteArchive
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_Data::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "languages" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Languages" );

    if( !IsMasterEmpty() )
        output << xml::start( "language" )
                 << xml::attribute( "code", master_ )
                 << xml::attribute( "master", true )
               << xml::end;
    for( auto it = activeLanguages_.begin(); it != activeLanguages_.end(); ++it )
        output << xml::start( "language" )
                 << xml::attribute( "code", it->GetCode() )
               << xml::end;
    output << xml::end; //! languages
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::Master
// Created: ABR 2013-08-29
// -----------------------------------------------------------------------------
const std::string& ADN_Languages_Data::Master() const
{
    return master_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::SetMaster
// Created: ABR 2013-10-01
// -----------------------------------------------------------------------------
void ADN_Languages_Data::SetMaster( const std::string& language )
{
    if( master_ == language )
        throw MASA_EXCEPTION( "Can't set master language with the actual master language" );

    for( auto it = activeLanguages_.begin(); it != activeLanguages_.end(); ++it )
        if( it->GetCode() == language )
        {
            activeLanguages_.erase( it );
            break;
        }
    if( !IsMasterEmpty() )
        activeLanguages_.push_back( allLanguages_.Get( master_ ) );
    InternalSetMaster( language );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::ChangeLanguage
// Created: ABR 2013-10-18
// -----------------------------------------------------------------------------
void ADN_Languages_Data::ChangeLanguage( const std::string& language ) const
{
    ADN_Workspace& workspace = ADN_Workspace::GetWorkspace();
    workspace.SetIsSwappingLanguage( true );
    workspace.GetLanguageController().ChangeLanguage( language );
    workspace.SetIsSwappingLanguage( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::InitializeLanguages
// Created: ABR 2013-10-18
// -----------------------------------------------------------------------------
void ADN_Languages_Data::InitializeLanguages() const
{
    std::function< bool ( kernel::LocalizedString& ) > initializer = boost::bind( &kernel::LocalizedString::Initialize, _1, GetActiveLanguages() );
    ADN_Workspace::GetWorkspace().ApplyOnData( boost::bind( &ADN_Data_ABC::ApplyOnTranslations, _1, boost::cref( initializer ) ) );
    ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissionSheetPathContext()->Apply( initializer );
}

namespace
{
    bool SetFinished( kernel::LocalizedString& text, const std::string& language )
    {
        text.SetType( language, kernel::eTranslationType_None );
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::SwapMaster
// Created: ABR 2013-10-08
// -----------------------------------------------------------------------------
void ADN_Languages_Data::SwapMaster()
{
    const std::function< bool ( kernel::LocalizedString& ) > swapper = boost::bind( &kernel::LocalizedString::SwapKey, _1, Master(), tools::Language::Current() ) ;
    ADN_Workspace::GetWorkspace().ApplyOnData( boost::bind( &ADN_Data_ABC::ApplyOnTranslations, _1, boost::cref( swapper ) ) );
    ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissionSheetPathContext()->Apply( boost::bind( &SetFinished, _1, tools::Language::Current() ) );
    ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissionSheetPathContext()->Apply( swapper );

    if( IsMasterEmpty() )
        throw MASA_EXCEPTION ( "Can't swap languages if master is empty" );
    const std::string& current = tools::Language::Current();
    if( master_ == current )
        throw MASA_EXCEPTION( "Can't swap master language with the actual master language" );

    for( auto it = activeLanguages_.begin(); it != activeLanguages_.end(); ++it )
        if( it->GetCode() == current )
        {
            activeLanguages_.erase( it );
            break;
        }
    activeLanguages_.push_back( allLanguages_.Get( master_ ) );
    InternalSetMaster( current );

    ChangeLanguage( Master() );

    ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::InternalSetMaster
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
void ADN_Languages_Data::InternalSetMaster( const std::string& language )
{
    if( IsCurrentMaster() )
        tools::Language::SetCurrent( language );
    master_ = language;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::IsMaster
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
bool ADN_Languages_Data::IsMaster( const std::string& language ) const
{
    return language == master_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::IsMasterEmpty
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
bool ADN_Languages_Data::IsMasterEmpty() const
{
    return master_.empty();
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::IsCurrentMaster
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
bool ADN_Languages_Data::IsCurrentMaster() const
{
    return IsMaster( tools::Language::Current() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::GetAllLanguages
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
const tools::Languages& ADN_Languages_Data::GetAllLanguages() const
{
    return allLanguages_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::GetActiveLanguages
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
const tools::LanguagesVector& ADN_Languages_Data::GetActiveLanguages() const
{
    return activeLanguages_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::PurgeActiveLanguages
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
void ADN_Languages_Data::PurgeActiveLanguages()
{
    activeLanguages_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::AddActiveLanguage
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
void ADN_Languages_Data::AddActiveLanguage( const std::string& language )
{
    activeLanguages_.push_back( allLanguages_.Get( language ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::HasActiveLanguage
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
bool ADN_Languages_Data::HasActiveLanguage( const std::string& language ) const
{
    for( auto it = activeLanguages_.begin(); it != activeLanguages_.end(); ++it )
        if( it->GetCode() == language )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::CleanLocalDirectories
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
void ADN_Languages_Data::CleanLocalDirectories() const
{
    tools::Path::T_Paths localPaths;
    localPaths.push_back( ADN_Workspace::GetWorkspace().GetProject().GetLocalDir() );
    for( int i = 0; i < eNbrMissionType; ++i )
        localPaths.push_back( ADN_Workspace::GetWorkspace().GetProject().GetLocalMissionDir( static_cast< E_MissionType >( i ) ) );

    for( auto itLocalPath = localPaths.begin(); itLocalPath != localPaths.end(); ++itLocalPath )
    {
        tools::Path::T_Paths languagesPaths = itLocalPath->ListDirectories( false );
        for( auto itLanguagePath = languagesPaths.begin(); itLanguagePath != languagesPaths.end(); ++itLanguagePath )
        {
            bool found = false;
            for( auto itLanguage = activeLanguages_.begin(); !found && itLanguage != activeLanguages_.end(); ++itLanguage )
                found = itLanguagePath->ToUTF8() == itLanguage->GetCode();

            if( !found )
                ( *itLocalPath / *itLanguagePath ).RemoveAll();
        }
    }
}
