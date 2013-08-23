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
#include "ADN_ConsistencyChecker.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Tr.h"
#include "ADN_enums.h"
#include "clients_kernel/XmlTranslations.h"
#include "ENT/ENT_Tr.h"
#include "tools/Loader_ABC.h"
#include <xeuseuleu/xsl.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

tools::IdManager ADN_Missions_Data::idManager_;
tools::Path ADN_Missions_Data::imagePath_ = "Images";
tools::Path ADN_Missions_Data::imageTemporaryPath_ = "_MissionSheets/ImagesTemp";
tools::Path ADN_Missions_Data::xslTemporaryFile_ = "_adnTempXslt.xsl";
tools::Path ADN_Missions_Data::missionSheetTemporaryFile_ = "tempMissionSheet";

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_Data()
    : ADN_Data_ABC( eMissions )
{
    // $$$$ ABR 2013-04-23: Must be in E_MissionType order
    missionsVector_.push_back( std::make_pair< std::string, T_Mission_Vector >( "units", T_Mission_Vector() ) );
    missionsVector_.push_back( std::make_pair< std::string, T_Mission_Vector >( "automats", T_Mission_Vector() ) );
    missionsVector_.push_back( std::make_pair< std::string, T_Mission_Vector >( "populations", T_Mission_Vector() ) );
    missionsVector_.push_back( std::make_pair< std::string, T_Mission_Vector >( "fragorders", T_Mission_Vector() ) );

    for( auto it = missionsVector_.begin(); it != missionsVector_.end(); ++it )
        it->second.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::~ADN_Missions_Data()
{
    try
    {
        ( tools::Path::TemporaryPath() / ADN_Missions_Data::imageTemporaryPath_ ).Parent().RemoveAll();
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szMissions_ );
}

namespace
{
    template< typename T >
    void InitializeMissions( const ADN_Type_Vector_ABC< T >& vector )
    {
        for( ADN_Type_Vector_ABC< T >::const_iterator itMission = vector.begin(); itMission != vector.end(); ++itMission )
        {
            T* mission = *itMission;
            QRegExp regExp( "[/\"<>|*\?:\\\\]" );
            QString name( mission->strName_.GetData().c_str() );
            int indexBadCaracter = regExp.lastIndexIn( name );
            if( indexBadCaracter != -1 )
            {
                while( indexBadCaracter != -1 )
                {
                    name.replace( indexBadCaracter, 1, "-" );
                    indexBadCaracter = regExp.lastIndexIn( name );
                }
                mission->strName_ = name.toStdString();
            }
        }
    }
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
// Name: ADN_Missions_Data::NotifyElementDeleted
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_Data::NotifyElementDeleted( std::string elementName, E_MissionType missionType )
{
    const tools::Path missionDirectoryPath = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( missionType );
    tools::Path xmlFileName = missionDirectoryPath / tools::Path::FromUTF8( elementName ) + ".xml";
    tools::Path htmlFileName = missionDirectoryPath / tools::Path::FromUTF8( elementName ) + ".html";
    toDeleteMissionSheets_.push_back( xmlFileName );
    toDeleteMissionSheets_.push_back( htmlFileName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GenerateMissionSheet
// Created: NPT 2013-01-30
// -----------------------------------------------------------------------------
tools::Path ADN_Missions_Data::GenerateMissionSheet( int index, const QString& text )
{
    assert( index >= 0 && index < 4 );
    const tools::Path missionDir = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( static_cast< E_MissionType >( index ) );
    tools::Path tempFileName = missionSheetTemporaryFile_;
    ADN_Missions_ABC* mission = FindMission( index, text.toStdString() );
    if( mission )
    {
        mission->WriteMissionSheet( missionDir, tempFileName, index );
        mission->SetNeedsSaving( true );
    }
    return missionDir / tempFileName + ".html";
}

namespace
{
    void CopyImageToTempDir( E_MissionType modelType )
    {
        //get mission dir
        const tools::Path missionPath = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( modelType );
        const tools::Path tempDir = tools::Path::TemporaryPath() / ADN_Missions_Data::imageTemporaryPath_ + boost::lexical_cast< std::string >( modelType ).c_str();
        //copy images in temp directory for image temp save
        if( ( missionPath / ADN_Missions_Data::imagePath_ ).Exists() )
        {
            if( tempDir.Exists() )
            {
                try
                {
                    tempDir.RemoveAll();
                }
                catch( ... )
                {
                    // NOTHING
                }
            }
            ( missionPath / ADN_Missions_Data::imagePath_ ).Copy( tempDir, tools::Path::OverwriteIfExists );
        }
        else
            tempDir.CreateDirectories();
    }

    void CopyImageFromTempDir( E_MissionType type )
    {
        const tools::Path missionPath = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( type );
        const tools::Path tempDir = tools::Path::TemporaryPath() / ADN_Missions_Data::imageTemporaryPath_ + boost::lexical_cast< std::string >( type ).c_str();
        //copy images from temp directory for image temp reset
        if( tempDir.Exists() )
        {
            try
            {
                ( missionPath / ADN_Missions_Data::imagePath_ ).RemoveAll();
            }
            catch( ... )
            {
                // NOTHING
            }
            bool copyDone = false;
            int attemptNumber = 0;
            while( !copyDone )
            {
                try
                {
                    ++attemptNumber;
                    tempDir.Copy( missionPath / ADN_Missions_Data::imagePath_, tools::Path::OverwriteIfExists );
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
    try
    {
        ( tools::Path::TemporaryPath() / ADN_Missions_Data::imageTemporaryPath_ ).Parent().RemoveAll();
    }
    catch( ... )
    {
        // NOTHING
    }
    for( int type = 0; type != eNbrMissionTypes; ++type )
    {
        const tools::Path missionDir = tools::Path::TemporaryPath() / ADN_Missions_Data::imageTemporaryPath_ + boost::lexical_cast< std::string >( type ).c_str();
        if( !missionDir.Exists() )
            missionDir.CreateDirectories();
    }

    input >> xml::start( "missions" );
    for( int type = 0; type < eNbrMissionTypes; ++type )
        input >> xml::start( missionsVector_[ type ].first )
                >> xml::list( type == eMissionType_FragOrder ? "fragorder" : "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, static_cast< E_MissionType >( type ) ) )
              >> xml::end;
    input >> xml::end;

    CheckAndFixLoadingErrors();

    for( auto it = missionsVector_.begin(); it != missionsVector_.end(); ++it )
        it->second.CheckValidity();
    for( int type = 0; type < eNbrMissionTypes; ++type )
        CopyImageToTempDir( static_cast< E_MissionType >( type ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadMission
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadMission( xml::xistream& xis, E_MissionType type )
{
    const tools::Path& missionPath = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( type );
    if( type == eMissionType_FragOrder )
    {
        std::auto_ptr< ADN_Missions_FragOrder > spNew( new ADN_Missions_FragOrder( type, xis.attribute< unsigned int >( "id" ) ) );
        spNew->ReadArchive( xis, missionPath );
        missionsVector_[ type ].second.AddItem( spNew.release() );
    }
    else
    {
        std::auto_ptr< ADN_Missions_Mission > spNew( new ADN_Missions_Mission( type, xis.attribute< unsigned int >( "id" ) ) );
        ADN_Drawings_Data& drawings = ADN_Workspace::GetWorkspace().GetDrawings().GetData();
        spNew->ReadArchive( xis, drawings, missionPath );
        missionsVector_[ type ].second.AddItem( spNew.release() );
    }
}

namespace
{
    void WriteMissionSheets( E_MissionType type, const ADN_Missions_Data::T_Mission_Vector& missions )
    {
        const tools::Path missionDir = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( type );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->RenameDifferentNamedMissionSheet( missionDir );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->WriteMissionSheet( missionDir, tools::Path::FromUTF8( missions[i]->strName_.GetData() ), type );
    }

    void WriteMissions( xml::xostream& output, const std::string& name, E_MissionType type, const ADN_Missions_Data::T_Mission_Vector& missions )
    {
        //xml datas saving
        output << xml::start( name );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->WriteArchive( output );

        //save mission sheets
        WriteMissionSheets( type, missions );

        output << xml::end;
    }

}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::WriteArchive( xml::xostream& output )
{
    for( int type = 0; type < eNbrMissionTypes; ++type )
        if( missionsVector_[ type ].second.GetErrorStatus() == eError )
            throw MASA_EXCEPTION( tools::translate( "ADN_Missions_Data", "Invalid data on tab '%1', subtab '%2'" )
                                  .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( ENT_Tr::ConvertFromMissionType( static_cast< E_MissionType >( type ) ).c_str() ).toStdString() );

    output << xml::start( "missions" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Missions" );

    for( int type = 0; type < eNbrMissionTypes; ++type )
        WriteMissions( output, missionsVector_[ type ].first, static_cast< E_MissionType >( type ), missionsVector_[ type ].second );

    //move mission sheets to obsolete directory when mission is deleted
    for( auto it = toDeleteMissionSheets_.begin(); it != toDeleteMissionSheets_.end() ; ++it )
       MoveMissionSheetsToObsolete( *it );

    //save Images in temp directory
    for( int type = 0; type < eNbrMissionTypes; ++type )
        CopyImageFromTempDir( static_cast< E_MissionType >( type ) );

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MoveMissionSheetsToObsolete
// Created: NPT 2012-08-01
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MoveMissionSheetsToObsolete( const tools::Path& fileName ) const
{
    if( fileName.Exists() )
        fileName.Rename( fileName.Parent() / "obsolete" / fileName.FileName() );
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
    assert( missionType >= 0 && missionType < eNbrMissionTypes );
    return FindMission( missionsVector_[ missionType ].second, strName );
}

namespace
{
    void FillUsingMissionFromObjectVector( const std::string& objectName, const std::string& missionName, helpers::T_MissionGenObjectTypes_Infos_Vector& vector, QStringList& result, const std::string& prefix = "" )
    {
        for( auto itObject = vector.begin(); itObject != vector.end(); ++itObject )
        {
            ADN_Objects_Data_ObjectInfos* infos = ( *itObject )->ptrObject_.GetData();
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
    for( int type = 0; type < eNbrMissionTypes; ++type )
        FillUsingMission( object.strName_.GetData(), missionsVector_[ type ].second, result, ENT_Tr::ConvertFromMissionType( static_cast< E_MissionType >( type ) ) );
    return result;
}

namespace
{
    void CheckMissionTypeUniqueness( ADN_ConsistencyChecker& checker, const ADN_Missions_Data::T_Mission_Vector::const_iterator& rhs, const ADN_Missions_Data::T_Mission_Vector& missions, int subTab )
    {
        for( auto lhs = rhs + 1; lhs != missions.end(); ++lhs )
            if( (*lhs)->strName_.GetData() != (*rhs)->strName_.GetData() &&
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
    for( int type = 0; type < eNbrMissionTypes; ++type )
        for( auto it = missionsVector_[ type ].second.begin(); it != missionsVector_[ type ].second.end(); ++it )
        {
            if( type != eMissionType_FragOrder )
                CheckMissionTypeUniqueness( checker, it, missionsVector_[ type ].second, 0 );
            CheckParameters( checker, ( *it )->parameters_, ( *it )->strName_.GetData(), 0 );
            ( *it )->CheckMissionDataConsistency( checker );
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
    for( int type = 0; type < eNbrMissionTypes; ++type )
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
            for( int secondType = type; secondType < eNbrMissionTypes; ++secondType )
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
