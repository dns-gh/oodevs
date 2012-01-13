// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/PHY_UnitType.cpp $
// $Author: Nld $
// $Modtime: 8/03/05 15:37 $
// $Revision: 9 $
// $Workfile: PHY_UnitType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_UnitType.h"
#include "Postures/PHY_Posture.h"
#include "Composantes/PHY_ComposanteTypePion.h"
#include "Composantes/PHY_ComposantePion.h"
#include "Humans/PHY_HumanRank.h"
#include "Dotations/PHY_DotationLogisticType.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::sComposanteTypeData::sComposanteTypeData
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
PHY_UnitType::sComposanteTypeData::sComposanteTypeData()
    : nNbr_              ( 0 )
    , bMajor_            ( false )
    , bLoadable_         ( false )
    , bCanBePartOfConvoy_( false )
    , nNbrHumanInCrew_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType constructor
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
PHY_UnitType::PHY_UnitType( xml::xistream& xis )
    : dotationCapacitiesTC1_            ( "logistics", xis )
    , stockLogisticThresholdRatios_     ( PHY_DotationLogisticType::GetDotationLogisticTypes().size(), 0.1 )
    , composanteTypes_                  ()
    , postureTimes_                     ( PHY_Posture::GetPostures().size(), 0 )
    , rInstallationTime_                ( 0. )
    , rUninstallationTime_              ( 0. )
    , footprintRadius_                  ( 0 )
    , rSpeedModifier_                   ( 1 )
    , rCoupDeSondeLength_               ( 0. )
    , rCoupDeSondeWidth_                ( 0. )
    , commandersRepartition_            ()
    , rCoefDecontaminationPerTimeStep_  ( 0. )
    , bCanFly_                          ( false )
    , bIsAutonomous_                    ( false )
    , nReconEfficiency_                 ( 50 )
    , nCombatSupportEfficiency_         ( 50 )
    , nCombatEfficiency_                ( 50 )
    , nMobilitySupportEfficiency_       ( 50 )
    , nCounterMobilitySupportEfficiency_( 50 )
    , nProtectionSupportEfficiency_     ( 50 )
    , nEngineeringReconEfficiency_      ( 50 )
    , nUrbanAreaEfficiency_             ( 50 )
    , crossingHeight_                   ( eCrossingHeightLowAreas )
{
    xis >> xml::optional
            >> xml::attribute( "can-fly", bCanFly_ )
        >> xml::optional
            >> xml::attribute( "is-autonomous", bIsAutonomous_ );

    InitializeComposantes                 ( xis );
    InitializeCommanderRepartition        ( xis );
    InitializePostureTimes                ( xis );
    InitializeInstallationTimes           ( xis );
    InitializeCoupDeSonde                 ( xis );
    InitializeNBC                         ( xis );
    InitializeStockLogisticThresholdRatios( xis );
    InitializeEfficiencies                ( xis );
    InitializeCrossingHeight              ( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType destructor
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
PHY_UnitType::~PHY_UnitType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeStockLogisticThresholdRatios
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeStockLogisticThresholdRatios( xml::xistream& xis )
{
    xis >> xml::optional
            >> xml::start( "stocks" )
                >> xml::list( "stock", *this, &PHY_UnitType::ReadStock )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeEfficiencies
// Created: JSR 2010-11-29
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeEfficiencies( xml::xistream& xis )
{
    xis >> xml::optional
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
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeCrossingHeight
// Created: JSR 2011-01-07
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeCrossingHeight( xml::xistream& xis )
{
    crossingHeight_ = eCrossingHeightNever;
    std::string strCrossingHeight;
    xis >> xml::optional
            >> xml::start( "crossing-height" )
                >> xml::attribute( "height", strCrossingHeight )
            >> xml::end;
    if( strCrossingHeight == "1m" )
        crossingHeight_ = eCrossingHeightLowAreas;
    else if( strCrossingHeight == "0m" )
        crossingHeight_ = eCrossingHeightNever;
    else if( strCrossingHeight == ">1m" )
        crossingHeight_ = eCrossingHeightAlways;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadStock
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadStock( xml::xistream& xis )
{
    const PHY_DotationLogisticType* pType = PHY_DotationLogisticType::Find( xis.attribute< std::string >( "category" ) );
    if( !pType )
        xis.error( "Unknown logistic dotation type" );
    assert( stockLogisticThresholdRatios_.size() > pType->GetID() );
    double rThreshold = xis.attribute< double >( "threshold" );
    if( rThreshold < 0 || rThreshold > 100 )
        xis.error( "stock: threshold not in [0..100]" );
    rThreshold /= 100.;
    stockLogisticThresholdRatios_[ pType->GetID() ] = rThreshold;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeNBC
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeNBC( xml::xistream& xis )
{
    float rTmp;
    xis >> xml::start( "nbc" );
    tools::ReadTimeAttribute( xis, "decontamination-delay", rTmp );
    rTmp = std::max<float>( 1.0, rTmp );

    rCoefDecontaminationPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rTmp );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeComposantes
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeComposantes( xml::xistream& xis )
{
    xis >> xml::start( "equipments" )
            >> xml::list( "equipment", *this, &PHY_UnitType::ReadEquipment )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadEquipment
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadEquipment( xml::xistream& xis )
{
    const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::Find( xis.attribute< std::string >( "type" ) );
    if( !pComposanteType )
        xis.error( "Unknown composante type" );
    if( composanteTypes_.find( pComposanteType ) != composanteTypes_.end() )
        xis.error( "Composante type already exist" );
    sComposanteTypeData& compData = composanteTypes_[ pComposanteType ];
    compData.bMajor_ = false;
    compData.bLoadable_ = false;
    compData.bCanBePartOfConvoy_ = false;
    compData.nNbrHumanInCrew_ = 0;
    xis >> xml::optional
            >> xml::attribute( "major", compData.bMajor_ )
        >> xml::optional
            >> xml::attribute( "loadable", compData.bLoadable_ )
        >> xml::optional
            >> xml::attribute( "convoyer", compData.bCanBePartOfConvoy_ )
        >> xml::attribute( "crew", compData.nNbrHumanInCrew_ );
    if( compData.nNbrHumanInCrew_ < 0 )
        xis.error( "equipment: crew < 0" );
    if( compData.nNbrHumanInCrew_ == 0 && !IsAutonomous() )
        xis.error( "Composante not viable : no humans in crew" );
    xis >> xml::attribute( "count", compData.nNbr_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeCommanderRepartition
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeCommanderRepartition( xml::xistream& xis )
{
    xis >> xml::start( "crew-ranks" )
            >> xml::list( "crew-rank", *this, &PHY_UnitType::ReadCrewRank )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadCrewRank
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadCrewRank( xml::xistream& xis )
{
    const PHY_HumanRank::T_HumanRankMap& ranks = PHY_HumanRank::GetHumanRanks();
    PHY_HumanRank::CIT_HumanRankMap it = ranks.find( xis.attribute< std::string >( "type" ) );
    const PHY_HumanRank& rank = *it->second;
    if( !rank.IsCommander() )
        return;
    if( commandersRepartition_.find( &rank ) != commandersRepartition_.end() )
        xis.error( "crew-rank: type undefined" );
    unsigned int nValue = xis.attribute< unsigned int >( "count" );
    if( nValue > 0 )
        commandersRepartition_[ &rank ] = nValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializePostureTimes
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializePostureTimes( xml::xistream& xis )
{
    xis >> xml::start( "postures" )
            >> xml::list( "posture", *this, &PHY_UnitType::ReadPosture )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadPosture
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadPosture( xml::xistream& xis )
{
    const PHY_Posture::T_PostureMap& postures = PHY_Posture::GetPostures();
    PHY_Posture::CIT_PostureMap it = postures.find( xis.attribute< std::string >( "name" ) );
    const PHY_Posture& posture = *it->second;
    assert( postureTimes_.size() > posture.GetID() );
    double rTime;
    tools::ReadTimeAttribute( xis, "setup-time", rTime );
    postureTimes_[ posture.GetID() ] = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rTime ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeInstallationTimes
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeInstallationTimes( xml::xistream& xis )
{
    xis >> xml::list( "setup", *this, &PHY_UnitType::ReadSetup );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadSetup
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadSetup( xml::xistream& xis )
{
    if( tools::ReadTimeAttribute( xis, "installation-time", rInstallationTime_ ) && tools::ReadTimeAttribute( xis, "uninstallation-time", rUninstallationTime_ ) )
    {
        if( rInstallationTime_ < 0 )
            xis.error( "setup: installation-time < 0" );
        if( rUninstallationTime_ < 0 )
            xis.error( "setup: uninstallation-time < 0" );
        rInstallationTime_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rInstallationTime_ ) );
        rUninstallationTime_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rUninstallationTime_ ) );
    }
    xis >> xml::optional >> xml::attribute( "footprint-radius", footprintRadius_ )
        >> xml::optional >> xml::attribute( "speed-modifier", rSpeedModifier_ );
    if( footprintRadius_ < 0 )
        xis.error( "setup: footprint-radius < 0" );
    if( rSpeedModifier_ < 0 || rSpeedModifier_ > 1 )
        xis.error( "setup: speed-modifier not in [0,1]" );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeCoupDeSonde
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeCoupDeSonde( xml::xistream& xis )
{
    rCoupDeSondeLength_ = 0.;
    rCoupDeSondeWidth_ = 0.;
    xis >> xml::list( "drill-blow", *this, &PHY_UnitType::ReadDrill );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadDrill
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadDrill( xml::xistream& xis )
{
    xis >> xml::attribute( "width", rCoupDeSondeWidth_ )
        >> xml::attribute( "length", rCoupDeSondeLength_ );
    if( rCoupDeSondeLength_ < rCoupDeSondeWidth_ )
        xis.error( "Length should be greater than width" );
    rCoupDeSondeLength_ = MIL_Tools::ConvertMeterToSim( rCoupDeSondeLength_ );
    rCoupDeSondeWidth_  = MIL_Tools::ConvertMeterToSim( rCoupDeSondeWidth_  );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetPostureTime
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
double PHY_UnitType::GetPostureTime( const PHY_Posture& posture ) const
{
    assert( postureTimes_.size() > posture.GetID() );
    return postureTimes_[ posture.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetStockLogisticThresholdRatio
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
double PHY_UnitType::GetStockLogisticThresholdRatio( const PHY_DotationLogisticType& type ) const
{
    assert( stockLogisticThresholdRatios_.size() > type.GetID() );
    return stockLogisticThresholdRatios_[ type.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InstanciateComposantes
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
void PHY_UnitType::InstanciateComposantes( PHY_RolePion_Composantes& role ) const
{
    for( CIT_ComposanteTypeMap itComposanteType = composanteTypes_.begin(); itComposanteType != composanteTypes_.end(); ++itComposanteType )
    {
        const PHY_ComposanteTypePion& compType = *itComposanteType->first;
        const sComposanteTypeData& compData = itComposanteType->second;
        for( unsigned int i = 0; i < compData.nNbr_; ++i )
            compType.InstanciateComposante( role, compData.nNbrHumanInCrew_ , compData.bMajor_, compData.bLoadable_, compData.bCanBePartOfConvoy_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetTC1Capacities
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
const PHY_DotationCapacities& PHY_UnitType::GetTC1Capacities() const
{
    return dotationCapacitiesTC1_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetCommanderRepartition
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
const PHY_UnitType::T_CommanderRepartitionMap& PHY_UnitType::GetCommanderRepartition() const
{
    return commandersRepartition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetCoupDeSondeLength
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
double PHY_UnitType::GetCoupDeSondeLength() const
{
    return rCoupDeSondeLength_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetCoupDeSondeWidth
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
double PHY_UnitType::GetCoupDeSondeWidth() const
{
    return rCoupDeSondeWidth_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetCoefDecontaminationPerTimeStep
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
double PHY_UnitType::GetCoefDecontaminationPerTimeStep() const
{
    return rCoefDecontaminationPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::CanFly
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
bool PHY_UnitType::CanFly() const
{
    return bCanFly_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::IsAutonomous
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_UnitType::IsAutonomous() const
{
    return bIsAutonomous_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetInstallationTime
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
double PHY_UnitType::GetInstallationTime() const
{
    return rInstallationTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetUninstallationTime
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
double PHY_UnitType::GetUninstallationTime() const
{
    return rUninstallationTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetFootprintRadius
// Created: JSR 2012-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_UnitType::GetFootprintRadius() const
{
    return footprintRadius_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetSpeedModifier
// Created: JSR 2012-01-12
// -----------------------------------------------------------------------------
double PHY_UnitType::GetSpeedModifier() const
{
    return rSpeedModifier_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetPionEfficiency
// Created: JSR 2010-11-30
// -----------------------------------------------------------------------------
unsigned int PHY_UnitType::GetPionEfficiency( E_PionEfficiency pionEfficiency ) const
{
    switch( pionEfficiency )
    {
    case ePionEfficiencyRecon:
        return nReconEfficiency_;
    case ePionEfficiencyCombatSupport:
        return nCombatSupportEfficiency_;
    case ePionEfficiencyCombat:
        return nCombatEfficiency_;
    case ePionEfficiencyMobilitySupport:
        return nMobilitySupportEfficiency_;
    case ePionEfficiencyCounterMobilitySupport:
        return nCounterMobilitySupportEfficiency_;
    case ePionEfficiencyProtectionSupport:
        return nProtectionSupportEfficiency_;
    case ePionEfficiencyEngineeringSupport:
        return nEngineeringReconEfficiency_;
    case ePionEfficiencyUrbanArea:
        return nUrbanAreaEfficiency_;
    default:
        throw std::exception( "GetPionEfficiency : bad value" );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetCrossingHeight
// Created: JSR 2011-01-07
// -----------------------------------------------------------------------------
E_CrossingHeight PHY_UnitType::GetCrossingHeight() const
{
    return crossingHeight_;
}
