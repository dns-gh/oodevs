// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.cpp $
// $Author: Age $
// $Modtime: 4/05/05 15:04 $
// $Revision: 21 $
// $Workfile: PHY_ComposanteTypePion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ComposanteTypePion.h"
#include "PHY_ComposantePion.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Composantes/PHY_ActiveProtection.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationConsumptions.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Humans/Human_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Logistic/PHY_MaintenanceLevel.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Radars/PHY_RadarType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/BuildableCapacity.h"
#include "Entities/Objects/BypassableCapacity.h"
#include "Entities/Objects/MobilityCapacity.h"
#include "Entities/Objects/ObjectTypeResolver_ABC.h"
#include "PHY_ComposanteTypeObjectData.h"
#include "ENT/ENT_Tr.h"
#include "MIL_Random.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"
#include "MT_Tools/MT_Logger.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/foreach.hpp>

PHY_ComposanteTypePion::T_ComposanteTypeMap PHY_ComposanteTypePion::composantesTypes_;

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::sNTICapability::CanRepair
// Created: NLD 2004-12-23
// Modified: JVT 2005-02-03
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::sNTICapability::CanRepair( const PHY_Breakdown& breakdown ) const
{
    if( breakdown.GetMaintenanceLevel() != *pMaintenanceLevel_ )
        return false;
    if( breakdown.AffectElectronic() && !bElectronic_ )
        return false;
    if( breakdown.AffectMobility() && !bMobility_ )
        return false;
    if( breakdown.GetTheoricRepairTime() > nMaxTime_ )
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::Initialize( const MIL_Time_ABC& time, xml::xistream& xis, const ObjectTypeResolver_ABC& resolver )
{
    MT_LOG_INFO_MSG( "Initializing composante types" );
    xis >> xml::start( "equipments" )
            >> xml::list( "equipment", boost::bind( &PHY_ComposanteTypePion::ReadElement, _1, boost::cref( time ), boost::cref( resolver ) ) )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadElement
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadElement( xml::xistream& xis, const MIL_Time_ABC& time, const ObjectTypeResolver_ABC& resolver )
{
    std::set< unsigned int > ids_;
    std::string strComposanteType;
    xis >> xml::attribute( "name", strComposanteType );
    PHY_ComposanteTypePion*& pComposanteType = composantesTypes_[ strComposanteType ];
    if( pComposanteType )
        throw MASA_EXCEPTION( xis.context() + "Composante type '" + strComposanteType + "' already registered" );
    pComposanteType = new PHY_ComposanteTypePion( time, strComposanteType, xis, resolver );

    if( !ids_.insert( pComposanteType->GetMosID().id() ).second )
        throw MASA_EXCEPTION( xis.context() + "Composante ID already used" );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::Terminate()
{
    for( auto it = composantesTypes_.begin(); it != composantesTypes_.end(); ++it )
        delete it->second;
    composantesTypes_.clear();
}

namespace
{
    double ReadMaxSpeed( xml::xistream& xis )
    {
        double rMaxSpeed = -1;
        xis >> xml::start( "speeds" )
                >> xml::attribute( "max", rMaxSpeed ) // km/h
            >> xml::end;
        if( rMaxSpeed <= 0 )
            throw MASA_EXCEPTION( xis.context() + "speeds: max <= 0" );
        return MIL_Tools::ConvertSpeedMosToSim( rMaxSpeed ); // m/tick
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion constructor
// Created: NLD 2004-08-04
// Modified: JVT 2004-10-20
// -----------------------------------------------------------------------------
PHY_ComposanteTypePion::PHY_ComposanteTypePion( const MIL_Time_ABC& time, const std::string& strName, xml::xistream& xis, const ObjectTypeResolver_ABC& resolver )
    : PHY_ComposanteType_ABC                     ( strName, xis )
    , time_                                      ( time )
    , speeds_                                    ( xis, ReadMaxSpeed( xis ) )
    , rMaxSlope_                                 ( 1. )
    , rSlopeDeceleration_                        ( 0 )
    , dotationCapacities_                        ( "composition", xis )
    , objectData_                                ( )
    , consumptions_                              ( PHY_ConsumptionType::GetConsumptionTypes().size(), 0 )
    , rNbrHumansLoadedPerTimeStep_               ( 0. )
    , rNbrHumansUnloadedPerTimeStep_             ( 0. )
    , rSensorRotationAngle_                      ( 0. )
    , rWeight_                                   ( 0. )
    , rPionTransporterWeightCapacity_            ( 0. )
    , rPionTransporterWeightLoadedPerTimeStep_   ( 0. )
    , rPionTransporterWeightUnloadedPerTimeStep_ ( 0. )
    , bCanTransportDestroyed_                    ( false )
    , nCrowdTransporterCapacity_                 ( 0 )
    , rCrowdTransporterLoadedPerTimeStep_        ( 0. )
    , rCrowdTransporterUnloadedPerTimeStep_      ( 0. )
    , rHaulerWeightCapacity_                     ( 0. )
    , rHaulerLoadingTime_                        ( 0. )
    , rHaulerUnloadingTime_                      ( 0. )
    , woundCollectionCapabilities_               ( PHY_HumanWound::GetHumanWounds().size(), false )
    , bCanCollectMentalDiseases_                 ( false )
    , bCanCollectContaminated_                   ( false )
    , bCanEvacuateMentalDiseases_                ( false )
    , bCanEvacuateContaminated_                  ( false )
    , bCanHealMentalDiseases_                    ( false )
    , bCanHealContaminated_                      ( false )
    , bCanSortHumans_                            ( false )
    , bCanDiagnoseHumans_                        ( false )
    , bCanHealWounds_                            ( false )
    , nAmbulanceCollectionCapacity_              ( 0 )
    , rNbrHumansLoadedForCollectionPerTimeStep_  ( 0. )
    , rNbrHumansUnloadedForCollectionPerTimeStep_( 0. )
    , woundEvacuationCapabilities_               ( PHY_HumanWound::GetHumanWounds().size(), false )
    , nAmbulanceEvacuationCapacity_              ( 0 )
    , rNbrHumansLoadedForEvacuationPerTimeStep_  ( 0. )
    , rNbrHumansUnloadedForEvacuationPerTimeStep_( 0. )
    , woundHealingCapabilities_                  ( PHY_HumanWound::GetHumanWounds().size(), false )
    , pStockTransporterNature_                   ( 0 )
    , rStockTransporterMinWeightCapacity_        ( 0 )
    , rStockTransporterMaxWeightCapacity_        ( 0 )
    , rStockTransporterMinVolumeCapacity_        ( 0 )
    , rStockTransporterMaxVolumeCapacity_        ( 0 )
{
    xis >> xml::optional >> xml::attribute( "max-slope", rMaxSlope_ )
        >> xml::optional  >> xml::attribute( "slope-deceleration", rSlopeDeceleration_ )
        >> xml::attribute( "weight", rWeight_ );
    if( rMaxSlope_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "element: max-slope < 0" );
    if( rSlopeDeceleration_ < 0 || rSlopeDeceleration_ > 1 )
        throw MASA_EXCEPTION( xis.context() + "element: slope-deceleration not in [0..1]" );
    if( rWeight_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "element: weight <= 0" );
    InitializeWeapons         ( xis );
    InitializeSensors         ( xis );
    InitializeRadars          ( xis );
    InitializeTransport       ( xis );
    InitializeObjects         ( xis, resolver );
    InitializeConsumptions    ( xis );
    InitializeAviationQuotas  ( xis );
    InitializeLogistic        ( xis );
    InitializeBreakdownTypes  ( xis );
    InitializeProtections     ( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_ComposanteTypePion::~PHY_ComposanteTypePion()
{
    for( auto it = objectData_.begin(); it != objectData_.end(); ++it )
        delete *it;
    objectData_.clear();
    for( auto it = consumptions_.begin(); it != consumptions_.end(); ++it )
        delete *it;
    consumptions_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeBreakdownTypes
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeBreakdownTypes( xml::xistream& xis )
{
    if( GetProtection().IsHuman() )
        return;
    attritionBreakdownTypeProbabilities_.clear();
    randomBreakdownTypeProbabilities_.clear();
    xis >> xml::start( "breakdowns" )
            >> xml::list( "breakdown", *this, &PHY_ComposanteTypePion::InitializeBreakdown );
    xis >> xml::end;
    if( randomBreakdownTypeProbabilities_.empty()
        || std::fabs( 1. - randomBreakdownTypeProbabilities_.back().rProbabilityBound_ ) > 0.01 )
        throw MASA_EXCEPTION( xis.context() + "Total probability of random breakdowns is less than 100%" );
    if( attritionBreakdownTypeProbabilities_.empty()
        || std::fabs( 1. - attritionBreakdownTypeProbabilities_.back().rProbabilityBound_  ) > 0.01 )
        throw MASA_EXCEPTION( xis.context() + "Total probability of attrition breakdowns is less than 100%" );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeBreakdown
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeBreakdown( xml::xistream& xis )
{
    std::string breakdownType;
    xis >> xml::attribute( "origin", breakdownType );
    if( breakdownType == "random" )
        InitializeRandomBreakdownTypes( xis );
    if( breakdownType == "attrition" )
        InitializeAttritionBreakdownTypes( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeRandomBreakdownTypes
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeRandomBreakdownTypes( xml::xistream& xis )
{
    std::string strBuf;
    xis >> xml::attribute( "type", strBuf );
    const PHY_BreakdownType* pType = PHY_BreakdownType::Find( strBuf );
    if( !pType )
        throw MASA_EXCEPTION( xis.context() + "Unknown breakdown type '" + strBuf + "'" );
    double rPercentage;
    xis >> xml::attribute( "percentage", rPercentage );
    if( rPercentage < 0 || rPercentage > 100 )
        throw MASA_EXCEPTION( xis.context() + "random breakdown: percentage not in [0..100]" );
    rPercentage *= 0.01;
    if( !randomBreakdownTypeProbabilities_.empty() )
        rPercentage += randomBreakdownTypeProbabilities_.back().rProbabilityBound_;
    randomBreakdownTypeProbabilities_.push_back( sBreakdownTypeProbability( *pType, rPercentage ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeAttritionBreakdownTypes
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeAttritionBreakdownTypes( xml::xistream& xis )
{
    std::string strBuf;
    xis >> xml::attribute( "type", strBuf );
    const PHY_BreakdownType* pType = PHY_BreakdownType::Find( strBuf );
    if( !pType )
        throw MASA_EXCEPTION( xis.context() + "Unknown breakdown type '" + strBuf + "'" );
    double rPercentage;
    xis >> xml::attribute( "percentage", rPercentage );
    if( rPercentage < 0 || rPercentage > 100 )
        throw MASA_EXCEPTION( xis.context() + "attrition breakdown: percentage not in [0..100]" );
    rPercentage *= 0.01;
    if( !attritionBreakdownTypeProbabilities_.empty() )
        rPercentage += attritionBreakdownTypeProbabilities_.back().rProbabilityBound_;
    attritionBreakdownTypeProbabilities_.push_back( sBreakdownTypeProbability( *pType, rPercentage ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeWeapons
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeWeapons( xml::xistream& xis )
{
    xis >> xml::start( "weapon-systems" )
            >> xml::list( "weapon-system", *this, &PHY_ComposanteTypePion::ReadWeaponSystem )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadWeaponSystem
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadWeaponSystem( xml::xistream& xis )
{
    std::string strLauncher;
    std::string strAmmunition;
    bool bMajor = false;
    xis >> xml::attribute( "launcher", strLauncher )
        >> xml::attribute( "munition", strAmmunition )
        >> xml::optional
            >> xml::attribute( "major", bMajor );
    const PHY_WeaponType* pWeaponType = PHY_WeaponType::FindWeaponType( strLauncher, strAmmunition );
    if( !pWeaponType )
        throw MASA_EXCEPTION( xis.context() + "Unknown weapon type (" + strLauncher + ", " + strAmmunition + ")" );
    if( weaponTypes_.find( pWeaponType ) != weaponTypes_.end() )
        throw MASA_EXCEPTION( xis.context() + "Weapon type (" + strLauncher + ", " + strAmmunition + ") already initialized" );
    weaponTypes_[ pWeaponType ] = bMajor;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeProtections
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeProtections( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "active-protections" )
            >> xml::list( "protection", *this, &PHY_ComposanteTypePion::ReadActiveProtection )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadActiveProtection
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadActiveProtection( xml::xistream& xis )
{
    std::string strProtectionName;
    xis >> xml::attribute( "name", strProtectionName );
    const PHY_ActiveProtection* pProtection = PHY_ActiveProtection::Find( strProtectionName );
    if( !pProtection )
        throw MASA_EXCEPTION( xis.context() + "Unknow active protection which name is " + strProtectionName );
    activeProtections_.push_back( pProtection );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeSensors
// Created: NLD 2004-08-09
// Modified: JVT 2004-10-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeSensors( xml::xistream& xis )
{
    rSensorRotationAngle_ = std::numeric_limits< double >::max();
    xis >> xml::start( "sensors" )
            >> xml::list( "sensor", *this, &PHY_ComposanteTypePion::ReadSensor )
        >> xml::end;
    if( rSensorRotationAngle_ == std::numeric_limits< double >::max() )
        rSensorRotationAngle_ = 0.;
    else
        rSensorRotationAngle_ -= std::min( rSensorRotationAngle_, 0.1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadSensor
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadSensor( xml::xistream& xis )
{
    std::string strSensor;
    xis >> xml::attribute( "type", strSensor );
    const PHY_SensorType* pSensorType = PHY_SensorType::FindSensorType( strSensor );
    if( !pSensorType )
        throw MASA_EXCEPTION( xis.context() + "Unknown sensor type '" + strSensor + "'" );
    if( sensorTypes_.find( pSensorType ) != sensorTypes_.end() )
        throw MASA_EXCEPTION( xis.context() + "Sensor type '" + strSensor + "' already defined" );
    xis >> xml::attribute( "height", sensorTypes_[ pSensorType ] );
    if( pSensorType->GetTypeAgent() )
        rSensorRotationAngle_ = std::min( rSensorRotationAngle_, pSensorType->GetTypeAgent()->GetAngle() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeRadars
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeRadars( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( "radars" )
            >> xml::list( "radar", *this, &PHY_ComposanteTypePion::ReadRadar )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadRadar
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadRadar( xml::xistream& xis )
{
    std::string strRadar;
    xis >> xml::attribute( "type", strRadar );
    const PHY_RadarType* pRadarType = PHY_RadarType::Find( strRadar );
    if( !pRadarType )
        throw MASA_EXCEPTION( xis.context() + "Unknown radar type" );
    if( radarTypes_.find( pRadarType ) != radarTypes_.end() )
        throw MASA_EXCEPTION( xis.context() + "Radar type already defined" );
    xis >> xml::attribute( "height", radarTypes_[ pRadarType ] );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeTransport
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeTransport( xml::xistream& xis )
{
    xis >> xml::start( "transports" )
            >> xml::list( "crew", *this, &PHY_ComposanteTypePion::ReadTransportCrew )
            >> xml::list( "unit", *this, &PHY_ComposanteTypePion::ReadTransportUnit )
            >> xml::list( "crowd", *this, &PHY_ComposanteTypePion::ReadTransportCrowd )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadTransportCrew
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadTransportCrew( xml::xistream& xis )
{
    if( !tools::ReadTimeAttribute( xis, "man-boarding-time", rNbrHumansLoadedPerTimeStep_ ) || rNbrHumansLoadedPerTimeStep_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "crew: man-boarding-time < 0" );
    if( !tools::ReadTimeAttribute( xis, "man-unloading-time", rNbrHumansUnloadedPerTimeStep_ ) || rNbrHumansUnloadedPerTimeStep_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "crew: man-unloading-time < 0" );
    rNbrHumansLoadedPerTimeStep_   = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansLoadedPerTimeStep_   );
    rNbrHumansUnloadedPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansUnloadedPerTimeStep_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadTransportUnit
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadTransportUnit( xml::xistream& xis )
{
    bCanTransportDestroyed_ = xis.attribute< bool >( "can-transport-destroyed", false );
    rPionTransporterWeightCapacity_ = xis.attribute< double >( "capacity" );
    if( rPionTransporterWeightCapacity_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "unit: capacity <= 0" );
    if( !tools::ReadTimeAttribute( xis, "ton-loading-time", rPionTransporterWeightLoadedPerTimeStep_ )
      || rPionTransporterWeightLoadedPerTimeStep_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "unit: ton-loading-time < 0" );
    if( rPionTransporterWeightLoadedPerTimeStep_ > 0 )
        rPionTransporterWeightLoadedPerTimeStep_  = 1. / MIL_Tools::ConvertSecondsToSim( rPionTransporterWeightLoadedPerTimeStep_ );
    else
        rPionTransporterWeightLoadedPerTimeStep_ = std::numeric_limits< double >::max();
    if( !tools::ReadTimeAttribute( xis, "ton-unloading-time", rPionTransporterWeightUnloadedPerTimeStep_ )
      || rPionTransporterWeightUnloadedPerTimeStep_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "unit: ton-unloading-time < 0" );
    if( rPionTransporterWeightUnloadedPerTimeStep_ > 0 )
        rPionTransporterWeightUnloadedPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rPionTransporterWeightUnloadedPerTimeStep_ );
    else
        rPionTransporterWeightUnloadedPerTimeStep_ = std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadTransportCrowd
// Created: JSR 2011-08-08
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadTransportCrowd( xml::xistream& xis )
{
    nCrowdTransporterCapacity_ = xis.attribute< unsigned int >( "capacity" );
    if( nCrowdTransporterCapacity_ == 0 )
        throw MASA_EXCEPTION( xis.context() + "unit: capacity == 0" );
    if( !tools::ReadTimeAttribute( xis, "man-boarding-time", rCrowdTransporterLoadedPerTimeStep_ )
      || rCrowdTransporterLoadedPerTimeStep_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "unit: man-boarding-time < 0" );
    if( rCrowdTransporterLoadedPerTimeStep_ > 0 )
        rCrowdTransporterLoadedPerTimeStep_   = 1. / MIL_Tools::ConvertSecondsToSim( rCrowdTransporterLoadedPerTimeStep_ );
    else
        rCrowdTransporterLoadedPerTimeStep_ = std::numeric_limits< double >::max();
    if( !tools::ReadTimeAttribute( xis, "man-unloading-time", rCrowdTransporterUnloadedPerTimeStep_ )
      || rCrowdTransporterUnloadedPerTimeStep_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "unit: man-unloading-time < 0" );
    if( rCrowdTransporterUnloadedPerTimeStep_ > 0 )
        rCrowdTransporterUnloadedPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rCrowdTransporterUnloadedPerTimeStep_ );
    else
        rCrowdTransporterUnloadedPerTimeStep_ = std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeObjects
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeObjects( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver )
{
    xis >> xml::start( "objects" )
            >> xml::list( "object", *this, &PHY_ComposanteTypePion::ReadObject, resolver )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadObject
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadObject( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver )
{
    const std::string strType = xis.attribute< std::string >( "type", "" );
    try
    {
        const MIL_ObjectType_ABC& objectType = resolver.FindType( strType );
        if( objectData_.size() <= objectType.GetID() )
            objectData_.resize( objectType.GetID() + 1, 0 );
        const PHY_ComposanteTypeObjectData*& pObject = objectData_[ objectType.GetID() ];
        if( pObject )
            throw MASA_EXCEPTION( xis.context() + "Object type '" + strType + "' already instanciated" );
        pObject = new PHY_ComposanteTypeObjectData( xis );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeConsumptions
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeConsumptions( xml::xistream& xis )
{
    xis >> xml::start( "consumptions" )
            >> xml::list( "consumption", *this, &PHY_ComposanteTypePion::ReadConsumption )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeAviationQuotas
// Created: JSR 2013-10-21
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeAviationQuotas( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "aviation-quotas" )
            >> xml::list( "aviation-quota", [ & ]( xml::xistream& xis )
            {
                E_AviationRange range = ENT_Tr::ConvertToAviationRange( xis.attribute< std::string >( "range" ) );
                xis >> xml::list( "quota", *this, &PHY_ComposanteTypePion::ReadAviationQuotas, aviationResourceQuotas_[ range ] );
            } )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadConsumption
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadConsumption( xml::xistream& xis )
{
    const PHY_ConsumptionType::T_ConsumptionTypeMap& consumptionTypes = PHY_ConsumptionType::GetConsumptionTypes();
    auto it = consumptionTypes.find( xis.attribute< std::string >( "status" ) );
    const PHY_ConsumptionType& consumptionType = *it->second;
    consumptions_[ consumptionType.GetID() ] = new PHY_DotationConsumptions( consumptionType.GetName(), xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadAviationQuotas
// Created: JSR 2013-10-21
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadAviationQuotas( xml::xistream& xis, T_AviationResourceQuota& resourcesQuota )
{
    sAviationResourceQuota resourceQuota;
    unsigned int quota = xis.attribute< unsigned int >( "value" );
    resourceQuota.quota_ = quota * 0.01;
    const std::string category = xis.attribute< std::string >( "category" );
    const PHY_DotationType* type = PHY_DotationType::FindDotationType( category );
    if( type ==  PHY_DotationType::carburant_ )
        resourceQuota.dotationType_ = type;
    else
    {
        const PHY_AmmoDotationClass* ammoDotationClass = PHY_AmmoDotationClass::Find( category );
        if( !ammoDotationClass )
            throw MASA_EXCEPTION( xis.context() + "Unknown aviation resource quota category '" + category + "'" );
        resourceQuota.dotationType_ = PHY_DotationType::munition_;
        resourceQuota.ammoClass_ = ammoDotationClass;
    }
    resourcesQuota.push_back( resourceQuota );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeLogisticMaintenance
// Created: NLD 2004-12-21
// Modified: JVT 2005-02-03
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeLogisticMaintenance( xml::xistream& xis )
{
    xis >> xml::list( "maintenance-functions", *this, &PHY_ComposanteTypePion::ReadMaintenance );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadMaintenance
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadMaintenance( xml::xistream& xis )
{
    rHaulerLoadingTime_    = 0.;
    rHaulerUnloadingTime_  = 0.;
    rHaulerWeightCapacity_ = 0.;

    ntiCapabilities_.clear();

    xis >> xml::list( "towing", *this, &PHY_ComposanteTypePion::ReadTowing )
        >> xml::list( "repairing", *this, &PHY_ComposanteTypePion::ReadRepairing );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadRepairing
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadRepairing( xml::xistream& xis )
{
    const PHY_MaintenanceLevel::T_MaintenanceLevelMap& maintenanceLevels = PHY_MaintenanceLevel::GetMaintenanceLevels();
    std::string maintenanceType;

    xis >> xml::attribute( "category", maintenanceType );

    auto it = maintenanceLevels.find( maintenanceType );
    const PHY_MaintenanceLevel& maintenanceLevel = *it->second;

    sNTICapability ntiCapability( maintenanceLevel );

    const std::string types = xis.attribute< std::string >( "type" );
    ntiCapability.bElectronic_ = boost::contains( types, "EA" );
    ntiCapability.bMobility_   = boost::contains( types, "M" );

    double rTime = 0;
    if( tools::ReadTimeAttribute( xis, "max-reparation-time", rTime ) )
    {
        if( rTime <= 0 )
            throw MASA_EXCEPTION( xis.context() + "category: max-reparation-time <= 0" );
        ntiCapability.nMaxTime_ = (unsigned int)MIL_Tools::ConvertSecondsToSim( rTime );
    }

    if( !ntiCapability.bMobility_ && !ntiCapability.bElectronic_ )
        throw MASA_EXCEPTION( xis.context() + "NTI has not 'EA' nor 'M'" );

    if( !ntiCapabilities_.insert( ntiCapability ).second )
        throw MASA_EXCEPTION( xis.context() + "NTI already defined" );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadTowing
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadTowing( xml::xistream& xis )
{
    xis >> xml::attribute( "capacity", rHaulerWeightCapacity_ );
    tools::ReadTimeAttribute( xis, "loading-time", rHaulerLoadingTime_ );
    tools::ReadTimeAttribute( xis, "unloading-time", rHaulerUnloadingTime_ );

    if( rHaulerWeightCapacity_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "towing: capacity <= 0" );
    if( rHaulerLoadingTime_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "towing: loading-time" );

    rHaulerLoadingTime_   = MIL_Tools::ConvertSecondsToSim( rHaulerLoadingTime_ );
    rHaulerUnloadingTime_ = MIL_Tools::ConvertSecondsToSim( rHaulerUnloadingTime_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadWoundCapabilities
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::ReadWoundCapabilities( xml::xistream& xis, T_WoundCapabilityVector& container, const std::string attributeName ) const
{
    bool bHasCapability = false;
    const std::string strWounds = xis.attribute< std::string >( attributeName, "" );
    std::vector< std::string > wounds;
    boost::split( wounds, strWounds, boost::is_any_of( ", " ) );
    BOOST_FOREACH( const std::string& wound, wounds )
    {
        if( const PHY_HumanWound* pWound = PHY_HumanWound::Find( wound ) )
        {
            bHasCapability = true;
            container[ pWound->GetID() ] = true;
        }
    }
    return bHasCapability;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeLogisticMedical
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
//$$$ A splitter
void PHY_ComposanteTypePion::InitializeLogisticMedical( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( "health-functions" )
            >> xml::list( "caring", *this, &PHY_ComposanteTypePion::ReadCaring )
            >> xml::list( "collecting", *this, &PHY_ComposanteTypePion::ReadCollecting )
            >> xml::list( "relieving", *this, &PHY_ComposanteTypePion::ReadRelieving )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadRelieving
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadRelieving( xml::xistream& xis )
{
    ReadWoundCapabilities( xis, woundEvacuationCapabilities_, "wounded-transport" );

    xis >> xml::optional
            >> xml::attribute( "nbc-transport", bCanEvacuateContaminated_ )
        >> xml::optional
            >> xml::attribute( "reac-mental-transport", bCanEvacuateMentalDiseases_ )
        >> xml::attribute( "capacity", nAmbulanceEvacuationCapacity_ );

    tools::ReadTimeAttribute( xis, "man-loading-time", rNbrHumansLoadedForEvacuationPerTimeStep_ );
    tools::ReadTimeAttribute( xis, "man-unloading-time", rNbrHumansUnloadedForEvacuationPerTimeStep_ );

    if( nAmbulanceEvacuationCapacity_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "relieving: capacity <= 0" );
    if( rNbrHumansLoadedForEvacuationPerTimeStep_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "relieving: man-loading-time <= 0" );
    if( rNbrHumansUnloadedForEvacuationPerTimeStep_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "relieving: man-unloading-time <= 0" );

    rNbrHumansLoadedForEvacuationPerTimeStep_   = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansLoadedForEvacuationPerTimeStep_   );
    rNbrHumansUnloadedForEvacuationPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansUnloadedForEvacuationPerTimeStep_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadCollecting
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadCollecting( xml::xistream& xis )
{
    ReadWoundCapabilities( xis, woundCollectionCapabilities_, "wounded-transport" );

    xis >> xml::optional
            >> xml::attribute( "nbc-transport", bCanCollectContaminated_ )
        >> xml::optional
            >> xml::attribute( "reac-mental-transport", bCanCollectMentalDiseases_ )
        >> xml::attribute( "capacity", nAmbulanceCollectionCapacity_ );

    tools::ReadTimeAttribute( xis, "man-loading-time", rNbrHumansLoadedForCollectionPerTimeStep_ );
    tools::ReadTimeAttribute( xis, "man-unloading-time", rNbrHumansUnloadedForCollectionPerTimeStep_ );

    if( nAmbulanceCollectionCapacity_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "collecting: capacity <= 0" );
    if( rNbrHumansLoadedForCollectionPerTimeStep_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "collecting: man-loading-time <= 0" );
    if( rNbrHumansUnloadedForCollectionPerTimeStep_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "collecting: man-unloading-time <= 0" );

    rNbrHumansLoadedForCollectionPerTimeStep_   = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansLoadedForCollectionPerTimeStep_   );
    rNbrHumansUnloadedForCollectionPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansUnloadedForCollectionPerTimeStep_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadCaring
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadCaring( xml::xistream& xis )
{
    bCanDiagnoseHumans_ = true;
    xis >> xml::optional
            >> xml::attribute( "sorting", bCanSortHumans_ )
        >> xml::optional
            >> xml::attribute( "nbc", bCanHealContaminated_ )
        >> xml::optional
            >> xml::attribute( "psychiatry", bCanHealMentalDiseases_ );
    bCanHealWounds_ = ReadWoundCapabilities( xis, woundHealingCapabilities_, "caring" );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeLogisticSupply
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeLogisticSupply( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( "supply-functions" )
            >> xml::list( "carrying", *this, &PHY_ComposanteTypePion::ReadSupply )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadSupply
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadSupply( xml::xistream& xis )
{
    std::string strNature;
    xis >> xml::attribute( "nature", strNature )
        >> xml::attribute( "min-mass", rStockTransporterMinWeightCapacity_ )
        >> xml::attribute( "max-mass", rStockTransporterMaxWeightCapacity_ )
        >> xml::attribute( "min-volume", rStockTransporterMinVolumeCapacity_ )
        >> xml::attribute( "max-volume", rStockTransporterMaxVolumeCapacity_ );
    pStockTransporterNature_ = PHY_DotationNature::Find( strNature );
    if( !pStockTransporterNature_ )
        throw MASA_EXCEPTION( xis.context() + "Unkown dotation nature" );
    if( rStockTransporterMaxWeightCapacity_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "supply: mass <= 0" );
    if( rStockTransporterMaxVolumeCapacity_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "supply: volume <= 0" );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeLogistic
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeLogistic( xml::xistream& xis )
{
    xis >> xml::list( "logistic-functions", *this, &PHY_ComposanteTypePion::ReadLogistic );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadLogistic
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadLogistic( xml::xistream& xis )
{
    InitializeLogisticMaintenance( xis );
    InitializeLogisticMedical    ( xis );
    InitializeLogisticSupply     ( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InstanciateComposante
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_ComposantePion& PHY_ComposanteTypePion::InstanciateComposante( PHY_RolePion_Composantes& role, unsigned int nNbrHumanInCrew, bool bMajor, bool bLoadable, bool bCanBePartOfConvoy ) const
{
    return *new PHY_ComposantePion( time_, *this, role, nNbrHumanInCrew, bMajor, bLoadable, bCanBePartOfConvoy );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InstanciateWeapons
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InstanciateWeapons( std::back_insert_iterator< std::vector< PHY_Weapon* > > inserter ) const
{
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        inserter = &it->first->InstanciateWeapon( it->second );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InstanciateSensors
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InstanciateSensors( std::back_insert_iterator< std::vector< PHY_Sensor* > > inserter ) const
{
    for( auto it = sensorTypes_.begin(); it != sensorTypes_.end(); ++it )
        inserter = &it->first->InstanciateSensor( it->second );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::HasConstructionSpeeds
// Created: JSR 2012-02-07
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::HasConstructionSpeeds() const
{
    return speeds_.HasConstructionSpeeds();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetConstructionSpeed
// Created: JSR 2012-02-03
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetConstructionSpeed( const TerrainData& data ) const
{
    return speeds_.GetConstructionSpeed( data );
}

namespace
{
    template< typename Objects >
    bool CanDoIt( const Objects& objects, const MIL_ObjectType_ABC& objectType, std::function< bool( const PHY_ComposanteTypeObjectData& ) > f )
    {
        if( objectType.GetID() > objects.size() )
            return false;
        auto objectData = objects[ objectType.GetID() ];
        return objectData && f( *objectData );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanConstruct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanConstruct( const MIL_ObjectType_ABC& object ) const
{
    return CanDoIt( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.CanConstruct(); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanDestroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanDestroy( const MIL_ObjectType_ABC& object ) const
{
    return CanDoIt( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.CanDestroy(); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanMine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanMine( const MIL_ObjectType_ABC& object ) const
{
    return CanDoIt( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.CanMine(); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanExtinguish
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanExtinguish( const MIL_ObjectType_ABC& object ) const
{
    return CanDoIt( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.CanExtinguish(); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanDemine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanDemine( const MIL_ObjectType_ABC& object ) const
{
    return CanDoIt( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.CanDemine(); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanBypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanBypass( const MIL_ObjectType_ABC& object, bool bObjectIsMined ) const
{
    return CanDoIt( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.CanBypass( bObjectIsMined ); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanRemoveFromPath
// Created: BCI 2011-06-20
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanRemoveFromPath( const MIL_ObjectType_ABC& object, bool isObjectMined ) const
{
    return CanDoIt( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.CanRemoveFromPath( isObjectMined ); } );
}

namespace
{
    template< typename Objects >
    double GetTime( const Objects& objects, const MIL_ObjectType_ABC& objectType, std::function< double( const PHY_ComposanteTypeObjectData& ) > f )
    {
        if( objectType.GetID() > objects.size() )
            return std::numeric_limits< double >::max();
        const auto objectData = objects[ objectType.GetID() ];
        if( objectData )
            return f( *objectData );
        return std::numeric_limits< double >::max();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetConstructionTime
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetConstructionTime( const MIL_ObjectType_ABC& object, double rSizeCoef ) const
{
    return GetTime( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.GetConstructionTime( rSizeCoef ); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDestructionTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetDestructionTime( const MIL_ObjectType_ABC& object, double rSizeCoef ) const
{
    return GetTime( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.GetDestructionTime( rSizeCoef ); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMiningTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMiningTime( const MIL_ObjectType_ABC& object ) const
{
    return GetTime( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.GetMiningTime(); } );
}
// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDeminingTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetDeminingTime( const MIL_ObjectType_ABC& object ) const
{
    return GetTime( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.GetDeminingTime(); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetExtinguishingTime
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetExtinguishingTime( const MIL_ObjectType_ABC& object ) const
{
    return GetTime( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.GetExtinguishingTime(); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetBypassTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetBypassTime( const MIL_ObjectType_ABC& object, double rSizeCoef, bool bObjectIsMined ) const
{
    return GetTime( objectData_, object, [&]( const PHY_ComposanteTypeObjectData& data ){ return data.GetBypassTime( rSizeCoef, bObjectIsMined ); } );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxSpeed( const MIL_Object_ABC& object ) const
{
    if( objectData_.size() > object.GetType().GetID() )
    {
        const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetType().GetID() ];
        if( pObjectData )
        {
            double result = pObjectData->GetMaxSpeed( object );
            if( result != std::numeric_limits< double >::max() )
                return result;
        }
    }
    return object().GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDotationConsumptions
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
const PHY_DotationConsumptions* PHY_ComposanteTypePion::GetDotationConsumptions( const PHY_ConsumptionType& consumptionMode ) const
{
    assert( consumptions_.size() > consumptionMode.GetID() );
    return consumptions_[ consumptionMode.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetConsumptionMode
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& PHY_ComposanteTypePion::GetConsumptionMode( const MIL_ObjectType_ABC& object ) const
{
    const BuildableCapacity* capacity = object.GetCapacity< BuildableCapacity >();
    if( objectData_.size() <= object.GetID() )
        return capacity ? capacity->GetDefaultConsumptionMode() : PHY_ConsumptionType::working_;
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    if( pObjectData && pObjectData->GetConsumptionMode() )
        return *pObjectData->GetConsumptionMode();
    return capacity ? capacity->GetDefaultConsumptionMode() : PHY_ConsumptionType::working_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetBreakdownType
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
const PHY_BreakdownType* PHY_ComposanteTypePion::GetBreakdownType( const T_BreakdownTypeProbabilityVector& probasVector ) const
{
    const double rRandomValue = 1. - MIL_Random::rand_ii( 0., 1., MIL_Random::eBreakdowns );
    for( auto it = probasVector.begin(); it != probasVector.end(); ++it )
        if( rRandomValue <= it->rProbabilityBound_ )
            return it->pBreakdownType_;
    assert( false );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDangerosity
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& target, double rDistBtwSourceAndTarget, bool bUseAmmo ) const
{
    double rScore = 0.;
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rScore = std::max( rScore, it->first->GetDangerosity( firer, target, rDistBtwSourceAndTarget, bUseAmmo ) );
    return rScore;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToFireOn
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH, const PHY_DotationCategory* dotation ) const
{
    double rRange = 0;
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rRange = std::max( rRange, it->first->GetMaxRangeToFireOn( firer, targetComposanteType, rWantedPH, dotation ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMinRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    double rRange = std::numeric_limits< double >::max();
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rRange = std::min( rRange, it->first->GetMinRangeToFireOn( firer, targetComposanteType, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    double rRange = 0;
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rRange = std::max( rRange, it->first->GetMaxRangeToFireOnWithPosture( firer, target, targetComposanteType, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMinRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    double rRange = std::numeric_limits< double >::max();
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rRange = std::min( rRange, it->first->GetMinRangeToFireOnWithPosture( firer, target, targetComposanteType, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxRangeToIndirectFire( const MIL_Agent_ABC& firer, const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const
{
    double rRange = -1.;
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
    {
        if( it->first->GetDotationCategory() == dotationCategory )
            rRange = std::max( rRange, it->first->GetMaxRangeToIndirectFire( firer, bCheckDotationsAvailability ) );
    }
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToIndirectFire
// Created: GGE 2012-07-25
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxRangeToIndirectFire( const MIL_Agent_ABC& firer ) const
{
    double rRange = -1.;
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        if( it->first->GetDotationCategory().CanBeUsedForIndirectFire() )
            rRange = std::max( rRange, it->first->GetMaxRangeToIndirectFire( firer, false ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMinRangeToIndirectFire( const MIL_Agent_ABC& firer, const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const
{
    double rRange = std::numeric_limits< double >::max();

    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
    {
        if( it->first->GetDotationCategory() == dotationCategory )
            rRange = std::min( rRange, it->first->GetMinRangeToIndirectFire( firer, bCheckDotationsAvailability ) );
    }
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToFire
// Created: DDA 2010-05-03
// -----------------------------------------------------------------------------
 double PHY_ComposanteTypePion::GetMaxRangeToFire( const MIL_Agent_ABC&  pion, double rWantedPH ) const
 {
    double rRange = 0.;
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rRange = std::max( rRange, it->first->GetMaxRangeToFire( pion, rWantedPH ) );
    return rRange;
 }

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToDirectFire
// Created: MCO 2012-11-23
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxRangeToDirectFire() const
{
    double rRange = 0.;
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rRange = std::max( rRange, it->first->GetMaxRangeToDirectFire() );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::HasWeaponTypes
// Created: MMC 2013-06-03
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::HasWeaponTypes() const
{
    return !weaponTypes_.empty();
}

// =============================================================================
// LOGISTIC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanRepair
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanRepair( const PHY_Breakdown& breakdown ) const
{
    for( auto it = ntiCapabilities_.begin(); it != ntiCapabilities_.end(); ++it )
        if( it->CanRepair( breakdown ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanHaul1
// Created: NLD 2005-03-29
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanHaul1( const PHY_ComposanteTypePion& type ) const
{
    return ( rHaulerWeightCapacity_ - type.GetWeight() ) >= 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanEvacuateCasualty
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanEvacuateCasualty( const Human_ABC& human ) const
{
    if( !CanEvacuateCasualties() )
        return false;

    if( human.IsContaminated() )
        return bCanEvacuateContaminated_;

    if( human.IsWounded() )
        return woundEvacuationCapabilities_[ human.GetWound().GetID() ];

    if( human.IsMentalDiseased() )
        return bCanEvacuateMentalDiseases_;

    assert( false );
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanCollectCasualty
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanCollectCasualty( const Human_ABC& human ) const
{
    if( !CanCollectCasualties() )
        return false;

    if( human.IsContaminated() )
        return bCanCollectContaminated_;

    if( human.IsWounded() )
        return woundCollectionCapabilities_[ human.GetWound().GetID() ];

    if( human.IsMentalDiseased() )
        return bCanCollectMentalDiseases_;

    assert( false );
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanHealHumans
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanHealHumans() const
{
    return bCanHealContaminated_ || bCanHealMentalDiseases_ || bCanHealWounds_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanHealHuman
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanHealHuman( const Human_ABC& human ) const
{
    if( human.IsContaminated() )
        return bCanHealContaminated_;

    if( human.IsWounded() )
        return woundHealingCapabilities_[ human.GetWound().GetID() ];

    if( human.IsMentalDiseased() )
        return bCanHealMentalDiseases_;

    assert( false );
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_ComposanteTypePion::GetHealingTime( const Human_ABC& human ) const
{
    if( human.IsContaminated() )
    {
        assert( bCanHealContaminated_  );
        return PHY_HumanWound::GetContaminatedHealingTime();
    }

    if( human.IsWounded() )
    {
        assert( woundHealingCapabilities_[ human.GetWound().GetID() ] );
        return human.GetWound().GetHealingTime();
    }

    if( human.IsMentalDiseased() )
    {
        assert( bCanHealMentalDiseases_ );
        return PHY_HumanWound::GetMentalDiseaseHealingTime();
    }
    assert( false );
    return std::numeric_limits< unsigned int >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::Heal
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_ComposanteTypePion::Heal( Human_ABC& human ) const
{
    if( human.IsContaminated() )
    {
        assert( bCanHealContaminated_ );
        human.HealContamination();
        return PHY_HumanWound::GetContaminatedRestingTime();
    }

    if( human.IsWounded() )
    {
        assert( woundHealingCapabilities_[ human.GetWound().GetID() ] );
        const unsigned int nRestingTime = human.GetWound().GetRestingTime();
        human.HealWound();
        return nRestingTime;
    }

    if( human.IsMentalDiseased() )
    {
        assert( bCanHealMentalDiseases_ );
        human.HealMentalDisease();
        return PHY_HumanWound::GetMentalDiseaseRestingTime();
    }
    assert( false );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanTransportStock
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanTransportStock( const PHY_DotationCategory& dotationCategory ) const
{
    return rStockTransporterMaxWeightCapacity_ > 0 &&
        rStockTransporterMaxVolumeCapacity_ > 0 &&
        pStockTransporterNature_ == &dotationCategory.GetNature();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::UseAmmunition
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::UseAmmunition( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    for( auto it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
        (*it)->UseAmmunition( category, pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetPHModifier
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetPHModifier( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    double result = 1.;
    for( auto it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
        result *= (*it)->GetPHModifier( category, pion );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CounterIndirectFire
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CounterIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    for( auto it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
        if( (*it)->CounterIndirectFire( category, pion ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::DestroyIndirectFire
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::DestroyIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    for( auto it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
        if( (*it)->DestroyIndirectFire( category, pion ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetAttritionIndexComposante
// Created: DDA 2010-04-22
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetAttritionIndexComposante( const PHY_MaterialCompositionType& material ) const
{
    double rRange = 0.;
    for( auto it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rRange = std::max( rRange, it->first->GetDotationCategory().GetUrbanAttritionScore( material ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanHaveBreakdown
// Created: LDC 2012-08-17
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanHaveBreakdown() const
{
    return !randomBreakdownTypeProbabilities_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanHaveBreakdown
// Created: LDC 2012-08-17
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanHaveBreakdown( const PHY_BreakdownType* type ) const
{
    for( auto it = attritionBreakdownTypeProbabilities_.begin(); it != attritionBreakdownTypeProbabilities_.end();++it )
        if( it->pBreakdownType_ == type )
            return true;
    for( auto it = randomBreakdownTypeProbabilities_.begin(); it != randomBreakdownTypeProbabilities_.end();++it )
        if( it->pBreakdownType_ == type )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::Find
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
const PHY_ComposanteTypePion* PHY_ComposanteTypePion::Find( const std::string& strName )
{
    auto it = composantesTypes_.find( strName );
    if( it == composantesTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::Find
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
const PHY_ComposanteTypePion* PHY_ComposanteTypePion::Find( uint32_t id )
{
    for( auto it = composantesTypes_.begin(); it != composantesTypes_.end(); ++it )
    {
        const PHY_ComposanteTypePion& composanteType = *it->second;
        if( composanteType.GetMosID().id() == id )
            return &composanteType;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDotationCapacities
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
const PHY_DotationCapacities& PHY_ComposanteTypePion::GetDotationCapacities() const
{
    return dotationCapacities_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxSpeed( const TerrainData& data ) const
{
    return speeds_.GetMaxSpeed( data );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxSpeed() const
{
    return speeds_.GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanTransportHumans
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanTransportHumans() const
{
    return rNbrHumansLoadedPerTimeStep_ != 0. && rNbrHumansUnloadedPerTimeStep_ != 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansLoadedPerTimeStep
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetNbrHumansLoadedPerTimeStep() const
{
    return rNbrHumansLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansUnloadedPerTimeStep
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetNbrHumansUnloadedPerTimeStep() const
{
    return rNbrHumansUnloadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanTransportPion
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanTransportPion() const
{
    return rPionTransporterWeightCapacity_ > 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetPionTransporterWeightCapacity
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetPionTransporterWeightCapacity() const
{
    return rPionTransporterWeightCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetPionTransporterWeightLoadedPerTimeStep
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetPionTransporterWeightLoadedPerTimeStep() const
{
    return rPionTransporterWeightLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetPionTransporterWeightUnloadedPerTimeStep
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetPionTransporterWeightUnloadedPerTimeStep() const
{
    return rPionTransporterWeightUnloadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanTransportDestroyed
// Created: JSR 2014-02-07
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanTransportDestroyed() const
{
    return bCanTransportDestroyed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanTransportCrowd
// Created: JSR 2011-08-08
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanTransportCrowd() const
{
    return nCrowdTransporterCapacity_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetCrowdTransporterCapacity
// Created: JSR 2011-08-08
// -----------------------------------------------------------------------------
unsigned int PHY_ComposanteTypePion::GetCrowdTransporterCapacity() const
{
    return nCrowdTransporterCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetCrowdTransporterLoadedPerTimeStep
// Created: JSR 2011-08-08
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetCrowdTransporterLoadedPerTimeStep() const
{
    return rCrowdTransporterLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetCrowdTransporterUnloadedPerTimeStep
// Created: JSR 2011-08-08
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetCrowdTransporterUnloadedPerTimeStep() const
{
    return rCrowdTransporterUnloadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetSensorRotationAngle_
// Created: JVT 2004-10-20
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetSensorRotationAngle() const
{
    return rSensorRotationAngle_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetWeight
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetWeight() const
{
    return rWeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxSlope() const
{
    return rMaxSlope_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetSlopeDeceleration
// Created: JSR 2013-11-07
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetSlopeDeceleration() const
{
    return rSlopeDeceleration_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::operator==
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::operator==( const PHY_ComposanteTypePion& rhs ) const
{
    return this == &rhs;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::operator!=
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::operator!=( const PHY_ComposanteTypePion& rhs ) const
{
    return this != &rhs;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::sNTICapability constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
PHY_ComposanteTypePion::sNTICapability::sNTICapability()
    : pMaintenanceLevel_( 0 )
    , bMobility_        ( false )
    , bElectronic_      ( false )
    , nMaxTime_         ( std::numeric_limits< unsigned int >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::sNTICapability constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
PHY_ComposanteTypePion::sNTICapability::sNTICapability( const PHY_MaintenanceLevel& maintenanceLevel )
    : pMaintenanceLevel_( &maintenanceLevel )
    , bMobility_        ( false  )
    , bElectronic_      ( false )
    , nMaxTime_         ( std::numeric_limits< unsigned int >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::sNTICapability::operator<
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::sNTICapability::operator < ( const sNTICapability& rhs ) const
{
    return pMaintenanceLevel_ < rhs.pMaintenanceLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanRepair
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanRepair() const
{
    return !ntiCapabilities_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanHaul
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanHaul() const
{
    return rHaulerWeightCapacity_ > 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetHaulerWeightCapacity
// Created: NLD 2005-03-29
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetHaulerWeightCapacity() const
{
    return rHaulerWeightCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetHaulerLoadingTime
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetHaulerLoadingTime() const
{
    return rHaulerLoadingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetHaulerUnloadingTime
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetHaulerUnloadingTime() const
{
    return rHaulerUnloadingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanDiagnoseHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanDiagnoseHumans() const
{
    return bCanDiagnoseHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanSortHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanSortHumans() const
{
    return bCanSortHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanEvacuateCasualties
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanEvacuateCasualties() const
{
    return nAmbulanceEvacuationCapacity_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanCollectCasualties
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanCollectCasualties() const
{
    return nAmbulanceCollectionCapacity_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetAmbulanceCollectionCapacity
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
unsigned int PHY_ComposanteTypePion::GetAmbulanceCollectionCapacity() const
{
    return nAmbulanceCollectionCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansLoadedForCollectionPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetNbrHumansLoadedForCollectionPerTimeStep() const
{
    return rNbrHumansLoadedForCollectionPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansUnloadedForCollectionPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetNbrHumansUnloadedForCollectionPerTimeStep() const
{
    return rNbrHumansUnloadedForCollectionPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetAmbulanceEvacuationCapacity
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
unsigned int PHY_ComposanteTypePion::GetAmbulanceEvacuationCapacity() const
{
    return nAmbulanceEvacuationCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansLoadedForEvacuationPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetNbrHumansLoadedForEvacuationPerTimeStep() const
{
    return rNbrHumansLoadedForEvacuationPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansUnloadedForEvacuationPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetNbrHumansUnloadedForEvacuationPerTimeStep() const
{
    return rNbrHumansUnloadedForEvacuationPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetStockTransporterCapacity
// Created: NLD 2005-07-18
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::GetStockTransporterCapacity( double& rWeightMax, double& rVolumeMax ) const
{
    rWeightMax = rStockTransporterMaxWeightCapacity_;
    rVolumeMax = rStockTransporterMaxVolumeCapacity_;
}

std::pair< double, double > PHY_ComposanteTypePion::GetStockTransporterCapacity() const
{
    return std::make_pair( rStockTransporterMinWeightCapacity_, rStockTransporterMinVolumeCapacity_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetStockTransporterNature
// Created: NLD 2006-03-27
// -----------------------------------------------------------------------------
const PHY_DotationNature* PHY_ComposanteTypePion::GetStockTransporterNature() const
{
    return pStockTransporterNature_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetAviationResourceQuota
// Created: JSR 2013-10-21
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetAviationResourceQuota( E_AviationRange aviationRange, const PHY_DotationType* dotationType, const PHY_AmmoDotationClass* ammoClass ) const
{
    auto itQuotas = aviationResourceQuotas_.find( aviationRange );
    if( itQuotas != aviationResourceQuotas_.end() )
    {
        for( auto it = itQuotas->second.begin(); it != itQuotas->second.end(); ++it )
        {
            if( it->dotationType_ == dotationType && it->ammoClass_ == ammoClass )
                return it->quota_;
        }
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::sBreakdownTypeProbability constructor
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
PHY_ComposanteTypePion::sBreakdownTypeProbability::sBreakdownTypeProbability( const PHY_BreakdownType& breakdownType, double rProbabilityBound )
    : pBreakdownType_   ( &breakdownType )
    , rProbabilityBound_( rProbabilityBound )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetRandomBreakdownType
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
const PHY_BreakdownType* PHY_ComposanteTypePion::GetRandomBreakdownType() const
{
    return GetBreakdownType( randomBreakdownTypeProbabilities_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetAttritionBreakdownType
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
const PHY_BreakdownType* PHY_ComposanteTypePion::GetAttritionBreakdownType() const
{
    return GetBreakdownType( attritionBreakdownTypeProbabilities_ );
}

void boost::archive::save( MIL_CheckPointOutArchive& a, const PHY_ComposanteTypePion* t )
{
    boost::archive::save( a, static_cast< const PHY_ComposanteType_ABC* >( t ) );
}

void boost::archive::load( MIL_CheckPointInArchive& a, const PHY_ComposanteTypePion*& t )
{
    boost::archive::load( a, (const PHY_ComposanteType_ABC*&)t );
}
