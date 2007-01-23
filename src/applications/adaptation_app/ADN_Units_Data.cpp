//*****************************************************************************
//
// $Created: JDY 03-07-24 $
// $input: /MVW_v10/Build/SDK/ADN/Src/ADN_Units_Data.cpp $
// $Author: Ape $
// $Modtime: 7/06/05 10:04 $
// $Revision: 20 $
// $Workfile: ADN_Units_Data.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Units_Data.h"

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_DataException.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"

#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"

#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

#include <sstream>

// =============================================================================
// 
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ComposanteInfos::ComposanteInfos
// Created: JDY 03-07-25
//-----------------------------------------------------------------------------
ADN_Units_Data::ComposanteInfos::ComposanteInfos()
: ADN_Ref_ABC   ()
, ptrComposante_( ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantes(),0)
, bLoadable_    ( false )
, bMajor_       ( false )
, bConveyor_    ( false )
, nNbrHumanInCrew_( 0 )
, nNb_          ( 1 )
{
    BindExistenceTo( &ptrComposante_ );

    bLoadable_.SetDataName( "la propriété" );
    bLoadable_.SetParentNode( *this );
    bMajor_.SetDataName( "la primauté" );
    bMajor_.SetParentNode( *this );
    bConveyor_.SetDataName( "hein ?" ); //XXX
    bConveyor_.SetParentNode( *this );    
    nNbrHumanInCrew_.SetDataName( "Comprends pas ..." );
    nNbrHumanInCrew_.SetParentNode( *this );    
}


// -----------------------------------------------------------------------------
// Name: ComposanteInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::ComposanteInfos::GetNodeName()
{
    std::string strResult( "d'une composante de type " );
    return strResult + GetItemName();
}


// -----------------------------------------------------------------------------
// Name: ComposanteInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::ComposanteInfos::GetItemName()
{
    return ptrComposante_.GetData()->strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: ComposanteInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Units_Data::ComposanteInfos* ADN_Units_Data::ComposanteInfos::CreateCopy()
{
    ComposanteInfos* pCopy = new ComposanteInfos();

    pCopy->ptrComposante_ = ptrComposante_.GetData();
    pCopy->bMajor_ = bMajor_.GetData();
    pCopy->bLoadable_ = bLoadable_.GetData();
    pCopy->bConveyor_ = bConveyor_.GetData();
    pCopy->nNbrHumanInCrew_ = nNbrHumanInCrew_.GetData();
    pCopy->nNb_ = nNb_.GetData();

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: ComposanteInfos::ReadArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::ComposanteInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Equipement" );

    std::string strName;
    std::string strMain;
    std::string strProperty;

    input.ReadAttribute( "nom", strName );
    ADN_Composantes_Data::ComposanteInfos* pComposante = ADN_Workspace::GetWorkspace().GetComposantes().GetData().FindComposante( strName );
    if( pComposante == 0 )
        input.ThrowError( tr( "Composante %1 does not exist." ).ascii() );
    ptrComposante_ = pComposante;

    input.ReadAttribute( "majeur", bMajor_, ADN_XmlInput_Helper::eNothing );
    input.ReadAttribute( "embarquable", bLoadable_, ADN_XmlInput_Helper::eNothing );
    input.ReadAttribute( "convoyeur", bConveyor_, ADN_XmlInput_Helper::eNothing );
    input.ReadAttribute( "equipage", nNbrHumanInCrew_, ADN_XmlInput_Helper::eNothing );

    int nNbr;
    input.Read( nNbr );
    nNb_ = nNbr;

    input.EndSection(); // Equipement
}


// -----------------------------------------------------------------------------
// Name: ComposanteInfos::WriteArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::ComposanteInfos::WriteArchive( MT_OutputArchive_ABC& output, bool bIsAutonomous )
{
    output.Section( "Equipement" );
    output.WriteAttribute( "nom", ptrComposante_.GetData()->strName_.GetData() );

    if( bMajor_.GetData() )
        output.WriteAttribute( "majeur", "true" );
    if( bLoadable_.GetData() )
        output.WriteAttribute( "embarquable", "true" );
    if( bConveyor_.GetData() )
        output.WriteAttribute( "convoyeur", "true" );
    
    output.WriteAttribute( "equipage", nNbrHumanInCrew_.GetData() );
    if( !bIsAutonomous && nNbrHumanInCrew_.GetData() == 0 )
        throw ADN_DataException( "Mauvaises données dans les catégories",
            "Il existe un pion dont un equipement a un équipage vide." );

    output << nNb_.GetData();

    output.EndSection(); // Equipement
}

// =============================================================================
// StockLogThresholdInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockLogThresholdInfos::StockLogThresholdInfos
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
ADN_Units_Data::StockLogThresholdInfos::StockLogThresholdInfos( E_StockCategory eCategory )
: ADN_DataTreeNode_ABC()
, eCategory_          ( eCategory )
, rLogThreshold_      ( 0. )
{
    ADN_Type_Enum< E_StockCategory, eNbrStockCategory >::SetConverter( &ADN_Tr::ConvertFromStockCategory );
    eCategory_.SetDataName( "la catégorie de stock" );
    eCategory_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockLogThresholdInfos::GetNodeName
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::StockLogThresholdInfos::GetNodeName()
{
    return std::string();
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockLogThresholdInfos::GetItemName
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::StockLogThresholdInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockLogThresholdInfos::ReadArchive
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockLogThresholdInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "SeuilLogistique" );
    std::string strCategory;
    input.ReadAttribute( "categorie", strCategory );
    E_StockCategory eCategory = ADN_Tr::ConvertToStockCategory( strCategory );
    if( eCategory == (E_StockCategory)-1 )
        input.ThrowError( MT_FormatString( "La category de stock '%s' est invalide", strCategory.c_str() ) );
    eCategory_ = eCategory;
    input.ReadAttribute( "seuil", rLogThreshold_ );
    input.EndSection(); // SeuilLogistique
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockLogThresholdInfos::WriteArchive
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockLogThresholdInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "SeuilLogistique" );
    output.WriteAttribute( "categorie", ADN_Tr::ConvertFromStockCategory( eCategory_.GetData() ) );
    output.WriteAttribute( "seuil"    , rLogThreshold_.GetData() );
    output.EndSection(); // SeuilLogistique
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockLogThresholdInfos::CreateCopy
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
ADN_Units_Data::StockLogThresholdInfos* ADN_Units_Data::StockLogThresholdInfos::CreateCopy()
{
    StockLogThresholdInfos* pCopy = new StockLogThresholdInfos();
    pCopy->eCategory_     = eCategory_.GetData();
    pCopy->rLogThreshold_ = rLogThreshold_.GetData();
    return pCopy;
}

// =============================================================================
// StockInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockInfos::StockInfos
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
ADN_Units_Data::StockInfos::StockInfos()
: ADN_DataTreeNode_ABC()
, vLogThresholds_     ()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockInfos::GetNodeName
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::StockInfos::GetNodeName()
{
    return std::string();
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockInfos::GetItemName
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::StockInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockInfos::ReadArchive
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    if( input.BeginList( "SeuilsLogistiques", ADN_XmlInput_Helper::eNothing ) )
    {
        while( input.NextListElement() )
        {
            std::auto_ptr< StockLogThresholdInfos > spNew( new StockLogThresholdInfos() );
            spNew->ReadArchive( input );
            vLogThresholds_.AddItem( spNew.release() );
        }
        input.EndList(); // SeuilsLogistiques
    }
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockInfos::WriteArchive
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockInfos::WriteArchive( const std::string& strName, MT_OutputArchive_ABC& output )
{
    output.Section( strName );
    if( vLogThresholds_.size() != 0 )
    {
        output.Section( "SeuilsLogistiques" );
        for( IT_StockLogThresholdInfos_Vector it = vLogThresholds_.begin(); it != vLogThresholds_.end(); ++it )
            (*it)->WriteArchive( output );
        output.EndSection(); // SeuilsLogistiques
    }
    output.EndSection(); // strName
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockInfos::CopyFrom
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockInfos::CopyFrom( StockInfos& src )
{
    for( IT_StockLogThresholdInfos_Vector it = src.vLogThresholds_.begin(); it != src.vLogThresholds_.end(); ++it )
        vLogThresholds_.AddItemNoEmit( (*it)->CreateCopy() );
}

// =============================================================================
// PointInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PointInfos::PointInfos
// Created: AGN 2003-12-01
// -----------------------------------------------------------------------------
ADN_Units_Data::PointInfos::PointInfos()
: nTypeTerrain_( (E_KeyPoint)-1 )
, nDistance_( 100 )
{
    nDistance_.SetDataName( "la distance avant un point" );
    nDistance_.SetParentNode( *this );
}


// -----------------------------------------------------------------------------
// Name: PointInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::PointInfos::GetNodeName()
{
    std::string strResult( "sur un terrain " );
    return strResult + ADN_Tr::ConvertFromKeyPoint( nTypeTerrain_, ENT_Tr_ABC::eToTr );
}


// -----------------------------------------------------------------------------
// Name: PointInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::PointInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: PointInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Units_Data::PointInfos* ADN_Units_Data::PointInfos::CreateCopy()
{
    PointInfos* pCopy = new PointInfos();
    pCopy->nTypeTerrain_ = nTypeTerrain_;
    pCopy->nDistance_ = nDistance_.GetData();
    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: PointInfos::ReadArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::PointInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Point" );

    std::string strType;
    input.ReadAttribute( "type", strType );

    nTypeTerrain_ = ADN_Tr::ConvertToKeyPoint( strType );

    int nDistance;
    input.Read( nDistance );
    nDistance_ = nDistance;

    input.EndSection(); // Point
}


// -----------------------------------------------------------------------------
// Name: PointInfos::WriteArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::PointInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Point" );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromKeyPoint( nTypeTerrain_ ) );
    output << nDistance_.GetData();
    output.EndSection(); // Point
}

// =============================================================================
// 
// =============================================================================

//-----------------------------------------------------------------------------
// Name: PostureInfos::PostureInfos
// Created: JDY 03-07-25
//-----------------------------------------------------------------------------
ADN_Units_Data::PostureInfos::PostureInfos(const E_UnitPosture& nPosture)
:   nPosture_       ( nPosture )
,   timeToActivate_ ( "0s" )
{
    timeToActivate_.SetParentNode( *this );
    timeToActivate_.SetDataName( "le délai de mise en posture" );
}


// -----------------------------------------------------------------------------
// Name: PostureInfos::GetNodeName

// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::PostureInfos::GetNodeName()
{
    return ENT_Tr::ConvertFromUnitPosture( nPosture_, ENT_Tr_ABC::eToTr );
}


// -----------------------------------------------------------------------------
// Name: PostureInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::PostureInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: PostureInfos::ReadArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::PostureInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( ADN_Tools::ComputePostureScriptName( nPosture_ ) );

    std::string strTmp;
    input.Read( strTmp );
    timeToActivate_ = strTmp;

    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: PostureInfos::WriteArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::PostureInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( ADN_Tools::ComputePostureScriptName( nPosture_ ) );
    output << timeToActivate_.GetData();
    output.EndSection();
}


// =============================================================================
// 
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos
// Created: JDY 03-07-25
//-----------------------------------------------------------------------------
ADN_Units_Data::UnitInfos::UnitInfos()
: ADN_Ref_ABC()
, eTypeId_((E_AgentTypePion)0)
, strName_()
, nMosId_( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetNextId() )
, ptrModel_( ADN_Workspace::GetWorkspace().GetModels().GetData().GetUnitModelsInfos(), 0 )
, eNatureLevel_((E_NatureLevel)0)
, nNbOfficer_(0)
, nNbNCOfficer_(0)
, decontaminationDelay_( "0s" )
, vComposantes_()
, vPostures_( false )
, vPointInfos_( false )
, bProbe_(false)
, bStock_( false )
, stocks_()
, bStrengthRatioFeedbackTime_( false )
, strengthRatioFeedbackTime_( "0s" )
, rProbeWidth_(0)
, rProbeLength_(0)
, bCanFly_( false )
, bIsAutonomous_( false )
, rWeaponsReach_( 0.0 )
, rSensorsReach_( 0.0 )
, bInstallationDelay_( false )
, installationDelay_( "0s" )
, uninstallationDelay_( "0s" )
{
    BindExistenceTo(&ptrModel_);

    vPostures_.SetParentNode( *this );
    vPointInfos_.SetParentNode( *this );

    ADN_Type_Enum<E_AgentTypePion,eNbrAgentTypePion>::SetConverter( &ADN_Tr::ConvertFromAgentTypePion );
    eTypeId_.SetDataName( "le type" );
    eTypeId_.SetParentNode( *this );

    strName_.SetDataName( "le nom" );
    strName_.SetParentNode( *this );

    nNbOfficer_.SetDataName( "le nombre d'officiers" );
    nNbOfficer_.SetParentNode( *this );

    nNbNCOfficer_.SetDataName( "le nombre de sous-officiers" );
    nNbNCOfficer_.SetParentNode( *this );

    ptrModel_.SetNodeName( "le modèle décisionnel" );
    ptrModel_.SetParentNode( *this );

    vComposantes_.SetItemTypeName( "une composante" );
    vComposantes_.SetParentNode( *this );

    ADN_Type_Enum<E_NatureLevel,eNbrNatureLevel>::SetConverter( &ENT_Tr::ConvertFromNatureLevel );
    eNatureLevel_.SetDataName( "la taille" );
    eNatureLevel_.SetParentNode( *this );

    ADN_Type_Enum<E_NatureAtlasType,eNbrNatureAtlas>::SetConverter( &ADN_Tr::ConvertFromNatureAtlasType );
    eNatureAtlas_.SetDataName( "la nature ATLAS" );
    eNatureAtlas_.SetParentNode( *this );

    strNature_.SetDataName( "la nature, les arbres et les oiseaux" );
    strNature_.SetParentNode( *this );

    // postures initialization
    for( int i = ePostureNeedTimeStart; i < eNbrUnitPosture; ++i )
        vPostures_.AddItem( new PostureInfos((E_UnitPosture)i) );
}


//-----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos
// Created: JDY 03-07-28
//-----------------------------------------------------------------------------
ADN_Units_Data::UnitInfos::~UnitInfos()
{
    vComposantes_.Reset();
    vPostures_.Reset();
    vPointInfos_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::UnitInfos::GetNodeName()
{
    std::string strResult( "du pion " );
    return strResult + strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::UnitInfos::GetItemName()
{
    return strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::CreateCopy
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Units_Data::UnitInfos* ADN_Units_Data::UnitInfos::CreateCopy()
{
    UnitInfos* pCopy = new UnitInfos();

    pCopy->eTypeId_ = eTypeId_.GetData();
    pCopy->ptrModel_ = ptrModel_.GetData();
    pCopy->eNatureLevel_ = eNatureLevel_.GetData();
    pCopy->eNatureAtlas_ = eNatureAtlas_.GetData();
    pCopy->strNature_     = strNature_.GetData();
    pCopy->nNbOfficer_ = nNbOfficer_.GetData();
    pCopy->nNbNCOfficer_ = nNbNCOfficer_.GetData();
    pCopy->decontaminationDelay_ = decontaminationDelay_.GetData();
    pCopy->bCanFly_ = bCanFly_.GetData();
    pCopy->bIsAutonomous_ = bIsAutonomous_.GetData();
    pCopy->rWeaponsReach_ = rWeaponsReach_.GetData();
    pCopy->rSensorsReach_ = rSensorsReach_.GetData();

    for( T_ComposanteInfos_Vector::iterator itComposante = vComposantes_.begin(); itComposante != vComposantes_.end(); ++itComposante )
        pCopy->vComposantes_.AddItem( (*itComposante)->CreateCopy() );

    for( uint i = 0; i < vPostures_.size(); ++i )
        pCopy->vPostures_[ i ]->timeToActivate_ = vPostures_[ i ]->timeToActivate_.GetData();

    for( T_PointInfos_Vector::iterator itPoint = vPointInfos_.begin(); itPoint != vPointInfos_.end(); ++itPoint )
        pCopy->vPointInfos_.AddItem( (*itPoint)->CreateCopy() );

    pCopy->bTC1_ = bTC1_.GetData();
    pCopy->contenancesTC1_.CopyFrom( contenancesTC1_ );
    pCopy->bStock_ = bStock_.GetData();
    pCopy->stocks_.CopyFrom( stocks_ );

    pCopy->bProbe_ = bProbe_.GetData();
    pCopy->rProbeLength_ = rProbeLength_.GetData();
    pCopy->rProbeWidth_ = rProbeWidth_.GetData();

    pCopy->bStrengthRatioFeedbackTime_ = bStrengthRatioFeedbackTime_.GetData();
    pCopy->strengthRatioFeedbackTime_  = strengthRatioFeedbackTime_.GetData();

    pCopy->bInstallationDelay_  = bInstallationDelay_.GetData();
    pCopy->installationDelay_   = installationDelay_.GetData();
    pCopy->uninstallationDelay_ = uninstallationDelay_.GetData();

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: UnitInfos::ReadArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Unite" );

    input.ReadAttribute( "nom", strName_ );

    std::string strType;
    input.ReadAttribute( "type", strType );
    eTypeId_ = ADN_Tr::ConvertToAgentTypePion( strType );
    if( eTypeId_ == (E_AgentTypePion)-1 )
        input.ThrowError( MT_FormatString( "Le type d'unité '%s' est invalide", strType.c_str() ) );

    std::string strModel;
    input.ReadField( "ModeleDecisionnel", strModel );
    ADN_Models_Data::ModelInfos* pModel = ADN_Workspace::GetWorkspace().GetModels().GetData().FindUnitModel( strModel );
    if( !pModel )
        throw ADN_DataException( "Donnée invalide", MT_FormatString( "Modele '%s' inconnu - réferencé par le pion '%s'", strModel.c_str(), strName_.GetData().c_str() ), "" );
    ptrModel_ = pModel;

    input.Section( "Nature" );
    std::string strTmp;

    // Niveau
    input.Section( "Niveau" );
    input.ReadAttribute( "type", strTmp );

    E_NatureLevel eNatureLevelType = ENT_Tr::ConvertToNatureLevel( strTmp );
    if( eNatureLevelType == (E_NatureLevel)-1 )
        input.ThrowError( MT_FormatString( "Le niveau hiéarchique '%s' est invalide.", strTmp.c_str() ) );
    eNatureLevel_=eNatureLevelType;

    input.EndSection(); // Niveau

    input.Section( "QualificatifAtlas" );
    input.ReadAttribute( "type", strTmp );

    E_NatureAtlasType eNatureAtlasType = ADN_Tr::ConvertToNatureAtlasType( strTmp );
    if( eNatureAtlasType == (E_NatureAtlasType)-1 )
        input.ThrowError( MT_FormatString( "Le qualificatif ATLAS '%s' est invalide", strTmp.c_str() ) );
    eNatureAtlas_=eNatureAtlasType;
    input.EndSection(); // QualificatifAtlas

    input.Section( "Nature" );
    input.ReadAttribute( "type", strTmp );
    strNature_ = strTmp;
    input.EndSection(); // Nature
        
    input.EndSection(); // Nature

    input.BeginList( "Equipements" );
    while( input.NextListElement() )
    {
        std::auto_ptr<ComposanteInfos> spNew( new ComposanteInfos() );
        spNew->ReadArchive( input );
        vComposantes_.AddItem( spNew.release() );
    }
    input.EndList(); // Equipements

    if( input.Section( "RepartitionDuCommandement", ADN_XmlInput_Helper::eNothing ) )
    {
        input.ReadField( "Officier", nNbOfficer_, ADN_XmlInput_Helper::eNothing );
        input.ReadField( "SousOfficier", nNbNCOfficer_, ADN_XmlInput_Helper::eNothing );
        input.EndSection(); // RepartitionDuCommandement
    }

    bTC1_ = contenancesTC1_.ReadArchive( "ContenanceTC1", input, true );
    // $$$$ SBO 2006-01-03: Hack to reset TC1 normalized consumptions...
    for( ADN_Composantes_Data::T_CategoryInfos_Vector::iterator it = contenancesTC1_.categories_.begin(); it != contenancesTC1_.categories_.end(); ++it )
        (*it)->rNormalizedConsumption_ = 0.;

    bStock_ = input.Section( "Stocks", ADN_XmlInput_Helper::eNothing );
    if( bStock_.GetData() )
    {
        stocks_.ReadArchive( input );
        input.EndSection(); // Stocks
    }

    input.Section( "TempsMiseEnPosture" );
    for( IT_PostureInfos_Vector itPosture = vPostures_.begin(); itPosture != vPostures_.end(); ++itPosture )
    {
        (*itPosture)->ReadArchive( input );
    }
    input.EndSection(); // TempsMiseEnPosture

    if( input.Section( "Installation", ADN_XmlInput_Helper::eNothing ) )
    {
        bInstallationDelay_ = true;
        input.ReadField( "DelaiInstallation", installationDelay_ );
        input.ReadField( "DelaiDesinstallation", uninstallationDelay_ );
        input.EndSection();
    }

    input.ReadField( "DelaiDecontaminationNBC", decontaminationDelay_ );


    if( input.BeginList( "DistancesAvantPoints", ADN_XmlInput_Helper::eNothing ) )
    {
        while( input.NextListElement() )
        {
            std::auto_ptr<PointInfos> spNew( new PointInfos() );
            spNew->ReadArchive( input );
            vPointInfos_.AddItem( spNew.release() );
        }
        input.EndList(); // DistancesAvantPoints
    }

    if( input.Section( "CoupDeSonde", ADN_XmlInput_Helper::eNothing ) )
    {
        bProbe_ = true;
        input.ReadField( "Largeur", rProbeWidth_ );
        input.ReadField( "Longueur", rProbeLength_ );
        input.EndSection();
    }

    if( input.Section( "RapportDeForce", ADN_XmlInput_Helper::eNothing ) )
    {
        bStrengthRatioFeedbackTime_ = true;
        input.ReadField( "TempsDeRemontee", strengthRatioFeedbackTime_ );
        input.EndSection(); // RapportDeForce
    }

    if( input.Section( "PeutVoler", ADN_XmlInput_Helper::eNothing ) )
    {
        bCanFly_ = true;
        input.EndSection();
    }

    if( input.Section( "EstAutonome", ADN_XmlInput_Helper::eNothing ) )
    {
        bIsAutonomous_ = true;
        input.EndSection();
    }

    input.ReadField( "PorteeArmes", rWeaponsReach_, ADN_XmlInput_Helper::eNothing );
    input.ReadField( "PorteeCapteurs", rSensorsReach_, ADN_XmlInput_Helper::eNothing );

    input.EndSection(); // Unite
}


// -----------------------------------------------------------------------------
// Name: UnitInfos::WriteArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Unite" );

    output.WriteAttribute( "nom", strName_.GetData() );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromAgentTypePion( eTypeId_.GetData() ) );
    output.WriteField( "MosID", nMosId_.GetData() );
    output.WriteField( "ModeleDecisionnel", ptrModel_.GetData()->strName_.GetData() );

    // nature
    output.Section("Nature");

    // Niveau
    output.Section( "Niveau" );
    output.WriteAttribute( "type", ENT_Tr::ConvertFromNatureLevel(eNatureLevel_.GetData()) );
    output.EndSection(); // Niveau

    // atlas
    output.Section( "QualificatifAtlas" );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromNatureAtlasType( eNatureAtlas_.GetData() ) );
    output.EndSection(); // QualificatifAtlas

    output.Section( "Nature" );
    output.WriteAttribute( "type", strNature_.GetData() );
    output.EndSection(); // CapaciteMission

    output.EndSection(); //Nature

    output.BeginList( "Equipements", vComposantes_.size() );
    for( IT_ComposanteInfos_Vector itComposante = vComposantes_.begin(); itComposante != vComposantes_.end(); ++itComposante )
    {
        (*itComposante)->WriteArchive( output, bIsAutonomous_.GetData() );
    }
    output.EndList(); // Equipements

    output.Section( "RepartitionDuCommandement" );
    output.WriteField( "Officier", nNbOfficer_.GetData() );
    output.WriteField( "SousOfficier", nNbNCOfficer_.GetData() );
    output.EndSection(); // RepartitionDuCommandement

    if( bTC1_.GetData() )
        contenancesTC1_.WriteArchive( "ContenanceTC1", output );
    if( bStock_.GetData() )
        stocks_.WriteArchive( "Stocks", output );

    output.Section( "TempsMiseEnPosture" );
    for( IT_PostureInfos_Vector itPosture = vPostures_.begin(); itPosture != vPostures_.end(); ++itPosture )
    {
        (*itPosture)->WriteArchive( output );
    }
    output.EndSection(); // TempsMiseEnPosture

    if( bInstallationDelay_.GetData() )
    {
        output.Section( "Installation" );
        output.WriteField( "DelaiInstallation", installationDelay_.GetData() );
        output.WriteField( "DelaiDesinstallation", uninstallationDelay_.GetData() );
        output.EndSection(); // Installation
    }

    output.WriteField( "DelaiDecontaminationNBC", decontaminationDelay_.GetData() );

    if( ! vPointInfos_.empty() )
    {
        output.BeginList( "DistancesAvantPoints", vPointInfos_.size() );
        for( IT_PointInfos_Vector itPoint = vPointInfos_.begin(); itPoint != vPointInfos_.end(); ++itPoint )
            (*itPoint)->WriteArchive( output );
        output.EndList(); // DistancesAvantPoints
    }

    if( bProbe_.GetData() )
    {
        output.Section( "CoupDeSonde" );
        output.WriteField( "Largeur", rProbeWidth_.GetData() );
        output.WriteField( "Longueur", rProbeLength_.GetData() );
        output.EndSection(); // CoupDeSonde
    }

    if( bStrengthRatioFeedbackTime_.GetData() )
    {
        output.Section( "RapportDeForce" );
        output.WriteField( "TempsDeRemontee", strengthRatioFeedbackTime_.GetData() );
        output.EndSection(); // RapportDeForce
    }

    if( bCanFly_.GetData() )
    {
        output.Section( "PeutVoler" );
        output.EndSection();
    }

    if( bIsAutonomous_.GetData() )
    {
        output.Section( "EstAutonome" );
        output.EndSection();
    }

    output.WriteField( "PorteeArmes", rWeaponsReach_.GetData() );
    output.WriteField( "PorteeCapteurs", rSensorsReach_.GetData() );

    output.EndSection(); // Unite
}


// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ADN_Units_Data constructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Units_Data::ADN_Units_Data()
: ADN_Data_ABC()
, nNextId_    ( 1 )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Units_Data destructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Units_Data::~ADN_Units_Data()
{
    Reset();
}


//-----------------------------------------------------------------------------
// Name: ADN_Units_Data::Reset
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
void ADN_Units_Data::Reset()
{
    nNextId_ = 1;
    vUnits_.Reset();
}


//-----------------------------------------------------------------------------
// Name: ADN_Munitions_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Units_Data::FilesNeeded(T_StringList& files) const
{
    files.push_back(ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUnits_.GetData());
}


// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::GetNextId
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
int ADN_Units_Data::GetNextId()
{
    return nNextId_++;
}


// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Units_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "Pions" );

    while( input.NextListElement() )
    {
        std::auto_ptr<UnitInfos> spNew( new UnitInfos() );
        spNew->ReadArchive( input );
        vUnits_.AddItem( spNew.release() );
    }
    vUnits_.AddItem( 0 );  // Signals the end of the vector, allows certain parts of the gui to update.

    input.EndList(); // Pions
}


// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Units_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.BeginList( "Pions", vUnits_.size() );

    for( IT_UnitInfos_Vector it = vUnits_.begin(); it != vUnits_.end(); ++it )
    {
        (*it)->WriteArchive( output );
    }

    output.EndList(); // Pions
}


// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::GetUnitsThatUse
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::GetUnitsThatUse( ADN_Composantes_Data::ComposanteInfos& composante )
{
    std::string strResult;
    for( IT_UnitInfos_Vector it = vUnits_.begin(); it != vUnits_.end(); ++it )
    {
        UnitInfos* pUnit = *it;
        for( IT_ComposanteInfos_Vector it2 = pUnit->vComposantes_.begin(); it2 != pUnit->vComposantes_.end(); ++it2 )
        {
            if( (*it2)->ptrComposante_.GetData() == &composante )
            {
                if( strResult != "" )
                    strResult += "<br>";
                strResult += "<nobr>" + pUnit->strName_.GetData() + "</nobr>";
                break;
            }
        }
    }
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::GetUnitsThatUse
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
std::string ADN_Units_Data::GetUnitsThatUse( ADN_Models_Data::ModelInfos& model )
{
    std::string strResult;
    for( IT_UnitInfos_Vector it = vUnits_.begin(); it != vUnits_.end(); ++it )
    {
        UnitInfos* pUnit = *it;
        if( (*it)->ptrModel_.GetData() == &model )
        {
            if( strResult != "" )
                strResult += "<br>";
            strResult += "<nobr>" + pUnit->strName_.GetData() + "</nobr>";
        }
    }
    return strResult;
}
