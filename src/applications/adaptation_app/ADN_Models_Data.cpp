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

#include "ADN_Workspace.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Models_Gui.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_enums.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"
#include "clients_kernel/XmlTranslations.h"

namespace
{
    std::string MakePluralFromEntityType( E_EntityType type )
    {
        std::string result = ADN_Tr::ConvertFromEntityType( type, ENT_Tr::eToSim );
        if( type != eEntityType_Population )
            result += 's';
        return result;
    }
}

// =============================================================================
// OrderInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: OrderInfos::OrderInfos
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
ADN_Models_Data::OrderInfos::OrderInfos( const ADN_Missions_Data::T_Mission_Vector& missions, ADN_Missions_ABC* mission /* = 0 */ )
    : ADN_CrossedRef< ADN_Missions_ABC >( missions, mission, true )
{
    // NOTHING
}

ADN_Models_Data::OrderInfos::OrderInfos( ADN_Missions_FragOrder* fragorder, const std::string& name )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder ), fragorder, true )
{
    strName_ = name;
}

// -----------------------------------------------------------------------------
// Name: OrderInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::OrderInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this );
}

// -----------------------------------------------------------------------------
// Name: OrderInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::OrderInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "fragorder" )
             << xml::attribute( "name", *this )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::OrderInfos::CreateCopy
// Created: HBD 2010-08-31
// -----------------------------------------------------------------------------
ADN_Models_Data::OrderInfos* ADN_Models_Data::OrderInfos::CreateCopy()
{
    return new OrderInfos( GetVector(), GetCrossedElement() );
}

// =============================================================================
// MissionInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionInfos::~MissionInfos
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::MissionInfos::MissionInfos( const ADN_Missions_Data::T_Mission_Vector& missions, ADN_Missions_ABC* mission /* = 0 */ )
    : ADN_CrossedRef< ADN_Missions_ABC >( missions, mission, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::~MissionInfos
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::MissionInfos::~MissionInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::CreateCopy
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::MissionInfos* ADN_Models_Data::MissionInfos::CreateCopy()
{
    MissionInfos* pMission = new MissionInfos( GetVector(), GetCrossedElement() );

    pMission->vOrders_.reserve( vOrders_.size() );
    for( auto it = vOrders_.begin(); it != vOrders_.end(); ++it )
        pMission->vOrders_.AddItem( ( *it )->CreateCopy() );
    return pMission;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::MissionInfos::ReadFragOrder
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::ReadFragOrder( xml::xistream& input )
{
    std::auto_ptr< OrderInfos > spNew( new OrderInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder ) ) );
    spNew->ReadArchive( input );
    vOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this );
    ADN_Missions_ABC* mission = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindMission( GetVector(), strName_.GetData() );
    if( !mission )
        throw MASA_EXCEPTION( tools::translate( "Models_Data", "Doctrine models - Invalid mission '%1'" ).arg( strName_.GetData().c_str() ).toStdString() );
    SetCrossedElement( mission );
    input >> xml::list( "fragorder", *this, &ADN_Models_Data::MissionInfos::ReadFragOrder );
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "mission" )
             << xml::attribute( "name", *this );
    for( auto it = vOrders_.begin(); it != vOrders_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// =============================================================================
//
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ModelInfos
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos::ModelInfos()
    : isMasalife_( false )
    , type_( eNbrEntityType )
{
    assert( false ); // $$$$ ABR 2013-08-23: useless constructor, needed by ADN_Wizard...
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ModelInfos
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos::ModelInfos( E_EntityType type )
    : isMasalife_( false )
    , type_( type )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eModels, MakePluralFromEntityType( type ) ) );
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::ModelInfos
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos::~ModelInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::AddFragOrder
// Created: HBD 2010-09-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::AddFragOrder( ADN_Missions_FragOrder* fragorder, const std::string& name )
{
    std::auto_ptr< OrderInfos > spNew( new OrderInfos( fragorder, name ) );
    vFragOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::RemoveFragOder
// Created: HBD 2010-09-06
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::RemoveFragOder( const std::string& order )
{
    for( auto it = vFragOrders_.begin(); it != vFragOrders_.end(); ++it )
        if( (*it)->strName_ == order)
        {
            vFragOrders_.RemItem( *it );
            break;
        }
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::CreateCopy
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos* ADN_Models_Data::ModelInfos::CreateCopy()
{
    ModelInfos* pNewInfo = new ModelInfos( type_ );
    pNewInfo->strDiaType_ = strDiaType_.GetData();
    pNewInfo->strFile_ = strFile_.GetData();
    pNewInfo->isMasalife_ = isMasalife_.GetData();

    pNewInfo->vMissions_.reserve( vMissions_.size() );
    for( T_MissionInfos_Vector::iterator itMission = vMissions_.begin(); itMission != vMissions_.end(); ++itMission )
        pNewInfo->vMissions_.AddItem( (*itMission)->CreateCopy() );
    pNewInfo->vFragOrders_.reserve( vFragOrders_.size() );
    for( T_OrderInfos_Vector::iterator itOrder = vFragOrders_.begin(); itOrder != vFragOrders_.end(); ++itOrder )
        pNewInfo->vFragOrders_.AddItem( (*itOrder)->CreateCopy() );
    return pNewInfo;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ModelInfos::ReadMission
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::ReadMission( xml::xistream& input )
{
    std::auto_ptr<MissionInfos> spNew( new MissionInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( ADN_Tools::ConvertEntityTypeToMissionType( type_ ) ) ) );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vMissions_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eModels, -1, tools::translate( "ADN_Models_Data", "Missions" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ModelInfos::ReadOrder
// Created: HBD 2010-08-31
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::ReadOrder( xml::xistream& input )
{
    std::auto_ptr<OrderInfos> spNew( new OrderInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder ) ) );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vFragOrders_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eModels, -1, tools::translate( "ADN_Models_Data", "Frag orders" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "dia-type", strDiaType_ )
          >> xml::attribute( "file", strFile_ )
          >> xml::attribute( "masalife", isMasalife_ )
          >> xml::start( "missions" )
            >> xml::list( "mission", *this, &ADN_Models_Data::ModelInfos::ReadMission )
        >> xml::end
        >> xml::optional
        >> xml::start( "fragorders" )
            >> xml::list( "fragorder", *this, &ADN_Models_Data::ModelInfos::ReadOrder )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::WriteArchive( const std::string& type, xml::xostream& output )
{
    output << xml::start( type )
            <<  xml::attribute( "name", *this )
            <<  xml::attribute( "dia-type", strDiaType_ )
            <<  xml::attribute( "file", strFile_.GetData().Normalize() )
            <<  xml::attribute( "masalife", isMasalife_ )
            <<  xml::start( "missions" );
    for( auto it = vMissions_.begin(); it != vMissions_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end
        << xml::start( "fragorders" );
    for( auto it = vFragOrders_.begin(); it != vFragOrders_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end
        << xml::end;
}

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
    std::auto_ptr< ModelInfos > spNew( new ModelInfos( type ) );
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
        input >> xml::start( MakePluralFromEntityType( type ) )
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
        output << xml::start( MakePluralFromEntityType( type ) );
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
        if( ModelInfos* pModel = *it )
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
