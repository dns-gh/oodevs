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
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Categories_Data.h"
#include "ADN_Units_GUI.h"
#include "ADN_UnitSymbolWidget.h"
#include "clients_kernel/SymbolFactory.h"
#include "ADN_ConsistencyChecker.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"
#include "clients_kernel/XmlTranslations.h"

tools::IdManager ADN_Units_Data::idManager_;

// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ComposanteInfos::ComposanteInfos
// Created: JDY 03-07-25
//-----------------------------------------------------------------------------
ADN_Units_Data::ComposanteInfos::ComposanteInfos( const ADN_Equipments_Data::T_EquipmentInfos_Vector& equipments, ADN_Equipments_Data::EquipmentInfos* equipment /* = 0 */ )
    : ADN_CrossedRef( equipments, equipment, true )
    , bLoadable_( false )
    , bMajor_( false )
    , bConveyor_( false )
    , nNbrHumanInCrew_( 0 )
    , nNb_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ComposanteInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Units_Data::ComposanteInfos* ADN_Units_Data::ComposanteInfos::CreateCopy()
{
    ComposanteInfos* pCopy = new ComposanteInfos( GetVector(), GetCrossedElement() );
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
    input >> xml::attribute( "type", *this )
          >> xml::attribute( "count", nNb_ )
          >> xml::optional >> xml::attribute( "major", bMajor_ )
          >> xml::optional >> xml::attribute( "crew", nNbrHumanInCrew_ )
          >> xml::optional >> xml::attribute( "convoyer", bConveyor_ )
          >> xml::optional >> xml::attribute( "loadable", bLoadable_ );
}

// -----------------------------------------------------------------------------
// Name: ComposanteInfos::WriteArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::ComposanteInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "equipment" )
             << xml::attribute( "type", *this )
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
ADN_Units_Data::StockLogThresholdInfos::StockLogThresholdInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_LogisticSupplyClasses_Data >( eLogisticSupplyClasses ).GetLogisticSupplyClasses(), 0, false )
    , rLogThreshold_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockLogThresholdInfos::ReadArchive
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockLogThresholdInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "logistic-supply-class", *this )
          >> xml::attribute( "threshold", rLogThreshold_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockLogThresholdInfos::WriteArchive
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockLogThresholdInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "stock" )
             << xml::attribute( "logistic-supply-class", *this )
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
    pCopy->SetCrossedElement( GetCrossedElement() );
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
    : vLogThresholds_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockInfos::ReadStock
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockInfos::ReadStock( xml::xistream& input, ADN_Type_Bool& stockThresholds )
{
    std::unique_ptr< StockLogThresholdInfos > spNew( new StockLogThresholdInfos() );
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
    for( auto it = vLogThresholds_.begin(); it != vLogThresholds_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::StockInfos::CopyFrom
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void ADN_Units_Data::StockInfos::CopyFrom( StockInfos& src )
{
    for( auto it = src.vLogThresholds_.begin(); it != src.vLogThresholds_.end(); ++it )
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
    // NOTHING
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
    :   nPosture_        ( nPosture )
    ,   timeToActivate_  ( "0s" )
    ,   timeToDeactivate_( "0s" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PostureInfos::ReadArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::PostureInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "setup-time", timeToActivate_ )
          >> xml::attribute( "tear-down-time", timeToDeactivate_ );
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
            << xml::attribute( "tear-down-time", timeToDeactivate_ )
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
    : eTypeId_                          ( static_cast< E_AgentTypePion >( 0 ) )
    , eNbcSuit_                         ( static_cast< E_AgentNbcSuit >( 0 ) )
    , nId_                              ( ADN_Units_Data::idManager_.GetNextId() )
    , ptrModel_                         ( ADN_Workspace::GetWorkspace().GetModels().GetData().GetModels( eEntityType_Pawn ), 0 )
    , eNatureLevel_                     ( static_cast< E_NatureLevel >( 0 ) )
    , nNbOfficer_                       ( 0 )
    , nNbNCOfficer_                     ( 0 )
    , decontaminationDelay_             ( "1s" )
    , vComposantes_                     ()
    , vPostures_                        ( false )
    , vPointInfos_                      ( false )
    , bProbe_                           ( false )
    , bRanges_                          ( false )
    , nSensorRange_                     ( 0 )
    , nEquipmentRange_                  ( 0 )
    , bTC1_                             ( false )
    , bStock_                           ( false )
    , stocks_                           ()
    , bStrengthRatioFeedbackTime_       ( false )
    , strengthRatioFeedbackTime_        ( "0s" )
    , rProbeWidth_                      ( 0 )
    , rProbeLength_                     ( 0 )
    , bCanFly_                          ( false )
    , standardFlyingHeight_             ( 0 )
    , tacticalFlyingHeight_             ( 0 )
    , eCrossingHeight_                  ( static_cast< E_CrossingHeight >( 0 ) )
    , bIsAutonomous_                    ( false )
    , footprint_                        ( 0. )
    , bInstallationDelay_               ( false )
    , installationDelay_                ( "0s" )
    , uninstallationDelay_              ( "0s" )
    , nFootprintRadius_                 ( 0 )
    , rSpeedModifier_                   ( 1 )
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
    Initialize();
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos
// Created: JDY 03-07-25
//-----------------------------------------------------------------------------
ADN_Units_Data::UnitInfos::UnitInfos( unsigned int id )
    : eTypeId_                          ( static_cast< E_AgentTypePion >( 0 ) )
    , eNbcSuit_                         ( static_cast< E_AgentNbcSuit >( 0 ) )
    , nId_                              ( id )
    , ptrModel_                         ( ADN_Workspace::GetWorkspace().GetModels().GetData().GetModels( eEntityType_Pawn ), 0 )
    , eNatureLevel_                     ( static_cast< E_NatureLevel >( 0 ) )
    , nNbOfficer_                       ( 0 )
    , nNbNCOfficer_                     ( 0 )
    , decontaminationDelay_             ( "1s" )
    , vComposantes_                     ()
    , vPostures_                        ( false )
    , vPointInfos_                      ( false )
    , bProbe_                           ( false )
    , bRanges_                          ( false )
    , nSensorRange_                     ( 0 )
    , nEquipmentRange_                  ( 0 )
    , bStock_                           ( false )
    , stocks_                           ()
    , bStrengthRatioFeedbackTime_       ( false )
    , strengthRatioFeedbackTime_        ( "0s" )
    , rProbeWidth_                      ( 0 )
    , rProbeLength_                     ( 0 )
    , bCanFly_                          ( false )
    , standardFlyingHeight_             ( 0 )
    , tacticalFlyingHeight_             ( 0 )
    , eCrossingHeight_                  ( static_cast< E_CrossingHeight >( 0 ) )
    , bIsAutonomous_                    ( false )
    , footprint_                        ( 0. )
    , bInstallationDelay_               ( false )
    , installationDelay_                ( "0s" )
    , uninstallationDelay_              ( "0s" )
    , nFootprintRadius_                 ( 0 )
    , rSpeedModifier_                   ( 1 )
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
    Initialize();
    ADN_Units_Data::idManager_.Lock( id );
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos
// Created: JDY 03-07-28
//-----------------------------------------------------------------------------
ADN_Units_Data::UnitInfos::~UnitInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos::Initialize
// Created: ABR 2012-09-18
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::Initialize()
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eUnits, "units" ) );
    BindExistenceTo(&ptrModel_);

    // postures initialization
    for( int i = eUnitPosture_PosturePosteReflexe; i < eNbrUnitPosture; ++i )
        vPostures_.AddItem( new PostureInfos( static_cast< E_UnitPosture >( i ) ) );

    ptrNatureSymbol_.SetVector( ADN_Workspace::GetWorkspace().GetSymbols().GetData().GetSymbolsUnits() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::CreateCopy
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Units_Data::UnitInfos* ADN_Units_Data::UnitInfos::CreateCopy()
{
    UnitInfos* pCopy = new UnitInfos();

    pCopy->eTypeId_ = eTypeId_.GetData();
    pCopy->eNbcSuit_ = eNbcSuit_.GetData();
    pCopy->ptrModel_ = ptrModel_.GetData();
    pCopy->eNatureLevel_ = eNatureLevel_.GetData();
    pCopy->eNatureAtlas_ = eNatureAtlas_.GetData();
    pCopy->strNature_ = strNature_.GetData();
    pCopy->nNbOfficer_ = nNbOfficer_.GetData();
    pCopy->nNbNCOfficer_ = nNbNCOfficer_.GetData();
    pCopy->decontaminationDelay_ = decontaminationDelay_.GetData();
    pCopy->bCanFly_ = bCanFly_.GetData();
    pCopy->standardFlyingHeight_ = standardFlyingHeight_.GetData();
    pCopy->tacticalFlyingHeight_ = tacticalFlyingHeight_.GetData();
    pCopy->eCrossingHeight_ = eCrossingHeight_.GetData();
    pCopy->bIsAutonomous_ = bIsAutonomous_.GetData();
    pCopy->footprint_ = footprint_.GetData();

    for( auto itComposante = vComposantes_.begin(); itComposante != vComposantes_.end(); ++itComposante )
        pCopy->vComposantes_.AddItem( (*itComposante)->CreateCopy() );

    for( uint i = 0; i < vPostures_.size(); ++i )
    {
        pCopy->vPostures_[ i ]->timeToActivate_ = vPostures_[ i ]->timeToActivate_.GetData();
        pCopy->vPostures_[ i ]->timeToDeactivate_ = vPostures_[ i ]->timeToDeactivate_.GetData();
    }
    for( auto itPoint = vPointInfos_.begin(); itPoint != vPointInfos_.end(); ++itPoint )
        pCopy->vPointInfos_.AddItem( (*itPoint)->CreateCopy() );

    pCopy->bTC1_ = bTC1_.GetData();
    pCopy->contenancesTC1_.CopyFrom( contenancesTC1_ );
    pCopy->bStock_ = bStock_.GetData();
    pCopy->stocks_.CopyFrom( stocks_ );

    pCopy->bProbe_ = bProbe_.GetData();
    pCopy->rProbeLength_ = rProbeLength_.GetData();
    pCopy->rProbeWidth_ = rProbeWidth_.GetData();

    pCopy->bRanges_ = bRanges_.GetData();
    pCopy->nSensorRange_ = nSensorRange_.GetData();
    pCopy->nEquipmentRange_ = nEquipmentRange_.GetData();

    pCopy->bStrengthRatioFeedbackTime_ = bStrengthRatioFeedbackTime_.GetData();
    pCopy->strengthRatioFeedbackTime_  = strengthRatioFeedbackTime_.GetData();

    pCopy->bInstallationDelay_  = bInstallationDelay_.GetData();
    pCopy->installationDelay_   = installationDelay_.GetData();
    pCopy->uninstallationDelay_ = uninstallationDelay_.GetData();

    pCopy->nFootprintRadius_ = nFootprintRadius_.GetData();
    pCopy->rSpeedModifier_ = rSpeedModifier_.GetData();

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
    std::unique_ptr<ComposanteInfos> spNew( new ComposanteInfos( ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipments() ) );
    spNew->ReadArchive( input );
    vComposantes_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos::ReadCrew
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::ReadCrew( xml::xistream& input )
{
    std::string type = input.attribute< std::string >( "type" );
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
    auto itPosture = std::find_if( vPostures_.begin(), vPostures_.end(), PostureInfos::Cmp( posture ) );
    if( itPosture != vPostures_.end() )
        (*itPosture)->ReadArchive( input );
    else
        throw MASA_EXCEPTION( tools::translate( "Units_Data", "Unit types - Invalid stance '%1'" ).arg( posture.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::UnitInfos::ReadPointDistance
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::ReadPointDistance( xml::xistream& input )
{
    std::unique_ptr<PointInfos> spNew( new PointInfos() );
    spNew->ReadArchive( input );
    vPointInfos_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::ReadArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::ReadArchive( xml::xistream& input )
{
    std::string strType, nbcSuit;
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "type", strType )
          >> xml::attribute( "decisional-model", ptrModel_ );

    eTypeId_ = ADN_Tr::ConvertToAgentTypePion( strType );
    if( eTypeId_ == (E_AgentTypePion)-1 )
        throw MASA_EXCEPTION( tools::translate( "Units_Data", "Unit types - Invalid unit type '%1'" ).arg( strType.c_str() ).toStdString() );

    std::string level, atlas;
    input >> xml::start( "nature" )
            >> xml::attribute( "level", level )
            >> xml::attribute( "atlas-nature", atlas )
            >> xml::attribute( "nature-app6", strNature_ )
            >> xml::attribute( "nature-app6", ptrNatureSymbol_ )
            >> xml::optional >> xml::attribute( "depth", footprint_ )
          >> xml::end;
    CleanupNature();

    E_NatureLevel eNatureLevelType = ENT_Tr::ConvertToNatureLevel( level );
    if( eNatureLevelType == (E_NatureLevel)-1 )
        throw MASA_EXCEPTION( tools::translate( "Units_Data", "Unit types - Invalid hierarchical level '%1'" ).arg( level.c_str() ).toStdString() );
    eNatureLevel_=eNatureLevelType;

    E_NatureAtlasType eNatureAtlasType = ADN_Tr::ConvertToNatureAtlasType( atlas );
    if( eNatureAtlasType == (E_NatureAtlasType)-1 )
        throw MASA_EXCEPTION( tools::translate( "Units_Data", "Unit types - Invalid 'Atlas' attribute '%1'" ).arg( atlas.c_str() ).toStdString() );
    eNatureAtlas_=eNatureAtlasType;

    input >> xml::start( "equipments" )
            >> xml::list( "equipment", *this, &ADN_Units_Data::UnitInfos::ReadEquipment )
          >> xml::end
          >> xml::start( "crew-ranks" )
            >> xml::list( "crew-rank", *this, &ADN_Units_Data::UnitInfos::ReadCrew )
          >> xml::end
          >> xml::optional >> xml::start( "logistics" )
            >> xml::list( "category", contenancesTC1_, &ADN_Equipments_Data::ResourceInfos::ReadCategory, strName_.GetData() )
          >> xml::end;
    bTC1_ = ! contenancesTC1_.categories_.empty();
    for( ADN_Equipments_Data::T_CategoryInfos_Vector::iterator it = contenancesTC1_.categories_.begin(); it != contenancesTC1_.categories_.end(); ++it )
        (*it)->rNormalizedConsumption_ = 0.;

    stocks_.ReadArchive( input, bStock_ );

    input >> xml::start( "postures" )
            >> xml::list( "posture", *this, &ADN_Units_Data::UnitInfos::ReadPosture )
          >> xml::end;

    input >> xml::optional
            >> xml::start( "setup" )
                >> xml::optional >> xml::attribute( "installation-time", installationDelay_ )
                >> xml::optional >> xml::attribute( "uninstallation-time", uninstallationDelay_ )
                >> xml::optional >> xml::attribute( "footprint-radius", nFootprintRadius_ )
                >> xml::optional >> xml::attribute( "speed-modifier", rSpeedModifier_ )
            >> xml::end;
    bInstallationDelay_ = installationDelay_ != "0s" || uninstallationDelay_ != "0s";
    input >> xml::start( "nbc" )
            >> xml::attribute( "decontamination-delay", decontaminationDelay_ )
            >> xml::attribute( "suit", nbcSuit )
          >> xml::end
          >> xml::optional >> xml::start( "distance-before-points" )
            >> xml::list( "distance-before-point", *this, &ADN_Units_Data::UnitInfos::ReadPointDistance )
          >> xml::end
          >> xml::optional >> xml::start( "drill-blow" )
            >> xml::attribute( "width", rProbeWidth_ )
            >> xml::attribute( "length", rProbeLength_ )
          >> xml::end
          >> xml::optional >> xml::start( "sensors-range" )
            >> xml::attribute( "range", nSensorRange_ )
          >> xml::end
          >> xml::optional >> xml::start( "equipments-range" )
            >> xml::attribute( "range", nEquipmentRange_ )
          >> xml::end
          >> xml::optional >> xml::start( "force-ratio" )
            >> xml::attribute( "feedback-time", strengthRatioFeedbackTime_ )
          >> xml::end;

    eNbcSuit_ = ENT_Tr::ConvertToAgentNbcSuit( nbcSuit );
    if( eNbcSuit_ == ( E_AgentNbcSuit )-1 )
        throw MASA_EXCEPTION( tools::translate( "Units_Data", "Unit types - Invalid nbc suit level '%1'" ).arg( nbcSuit.c_str() ).toStdString() );

    bProbe_ = rProbeWidth_ != 0. || rProbeLength_ != 0.;
    bRanges_ = nSensorRange_ != 0 || nEquipmentRange_ != 0;
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
        throw MASA_EXCEPTION( tools::translate( "Units_Data", "Unit types - Invalid crossing height '%1'" ).arg( crossingHeight.c_str() ).toStdString() );
    eCrossingHeight_ = eCrossingHeight;

    input >> xml::optional >> xml::attribute( "can-fly", bCanFly_ )
          >> xml::optional >> xml::attribute( "standard-flying-height", standardFlyingHeight_ )
          >> xml::optional >> xml::attribute( "tactical-flying-height", tacticalFlyingHeight_ )
          >> xml::optional >> xml::attribute( "is-autonomous", bIsAutonomous_ );
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::WriteArchive
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "unit" )
            << xml::attribute( "name", *this )
            << xml::attribute( "type", eTypeId_.Convert() )
            << xml::attribute( "decisional-model", ptrModel_ )
            << xml::attribute( "id", nId_ );

    output << xml::start( "nature" )
            << xml::attribute( "level", eNatureLevel_.Convert() )
            << xml::attribute( "atlas-nature", eNatureAtlas_.Convert() )
            << xml::attribute( "nature-app6", strNature_ );
    if( footprint_.GetData() )
        output << xml::attribute( "depth", footprint_ );
    output << xml::end;

    output << xml::start( "equipments" );
    for( auto itComposante = vComposantes_.begin(); itComposante != vComposantes_.end(); ++itComposante )
        (*itComposante)->WriteArchive( output );
    output << xml::end;

    output << xml::start( "sensors-range" )
            << xml::attribute( "range", nSensorRange_ )
            << xml::end;
    output << xml::start( "equipments-range" )
            << xml::attribute( "range", nEquipmentRange_ )
            << xml::end;

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
    for( auto itPosture = vPostures_.begin(); itPosture != vPostures_.end(); ++itPosture )
        (*itPosture)->WriteArchive( output );
    output << xml::end;

    if( bInstallationDelay_.GetData() || nFootprintRadius_ != 0 || rSpeedModifier_ != 1. )
    {
        output << xml::start( "setup" );
        if( bInstallationDelay_.GetData() )
        {
            output << xml::attribute( "installation-time", installationDelay_ )
                << xml::attribute( "uninstallation-time", uninstallationDelay_ );
        }
        if( nFootprintRadius_ != 0 || rSpeedModifier_ != 1. )
        {
            output << xml::attribute( "footprint-radius", nFootprintRadius_ )
                << xml::attribute( "speed-modifier", rSpeedModifier_ );
        }
        output << xml::end;
    }

    output << xml::start( "nbc" )
            << xml::attribute( "decontamination-delay", decontaminationDelay_ )
            << xml::attribute( "suit", eNbcSuit_.Convert() )
           << xml::end;

    if( ! vPointInfos_.empty() )
    {
        output << xml::start( "distance-before-points" );
        for( auto itPoint = vPointInfos_.begin(); itPoint != vPointInfos_.end(); ++itPoint )
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
        output << xml::start( "repartition" );
        repartition_.WriteArchive( output );
        output  << xml::end;
    }

    output << xml::start( "crossing-height" )
            << xml::attribute( "height", eCrossingHeight_.Convert() )
           << xml::end;

    if( bCanFly_.GetData() )
        output << xml::attribute( "can-fly", bCanFly_ );
    if( standardFlyingHeight_.GetData() > 0 )
        output << xml::attribute( "standard-flying-height", standardFlyingHeight_ );
    if( tacticalFlyingHeight_.GetData() > 0 )
        output << xml::attribute( "tactical-flying-height", tacticalFlyingHeight_ );

    if( bIsAutonomous_.GetData() )
        output << xml::attribute( "is-autonomous", bIsAutonomous_ );

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::CheckDatabaseValidity
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    if( ptrModel_.GetData() == 0 )
        checker.AddError( eMissingDecisionalModel, strName_.GetData(), eUnits );

    if( !bIsAutonomous_.GetData() )
    for( auto itComposante = vComposantes_.begin(); itComposante != vComposantes_.end(); ++itComposante )
    {
        if( (*itComposante)->nNbrHumanInCrew_.GetData() == 0 )
            checker.AddError( eNoCrew, strName_.GetData(), eUnits, -1, (*itComposante)->GetCrossedElement()->strName_.GetData() );
        ( *itComposante )->CheckValidity( checker, strName_.GetData(), eUnits, -1, tools::translate( "ADN_Units_Data", "Equipments" ).toStdString() );
    }
    for( auto it = stocks_.vLogThresholds_.begin(); it != stocks_.vLogThresholds_.end(); ++it )
        ( *it )->CheckValidity( checker, strName_.GetData(), eUnits, -1, tools::translate( "ADN_Units_Data", "Stock" ).toStdString() );
    if( rProbeLength_.GetData() < rProbeWidth_.GetData() )
        checker.AddError( eInvalidProbe, strName_.GetData(), eUnits, -1 );
    if( bIsCivilian_.GetData() )
        repartition_.CheckNoError( strName_.GetData().c_str(), checker, eUnits );
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::CleanupNature
// Created: RPD 2011-04-11
// -----------------------------------------------------------------------------
void ADN_Units_Data::UnitInfos::CleanupNature()
{
    auto pSymbolUnit = ADN_Workspace::GetWorkspace().GetSymbols().GetData().FindSymbolUnit( strNature_.GetData() );
    if( !pSymbolUnit )
        strNature_ = "undefined";
    else if( strNature_ != pSymbolUnit->strName_.GetData() )
        strNature_ = pSymbolUnit->strName_.GetData();
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_Data constructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Units_Data::ADN_Units_Data()
    : ADN_Data_ABC( eUnits )
{
    vUnits_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_Data destructor
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Units_Data::~ADN_Units_Data()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Munitions_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Units_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUnits_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::ReadUnit
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::ReadUnit( xml::xistream& input )
{
    std::unique_ptr<UnitInfos> spNew( new UnitInfos( input.attribute< unsigned int >( "id" ) ) );
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
    vUnits_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::WriteArchive
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
void ADN_Units_Data::WriteArchive( xml::xostream& output ) const
{
    if( vUnits_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "units" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Units" );
    for( auto it = vUnits_.begin(); it != vUnits_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::GetUnitsThatUse
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
QStringList ADN_Units_Data::GetUnitsThatUse( ADN_Equipments_Data::EquipmentInfos& composante )
{
    QStringList result;
    for( auto it = vUnits_.begin(); it != vUnits_.end(); ++it )
    {
        UnitInfos* pUnit = *it;
        for( auto  it2 = pUnit->vComposantes_.begin(); it2 != pUnit->vComposantes_.end(); ++it2 )
            if( (*it2)->GetCrossedElement() == &composante )
            {
                result << pUnit->strName_.GetData().c_str();
                break;
            }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::GetUnitsThatUse
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
QStringList ADN_Units_Data::GetUnitsThatUse( ADN_Models_ModelInfos& model )
{
    QStringList result;
    for( auto it = vUnits_.begin(); it != vUnits_.end(); ++it )
    {
        UnitInfos* pUnit = *it;
        if( (*it)->ptrModel_.GetData() == &model )
            result << pUnit->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::GetUnitsThatUse
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
QStringList ADN_Units_Data::GetUnitsThatUse( ADN_LogisticSupplyClasses_Data::LogisticSupplyClass& supply )
{
    QStringList result;
    for( auto it = vUnits_.begin(); it != vUnits_.end(); ++it )
        for( auto itStock = ( *it )->stocks_.vLogThresholds_.begin(); itStock != ( *it )->stocks_.vLogThresholds_.end(); ++itStock )
        {
            ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* infos = ( *itStock )->GetCrossedElement();
            if( infos && infos->strName_.GetData() == supply.strName_.GetData() )
                result << ( *it )->strName_.GetData().c_str();
        }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::CheckDatabaseValidity
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
void ADN_Units_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = vUnits_.begin(); it != vUnits_.end(); ++it )
        (*it)->CheckDatabaseValidity( checker );
}
