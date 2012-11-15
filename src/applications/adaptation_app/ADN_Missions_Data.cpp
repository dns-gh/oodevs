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
#include "ADN_Objects_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_DataException.h"
#include "ADN_Tr.h"
#include "ADN_enums.h"
#include "tools/Loader_ABC.h"
#include <tools/XmlCrc32Signature.h>
#include <xeuseuleu/xsl.hpp>

#include <boost/bind.hpp>

tools::IdManager ADN_Missions_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_Data()
{
    // NOTHING
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

namespace
{

std::string GetMissionDir( E_EntityType elementType )
{
    const ADN_Project_Data::DataInfos& info = ADN_Workspace::GetWorkspace().GetProject().GetDataInfos();
    switch( elementType )
    {
    case eEntityType_Pawn:
        return info.szUnitsMissionPath_.GetData();
    case eEntityType_Automat:
        return info.szAutomataMissionPath_.GetData();
    case eEntityType_Population:
        return info.szCrowdsMissionPath_.GetData();
    default:
        return info.szFragOrdersMissionPath_.GetData();
    }
}

}  // namespace

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::NotifyElementDeleted
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_Data::NotifyElementDeleted( std::string elementName, E_EntityType elementType )
{
    const std::string missionDirectoryPath = GetMissionDir( elementType );
    std::string directoryPath = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData();
    std::string fileName = directoryPath + missionDirectoryPath + std::string( "/" + elementName + ".html");
    toDeleteMissionSheets_.push_back( fileName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadArchive( xml::xistream& input )
{
    // MGD 2010-10-03 : Hack to hide context @TODO remove with context deletion
    xml::xistringstream unitContextFlow(
        "<context>"
        "  <parameter dia-name='dangerDirection_' name='Direction dangereuse' optional='false' type='Heading'/>"
        "  <parameter dia-name='phaseLines_' max-occurs='unbounded' min-occurs='1' name='Limas' optional='true' type='PhaseLine'/>"
        "  <parameter dia-name='boundaryLimit1_' name='Limit 1' optional='true' type='Limit'/>"
        "  <parameter dia-name='boundaryLimit2_' name='Limit 2' optional='true' type='Limit'/>"
        "</context>" );
    xml::xistringstream automatContextFlow(
        "<context>"
        "  <parameter dia-name='dangerDirection_' name='Direction dangereuse' optional='false' type='Heading'/>"
        "  <parameter dia-name='phaseLines_' max-occurs='unbounded' min-occurs='1' name='Limas' optional='true' type='PhaseLine'/>"
        "  <parameter dia-name='boundaryLimit1_' name='Limit 1' optional='false' type='Limit'/>"
        "  <parameter dia-name='boundaryLimit2_' name='Limit 2' optional='false' type='Limit'/>"
        "</context>" );
    xml::xistringstream crowContextFlow( "<context></context>" );
    ReadContext( unitContextFlow, unitContext_ );
    ReadContext( automatContextFlow, automatContext_ );
    ReadContext( crowContextFlow, populationContext_ );

    input >> xml::start( "missions" )
            >> xml::start( "units" )
                >> xml::list( "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, boost::ref( unitMissions_ ), unitContext_.size(), eEntityType_Pawn ) )
            >> xml::end
            >> xml::start( "automats" )
                >> xml::list( "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, boost::ref( automatMissions_ ), automatContext_.size(), eEntityType_Automat ) )
            >> xml::end
            >> xml::start( "populations" )
                >> xml::list( "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, boost::ref( populationMissions_ ), populationContext_.size(), eEntityType_Population ) )
            >> xml::end
            >> xml::start( "fragorders" )
                >> xml::list( "fragorder", *this, &ADN_Missions_Data::ReadFragOrder )
            >> xml::end
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadFragOrder
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadFragOrder( xml::xistream& xis )
{
    std::auto_ptr< ADN_Missions_FragOrder > spNew( new ADN_Missions_FragOrder( xis.attribute< unsigned int >( "id" ) ) );
    const std::string baseDir = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData();
    spNew->ReadArchive( xis, baseDir, GetMissionDir( eNbrEntityTypes )  );
    fragOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadMission
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadMission( xml::xistream& xis, T_Mission_Vector& missions, std::size_t contextLength, E_EntityType modelType )
{
    std::auto_ptr< ADN_Missions_Mission > spNew( new ADN_Missions_Mission( xis.attribute< unsigned int >( "id" ) ) );
    const std::string baseDir = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData();
    ADN_Drawings_Data& drawings = ADN_Workspace::GetWorkspace().GetDrawings().GetData();
    spNew->ReadArchive( xis, contextLength, drawings, baseDir, GetMissionDir( modelType ) );
    missions.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadContext
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadContext( xml::xistream& input, T_MissionParameter_Vector& context )
{
    input >> xml::start("context")
            >> xml::list( "parameter", *this, &ADN_Missions_Data::ReadContextParameter, context )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadContextParameter
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadContextParameter( xml::xistream& input, T_MissionParameter_Vector& context )
{
    std::auto_ptr< ADN_Missions_Parameter > spNew( new ADN_Missions_Parameter() );
    spNew->ReadArchive( input );
    context.AddItem( spNew.release() );
}

namespace
{
    void WriteMissions( xml::xostream& output, const std::string& name, E_EntityType type, 
        const ADN_Missions_Data::T_MissionParameter_Vector& context, const ADN_Missions_Data::T_Mission_Vector& missions )
    {
        //xml datas saving
        output << xml::start( name );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->WriteArchive( output, name, context );

        //mission sheets saving
        const std::string baseDir = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData();
        const std::string missionDir = GetMissionDir( type );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->RemoveDifferentNamedMissionSheet( baseDir, missionDir );
         for( unsigned int i = 0; i < missions.size(); ++i )
             missions[i]->WriteMissionSheet( baseDir, missionDir );
        output << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "missions" );
    ADN_Tools::AddSchema( output, "Missions" );
    WriteMissions( output, "units", eEntityType_Pawn, unitContext_, unitMissions_ );
    WriteMissions( output, "automats", eEntityType_Automat, automatContext_, automatMissions_ );
    WriteMissions( output, "populations", eEntityType_Population, populationContext_, populationMissions_ );

    //frag orders datas saving
    output << xml::start( "fragorders" );
    for( unsigned int i = 0; i < fragOrders_.size(); ++i )
        fragOrders_[i]->WriteArchive( output );

    const std::string baseDir = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData();
    const std::string missionDir = GetMissionDir( eNbrEntityTypes );
    //frag orders mission sheets saving
    for( unsigned int i = 0; i < fragOrders_.size(); ++i )
        fragOrders_[i]->RemoveDifferentNamedMissionSheet( baseDir, missionDir );
    for( unsigned int i = 0; i < fragOrders_.size(); ++i )
         fragOrders_[i]->WriteMissionSheet( baseDir, missionDir );

    //move mission sheets to obsolete directory when mission is deleted
    for( IT_StringList it = toDeleteMissionSheets_.begin(); it != toDeleteMissionSheets_.end() ; ++it )
       MoveMissionSheetsToObsolete( *it );

    output << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MoveMissionSheetsToObsolete
// Created: NPT 2012-08-01
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MoveMissionSheetsToObsolete( std::string fileName )
{
    std::string newFileName = fileName;
    size_t pos = newFileName.find_last_of("//");
    newFileName.insert(pos,"/obsolete");
    std::rename( fileName.c_str(), newFileName.c_str() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetFragOrders
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_FragOrder_Vector& ADN_Missions_Data::GetFragOrders()
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
ADN_Missions_FragOrder* ADN_Missions_Data::FindFragOrder( const std::string& strName )
{
    IT_FragOrder_Vector it = std::find_if( fragOrders_.begin(), fragOrders_.end(), ADN_Tools::NameCmp< ADN_Missions_FragOrder >( strName ) );
    if( it == fragOrders_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FindMission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Mission* ADN_Missions_Data::FindMission( ADN_Missions_Data::T_Mission_Vector& missions, const std::string& strName )
{
    IT_Mission_Vector it = std::find_if( missions.begin(), missions.end(), ADN_Tools::NameCmp< ADN_Missions_Mission >( strName ) );
    if( it == missions.end() )
        return 0;
    return *it;
}

namespace
{
    template< typename T >
    void FillUsingMission( const std::string& objectName, const ADN_Type_Vector_ABC< T >& vector, QStringList& result, const QString& prefix = "" )
    {
        for( ADN_Type_Vector_ABC< T >::const_iterator itMission = vector.begin(); itMission != vector.end(); ++itMission )
            for( ADN_Missions_Data::T_MissionParameter_Vector::const_iterator itParam = ( *itMission )->parameters_.begin(); itParam != ( *itMission )->parameters_.end(); ++itParam )
                if( ( *itParam )->type_.GetData() == eMissionParameterTypeGenObject )
                    for( helpers::CIT_MissionGenObjectTypes_Infos_Vector itObject = ( *itParam )->genObjects_.begin(); itObject != ( *itParam )->genObjects_.end(); ++itObject )
                        if( ( *itObject )->isAllowed_.GetData() && ( *itObject )->ptrObject_.GetData()->strName_.GetData() == objectName )
                            result << ( ( prefix.isEmpty() ) ? ( *itMission )->strName_.GetData().c_str() : QString( "%1 - %2" ).arg( prefix ).arg( ( *itMission )->strName_.GetData().c_str() ) );
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
    for( ADN_Missions_Data::CIT_Mission_Vector it = unitMissions_.begin(); it != unitMissions_.end(); ++it )
    {
        CheckMissionTypeUniqueness( checker, it, unitMissions_, 0 );
        CheckParameters( checker, ( *it )->parameters_, ( *it )->strName_.GetData(), 0 );
    }
    for( ADN_Missions_Data::CIT_Mission_Vector it = automatMissions_.begin(); it != automatMissions_.end(); ++it )
    {
        CheckMissionTypeUniqueness( checker, it, automatMissions_, 1 );
        CheckParameters( checker, ( *it )->parameters_, ( *it )->strName_.GetData(), 1 );
    }
    for( ADN_Missions_Data::CIT_Mission_Vector it = populationMissions_.begin(); it != populationMissions_.end(); ++it )
    {
        CheckMissionTypeUniqueness( checker, it, populationMissions_, 2 );
        CheckParameters( checker, ( *it )->parameters_, ( *it )->strName_.GetData(), 2 );
    }
    for( ADN_Missions_Data::CIT_FragOrder_Vector it = fragOrders_.begin(); it != fragOrders_.end(); ++it )
    {
        CheckParameters( checker, ( *it )->parameters_, ( *it )->strName_.GetData(), 3 );
    }
}
