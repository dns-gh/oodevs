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

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_DataException.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

// =============================================================================
// OrderInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: OrderInfos::OrderInfos
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
ADN_Models_Data::OrderInfos::OrderInfos()
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, fragOrder_( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetFragOrders(), 0 )
{
    this->BindExistenceTo( &fragOrder_ );
}

ADN_Models_Data::OrderInfos::OrderInfos( ADN_Missions_Data::FragOrder* fragorder, const std::string& name )
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, fragOrder_( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetFragOrders(), 0 )
{
    fragOrder_ = fragorder;
    strName_ = name;
    this->BindExistenceTo( &fragOrder_ );
}

// -----------------------------------------------------------------------------
// Name: OrderInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Models_Data::OrderInfos::GetItemName()
{
    return strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: OrderInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::OrderInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ );
    ADN_Missions_Data::FragOrder* fragOrder = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindFragOrder( strName_.GetData() );
    if( !fragOrder )
        throw ADN_DataException( tools::translate( "Models_Data", "Invalid data" ).ascii(), tools::translate( "Models_Data", "Doctrine models - Invalid frag order '%1'" ).arg( strName_.GetData().c_str() ).ascii() );
    fragOrder_ = fragOrder;
}

// -----------------------------------------------------------------------------
// Name: OrderInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::OrderInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "fragorder" )
            << xml::attribute( "name", fragOrder_.GetData()->strName_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::OrderInfos::CreateCopy
// Created: HBD 2010-08-31
// -----------------------------------------------------------------------------
ADN_Models_Data::OrderInfos* ADN_Models_Data::OrderInfos::CreateCopy()
{
    OrderInfos* result = new OrderInfos();
    result->fragOrder_ = fragOrder_.GetData();
    result->strName_ = strName_.GetData();
    return result;
}

// =============================================================================
// MissionInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionInfos::~MissionInfos
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::MissionInfos::MissionInfos( ADN_Missions_Data::T_Mission_Vector& missions )
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , mission_( missions, 0 )
{
    vOrders_.SetItemTypeName( "un ordre" );
    vOrders_.SetNodeName( "la liste des ordres" );
    vOrders_.SetParentNode( *this );
    this->BindExistenceTo( &mission_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::~MissionInfos
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::MissionInfos::~MissionInfos()
{
    vOrders_.Reset();
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Models_Data::MissionInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Models_Data::MissionInfos::GetNodeName()
{
    std::string strResult( "de la mission " );
    return strResult + mission_.GetNodeName();
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::CreateCopy
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::MissionInfos* ADN_Models_Data::MissionInfos::CreateCopy()
{
    MissionInfos* pMission = new MissionInfos( mission_.GetVector() );
    pMission->mission_ = mission_.GetData();
    pMission->strName_ = strName_.GetData();

    pMission->vOrders_.reserve( vOrders_.size() );
    for( T_OrderInfos_Vector::iterator it = vOrders_.begin(); it != vOrders_.end(); ++it )
    {
        OrderInfos* pOrder = new OrderInfos();
        pOrder->fragOrder_ = (*it)->fragOrder_.GetData();
        pOrder->strName_   = (*it)->strName_.GetData();
        pMission->vOrders_.AddItem( pOrder );
    }
    return pMission;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::MissionInfos::ReadFragOrder
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::ReadFragOrder( xml::xistream& input )
{
    std::auto_ptr< OrderInfos > spNew( new OrderInfos() );
    spNew->ReadArchive( input );
    vOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ );
    ADN_Missions_Data::Mission* mission = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindMission( mission_.GetVector(), strName_.GetData() );
    if( !mission )
        throw ADN_DataException( tools::translate( "Models_Data", "Invalid data" ).ascii(), tools::translate( "Models_Data", "Doctrine models - Invalid mission '%1'" ).arg( strName_.GetData().c_str() ).ascii() );
    mission_ = mission;
    mission_.GetData()->strName_.Connect( &strName_ );
    input >> xml::list( "fragorder", *this, &ADN_Models_Data::MissionInfos::ReadFragOrder );
}

// -----------------------------------------------------------------------------
// Name: MissionInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "mission" )
            << xml::attribute( "name", mission_.GetData()->strName_ );
    for( IT_OrderInfos_Vector it = vOrders_.begin(); it != vOrders_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// =============================================================================
//
// =============================================================================

namespace
{
    ADN_Missions_Data::T_Mission_Vector dummy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ModelInfos
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos::ModelInfos()
    : ADN_Ref_ABC()
    , missions_ ( dummy )
{
    isMasalife_ = false;
}

//-----------------------------------------------------------------------------
// Name: ModelInfos::ModelInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos::ModelInfos( ADN_Missions_Data::T_Mission_Vector& missions )
    : ADN_Ref_ABC()
    , missions_( missions )
{
    strDiaType_ = "T_Pion";
    strFile_ = "DEC\\For Tests\\Empty\\Files.hal";
    isMasalife_ = false;

    strName_.SetDataName( "le nom" );
    strName_.SetParentNode( *this );
    strDiaType_.SetDataName( "le type DirectIA" );
    strDiaType_.SetParentNode( *this );
    strFile_.SetDataName( "le fichier de script" );
    strFile_.SetParentNode( *this );
    isMasalife_.SetDataName( "is masalife" );
    isMasalife_.SetParentNode( *this );

    vMissions_.SetParentNode( *this );
    vMissions_.SetItemTypeName( "une mission" );
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::ModelInfos
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos::~ModelInfos()
{
    vMissions_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Models_Data::ModelInfos::GetNodeName()
{
    std::string strResult( "du modèle " );
    return strResult + strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::AddFragOrder
// Created: HBD 2010-09-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::AddFragOrder( ADN_Missions_Data::FragOrder* fragorder, const std::string& name )
{
    std::auto_ptr<OrderInfos> spNew( new OrderInfos( fragorder, name ) );
    vFragOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::RemoveFragOder
// Created: HBD 2010-09-06
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::RemoveFragOder( const std::string& order )
{
    for( T_OrderInfos_Vector::iterator it = vFragOrders_.begin(); it != vFragOrders_.end(); ++it )
        if ( (*it)->strName_ == order)
        {
            vFragOrders_.RemItem( *it );
            break;
        }
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Models_Data::ModelInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::CreateCopy
// Created: AGN 2003-12-03
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos* ADN_Models_Data::ModelInfos::CreateCopy()
{
    ModelInfos* pNewInfo = new ModelInfos( missions_ );
    pNewInfo->strDiaType_ = strDiaType_.GetData();
    pNewInfo->strFile_ = strFile_.GetData();
    pNewInfo->isMasalife_ = isMasalife_.GetData();

    pNewInfo->vMissions_.reserve( vMissions_.size() );
    for( T_MissionInfos_Vector::iterator itMission = vMissions_.begin(); itMission != vMissions_.end(); ++itMission )
    {
        MissionInfos* pNewMission = (*itMission)->CreateCopy();
        pNewInfo->vMissions_.AddItem( pNewMission );
    }
    pNewInfo->vFragOrders_.reserve( vFragOrders_.size() );
    for( T_OrderInfos_Vector::iterator itOrder = vFragOrders_.begin(); itOrder != vFragOrders_.end(); ++itOrder )
    {
        OrderInfos* pNewOrder = (*itOrder)->CreateCopy();
        pNewInfo->vFragOrders_.AddItem( pNewOrder );
    }
    return pNewInfo;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ModelInfos::ReadMission
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::ReadMission( xml::xistream& input )
{
    std::auto_ptr<MissionInfos> spNew( new MissionInfos( missions_ ) );
    spNew->ReadArchive( input );
    vMissions_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ModelInfos::ReadOrder
// Created: HBD 2010-08-31
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::ReadOrder( xml::xistream& input )
{
    std::auto_ptr<OrderInfos> spNew( new OrderInfos() );
    spNew->ReadArchive( input );
    vFragOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ModelInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ )
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
    std::string strFileName = strFile_.GetData();
    std::replace( strFileName.begin(), strFileName.end(), '/', '\\' );

    output << xml::start( type )
            <<  xml::attribute( "name", strName_ )
            <<  xml::attribute( "dia-type", strDiaType_ )
            <<  xml::attribute( "file", strFileName )
            <<  xml::attribute( "masalife", isMasalife_ )
            <<  xml::start( "missions" );
    for( IT_MissionInfos_Vector it = vMissions_.begin(); it != vMissions_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end
        << xml::start( "fragorders" );
    for( IT_OrderInfos_Vector it = vFragOrders_.begin(); it != vFragOrders_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end
        << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data constructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Models_Data::ADN_Models_Data()
{
    vUnitModels_.SetItemTypeName( "un modèle" );
    vAutomataModels_.SetItemTypeName( "un modèle" );
    vPopulationModels_.SetItemTypeName( "un modèle" );
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data destructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Models_Data::~ADN_Models_Data()
{
    Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data::Reset
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
void ADN_Models_Data::Reset()
{
    vUnitModels_.Reset();
    vAutomataModels_.Reset();
    vPopulationModels_.Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Models_Data::FilesNeeded(T_StringList& files) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szModels_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadUnit
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadUnit( xml::xistream& input )
{
    std::auto_ptr<ModelInfos> spNew( new ModelInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissions() ) );
    spNew->ReadArchive( input );
    vUnitModels_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadAutomat
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadAutomat( xml::xistream& input )
{
    std::auto_ptr<ModelInfos> spNew( new ModelInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetAutomatMissions() ) );
    spNew->ReadArchive( input );
    vAutomataModels_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadPopulation
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadPopulation( xml::xistream& input )
{
    std::auto_ptr<ModelInfos> spNew( new ModelInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetPopulationMissions() ) );
    spNew->ReadArchive( input );
    vPopulationModels_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "models" )
            >> xml::start( "units" )
                >> xml::list( "unit", *this, &ADN_Models_Data::ReadUnit )
            >> xml::end
            >> xml::start( "automats" )
                >> xml::list( "automat", *this, &ADN_Models_Data::ReadAutomat )
            >> xml::end
            >> xml::start( "crowd" )
                >> xml::list( "crowd", *this, &ADN_Models_Data::ReadPopulation )
            >> xml::end
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "models" );
    ADN_Tools::AddSchema( output, "Models" );
    output  << xml::start( "units" );
    for( IT_ModelInfos_Vector it1 = vUnitModels_.begin(); it1 != vUnitModels_.end(); ++it1 )
        (*it1)->WriteArchive( "unit", output );
    output << xml::end
            << xml::start( "automats" );
    for( IT_ModelInfos_Vector it2 = vAutomataModels_.begin(); it2 != vAutomataModels_.end(); ++it2 )
        (*it2)->WriteArchive( "automat", output );
    output << xml::end
            << xml::start( "crowd" );
    for( IT_ModelInfos_Vector it2 = vPopulationModels_.begin(); it2 != vPopulationModels_.end(); ++it2 )
        (*it2)->WriteArchive( "crowd", output );
    output << xml::end
          << xml::end;
}
