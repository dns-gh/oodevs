// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Models_Data.h"
#include "ADN_Missions_FragOrder.h"
#include "ADN_Missions_Mission.h"
#include "ADN_Models_MissionInfos.h"
#include "ADN_Models_ModelInfos.h"
#include "ADN_Models_OrderInfos.h"
#include "ADN_Project_Data.h"

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data constructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Models_Data::ADN_Models_Data()
    : ADN_Data_ABC( eModels )
{
    for( int i = 0; i < eNbrEntityType; ++i )
        vModels_[ i ].AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data destructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Models_Data::~ADN_Models_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadSourcePath
// Created: ABR 2013-09-13
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadSourcePath( xml::xistream& xis )
{
    static const tools::Path basePath( "decisional" );
    sourcePaths_.push_back( ( basePath / xis.attribute< tools::Path >( "directory" ) ).Normalize().ToUnicode() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::Initialize
// Created: JSR 2013-05-22
// -----------------------------------------------------------------------------
void ADN_Models_Data::Initialize()
{
    tools::Path p = ADN_Workspace::GetWorkspace().GetProject().GetWorkDirInfos().GetWorkingDirectory().GetData();
    const tools::Path root = p.Root();
    while( p != root && p.FileName().ToLower().ToUTF8() != "physical" )
        p = p.Parent();
    tools::Path decisionalFile = p.Parent() / "decisional/decisional.xml";
    if( decisionalFile.Exists() )
    {
        tools::Xifstream xis( decisionalFile );
        xis >> xml::start( "decisional" )
                >> xml::start( "RepertoiresSources" )
                    >> xml::list( "RepertoireSources" , *this, &ADN_Models_Data::ReadSourcePath );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Models_Data::FilesNeeded(tools::Path::T_Paths& files) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szModels_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadModels
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadModels( xml::xistream& input, E_EntityType type )
{
    std::auto_ptr< ADN_Models_ModelInfos > spNew( new ADN_Models_ModelInfos( type ) );
    spNew->ReadArchive( input );
    vModels_[ type ].AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "models" );
    for( int i = 0; i < eNbrEntityType; ++i )
    {
        E_EntityType type = static_cast< E_EntityType >( i );
        input >> xml::start( ADN_Tools::MakePluralFromEntityType( type ) )
                >> xml::list( ADN_Tr::ConvertFromEntityType( type, ENT_Tr::eToSim ), *this, &ADN_Models_Data::ReadModels, type )
              >> xml::end;
        vModels_[ i ].CheckValidity();
    }
    input >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::WriteArchive( xml::xostream& output ) const
{
    for( int i = 0; i < eNbrEntityType; ++i )
        if( vModels_[ i ].GetErrorStatus() == eError )
            throw MASA_EXCEPTION( tools::translate( "ADN_Models_Data", "Invalid data on tab '%1', subtab '%2'" )
                                  .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( ADN_Tr::ConvertFromEntityType( static_cast< E_EntityType >( i ) ).c_str() ).toStdString() );
    output << xml::start( "models" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Models" );
    for( int i = 0; i < eNbrEntityType; ++i )
    {
        E_EntityType type = static_cast< E_EntityType >( i );
        output << xml::start( ADN_Tools::MakePluralFromEntityType( type ) );
        for( auto it = vModels_[ i ].begin(); it != vModels_[ i ].end(); ++it )
            ( *it )->WriteArchive( ADN_Tr::ConvertFromEntityType( type, ENT_Tr::eToSim ), output );
        output << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetModelsThatUse
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
QStringList ADN_Models_Data::GetModelsThatUse( E_EntityType type, ADN_Missions_Mission& mission )
{
    QStringList result;
    for( auto it = vModels_[ type ].begin(); it != vModels_[ type ].end(); ++it )
        if( auto* pModel = *it )
            for( auto missionIt = pModel->vMissions_.begin(); missionIt != pModel->vMissions_.end(); ++missionIt )
                if( ( *missionIt )->GetCrossedElement() == &mission )
                {
                    result << pModel->strName_.GetData().c_str();
                    break;
                }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetModelsThatUse
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_Models_Data::GetModelsThatUse( E_EntityType type, ADN_Missions_FragOrder& fragOrder )
{
    QStringList result;
    for( auto it = vModels_[ type ].begin(); it != vModels_[ type ].end(); ++it )
    {
        bool added = false;
        for( auto itOrder = ( *it )->vFragOrders_.begin(); !added && itOrder != ( *it )->vFragOrders_.end(); ++itOrder )
        {
            if( ( *itOrder )->strName_.GetData() == fragOrder.strName_.GetData() )
            {
                added = true;
                result << ( *it )->strName_.GetData().c_str();
            }
        }
        for( auto itMission = ( *it )->vMissions_.begin(); !added && itMission != ( *it )->vMissions_.end(); ++itMission )
        {
            for( auto itOrder = ( *itMission )->vOrders_.begin(); !added && itOrder != ( *itMission )->vOrders_.end(); ++itOrder )
            {
                if( ( *itOrder )->strName_.GetData() == fragOrder.strName_.GetData() )
                {
                    added = true;
                    result << ( *it )->strName_.GetData().c_str();
                }
            }
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::CheckDatabaseValidity
// Created: JSR 2013-04-11
// -----------------------------------------------------------------------------
void ADN_Models_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( int i = 0; i < eNbrEntityType; ++i )
        for( auto it = vModels_[ i ].begin(); it != vModels_[ i ].end(); ++it )
            ( *it )->CheckValidity( checker, ( *it )->strName_.GetData(), eModels, i );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetModels
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
ADN_Type_Vector_ABC< ADN_Models_ModelInfos >& ADN_Models_Data::GetModels( E_EntityType type )
{
    return vModels_[ type ];
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::FindModel
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
ADN_Models_ModelInfos* ADN_Models_Data::FindModel( E_EntityType type, const std::string& strName )
{
    auto it = std::find_if( vModels_[ type ].begin(), vModels_[ type ].end(), ADN_Tools::NameCmp( strName ) );
    if( it == vModels_[ type ].end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetSourcePath
// Created: ABR 2013-09-13
// -----------------------------------------------------------------------------
const ADN_Models_Data::T_SourcePaths& ADN_Models_Data::GetSourcePaths() const
{
    return sourcePaths_;
}
