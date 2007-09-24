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
#include "xeumeuleu/xml.h"

using namespace xml;

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
    : dotationCapacitiesTC1_          ( "logistics", xis )
    , stockLogisticThresholdRatios_   ( PHY_DotationLogisticType::GetDotationLogisticTypes().size(), 0.1 )
    , composanteTypes_                ()
    , postureTimes_                   ( PHY_Posture::GetPostures().size(), 0 )
    , rInstallationTime_              ( 0. )
    , rUninstallationTime_            ( 0. )
    , rCoupDeSondeLength_             ( 0. )
    , rCoupDeSondeWidth_              ( 0. )
    , commandersRepartition_          ()
    , rCoefDecontaminationPerTimeStep_( 0. )
    , bCanFly_                        ( false )
    , bIsAutonomous_                  ( false )
{
    xis >> optional()
            >> attribute( "can-fly", bCanFly_ )
        >> optional()
            >> attribute( "is-autonomous", bIsAutonomous_ );

    InitializeComposantes                 ( xis );
    InitializeCommanderRepartition        ( xis );
    InitializePostureTimes                ( xis );
    InitializeInstallationTimes           ( xis );
    InitializeCoupDeSonde                 ( xis );
    InitializeNBC                         ( xis );
    InitializeStockLogisticThresholdRatios( xis );
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
    xis >> optional()
            >> start( "stocks" )
                >> list( "stock", *this, &PHY_UnitType::ReadStock )
            >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadStock
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadStock( xml::xistream& xis )
{
    std::string strCategory;
    xis >> attribute( "category", strCategory );

    const PHY_DotationLogisticType* pType = PHY_DotationLogisticType::Find( strCategory );
    if( !pType )
        xis.error( "Unknown logistic dotation type" );

    assert( stockLogisticThresholdRatios_.size() > pType->GetID() );

    MT_Float rThreshold = 0.;
    xis >> attribute( "threshold", rThreshold );
    if( rThreshold < 0 || rThreshold > 100 )
        xis.error( "stock: thresolh not in [0..100]" );

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
    xis >> start( "nbc" );
    tools::ReadTimeAttribute( xis, "decontamination-delay", rTmp );
    if( rTmp <= 0 )
        xis.error( "nbc: decontamination-delay <= 0" );
    rCoefDecontaminationPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rTmp );
    xis >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeComposantes
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeComposantes( xml::xistream& xis )
{
    xis >> start( "equipments" )
            >> list( "equipment", *this, &PHY_UnitType::ReadEquipment )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadEquipment
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadEquipment( xml::xistream& xis )
{
    std::string strComposanteType;
    xis >> attribute( "type", strComposanteType );

    const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::Find( strComposanteType );
    if( !pComposanteType )
        xis.error( "Unknown composante type" );

    if( composanteTypes_.find( pComposanteType ) != composanteTypes_.end() )
        xis.error( "Composante type already exist" );

    sComposanteTypeData& compData = composanteTypes_[ pComposanteType ];
    compData.bMajor_ = false;
    compData.bLoadable_ = false;
    compData.bCanBePartOfConvoy_ = false;
    compData.nNbrHumanInCrew_ = 0;

    xis >> optional()
            >> attribute( "major", compData.bMajor_ )
        >> optional()
            >> attribute( "loadable", compData.bLoadable_ )
        >> optional()
            >> attribute( "convoyer", compData.bCanBePartOfConvoy_ )
        >> attribute( "crew", compData.nNbrHumanInCrew_ );

    if( compData.nNbrHumanInCrew_ < 0 )
        xis.error( "equipment: crew < 0" );

    if( compData.nNbrHumanInCrew_ == 0 && !IsAutonomous() )
        xis.error( "Composante not viable : no humans in crew" );

    xis >> attribute( "count", compData.nNbr_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeCommanderRepartition
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeCommanderRepartition( xml::xistream& xis )
{
    xis >> start( "crew-ranks" )
            >> list( "crew-rank", *this, &PHY_UnitType::ReadCrewRank )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadCrewRank
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadCrewRank( xml::xistream& xis )
{
    const PHY_HumanRank::T_HumanRankMap& ranks = PHY_HumanRank::GetHumanRanks();
    std::string crewType;

    xis >> attribute( "type", crewType );

    PHY_HumanRank::CIT_HumanRankMap it = ranks.find( crewType );
    const PHY_HumanRank& rank = *it->second;
    if( !rank.IsCommander() )
        return;

    if( commandersRepartition_.find( &rank ) != commandersRepartition_.end() )
        xis.error( "crew-rank: type undefined" );        

    uint nValue = 0;
    xis >> attribute( "count", nValue );
    if( nValue > 0 )
        commandersRepartition_[ &rank ] = nValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializePostureTimes
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializePostureTimes( xml::xistream& xis )
{
    xis >> start( "postures" )
            >> list( "posture", *this, &PHY_UnitType::ReadPosture )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadPosture
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadPosture( xml::xistream& xis )
{
    const PHY_Posture::T_PostureMap& postures = PHY_Posture::GetPostures();
    std::string postureName;

    xis >> attribute( "name", postureName );

    PHY_Posture::CIT_PostureMap it = postures.find( postureName );
    const PHY_Posture& posture = *it->second;

    assert( postureTimes_.size() > posture.GetID() );
    MT_Float rTime;
    tools::ReadTimeAttribute( xis, "setup-time", rTime );
    postureTimes_[ posture.GetID() ] = (uint)MIL_Tools::ConvertSecondsToSim( rTime );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeInstallationTimes
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeInstallationTimes( xml::xistream& xis )
{
    xis >> list( "setup", *this, &PHY_UnitType::ReadSetup );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadSetup
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadSetup( xml::xistream& xis )
{
    tools::ReadTimeAttribute( xis, "installation-time", rInstallationTime_ );
    tools::ReadTimeAttribute( xis, "uninstallation-time", rUninstallationTime_ );

    if( rInstallationTime_ < 0 )
        xis.error( "setup: installation-time < 0" );
    if( rUninstallationTime_ < 0 )
        xis.error( "setup: uninstallation-time < 0" );

    rInstallationTime_   = (uint)MIL_Tools::ConvertSecondsToSim( rInstallationTime_   );
    rUninstallationTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rUninstallationTime_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeCoupDeSonde
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeCoupDeSonde( xml::xistream& xis )
{
    rCoupDeSondeLength_ = 0.;
    rCoupDeSondeWidth_  = 0.;
    xis >> list( "drill-blow", *this, &PHY_UnitType::ReadDrill );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::ReadDrill
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_UnitType::ReadDrill( xml::xistream& xis )
{
    xis >> attribute( "width", rCoupDeSondeWidth_ )
        >> attribute( "length", rCoupDeSondeLength_ );

    if ( rCoupDeSondeLength_ < rCoupDeSondeWidth_ )
        xis.error( "Length should be greater than width" );

    rCoupDeSondeLength_ = MIL_Tools::ConvertMeterToSim( rCoupDeSondeLength_ );
    rCoupDeSondeWidth_  = MIL_Tools::ConvertMeterToSim( rCoupDeSondeWidth_  );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetPostureTime
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MT_Float PHY_UnitType::GetPostureTime( const PHY_Posture& posture ) const
{
    assert( postureTimes_.size() > posture.GetID() );
    return postureTimes_[ posture.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetStockLogisticThresholdRatio
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
MT_Float PHY_UnitType::GetStockLogisticThresholdRatio( const PHY_DotationLogisticType& type ) const
{
    assert( stockLogisticThresholdRatios_.size() > type.GetID() );
    return stockLogisticThresholdRatios_[ type.GetID() ];
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InstanciateComposantes
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
void PHY_UnitType::InstanciateComposantes( PHY_RolePion_Composantes& role ) const
{
    for( CIT_ComposanteTypeMap itComposanteType = composanteTypes_.begin(); itComposanteType != composanteTypes_.end(); ++itComposanteType )
    {
        const PHY_ComposanteTypePion& compType = *itComposanteType->first;
        const sComposanteTypeData&    compData =  itComposanteType->second;

        for( uint i = 0; i < compData.nNbr_; ++i )
            compType.InstanciateComposante( role, compData.nNbrHumanInCrew_, compData.bMajor_, compData.bLoadable_, compData.bCanBePartOfConvoy_ );
    }
}