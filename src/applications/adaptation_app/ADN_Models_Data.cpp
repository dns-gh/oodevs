// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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

#include "ADN_Xml_Exception.h"
#include "ADN_XmlInput_Helper.h"

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
    // NOTHING
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
void ADN_Models_Data::OrderInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "OrdreConduite" );
    input.ReadAttribute( "nom", strName_ );
    ADN_Missions_Data::FragOrder* fragOrder = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindFragOrder( strName_.GetData() );
    if( !fragOrder )
        throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Invalid frag order: " ).append( strName_.GetData().c_str() ).ascii() );
    fragOrder_ = fragOrder;
    input.EndSection(); // OrdreConduite
}


// -----------------------------------------------------------------------------
// Name: OrderInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::OrderInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "OrdreConduite" );
    output.WriteAttribute( "nom", fragOrder_.GetData()->strName_.GetData() );
    output.EndSection(); // OrdreConduite
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
// Name: MissionInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Mission" );
    input.ReadAttribute( "nom", strName_ );

    ADN_Missions_Data::Mission* mission = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindMission( mission_.GetVector(), strName_.GetData() );
    if( !mission )
        throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Invalid mission: " ).append( strName_.GetData().c_str() ).ascii() );
    mission_ = mission;
    if( input.BeginList( "OrdresConduite", ADN_XmlInput_Helper::eNothing ) )
    {
        while( input.NextListElement() )
        {
            std::auto_ptr< OrderInfos > spNew( new OrderInfos() );
            spNew->ReadArchive( input );
            vOrders_.AddItem( spNew.release() );
        }
        input.EndList(); // OrdresConduite
    }
    input.EndSection(); // Mission
}


// -----------------------------------------------------------------------------
// Name: MissionInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::MissionInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Mission" );
    output.WriteAttribute( "nom", mission_.GetData()->strName_.GetData() );

    if( ! vOrders_.empty() )
    {
        output.BeginList( "OrdresConduite", vOrders_.size() );
        for( IT_OrderInfos_Vector it = vOrders_.begin(); it != vOrders_.end(); ++it )
            (*it)->WriteArchive( output );
        output.EndList(); // OrdresConduite
    }
    output.EndSection(); // Mission
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
    // NOTHING
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

    strName_.SetDataName( "le nom" );
    strName_.SetParentNode( *this );
    strDiaType_.SetDataName( "le type DirectIA" );
    strDiaType_.SetParentNode( *this );
    strFile_.SetDataName( "le fichier de script" );
    strFile_.SetParentNode( *this );

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
    std::string strResult( "du mod�le " );
    return strResult + strName_.GetData();
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

    pNewInfo->vMissions_.reserve( vMissions_.size() );
    for( T_MissionInfos_Vector::iterator itMission = vMissions_.begin(); itMission != vMissions_.end(); ++itMission )
    {
        MissionInfos* pNewMission = (*itMission)->CreateCopy();
        pNewInfo->vMissions_.AddItem( pNewMission );
    }

    return pNewInfo;
}


// -----------------------------------------------------------------------------
// Name: ModelInfos::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Modele" );
    input.ReadAttribute( "nom", strName_ );
    input.ReadField( "DIAType", strDiaType_ );
    input.ReadField( "File", strFile_ );
    input.BeginList( "Missions" );
    while( input.NextListElement() )
    {
        std::auto_ptr<MissionInfos> spNew( new MissionInfos( missions_ ) );
        spNew->ReadArchive( input );
        vMissions_.AddItem( spNew.release() );
    }
    input.EndList(); // Missions
    input.EndSection(); // Modele
}


// -----------------------------------------------------------------------------
// Name: ModelInfos::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ModelInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Modele" );
    output.WriteAttribute( "nom", strName_.GetData() );
    output.WriteField( "DIAType", strDiaType_.GetData() );

    // For some reason, the users want this with backslashes.
    std::string strFileName = strFile_.GetData();
    strFileName = ADN_Tools::Replace( strFileName, '/', '\\' );
    output.WriteField( "File", strFileName );

    output.BeginList( "Missions", vMissions_.size() );
    for( IT_MissionInfos_Vector it = vMissions_.begin(); it != vMissions_.end(); ++it )
    {
        (*it)->WriteArchive( output );
    }
    output.EndList(); // Missions
    output.EndSection(); // Modele
}


//-----------------------------------------------------------------------------
// Name: ADN_Models_Data constructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Models_Data::ADN_Models_Data()
{
    vUnitModels_.SetItemTypeName( "un mod�le" );
    vAutomataModels_.SetItemTypeName( "un mod�le" );
    vPopulationModels_.SetItemTypeName( "un mod�le" );
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
    files.push_back( ComputeFileName() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Modeles" );

    input.BeginList( "Pions" );
    while( input.NextListElement() )
    {
        std::auto_ptr<ModelInfos> spNew( new ModelInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissions() ) );
        spNew->ReadArchive( input );
        vUnitModels_.AddItem( spNew.release() );
    }
    input.EndList(); // Pions

    input.BeginList( "Automates" );
    while( input.NextListElement() )
    {
        std::auto_ptr<ModelInfos> spNew( new ModelInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetAutomatMissions() ) );
        spNew->ReadArchive( input );
        vAutomataModels_.AddItem( spNew.release() );
    }
    input.EndList(); // Automates

    input.BeginList( "Populations" );
    while( input.NextListElement() )
    {
        std::auto_ptr<ModelInfos> spNew( new ModelInfos( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetPopulationMissions() ) );
        spNew->ReadArchive( input );
        vPopulationModels_.AddItem( spNew.release() );
    }
    input.EndList(); // Populations

    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Models_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Modeles" );

    output.BeginList( "Pions", vUnitModels_.size() );
    for( IT_ModelInfos_Vector it1 = vUnitModels_.begin(); it1 != vUnitModels_.end(); ++it1 )
        (*it1)->WriteArchive( output );
    output.EndList(); // Pions

    output.BeginList( "Automates", vAutomataModels_.size() );
    for( IT_ModelInfos_Vector it2 = vAutomataModels_.begin(); it2 != vAutomataModels_.end(); ++it2 )
        (*it2)->WriteArchive( output );
    output.EndList(); // Automates

    output.BeginList( "Populations", vPopulationModels_.size() );
    for( IT_ModelInfos_Vector it2 = vPopulationModels_.begin(); it2 != vPopulationModels_.end(); ++it2 )
        (*it2)->WriteArchive( output );
    output.EndList(); // Populations

    output.EndSection();
}


// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::ComputeFileName
// Created: APE 2005-02-15
// -----------------------------------------------------------------------------
std::string ADN_Models_Data::ComputeFileName() const
{
    std::string szDecisionalFile = ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szDecisional_.GetData();
    assert( szDecisionalFile.length() > 0 );

    int nPos = szDecisionalFile.rfind( '/', szDecisionalFile.length() -1 );
    if( nPos < 0 || nPos >= (int)szDecisionalFile.length() ) 
        nPos = szDecisionalFile.length();
    std::string szDecisionalDirectory = szDecisionalFile.substr( 0, nPos );
    std::string strFile = szDecisionalDirectory + "/" + ADN_Workspace::GetWorkspace().GetAiEngine().GetData().strModelsFile_.GetData();
    return strFile;
}
