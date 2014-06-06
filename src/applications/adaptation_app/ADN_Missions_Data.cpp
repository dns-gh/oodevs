// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_Data.h"
#include "moc_ADN_Missions_Data.cpp"
#include "ADN_ConsistencyChecker.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Tr.h"
#include "ADN_enums.h"
#include "ADN_Languages_Data.h"
#include "ADN_WorkspaceElement.h"

#include "clients_gui/Tools.h"
#include "clients_kernel/Context.h"
#include "clients_kernel/XmlTranslations.h"
#include "ENT/ENT_Tr.h"
#include "tools/Language.h"
#include "tools/Loader_ABC.h"

#include <xeuseuleu/xsl.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

tools::IdManager ADN_Missions_Data::idManager_;
tools::Path ADN_Missions_Data::imagePath_ = "Images";
tools::Path ADN_Missions_Data::temporaryPath_ = "tempMissionSheets";
tools::Path ADN_Missions_Data::xslTemporaryFile_ = "_adnTempXslt.xsl";
tools::Path ADN_Missions_Data::cssFile_ = "missionSheets.css";
tools::Path ADN_Missions_Data::obsoletePath_ = "obsolete";

namespace
{
    tools::Path::T_Paths toDeleteMissionSheets;

    template< typename T >
    void InitializeMissions( const ADN_Type_Vector_ABC< T >& vector )
    {
        for( auto itMission = vector.begin(); itMission != vector.end(); ++itMission )
        {
            T* mission = *itMission;
            mission->strName_ = tools::SanitizeFileName( QString::fromStdString( mission->strName_.GetData() ) ).ToUTF8();
        }
    }
    void PurgePath( const tools::Path& path )
    {
        try
        {
            path.RemoveAll();
        }
        catch( ... )
        {
            // NOTHING
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_Data()
    : ADN_Data_ABC( eMissions )
    , missionSheetPathContext_( boost::make_shared< kernel::Context >() )
{
    // $$$$ ABR 2013-04-23: Must be in E_MissionType order
    AddMissionType( "units" );
    AddMissionType( "automats" );
    AddMissionType( "populations" );
    AddMissionType( "fragorders" );
    // $$$$ ABR 2013-09-20: It would be great to synchronize xml tags (units, automats, etc.) with E_MissionType eToSim translation used for object name and translation context (unit, automat, etc.)
    // to avoid this kind of list, and allow us to iterate over E_MissionType
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::~ADN_Missions_Data()
{
    PurgePath( GetTemporaryPath() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::AddMissionType
// Created: ABR 2013-09-20
// -----------------------------------------------------------------------------
void ADN_Missions_Data::AddMissionType( const std::string& xmltag )
{
    missionsVector_.push_back( std::make_pair( xmltag, ADN_Missions_Data::T_Mission_Vector() ) );
    missionsVector_.back().second.AddUniquenessChecker( eError, ADN_Missions_Data::duplicateName_, &ADN_Tools::NameExtractor );
    missionSheetContexts_.push_back( boost::make_shared< kernel::Context >() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetMissionSheetsPath
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
tools::Path ADN_Missions_Data::GetMissionSheetsPath( int index )
{
    return ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( static_cast< E_MissionType >( index ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetLocalizedMissionSheetsPath
// Created: ABR 2013-10-18
// -----------------------------------------------------------------------------
tools::Path ADN_Missions_Data::GetLocalizedMissionSheetsPath( const std::string& language, const tools::Path& basePath )
{
    return ADN_Workspace::GetWorkspace().GetLanguages().GetData().IsMaster( language )
        ? basePath
        : basePath / ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szLocalesDirectory_ / tools::Path::FromUTF8( language );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetMissionSheetsImagesPath
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
tools::Path ADN_Missions_Data::GetMissionSheetsImagesPath( int index )
{
    return GetMissionSheetsPath( index ) / imagePath_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetCssFile
// Created: ABR 2013-08-28
// -----------------------------------------------------------------------------
tools::Path ADN_Missions_Data::GetCssFile()
{
    return GetMissionSheetsPath( 0 ) / ".." / cssFile_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetTemporaryPath
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
tools::Path ADN_Missions_Data::GetTemporaryPath()
{
    return tools::Path::TemporaryPath() / ADN_Missions_Data::temporaryPath_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetTemporaryPath
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
tools::Path ADN_Missions_Data::GetTemporaryPath( int index )
{
    return GetTemporaryPath() / ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( static_cast< E_MissionType >( index ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetTemporaryImagesPath
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
tools::Path ADN_Missions_Data::GetTemporaryImagesPath( int index )
{
    return GetTemporaryPath( index ) / imagePath_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetTemporaryCssFile
// Created: ABR 2013-08-28
// -----------------------------------------------------------------------------
tools::Path ADN_Missions_Data::GetTemporaryCssFile()
{
    return GetTemporaryPath( 0 ) / ".." / cssFile_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szMissions_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Initialize
// Created: NPT 2012-08-07
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Initialize()
{
    for( auto it = missionsVector_.begin(); it != missionsVector_.end(); ++it )
        InitializeMissions( it->second );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::DeleteMissionSheet
// Created: ABR 2013-08-28
// -----------------------------------------------------------------------------
void ADN_Missions_Data::DeleteMissionSheet( const tools::Path& filename )
{
    toDeleteMissionSheets.push_back( filename + ".xml" );
    toDeleteMissionSheets.push_back( filename + ".html" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::OnElementDeleted
// Created: ABR 2013-08-28
// -----------------------------------------------------------------------------
void ADN_Missions_Data::OnElementDeleted( boost::shared_ptr< kernel::LocalizedString > name, E_MissionType missionType )
{
    const tools::Path missionPath = GetMissionSheetsPath( missionType );
    DeleteMissionSheet( GetLocalizedMissionSheetsPath( ADN_Workspace::GetWorkspace().GetLanguages().GetData().Master(), missionPath ) / tools::Path::FromUTF8( name->Key() ) );
    const tools::LanguagesVector& languages = ADN_Workspace::GetWorkspace().GetLanguages().GetData().GetActiveLanguages();
    for( auto it = languages.begin(); it != languages.end(); ++it )
    {
        const std::string& value = name->Value( it->GetCode() );
        DeleteMissionSheet( GetLocalizedMissionSheetsPath( it->GetCode(), missionPath ) / tools::Path::FromUTF8( value.empty() ? name->Key() : value ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GenerateMissionSheet
// Created: NPT 2013-01-30
// -----------------------------------------------------------------------------
void ADN_Missions_Data::GenerateMissionSheet( int index, boost::shared_ptr< kernel::LocalizedString > text )
{
    assert( index >= 0 && index < 4 );
    ADN_Missions_ABC* mission = FindMission( index, text->Key() );
    if( !mission )
        throw MASA_EXCEPTION( "Mission not found: " + text->Key() );
    const tools::Path tempDir = GetLocalizedMissionSheetsPath( tools::Language::Current(), GetTemporaryPath( index ) );
    GetCssFile().Copy( GetTemporaryCssFile(), tools::Path::OverwriteIfExists );
    mission->WriteMissionSheet( tempDir, tools::Language::Current() );
    mission->missionSheetPath_.SetValue( tools::Language::Current(), ( tempDir / tools::Path::FromUTF8( mission->strName_.GetData() ) + ".html" ).ToUTF8() );
}

namespace
{
    void CopyImageToTempDir( E_MissionType type )
    {
        const tools::Path imagePath = ADN_Missions_Data::GetMissionSheetsImagesPath( type );
        const tools::Path tempImagePath = ADN_Missions_Data::GetTemporaryImagesPath( type );
        if( imagePath.Exists() )
            imagePath.Copy( tempImagePath, tools::Path::OverwriteIfExists );
        else
            tempImagePath.CreateDirectories();
    }

    void CopyImageFromTempDir( E_MissionType type )
    {
        const tools::Path imagePath = ADN_Missions_Data::GetMissionSheetsImagesPath( type );
        const tools::Path tempImagePath = ADN_Missions_Data::GetTemporaryImagesPath( type );
        if( tempImagePath.Exists() )
        {
            PurgePath( imagePath );
            bool copyDone = false;
            int attemptNumber = 0;
            while( !copyDone )
            {
                try
                {
                    ++attemptNumber;
                    tempImagePath.Copy( imagePath, tools::Path::OverwriteIfExists );
                    copyDone = true;
                }
                catch( std::exception& )
                {
                    if( attemptNumber == 42 )
                        throw;
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadArchive( xml::xistream& input )
{
    PurgePath( GetTemporaryPath() );

    input >> xml::start( "missions" );
    for( int type = 0; type < eNbrMissionType; ++type )
        input >> xml::start( missionsVector_[ type ].first )
                >> xml::list( type == eMissionType_FragOrder ? "fragorder" : "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, static_cast< E_MissionType >( type ) ) )
              >> xml::end;
    input >> xml::end;

    CheckAndFixLoadingErrors();

    for( auto it = missionsVector_.begin(); it != missionsVector_.end(); ++it )
        it->second.CheckValidity();
    for( int type = 0; type < eNbrMissionType; ++type )
        CopyImageToTempDir( static_cast< E_MissionType >( type ) );
}

namespace
{
    template< typename T >
    void ReadFullMission( xml::xistream& xis, E_MissionType type, ADN_Missions_Data::T_Mission_Vector& missions )
    {
        const tools::Path missionPath = ADN_Missions_Data::GetMissionSheetsPath( type );
        const tools::LanguagesVector& languages = ADN_Workspace::GetWorkspace().GetLanguages().GetData().GetActiveLanguages();
        std::unique_ptr< T > spNew( new T( type, xis.attribute< unsigned int >( "id" ) ) );
        spNew->ReadArchive( xis );
        spNew->ReadMissionSheet( missionPath, ADN_Workspace::GetWorkspace().GetLanguages().GetData().Master() );
        for( auto it = languages.begin(); it != languages.end(); ++it )
            spNew->ReadMissionSheet( ADN_Missions_Data::GetLocalizedMissionSheetsPath( it->GetCode(), missionPath ), it->GetCode() );
        missions.AddItem( spNew.release() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadMission
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadMission( xml::xistream& xis, E_MissionType type )
{
    if( type == eMissionType_FragOrder )
        ReadFullMission< ADN_Missions_FragOrder >( xis, type, missionsVector_[ type ].second );
    else
        ReadFullMission< ADN_Missions_Mission >( xis, type, missionsVector_[ type ].second );
}

namespace
{
    void WriteMissionSheets( E_MissionType type, const ADN_Missions_Data::T_Mission_Vector& missions, const std::string& language )
    {
        const tools::Path missionDir = ADN_Missions_Data::GetLocalizedMissionSheetsPath( language, ADN_Missions_Data::GetMissionSheetsPath( type ) );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->RenameDifferentNamedMissionSheet( missionDir, language );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->WriteMissionSheet( missionDir, language );
    }
    void WriteMissions( xml::xostream& output, const std::string& name, const ADN_Missions_Data::T_Mission_Vector& missions )
    {
        output << xml::start( name );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->WriteArchive( output );
        output << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FixConsistency
// Created: ABR 2013-10-10
// -----------------------------------------------------------------------------
bool ADN_Missions_Data::FixConsistency()
{
    ADN_Data_ABC::FixConsistency();
    for( auto itMissions = missionsVector_.begin(); itMissions != missionsVector_.end(); ++itMissions )
        for( auto itMission = itMissions->second.begin(); itMission != itMissions->second.end(); ++itMission )
            ( *itMission )->FixConsistency();
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::WriteArchive( xml::xostream& output ) const
{
    for( int type = 0; type < eNbrMissionType; ++type )
        if( missionsVector_[ type ].second.GetErrorStatus() == eError )
            throw MASA_EXCEPTION( tools::translate( "ADN_Missions_Data", "Invalid data on tab '%1', subtab '%2'" )
                                  .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( ENT_Tr::ConvertFromMissionType( static_cast< E_MissionType >( type ) ).c_str() ).toStdString() );

    // save xml
    output << xml::start( "missions" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Missions" );
    for( int type = 0; type < eNbrMissionType; ++type )
        WriteMissions( output, missionsVector_[ type ].first, missionsVector_[ type ].second );
    output << xml::end;

    // save mission sheets
    const tools::LanguagesVector& languages = ADN_Workspace::GetWorkspace().GetLanguages().GetData().GetActiveLanguages();
    for( int type = 0; type < eNbrMissionType; ++type )
    {
        WriteMissionSheets( static_cast< E_MissionType >( type ), missionsVector_[ type ].second, ADN_Workspace::GetWorkspace().GetLanguages().GetData().Master() );
        for( auto it = languages.begin(); it != languages.end(); ++it )
            WriteMissionSheets( static_cast< E_MissionType >( type ), missionsVector_[ type ].second, it->GetCode() );
    }

    // move mission sheets to obsolete directory when mission is deleted
    for( auto it = toDeleteMissionSheets.begin(); it != toDeleteMissionSheets.end() ; ++it )
       MoveMissionSheetsToObsolete( *it );
    toDeleteMissionSheets.clear();

    // save Images in temp directory
    for( int type = 0; type < eNbrMissionType; ++type )
        CopyImageFromTempDir( static_cast< E_MissionType >( type ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MoveMissionSheetsToObsolete
// Created: NPT 2012-08-01
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MoveMissionSheetsToObsolete( const tools::Path& fileName ) const
{
    if( fileName.Exists() )
        fileName.Rename( fileName.Parent() / ADN_Missions_Data::obsoletePath_ / fileName.FileName() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetMissions
// Created: ABR 2013-04-23
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_Vector& ADN_Missions_Data::GetMissions( E_MissionType type )
{
    return missionsVector_[ type ].second;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FindMission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_ABC* ADN_Missions_Data::FindMission( const ADN_Missions_Data::T_Mission_Vector& missions, const std::string& strName ) const
{
    auto it = std::find_if( missions.begin(), missions.end(), ADN_Tools::NameCmp( strName ) );
    if( it == missions.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FindMission
// Created: NPT 2013-02-19
// -----------------------------------------------------------------------------
ADN_Missions_ABC* ADN_Missions_Data::FindMission( int missionType, const std::string& strName ) const
{
    assert( missionType >= 0 && missionType < eNbrMissionType );
    return FindMission( missionsVector_[ missionType ].second, strName );
}

namespace
{
    void FillUsingMissionFromObjectVector( const std::string& objectName, const std::string& missionName, helpers::T_MissionGenObjectTypes_Infos_Vector& vector, QStringList& result, const std::string& prefix = "" )
    {
        for( auto itObject = vector.begin(); itObject != vector.end(); ++itObject )
        {
            ADN_Objects_Data_ObjectInfos* infos = ( *itObject )->GetCrossedElement();
            if( infos && ( *itObject )->isAllowed_.GetData() && infos->strName_.GetData() == objectName )
                result << ( ( prefix.empty() ) ? missionName.c_str() : QString( "%1 - %2" ).arg( prefix.c_str() ).arg( missionName.c_str() ) );
        }
    }

    template< typename T >
    void FillUsingMission( const std::string& objectName, const ADN_Type_Vector_ABC< T >& vector, QStringList& result, const std::string& prefix = "" )
    {
        for( auto itMission = vector.begin(); itMission != vector.end(); ++itMission )
            for( auto itParam = ( *itMission )->parameters_.begin(); itParam != ( *itMission )->parameters_.end(); ++itParam )
                if( ( *itParam )->type_.GetData() == eMissionParameterTypeGenObject )
                    FillUsingMissionFromObjectVector( objectName, ( *itMission )->strName_.GetData(), ( *itParam )->genObjects_, result, prefix );
                else if( ( *itParam )->type_.GetData() == eMissionParameterTypeObjectKnowledge )
                    FillUsingMissionFromObjectVector( objectName, ( *itMission )->strName_.GetData(), ( *itParam )->knowledgeObjects_, result, prefix );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetMissionsThatUse
// Created: ABR 2013-04-23
// -----------------------------------------------------------------------------
QStringList ADN_Missions_Data::GetMissionsThatUse( E_MissionType type, ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    FillUsingMission( object.strName_.GetData(), missionsVector_[ type ].second, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetAllMissionsThatUse
// Created: ABR 2012-08-03
// -----------------------------------------------------------------------------
QStringList ADN_Missions_Data::GetAllMissionsThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    for( int type = 0; type < eNbrMissionType; ++type )
        FillUsingMission( object.strName_.GetData(), missionsVector_[ type ].second, result, ENT_Tr::ConvertFromMissionType( static_cast< E_MissionType >( type ) ) );
    return result;
}

namespace
{
    void CheckMissionTypeUniqueness( ADN_ConsistencyChecker& checker, const ADN_Missions_Data::T_Mission_Vector::const_iterator& rhs, const ADN_Missions_Data::T_Mission_Vector& missions, int subTab )
    {
        for( auto lhs = rhs + 1; lhs != missions.end(); ++lhs )
            if( (*lhs)->strName_.GetKey() != (*rhs)->strName_.GetKey() &&
                (*lhs)->diaType_.GetData() == (*rhs)->diaType_.GetData() )
            {
                ADN_ConsistencyChecker::ConsistencyError error( eMissionTypeUniqueness );
                error.items_.push_back( checker.CreateGotoInfo( (*rhs)->strName_.GetData(), eMissions, subTab ) );
                error.items_.push_back( checker.CreateGotoInfo( (*lhs)->strName_.GetData(), eMissions, subTab ) );
                checker.AddError( error );
            }
    }

    void CheckParameters( ADN_ConsistencyChecker& checker, const ADN_Missions_Data::T_MissionParameter_Vector& parameters, const std::string& missionName, int subTab )
    {
        for( auto it = parameters.begin(); it != parameters.end(); ++it )
            if( ( *it )->type_.GetData() == eMissionParameterTypeLocationComposite )
            {
                bool hasChoice = false;
                for( std::size_t i = 0; i < ( *it )->choices_.size() && !hasChoice; ++i )
                    hasChoice = ( *it )->choices_[ i ]->isAllowed_.GetData();
                if( !hasChoice )
                    checker.AddError( eMissingChoiceComposite, missionName, eMissions, subTab );
            }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::CheckDatabaseValidity
// Created: ABR 2012-11-15
// -----------------------------------------------------------------------------
void ADN_Missions_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( int type = 0; type < eNbrMissionType; ++type )
        for( auto it = missionsVector_[ type ].second.begin(); it != missionsVector_[ type ].second.end(); ++it )
        {
            if( type != eMissionType_FragOrder )
                CheckMissionTypeUniqueness( checker, it, missionsVector_[ type ].second, 0 );
            CheckParameters( checker, ( *it )->parameters_, ( *it )->strName_.GetKey(), 0 );

            const tools::LanguagesVector& languages = ADN_Workspace::GetWorkspace().GetLanguages().GetData().GetActiveLanguages();

            ( *it )->CheckMissionDataConsistency( checker );
            ( *it )->CheckMissionDataConsistency( checker, ADN_Workspace::GetWorkspace().GetLanguages().GetData().Master() );
            for( auto itLang = languages.begin(); itLang != languages.end(); ++itLang )
                ( *it )->CheckMissionDataConsistency( checker, itLang->GetCode() );
        }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::CheckAndFixLoadingErrors
// Created: ABR 2013-04-23
// -----------------------------------------------------------------------------
void ADN_Missions_Data::CheckAndFixLoadingErrors() const
{
    std::vector< int > alreadyReportedIds;
    std::vector< ADN_Missions_ABC* > missionThatNeedANewId;

    // Check for duplicate id
    for( int type = 0; type < eNbrMissionType; ++type )
    {
        const ADN_Missions_Data::T_Mission_Vector& vector = missionsVector_[ type ].second;
        for( auto it = vector.begin(); it != vector.end(); ++it )
        {
            assert( *it != 0 );
            int id = ( *it )->id_.GetData();

            // If already reported, continue
            if( std::find( alreadyReportedIds.begin(), alreadyReportedIds.end(), id ) != alreadyReportedIds.end() )
                continue;

            // look for others with same id in each vector
            std::map< int, std::vector< ADN_Missions_ABC* > > missionsMap;
            size_t count = 0;
            for( int secondType = type; secondType < eNbrMissionType; ++secondType )
            {
                missionsMap[ secondType ] = missionsVector_[ secondType ].second.FindElements( boost::bind( &ADN_Tools::IdCompare< ADN_Missions_ABC >, _1, boost::cref( id ) ) );
                count += missionsMap[ secondType ].size();
            }

            assert( count != 0 );
            if( count > 1 ) // duplicate id, report it
            {
                alreadyReportedIds.push_back( id );
                ADN_ConsistencyChecker::ConsistencyError error( eInvalidIdInVector );
                for( auto itMap = missionsMap.begin(); itMap != missionsMap.end(); ++itMap )
                    for( auto itVector = itMap->second.begin(); itVector != itMap->second.end(); ++itVector )
                    {
                        error.items_.push_back( ADN_ConsistencyChecker::CreateGotoInfo( ( *itVector )->strName_.GetData(), eMissions, itMap->first ) );
                        if( error.items_.size() > 1 )
                            missionThatNeedANewId.push_back( *itVector );
                    }
                ADN_ConsistencyChecker::AddLoadingError( error );
            }
        }
    }

    // Fix ids
    for( auto it = missionThatNeedANewId.begin(); it != missionThatNeedANewId.end(); ++it )
        ( *it )->id_ = ADN_Missions_Data::idManager_.GetNextId();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetMissionSheetContext
// Created: ABR 2013-08-28
// -----------------------------------------------------------------------------
const boost::shared_ptr< kernel::Context >& ADN_Missions_Data::GetMissionSheetContext( E_MissionType type ) const
{
    return missionSheetContexts_[ type ];
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< kernel::Context >& ADN_Missions_Data::GetMissionSheetPathContext
// Created: ABR 2013-10-17
// -----------------------------------------------------------------------------
const boost::shared_ptr< kernel::Context >& ADN_Missions_Data::GetMissionSheetPathContext() const
{
    return missionSheetPathContext_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ApplyOnTranslations
// Created: ABR 2013-10-08
// -----------------------------------------------------------------------------
bool ADN_Missions_Data::ApplyOnTranslations( const boost::function< bool( kernel::LocalizedString& ) >& functor ) const
{
    if( ADN_Data_ABC::ApplyOnTranslations( functor ) )
        return true;
    for( auto it = missionSheetContexts_.begin(); it != missionSheetContexts_.end(); ++it )
        if( *it && ( *it )->Apply( functor ) )
            return true;
    return false;
}
