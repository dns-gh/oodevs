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
#include "ADN_Units_GUI.h"
#include "ADN_SymbolWidget.h"
#include "clients_kernel/SymbolFactory.h"
#include "ADN_DataException.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"

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
void ADN_Units_Data::ComposanteInfos::ReadArchive( xml::xistream& input )
{
    std::string strName;
    input >> xml::attribute( "type", strName )
          >> xml::attribute( "count", nNb_ )
          >> xml::optional >> xml::attribute( "major", bMajor_ )
          >> xml::optional >> xml::attribute( "crew", nNbrHumanInCrew_ )
          >> xml::optional >> xml::attribute( "convoyer", bConveyor_ )
          >> xml::optional >> xml::attribute( "loadable", bLoadable_ );
    ADN_Composantes_Data::ComposanteInfos* pComposante = ADN_Workspace::GetWorkspace().GetComposantes().GetData().FindComposante( strName );
    if( pComposante == 0 )
        throw ADN_DataException( tools::translate( "Units_Data",  "Invalid data" ).ascii(), tools::translate( "Units_Data", "Unit types - Invalid equipment '%1'" ).arg( strName.c_str() ).ascii() );
    ptrComposante_ = pComposante;
}

// -----------------------------------------------------------------------------
// Name: ComposanteInfos::WriteArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::ComposanteInfos::WriteArchive( xml::xostream& output, bool bIsAutonomous ) const
{
    if( !bIsAutonomous && nNbrHumanInCrew_.GetData() == 0 )
        throw ADN_DataException( tools::translate( "Units_Data", "Invalid data" ).ascii(), tools::translate( "Units_Data", "Unit has no crew in equipment '%1'" ).arg( ptrComposante_.GetData()->strName_.GetData().c_str() ).ascii() );

    output << xml::start( "equipment" )
            << xml::attribute( "type", ptrComposante_.GetData()->strName_ )
            << xml::attribute( "count", nNb_ )
            << xml::attribute( "crew", nNbrHumanInCrew_ );

    if( bMajor_.GetData() )
        output << xml::attribute( "major", bMajor_ );
    if( bConveyor_.GetData() )
        output << xml::attribute( "convoyer", bConveyor_ );
    if( bLoadable_.GetData() )
        output << xml::attribute( "loadable", bLoadable_ );
    output << xml::end;
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
void ADN_Units_Data::StockLogThresholdInfos::ReadArchive( xml::xistream& input )
{
    std::string strCategory;
    input >> xml::attribute( "category", strCategory )
          >> xml::attribute( "threshold", rLogThreshold_ );

    E_StockCategory eCategory = ADN_Tr::ConvertToStockCategory( strCategory );
    if( eCategory == (E_StockCategory)-1 )
        throw ADN_DataException( tools::translate( "Units_Data", "Invalid data" ).ascii(), tools::translate( "Units_Data", "Unit types - Invalid resource '%1'" ).arg( strCategory.c_str() ).ascii() );
    eCategory_ = eCategory;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockLogThresholdInfos::WriteArchive
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockLogThresholdInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "stock" )
            << xml::attribute( "category", ADN_Tr::ConvertFromStockCategory( eCategory_.GetData() ) )
            << xml::attribute( "threshold", rLogThreshold_ )
           << xml::end;
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
// Name: ADN_Units_Data::StockInfos::ReadStock
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockInfos::ReadStock( xml::xistream& input, ADN_Type_Bool& stockThresholds )
{
    std::auto_ptr< StockLogThresholdInfos > spNew( new StockLogThresholdInfos() );
    spNew->ReadArchive( input );
    vLogThresholds_.AddItem( spNew.release() );
    stockThresholds = true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockInfos::ReadArchive
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockInfos::ReadArchive( xml::xistream& input, ADN_Type_Bool& stockThresholds )
{
    input >> xml::optional >> xml::start( "stocks" )
            >> xml::list( "stock", *this, &ADN_Units_Data::StockInfos::ReadStock, stockThresholds )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockInfos::WriteArchive
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockInfos::WriteArchive( const std::string& strName, xml::xostream& output ) const
{
    output << xml::start( strName );
    for( CIT_StockLogThresholdInfos_Vector it = vLogThresholds_.begin(); it != vLogThresholds_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
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
void ADN_Units_Data::PointInfos::ReadArchive( xml::xistream& input )
{
    std::string strType;
    input >> xml::attribute( "type", strType )
          >> xml::attribute( "value", nDistance_ );
    nTypeTerrain_ = ADN_Tr::ConvertToKeyPoint( strType );
}

// -----------------------------------------------------------------------------
// Name: PointInfos::WriteArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::PointInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-before-point" )
            << xml::attribute( "type", ADN_Tr::ConvertFromKeyPoint( nTypeTerrain_ ) )
            << xml::attribute( "value", nDistance_ )
          << xml::end;
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
void ADN_Units_Data::PostureInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "setup-time", timeToActivate_ );
}

// -----------------------------------------------------------------------------
// Name: PostureInfos::WriteArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::PostureInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "posture" )
            << xml::attribute( "name", ADN_Tools::ComputePostureScriptName( nPosture_ ) )
            << xml::attribute( "setup-time", timeToActivate_ )
           << xml::end;
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
    , eTypeId_                          ( static_cast< E_AgentTypePion >( 0 ) )
    , strName_                          ()
    , nMosId_                           ( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetNextId() )
    , ptrModel_                         ( ADN_Workspace::GetWorkspace().GetModels().GetData().GetUnitModelsInfos(), 0 )
    , eNatureLevel_                     ( static_cast< E_NatureLevel >( 0 ) )
    , nNbOfficer_                       ( 0 )
    , nNbNCOfficer_                     ( 0 )
    , decontaminationDelay_             ( "1s" )
    , vComposantes_                     ()
    , vPostures_                        ( false )
    , vPointInfos_                      ( false )
    , bProbe_                           ( false )
    , bStock_                           ( false )
    , stocks_                           ()
    , bStrengthRatioFeedbackTime_       ( false )
    , strengthRatioFeedbackTime_        ( "0s" )
    , rProbeWidth_                      ( 0 )
    , rProbeLength_                     ( 0 )
    , bCanFly_                          ( false )
    , eCrossingHeight_                  ( static_cast< E_CrossingHeight >( 0 ) )
    , bIsAutonomous_                    ( false )
    , bInstallationDelay_               ( false )
    , installationDelay_                ( "0s" )
    , uninstallationDelay_              ( "0s" )
    , nReconEfficiency_                 ( 50 )
    , nCombatSupportEfficiency_         ( 50 )
    , nCombatEfficiency_                ( 50 )
    , nMobilitySupportEfficiency_       ( 50 )
    , nCounterMobilitySupportEfficiency_( 50 )
    , nProtectionSupportEfficiency_     ( 50 )
    , nEngineeringReconEfficiency_      ( 50 )
    , nUrbanAreaEfficiency_             ( 50 )
    , bIsCivilian_                      ( false )
    , repartition_                      ( tools::translate( "Units_Data", "Units" ) )
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

    vComposantes_.SetItemTypeName( "an equipment" );
    vComposantes_.SetParentNode( *this );

    ADN_Type_Enum<E_NatureLevel,eNbrNatureLevel>::SetConverter( &ENT_Tr::ConvertFromNatureLevel );
    eNatureLevel_.SetDataName( "la taille" );
    eNatureLevel_.SetParentNode( *this );

    ADN_Type_Enum<E_NatureAtlasType,eNbrNatureAtlas>::SetConverter( &ADN_Tr::ConvertFromNatureAtlasType );
    eNatureAtlas_.SetDataName( "la nature ATLAS" );
    eNatureAtlas_.SetParentNode( *this );

    strNature_.SetDataName( "la nature, les arbres et les oiseaux" );
    strNature_.SetParentNode( *this );

    ADN_Type_Enum< E_CrossingHeight, eNbrCrossingHeight >::SetConverter( &ADN_Tr::ConvertFromCrossingHeight );
    eCrossingHeight_.SetDataName( "la hauteur de franchisssement" );
    eCrossingHeight_.SetParentNode( *this );

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
    pCopy->eCrossingHeight_ = eCrossingHeight_.GetData();
    pCopy->bIsAutonomous_ = bIsAutonomous_.GetData();

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

    pCopy->nReconEfficiency_ = nReconEfficiency_.GetData();
    pCopy->nCombatSupportEfficiency_ = nCombatSupportEfficiency_.GetData();
    pCopy->nCombatEfficiency_ = nCombatEfficiency_.GetData();
    pCopy->nMobilitySupportEfficiency_ = nMobilitySupportEfficiency_.GetData();
    pCopy->nCounterMobilitySupportEfficiency_ = nCounterMobilitySupportEfficiency_.GetData();
    pCopy->nProtectionSupportEfficiency_ = nProtectionSupportEfficiency_.GetData();
    pCopy->nEngineeringReconEfficiency_ = nEngineeringReconEfficiency_.GetData();
    pCopy->nUrbanAreaEfficiency_ = nUrbanAreaEfficiency_.GetData();

    pCopy->bIsCivilian_        = bIsCivilian_.GetData();
    pCopy->repartition_        = repartition_;

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos::ReadEquipment
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::ReadEquipment( xml::xistream& input )
{
    std::auto_ptr<ComposanteInfos> spNew( new ComposanteInfos() );
    spNew->ReadArchive( input );
    vComposantes_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos::ReadCrew
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::ReadCrew( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "type", type );
    if( type == "Officier" )
        input >> xml::attribute( "count", nNbOfficer_ );
    else if( type == "SousOfficier" )
        input >> xml::attribute( "count", nNbNCOfficer_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos::ReadPosture
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::ReadPosture( xml::xistream& input )
{
    std::string posture = input.attribute< std::string >( "name" );
    IT_PostureInfos_Vector itPosture = std::find_if( vPostures_.begin(), vPostures_.end(), PostureInfos::Cmp( posture ) );
    if( itPosture != vPostures_.end() )
        (*itPosture)->ReadArchive( input );
    else
        throw ADN_DataException( tools::translate( "Units_Data", "Invalid data" ).ascii(), tools::translate( "Units_Data", "Unit types - Invalid stance '%1'" ).arg( posture.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos::ReadPointDistance
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::ReadPointDistance( xml::xistream& input )
{
    std::auto_ptr<PointInfos> spNew( new PointInfos() );
    spNew->ReadArchive( input );
    vPointInfos_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::ReadArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::ReadArchive( xml::xistream& input )
{
    std::string strType, strModel;
    input >> xml::attribute( "name", strName_ )
        >> xml::attribute( "type", strType )
        >> xml::attribute( "decisional-model", strModel );

    eTypeId_ = ADN_Tr::ConvertToAgentTypePion( strType );
    if( eTypeId_ == (E_AgentTypePion)-1 )
        throw ADN_DataException( tools::translate( "Units_Data", "Invalid data" ).ascii(), tools::translate( "Units_Data", "Unit types - Invalid unit type '%1'" ).arg( strType.c_str() ).ascii() );
    ADN_Models_Data::ModelInfos* pModel = ADN_Workspace::GetWorkspace().GetModels().GetData().FindUnitModel( strModel );
    if( !pModel )
        throw ADN_DataException( tools::translate( "Units_Data", "Invalid data" ).ascii(), tools::translate( "Units_Data", "Unit types - Invalid doctrine model '%1'" ).arg( strModel.c_str() ).ascii() );
    ptrModel_ = pModel;

    std::string level, atlas;
    input >> xml::start( "nature" )
            >> xml::attribute( "level", level )
            >> xml::attribute( "atlas-nature", atlas )
            >> xml::attribute( "nature-app6", strNature_ )
          >> xml::end;
    CleanupNature();
    E_NatureLevel eNatureLevelType = ENT_Tr::ConvertToNatureLevel( level );
    if( eNatureLevelType == (E_NatureLevel)-1 )
        throw ADN_DataException( tools::translate( "Units_Data", "Invalid data" ).ascii(), tools::translate( "Units_Data", "Unit types - Invalid hierarchical level '%1'" ).arg( level.c_str() ).ascii() );
    eNatureLevel_=eNatureLevelType;

    E_NatureAtlasType eNatureAtlasType = ADN_Tr::ConvertToNatureAtlasType( atlas );
    if( eNatureAtlasType == (E_NatureAtlasType)-1 )
        throw ADN_DataException( tools::translate( "Units_Data", "Invalid data" ).ascii(), tools::translate( "Units_Data", "Unit types - Invalid 'Atlas' attribute '%1'" ).arg( atlas.c_str() ).ascii() );
    eNatureAtlas_=eNatureAtlasType;

    input >> xml::start( "equipments" )
            >> xml::list( "equipment", *this, &ADN_Units_Data::UnitInfos::ReadEquipment )
          >> xml::end
          >> xml::start( "crew-ranks" )
            >> xml::list( "crew-rank", *this, &ADN_Units_Data::UnitInfos::ReadCrew )
          >> xml::end
          >> xml::optional >> xml::start( "logistics" )
            >> xml::list( "category", contenancesTC1_, &ADN_Composantes_Data::ResourceInfos::ReadCategory )
          >> xml::end;
    bTC1_ = ! contenancesTC1_.categories_.empty();
    for( ADN_Composantes_Data::T_CategoryInfos_Vector::iterator it = contenancesTC1_.categories_.begin(); it != contenancesTC1_.categories_.end(); ++it )
        (*it)->rNormalizedConsumption_ = 0.;

    stocks_.ReadArchive( input, bStock_ );

    input >> xml::start( "postures" )
            >> xml::list( "posture", *this, &ADN_Units_Data::UnitInfos::ReadPosture )
          >> xml::end;

    input >> xml::optional
            >> xml::start( "setup" )
                >> xml::attribute( "installation-time", installationDelay_ )
                >> xml::attribute( "uninstallation-time", uninstallationDelay_ )
            >> xml::end;
    bInstallationDelay_ = installationDelay_ != "0s" || uninstallationDelay_ != "0s";
    input >> xml::start( "nbc" )
            >> xml::attribute( "decontamination-delay", decontaminationDelay_ )
          >> xml::end
          >> xml::optional >> xml::start( "distance-before-points" )
            >> xml::list( "distance-before-point", *this, &ADN_Units_Data::UnitInfos::ReadPointDistance )
          >> xml::end
          >> xml::optional >> xml::start( "drill-blow" )
            >> xml::attribute( "width", rProbeWidth_ )
            >> xml::attribute( "length", rProbeLength_ )
          >> xml::end
          >> xml::optional >> xml::start( "force-ratio" )
            >> xml::attribute( "feedback-time", strengthRatioFeedbackTime_ )
          >> xml::end;
    bProbe_ = rProbeWidth_ != 0. || rProbeLength_ != 0.;
    bStrengthRatioFeedbackTime_ = strengthRatioFeedbackTime_ != "0s";

    input >> xml::optional
            >> xml::start( "efficiencies" )
                >> xml::attribute( "recon", nReconEfficiency_ )
                >> xml::attribute( "combat-support", nCombatSupportEfficiency_ )
                >> xml::attribute( "combat", nCombatEfficiency_ )
                >> xml::attribute( "mobility-support", nMobilitySupportEfficiency_ )
                >> xml::attribute( "counter-mobility-support", nCounterMobilitySupportEfficiency_ )
                >> xml::attribute( "protection-support", nProtectionSupportEfficiency_ )
                >> xml::attribute( "engineering-support", nEngineeringReconEfficiency_ )
                >> xml::attribute( "urban-area", nUrbanAreaEfficiency_ )
            >> xml::end;

    if( input.has_child("repartition") )
    {
        input >> xml::start( "repartition" );
        repartition_.ReadArchive( input );
        input >> xml::end;
        bIsCivilian_ = repartition_.male_ != 0 || repartition_.female_ != 0 || repartition_.children_ != 0;
    }

    std::string crossingHeight( ADN_Tr::ConvertFromCrossingHeight( eCrossingHeight_Never ) ) ;
    input >> xml::optional
            >> xml::start( "crossing-height" )
                >> xml::attribute( "height", crossingHeight )
            >> xml::end;

    E_CrossingHeight eCrossingHeight = ADN_Tr::ConvertToCrossingHeight( crossingHeight );
    if( eCrossingHeight == ( E_CrossingHeight )-1 )
        throw ADN_DataException( tools::translate( "Units_Data", "Invalid data" ).ascii(), tools::translate( "Units_Data", "Unit types - Invalid crossing height '%1'" ).arg( crossingHeight.c_str() ).ascii() );
    eCrossingHeight_ = eCrossingHeight;

    input >> xml::optional >> xml::attribute( "can-fly", bCanFly_ );
    input >> xml::optional >> xml::attribute( "is-autonomous", bIsAutonomous_ );
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::WriteArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "unit" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "type", ADN_Tr::ConvertFromAgentTypePion( eTypeId_.GetData() ) )
            << xml::attribute( "decisional-model", ptrModel_.GetData()->strName_ )
            << xml::attribute( "id", nMosId_ );

    output << xml::start( "nature" )
            << xml::attribute( "level", ENT_Tr::ConvertFromNatureLevel( eNatureLevel_.GetData() ) )
            << xml::attribute( "atlas-nature", ADN_Tr::ConvertFromNatureAtlasType( eNatureAtlas_.GetData() ) )
            << xml::attribute( "nature-app6", strNature_ )
          << xml::end;

    output << xml::start( "equipments" );
    for( CIT_ComposanteInfos_Vector itComposante = vComposantes_.begin(); itComposante != vComposantes_.end(); ++itComposante )
        (*itComposante)->WriteArchive( output, bIsAutonomous_.GetData() );
    output << xml::end;

    output << xml::start( "crew-ranks" )
            << xml::start( "crew-rank" )
             << xml::attribute( "type", "Officier" )
             << xml::attribute( "count", nNbOfficer_ )
            << xml::end
            << xml::start( "crew-rank" )
             << xml::attribute( "type", "SousOfficier" )
             << xml::attribute( "count", nNbNCOfficer_ )
            << xml::end
           << xml::end;

    if( bTC1_.GetData() )
    {
        output << xml::start( "logistics" );
        contenancesTC1_.WriteArchive( output );
        output << xml::end;
    }

    if( bStock_.GetData() )
        stocks_.WriteArchive( "stocks", output );

    output << xml::start( "postures" );
    for( CIT_PostureInfos_Vector itPosture = vPostures_.begin(); itPosture != vPostures_.end(); ++itPosture )
        (*itPosture)->WriteArchive( output );
    output << xml::end;

    if( bInstallationDelay_.GetData() )
        output << xml::start( "setup" )
                << xml::attribute( "installation-time", installationDelay_ )
                << xml::attribute( "uninstallation-time", uninstallationDelay_ )
               << xml::end;

    output << xml::start( "nbc" )
            << xml::attribute(  "decontamination-delay", decontaminationDelay_ )
           << xml::end;

    if( ! vPointInfos_.empty() )
    {
        output << xml::start( "distance-before-points" );
        for( CIT_PointInfos_Vector itPoint = vPointInfos_.begin(); itPoint != vPointInfos_.end(); ++itPoint )
            (*itPoint)->WriteArchive( output );
        output << xml::end;
    }

    if( bProbe_.GetData() )
        output << xml::start( "drill-blow" )
                << xml::attribute( "width", rProbeWidth_ )
                << xml::attribute( "length", rProbeLength_ )
               << xml::end;

    if( bStrengthRatioFeedbackTime_.GetData() )
        output << xml::start( "force-ratio" )
                << xml::attribute( "feedback-time", strengthRatioFeedbackTime_ )
               << xml::end;

    output << xml::start( "efficiencies" )
             << xml::attribute( "recon", nReconEfficiency_ )
             << xml::attribute( "combat-support", nCombatSupportEfficiency_ )
             << xml::attribute( "combat", nCombatEfficiency_ )
             << xml::attribute( "mobility-support", nMobilitySupportEfficiency_ )
             << xml::attribute( "counter-mobility-support", nCounterMobilitySupportEfficiency_ )
             << xml::attribute( "protection-support", nProtectionSupportEfficiency_ )
             << xml::attribute( "engineering-support", nEngineeringReconEfficiency_ )
             << xml::attribute( "urban-area", nUrbanAreaEfficiency_ )
           << xml::end;

    if( bIsCivilian_.GetData() )
    {
        repartition_.CheckNoError( "ADN_Units_Data" );
        output << xml::start( "repartition" );
        repartition_.WriteArchive( output );
        output  << xml::end;
    }

    output << xml::start( "crossing-height" )
            << xml::attribute( "height", ADN_Tr::ConvertFromCrossingHeight( eCrossingHeight_.GetData() ) )
           << xml::end;

    if( bCanFly_.GetData() )
        output << xml::attribute( "can-fly", bCanFly_ );

    if( bIsAutonomous_.GetData() )
        output << xml::attribute( "is-autonomous", bIsAutonomous_ );

    output << xml::end;
}


// -----------------------------------------------------------------------------
// Name: UnitInfos::CleanupNature
// Created: RPD 2011-04-11
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::CleanupNature()
{
    std::string nature = strNature_.GetData();
    if ( nature.size() > 10 )
    {
        if ( nature.compare( nature.size()-10, 10 ,"/undefined") == 0 )
        {
            nature.resize( nature.size()-10 );
            strNature_ = nature;
        }
    }
    if ( !ADN_Workspace::GetWorkspace().GetUnits().GetGui().GetSymbolWidget()->IsAvailable( strNature_.GetData() ) )
        strNature_ = "undefined";
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_Data constructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Units_Data::ADN_Units_Data()
    : ADN_Data_ABC()
    , nNextId_    ( 1 )
{
    // NOTHING
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
// Name: ADN_Units_Data::ReadUnit
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::ReadUnit( xml::xistream& input )
{
    std::auto_ptr<UnitInfos> spNew( new UnitInfos() );
    spNew->ReadArchive( input );
    vUnits_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::ReadArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Units_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "units" )
            >> xml::list( "unit", *this, &ADN_Units_Data::ReadUnit )
          >> xml::end;
    vUnits_.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Units_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "units" );
    ADN_Tools::AddSchema( output, "Units" );
    for( CIT_UnitInfos_Vector it = vUnits_.begin(); it != vUnits_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
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
