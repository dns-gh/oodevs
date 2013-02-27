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
#include "tools/Loader_ABC.h"
#include <xeuseuleu/xsl.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

tools::IdManager ADN_Missions_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_Data()
    : ADN_Data_ABC( eMissions )
{
    unitMissions_.AddUniquenessChecker( eError, duplicateName_ );
    automatMissions_.AddUniquenessChecker( eError, duplicateName_ );
    populationMissions_.AddUniquenessChecker( eError, duplicateName_ );
    fragOrders_.AddUniquenessChecker( eError, duplicateName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::~ADN_Missions_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::FilesNeeded( T_StringList& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szMissions_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Reset
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Reset()
{
    idManager_.Reset();
    unitMissions_.Reset();
    automatMissions_.Reset();
    populationMissions_.Reset();
    fragOrders_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Load
// Created: LDC 2010-09-24
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Load( const tools::Loader_ABC& fileLoader )
{
    T_StringList fileList;
    FilesNeeded( fileList );
    if( ! fileList.empty() )
    {
        const std::string strFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + fileList.front();
        fileLoader.LoadFile( strFile, boost::bind( &ADN_Missions_Data::ReadArchive, this, _1 ) );
    }
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
    InitializeMissions( unitMissions_ );
    InitializeMissions( automatMissions_ );
    InitializeMissions( populationMissions_ );
    InitializeMissions( fragOrders_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::NotifyElementDeleted
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_Data::NotifyElementDeleted( std::string elementName, E_MissionType missionType )
{
    const std::string missionDirectoryPath = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData()
                                           + ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( missionType );
    std::string xmlFileName = missionDirectoryPath + std::string( "/" + elementName + ".xml");
    std::string htmlFileName = missionDirectoryPath + std::string( "/" + elementName + ".html");
    toDeleteMissionSheets_.push_back( xmlFileName );
    toDeleteMissionSheets_.push_back( htmlFileName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GenerateMissionSheet
// Created: NPT 2013-01-30
// -----------------------------------------------------------------------------
QString ADN_Missions_Data::GenerateMissionSheet( int index, const QString& text )
{
    assert( index >= 0 && index < 4 );
    const std::string missionDir = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() 
                                 + ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( static_cast< E_MissionType >( index ) );
    std::string tempFileName = "tempMissionSheet";
    ADN_Missions_ABC* mission = FindMission( index, text.toStdString() );
    if( mission )
    {
        mission->WriteMissionSheet( missionDir, tempFileName, index );
        mission->needSheetSaving_ = true;
    }
    return QString( missionDir.c_str() ) + "/" + QString( tempFileName.c_str() ) + ".html";
}

namespace
{
    void CopyImageToTempDir( E_MissionType modelType )
    {
        //get mission dir
        const std::string& missionPath = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() 
            + ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( modelType );
        std::string tempDir = QDir::tempPath().toStdString() + "_MissionSheets/ImagesTemp" + QString::number( modelType ).toStdString();
        //copy images in temp directory for image temp save
        if( bfs::exists( missionPath + "/Images" ) )
        {
            if( bfs::exists( tempDir ) )
                 ADN_Tools::CleanDirectoryContent( tempDir, false );
            ADN_Tools::CopyDirToDir( missionPath + "/Images", tempDir, false, true );
        }
        else
            bfs::create_directories( tempDir );
    }

    void CopyImageFromTempDir( E_MissionType type )
    {
        const std::string& missionPath = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( type );
        std::string tempDir = QDir::tempPath().toStdString() + "_MissionSheets/ImagesTemp" + QString::number( type ).toStdString();
        //copy images from temp directory for image temp reset
        if( bfs::exists( tempDir ) )
        {
            ADN_Tools::CleanDirectoryContent( missionPath + "/Images", false );
            ADN_Tools::CopyDirToDir( tempDir, missionPath + "/Images", false, true );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "missions" )
            >> xml::start( "units" )
                >> xml::list( "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, boost::ref( unitMissions_ ), eMissionType_Pawn ) )
            >> xml::end
            >> xml::start( "automats" )
                >> xml::list( "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, boost::ref( automatMissions_ ), eMissionType_Automat ) )
            >> xml::end
            >> xml::start( "populations" )
                >> xml::list( "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, boost::ref( populationMissions_ ), eMissionType_Population ) )
            >> xml::end
            >> xml::start( "fragorders" )
                >> xml::list( "fragorder", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, boost::ref( fragOrders_ ), eMissionType_FragOrder ) )
            >> xml::end
          >> xml::end;
    unitMissions_.CheckValidity();
    automatMissions_.CheckValidity();
    populationMissions_.CheckValidity();
    fragOrders_.CheckValidity();
    CopyImageToTempDir( eMissionType_Pawn );
    CopyImageToTempDir( eMissionType_Automat );
    CopyImageToTempDir( eMissionType_Population );
    CopyImageToTempDir( eMissionType_FragOrder );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadMission
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadMission( xml::xistream& xis, T_Mission_Vector& missions, E_MissionType modelType )
{
    const std::string& missionPath = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() 
                                   + ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( modelType );

    if( modelType == eMissionType_FragOrder )
    {
        std::auto_ptr< ADN_Missions_FragOrder > spNew( new ADN_Missions_FragOrder( xis.attribute< unsigned int >( "id" ) ) );
        spNew->ReadArchive( xis, missionPath );
        missions.AddItem( spNew.release() );
    }
    else
    {
        std::auto_ptr< ADN_Missions_Mission > spNew( new ADN_Missions_Mission( xis.attribute< unsigned int >( "id" ) ) );
        ADN_Drawings_Data& drawings = ADN_Workspace::GetWorkspace().GetDrawings().GetData();
        spNew->ReadArchive( xis, drawings, missionPath );
        missions.AddItem( spNew.release() );
    }
}

namespace
{
    void WriteMissionSheets( E_MissionType type, const ADN_Missions_Data::T_Mission_Vector& missions )
    {
        const std::string missionDir = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + ADN_Workspace::GetWorkspace().GetProject().GetMissionDir( type );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->RenameDifferentNamedMissionSheet( missionDir );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->WriteMissionSheet( missionDir, missions[i]->strName_.GetData(), type );
    }

    void WriteMissions( xml::xostream& output, const std::string& name, E_MissionType type, const ADN_Missions_Data::T_Mission_Vector& missions )
    {
        //xml datas saving
        output << xml::start( name );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->WriteArchive( output, name );

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
    if( unitMissions_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( tools::translate( "ADN_Missions_Data", "Invalid data on tab '%1', subtab '%2'" )
                              .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( tools::translate( "ADN_Missions_Data", "Unit missions" ) ).toStdString() );
    if( automatMissions_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( tools::translate( "ADN_Missions_Data", "Invalid data on tab '%1', subtab '%2'" )
                              .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( tools::translate( "ADN_Missions_Data", "Automat missions" ) ).toStdString() );
    if( populationMissions_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( tools::translate( "ADN_Missions_Data", "Invalid data on tab '%1', subtab '%2'" )
                              .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( tools::translate( "ADN_Missions_Data", "Crowd missions" ) ).toStdString() );
    if( fragOrders_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( tools::translate( "ADN_Missions_Data", "Invalid data on tab '%1', subtab '%2'" )
                              .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( tools::translate( "ADN_Missions_Data", "Fragmentary orders" ) ).toStdString() );

    output << xml::start( "missions" );
    ADN_Tools::AddSchema( output, "Missions" );
    WriteMissions( output, "units", eMissionType_Pawn, unitMissions_ );
    WriteMissions( output, "automats", eMissionType_Automat, automatMissions_ );
    WriteMissions( output, "populations", eMissionType_Population, populationMissions_ );
    WriteMissions( output, "fragorders", eMissionType_FragOrder, fragOrders_ );

    //move mission sheets to obsolete directory when mission is deleted
    for( IT_StringList it = toDeleteMissionSheets_.begin(); it != toDeleteMissionSheets_.end() ; ++it )
       MoveMissionSheetsToObsolete( *it );

    //save Images in temp directory
    CopyImageFromTempDir( eMissionType_Pawn );
    CopyImageFromTempDir( eMissionType_Automat );
    CopyImageFromTempDir( eMissionType_Population );
    CopyImageFromTempDir( eMissionType_FragOrder );

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MoveMissionSheetsToObsolete
// Created: NPT 2012-08-01
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MoveMissionSheetsToObsolete( std::string fileName )
{
    if( bfs::exists( fileName ) )
    {
        std::string newFileName = fileName;
        size_t pos = newFileName.find_last_of("//");
        newFileName.insert(pos,"/obsolete");
        bfs::rename( fileName.c_str(), newFileName.c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetFragOrders
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_Vector& ADN_Missions_Data::GetFragOrders()
{
    return fragOrders_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetUnitMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_Vector& ADN_Missions_Data::GetUnitMissions()
{
    return unitMissions_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetAutomatMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_Vector& ADN_Missions_Data::GetAutomatMissions()
{
    return automatMissions_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetPopulationMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_Vector& ADN_Missions_Data::GetPopulationMissions()
{
    return populationMissions_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FindFragOrder
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_ABC* ADN_Missions_Data::FindFragOrder( const std::string& strName )
{
    auto it = std::find_if( fragOrders_.begin(), fragOrders_.end(), ADN_Tools::NameCmp< ADN_Missions_FragOrder >( strName ) );
    if( it == fragOrders_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FindMission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_ABC* ADN_Missions_Data::FindMission( const ADN_Missions_Data::T_Mission_Vector& missions, const std::string& strName )
{
    auto it = std::find_if( missions.begin(), missions.end(), ADN_Tools::NameCmp< ADN_Missions_Mission >( strName ) );
    if( it == missions.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FindMission
// Created: NPT 2013-02-19
// -----------------------------------------------------------------------------
ADN_Missions_ABC* ADN_Missions_Data::FindMission( int missionType, const std::string& strName )
{
    const T_Mission_Vector& vector = ( missionType == eMissionType_Pawn )? unitMissions_ :
                              ( missionType == eMissionType_Automat )? automatMissions_ :
                              ( missionType == eMissionType_Population )? populationMissions_ :
                              fragOrders_;
    return FindMission( vector, strName );
}

namespace
{
    void FillUsingMissionFromObjectVector( const std::string& objectName, const std::string& missionName, helpers::T_MissionGenObjectTypes_Infos_Vector& vector, QStringList& result, const QString& prefix = "" )
    {
        for( helpers::CIT_MissionGenObjectTypes_Infos_Vector itObject = vector.begin(); itObject != vector.end(); ++itObject )
            if( ( *itObject )->isAllowed_.GetData() && ( *itObject )->ptrObject_.GetData()->strName_.GetData() == objectName )
                result << ( ( prefix.isEmpty() ) ? missionName.c_str() : QString( "%1 - %2" ).arg( prefix ).arg( missionName.c_str() ) );
    }

    template< typename T >
    void FillUsingMission( const std::string& objectName, const ADN_Type_Vector_ABC< T >& vector, QStringList& result, const QString& prefix = "" )
    {
        for( ADN_Type_Vector_ABC< T >::const_iterator itMission = vector.begin(); itMission != vector.end(); ++itMission )
            for( ADN_Missions_Data::T_MissionParameter_Vector::const_iterator itParam = ( *itMission )->parameters_.begin(); itParam != ( *itMission )->parameters_.end(); ++itParam )
                if( ( *itParam )->type_.GetData() == eMissionParameterTypeGenObject )
                    FillUsingMissionFromObjectVector( objectName, ( *itMission )->strName_.GetData(), ( *itParam )->genObjects_, result, prefix );
                else if( ( *itParam )->type_.GetData() == eMissionParameterTypeObjectKnowledge )
                    FillUsingMissionFromObjectVector( objectName, ( *itMission )->strName_.GetData(), ( *itParam )->knowledgeObjects_, result, prefix );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetUnitMissionsThatUse
// Created: ABR 2012-08-03
// -----------------------------------------------------------------------------
QStringList ADN_Missions_Data::GetUnitMissionsThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    FillUsingMission( object.strName_.GetData(), unitMissions_, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetAutomatMissionsThatUse
// Created: ABR 2012-08-03
// -----------------------------------------------------------------------------
QStringList ADN_Missions_Data::GetAutomatMissionsThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    FillUsingMission( object.strName_.GetData(), automatMissions_, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetPopulationMissionsThatUse
// Created: ABR 2012-08-03
// -----------------------------------------------------------------------------
QStringList ADN_Missions_Data::GetPopulationMissionsThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    FillUsingMission( object.strName_.GetData(), populationMissions_, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetFragOrdersThatUse
// Created: ABR 2012-08-03
// -----------------------------------------------------------------------------
QStringList ADN_Missions_Data::GetFragOrdersThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    FillUsingMission( object.strName_.GetData(), fragOrders_, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetAllMissionsThatUse
// Created: ABR 2012-08-03
// -----------------------------------------------------------------------------
QStringList ADN_Missions_Data::GetAllMissionsThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    FillUsingMission( object.strName_.GetData(), unitMissions_, result, tools::translate( "ADN_Missions_data", "Unit missions" ) );
    FillUsingMission( object.strName_.GetData(), automatMissions_, result, tools::translate( "ADN_Missions_data", "Automat missions" ) );
    FillUsingMission( object.strName_.GetData(), populationMissions_, result, tools::translate( "ADN_Missions_data", "Crowd missions" ) );
    FillUsingMission( object.strName_.GetData(), fragOrders_, result, tools::translate( "ADN_Missions_data", "Fragmentary orders" ) );
    return result;
}

namespace
{
    void CheckMissionTypeUniqueness( ADN_ConsistencyChecker& checker, const ADN_Missions_Data::CIT_Mission_Vector& rhs, const ADN_Missions_Data::T_Mission_Vector& missions, int subTab )
    {
        for( ADN_Missions_Data::CIT_Mission_Vector lhs = rhs + 1; lhs != missions.end(); ++lhs )
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
        for( ADN_Missions_Data::CIT_MissionParameter_Vector it = parameters.begin(); it != parameters.end(); ++it )
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
    for( auto it = unitMissions_.begin(); it != unitMissions_.end(); ++it )
    {
        CheckMissionTypeUniqueness( checker, it, unitMissions_, 0 );
        CheckParameters( checker, ( *it )->parameters_, ( *it )->strName_.GetData(), 0 );
        ( *it )->CheckMissionDataConsistency( checker, eMissionType_Pawn );
    }
    for( auto it = automatMissions_.begin(); it != automatMissions_.end(); ++it )
    {
        CheckMissionTypeUniqueness( checker, it, automatMissions_, 1 );
        CheckParameters( checker, ( *it )->parameters_, ( *it )->strName_.GetData(), 1 );
        ( *it )->CheckMissionDataConsistency( checker, eMissionType_Automat );
    }
    for( auto it = populationMissions_.begin(); it != populationMissions_.end(); ++it )
    {
        CheckMissionTypeUniqueness( checker, it, populationMissions_, 2 );
        CheckParameters( checker, ( *it )->parameters_, ( *it )->strName_.GetData(), 2 );
        ( *it )->CheckMissionDataConsistency( checker, eMissionType_Population );
    }
    for( auto it = fragOrders_.begin(); it != fragOrders_.end(); ++it )
    {
        CheckParameters( checker, ( *it )->parameters_, ( *it )->strName_.GetData(), 3 );
        ( *it )->CheckMissionDataConsistency( checker, eMissionType_FragOrder );
    }
}
