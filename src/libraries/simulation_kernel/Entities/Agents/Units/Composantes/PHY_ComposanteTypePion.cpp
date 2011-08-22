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

#include <boost/algorithm/string.hpp>
#include "PHY_ComposantePion.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Composantes/PHY_ActiveProtection.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationConsumptions.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Units/Humans/PHY_HumanProtection.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Logistic/PHY_MaintenanceLevel.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Radars/PHY_RadarType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/BuildableCapacity.h"
#include "Entities/Objects/BypassableCapacity.h"
#include "Entities/Objects/MobilityCapacity.h"
#include "PHY_ComposanteTypeObjectData.h"
#include "protocol/Protocol.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

PHY_ComposanteTypePion::T_ComposanteTypeMap PHY_ComposanteTypePion::composantesTypes_;

struct PHY_ComposanteTypePion::LoadingWrapper
{
    void ReadElement( xml::xistream& xis, const MIL_Time_ABC& time )
    {
        PHY_ComposanteTypePion::ReadElement( xis , time);
    }
};

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
void PHY_ComposanteTypePion::Initialize( const MIL_Time_ABC& time, xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing composante types" );
    LoadingWrapper loader;
    xis >> xml::start( "equipments" )
            >> xml::list( "equipment", loader, &LoadingWrapper::ReadElement, time )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadElement
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadElement( xml::xistream& xis, const MIL_Time_ABC& time )
{
    std::set< unsigned int > ids_;
    std::string strComposanteType;
    xis >> xml::attribute( "name", strComposanteType );
    PHY_ComposanteTypePion*& pComposanteType = composantesTypes_[ strComposanteType ];
    if( pComposanteType )
        xis.error( "Composante type '" + strComposanteType + "' already registered" );
    pComposanteType = new PHY_ComposanteTypePion( time, strComposanteType, xis );

    if( !ids_.insert( pComposanteType->GetMosID().id() ).second )
        xis.error( "Composante ID already used" );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::Terminate()
{
    for( CIT_ComposanteTypeMap itComposante = composantesTypes_.begin(); itComposante != composantesTypes_.end(); ++itComposante )
        delete itComposante->second;
    composantesTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion constructor
// Created: NLD 2004-08-04
// Modified: JVT 2004-10-20
// -----------------------------------------------------------------------------
PHY_ComposanteTypePion::PHY_ComposanteTypePion( const MIL_Time_ABC& time, const std::string& strName, xml::xistream& xis )
    : PHY_ComposanteType_ABC                     ( strName, xis )
    , time_                                      ( time )
    , speeds_                                    ( xis, time.GetTickDuration() )
    , rMaxSlope_                                 ( 1. )
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
    , nCrowdTransporterCapacity_                 ( 0 )
    , rCrowdTransporterLoadedPerTimeStep_        ( 0. )
    , rCrowdTransporterUnloadedPerTimeStep_      ( 0. )
    , rHaulerWeightCapacity_                     ( 0. )
    , rHaulerLoadingTime_                        ( 0. )
    , rHaulerUnloadingTime_                      ( 0. )
    , woundCollectionCapabilities_               ( PHY_HumanWound::GetHumanWounds().size(), false )
    , bCanCollectMentalDiseases_                 ( false )
    , bCanCollectContaminated_                   ( false )
    , nAmbulanceCollectionCapacity_              ( 0 )
    , rNbrHumansLoadedForCollectionPerTimeStep_  ( 0. )
    , rNbrHumansUnloadedForCollectionPerTimeStep_( 0. )
    , woundEvacuationCapabilities_               ( PHY_HumanWound::GetHumanWounds().size(), false )
    , bCanEvacuateMentalDiseases_                ( false )
    , bCanEvacuateContaminated_                  ( false )
    , nAmbulanceEvacuationCapacity_              ( 0 )
    , rNbrHumansLoadedForEvacuationPerTimeStep_  ( 0. )
    , rNbrHumansUnloadedForEvacuationPerTimeStep_( 0. )
    , woundHealingCapabilities_                  ( PHY_HumanWound::GetHumanWounds().size(), false )
    , bCanHealMentalDiseases_                    ( false )
    , bCanHealContaminated_                      ( false )
    , bCanSortHumans_                            ( false )
    , bCanDiagnoseHumans_                        ( false )
    , bCanHealWounds_                            ( false )
    , pStockTransporterNature_                   ( 0 )
    , rStockTransporterWeightCapacity_           ( 0. )
    , rStockTransporterVolumeCapacity_           ( 0. )
{
    xis >> xml::optional
            >> xml::attribute( "max-slope", rMaxSlope_ )
        >> xml::attribute( "weight", rWeight_ );
    if( rMaxSlope_ < 0 || rMaxSlope_ > 1 )
        xis.error( "element: max-slope not in [0..1]" );
    if( rWeight_ <= 0 )
        xis.error( "element: weight <= 0" );
    InitializeHumanProtections( xis );
    InitializeWeapons         ( xis );
    InitializeSensors         ( xis );
    InitializeRadars          ( xis );
    InitializeTransport       ( xis );
    InitializeObjects         ( xis );
    InitializeConsumptions    ( xis );
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
    for( CIT_ObjectDataVector itObjectData = objectData_.begin(); itObjectData != objectData_.end(); ++itObjectData )
        delete *itObjectData;
    objectData_.clear();
    for( CIT_ConsumptionVector itConsumption = consumptions_.begin(); itConsumption != consumptions_.end(); ++itConsumption )
        delete *itConsumption;
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
        xis.error( "Total probability of random breakdowns is less than 100%" );
    if( attritionBreakdownTypeProbabilities_.empty()
        || std::fabs( 1. - attritionBreakdownTypeProbabilities_.back().rProbabilityBound_  ) > 0.01 )
        xis.error( "Total probability of attrition breakdowns is less than 100%" );
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
        xis.error( "Unknown breakdown type '" + strBuf + "'" );
    double rPercentage;
    xis >> xml::attribute( "percentage", rPercentage );
    if( rPercentage < 0 || rPercentage > 100 )
        xis.error( "random breakdown: percentage not in [0..100]" );
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
        xis.error( "Unknown breakdown type '" + strBuf + "'" );
    double rPercentage;
    xis >> xml::attribute( "percentage", rPercentage );
    if( rPercentage < 0 || rPercentage > 100 )
        xis.error( "attrition breakdown: percentage not in [0..100]" );
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
        xis.error( "Unknown weapon type (" + strLauncher + ", " + strAmmunition + ")" );
    if( weaponTypes_.find( pWeaponType ) != weaponTypes_.end() )
        xis.error( "Weapon type (" + strLauncher + ", " + strAmmunition + ") already initialized" );
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
        xis.error( "Unknow active protection which name is " + strProtectionName );
    activeProtections_.push_back( pProtection );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeWeapons
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeHumanProtections( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "human-protections" )
            >> xml::list( "human-protection", *this, &PHY_ComposanteTypePion::ReadHumanProtection )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadWeaponSystem
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadHumanProtection( xml::xistream& xis )
{
    std::string strProtectionName;
    xis >> xml::attribute( "name", strProtectionName );
    const PHY_HumanProtection* pHumanProtection = PHY_HumanProtection::Find( strProtectionName );
    if( !pHumanProtection )
        xis.error( "Unknow human protection which name is " + strProtectionName );
    humanProtections_.push_back( pHumanProtection );
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
            xis.error( "Unknown sensor type '" + strSensor + "'" );
        if( sensorTypes_.find( pSensorType ) != sensorTypes_.end() )
            xis.error( "Sensor type '" + strSensor + "' already defined" );
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
        xis.error( "Unknown radar type" );
    if( radarTypes_.find( pRadarType ) != radarTypes_.end() )
        xis.error( "Radar type already defined" );
    radarTypes_.insert( pRadarType );
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
        xis.error( "crew: man-boarding-time < 0" );
    if( !tools::ReadTimeAttribute( xis, "man-unloading-time", rNbrHumansUnloadedPerTimeStep_ ) || rNbrHumansUnloadedPerTimeStep_ <= 0 )
        xis.error( "crew: man-unloading-time < 0" );
    rNbrHumansLoadedPerTimeStep_   = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansLoadedPerTimeStep_   );
    rNbrHumansUnloadedPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansUnloadedPerTimeStep_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadTransportUnit
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadTransportUnit( xml::xistream& xis )
{
    rPionTransporterWeightCapacity_ = xis.attribute< double >( "capacity" );
    if( rPionTransporterWeightCapacity_ <= 0 )
        xis.error( "unit: capacity <= 0" );
    if( !tools::ReadTimeAttribute( xis, "ton-loading-time", rPionTransporterWeightLoadedPerTimeStep_ )
      || rPionTransporterWeightLoadedPerTimeStep_ < 0 )
        xis.error( "unit: ton-loading-time < 0" );
    if( rPionTransporterWeightLoadedPerTimeStep_ > 0 )
        rPionTransporterWeightLoadedPerTimeStep_  = 1. / MIL_Tools::ConvertSecondsToSim( rPionTransporterWeightLoadedPerTimeStep_ );
    else
        rPionTransporterWeightLoadedPerTimeStep_ = std::numeric_limits< double >::max();
    if( !tools::ReadTimeAttribute( xis, "ton-unloading-time", rPionTransporterWeightUnloadedPerTimeStep_ )
      || rPionTransporterWeightUnloadedPerTimeStep_ < 0 )
        xis.error( "unit: ton-unloading-time < 0" );
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
        xis.error( "unit: capacity == 0" );
    if( !tools::ReadTimeAttribute( xis, "man-boarding-time", rCrowdTransporterLoadedPerTimeStep_ )
      || rCrowdTransporterLoadedPerTimeStep_ < 0 )
        xis.error( "unit: man-boarding-time < 0" );
    if( rCrowdTransporterLoadedPerTimeStep_ > 0 )
        rCrowdTransporterLoadedPerTimeStep_   = 1. / MIL_Tools::ConvertSecondsToSim( rCrowdTransporterLoadedPerTimeStep_ );
    else
        rCrowdTransporterLoadedPerTimeStep_ = std::numeric_limits< double >::max();
    if( !tools::ReadTimeAttribute( xis, "man-unloading-time", rCrowdTransporterUnloadedPerTimeStep_ )
      || rCrowdTransporterUnloadedPerTimeStep_ < 0 )
        xis.error( "unit: man-unloading-time < 0" );
    if( rCrowdTransporterUnloadedPerTimeStep_ > 0 )
        rCrowdTransporterUnloadedPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rCrowdTransporterUnloadedPerTimeStep_ );
    else
        rCrowdTransporterUnloadedPerTimeStep_ = std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeObjects
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeObjects( xml::xistream& xis )
{
    xis >> xml::start( "objects" )
            >> xml::list( "object", *this, &PHY_ComposanteTypePion::ReadObject )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ReadObject
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadObject( xml::xistream& xis )
{
    std::string strType( xml::attribute( xis, "type", std::string() ) );

    try
    {
        const MIL_ObjectType_ABC& objectType = MIL_ObjectFactory::FindType( strType );
        if( objectData_.size() <= objectType.GetID() )
            objectData_.resize( objectType.GetID() + 1, 0 );
        const PHY_ComposanteTypeObjectData*& pObject = objectData_[ objectType.GetID() ];
        if( pObject )
            xis.error( "Object type '" + strType + "' already instanciated" );
        pObject = new PHY_ComposanteTypeObjectData( xis );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
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
// Name: PHY_ComposanteTypePion::ReadConsumption
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::ReadConsumption( xml::xistream& xis )
{
    const PHY_ConsumptionType::T_ConsumptionTypeMap& consumptionTypes = PHY_ConsumptionType::GetConsumptionTypes();
    std::string typeName;
    xis >> xml::attribute( "status", typeName );

    PHY_ConsumptionType::CIT_ConsumptionTypeMap it = consumptionTypes.find( typeName );
    const PHY_ConsumptionType& consumptionType = *it->second;
    consumptions_[ consumptionType.GetID() ] = new PHY_DotationConsumptions( consumptionType.GetName(), xis );
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

    PHY_MaintenanceLevel::CIT_MaintenanceLevelMap it = maintenanceLevels.find( maintenanceType );
    const PHY_MaintenanceLevel& maintenanceLevel = *it->second;

    sNTICapability ntiCapability( maintenanceLevel );

    std::string types;
    xis >> xml::attribute( "type", types );
    std::stringstream stream( types );
    std::string type;
    while( std::getline( stream, type, ',' ) )
    {
        boost::trim( type );
        if( type == "EA" )
            ntiCapability.bElectronic_ = true;
        if( type == "M" )
            ntiCapability.bMobility_   = true;
    }

    double rTime = 0;
    if( tools::ReadTimeAttribute( xis, "max-reparation-time", rTime ) )
    {
        if( rTime <= 0 )
            xis.error( "category: max-reparation-time <= 0" );
        ntiCapability.nMaxTime_ = (unsigned int)MIL_Tools::ConvertSecondsToSim( rTime );
    }

    if( !ntiCapability.bMobility_ && !ntiCapability.bElectronic_ )
        xis.error( "NTI has not 'EA' nor 'M'" );

    if( !ntiCapabilities_.insert( ntiCapability ).second )
        xis.error( "NTI already defined" );
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
        xis.error( "towing: capacity <= 0" );
    if( rHaulerLoadingTime_ < 0 )
        xis.error( "towing: loading-time" );

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

    std::string strWounds;
    xis >> xml::optional >> xml::attribute( attributeName, strWounds );
    std::stringstream stream( strWounds );
    std::string wound;
    while( std::getline( stream, wound, ',' ) )
    {
        boost::trim( wound );
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
        xis.error( "relieving: capacity <= 0" );
    if( rNbrHumansLoadedForEvacuationPerTimeStep_ <= 0 )
        xis.error( "relieving: man-loading-time <= 0" );
    if( rNbrHumansUnloadedForEvacuationPerTimeStep_ <= 0 )
        xis.error( "relieving: man-unloading-time <= 0" );

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
        xis.error( "collecting: capacity <= 0" );
    if( rNbrHumansLoadedForCollectionPerTimeStep_ <= 0 )
        xis.error( "collecting: man-loading-time <= 0" );
    if( rNbrHumansUnloadedForCollectionPerTimeStep_ <= 0 )
        xis.error( "collecting: man-unloading-time <= 0" );

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
        >> xml::attribute( "mass", rStockTransporterWeightCapacity_ )
        >> xml::attribute( "volume", rStockTransporterVolumeCapacity_ );

    pStockTransporterNature_ = PHY_DotationNature::Find( strNature );
    if( !pStockTransporterNature_ )
        xis.error( "Unkown dotation nature" );
    if( rStockTransporterWeightCapacity_ <= 0 )
        xis.error( "supply: mass <= 0" );
    if( rStockTransporterVolumeCapacity_ <= 0 )
        xis.error( "supply: volume <= 0" );
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


// =============================================================================
// INSTANCIATION
// =============================================================================

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
void PHY_ComposanteTypePion::InstanciateWeapons( std::back_insert_iterator < std::vector< PHY_Weapon* > > inserter ) const
{
    for( CIT_WeaponTypeMap it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
    {
        const PHY_WeaponType& weaponType = *it->first;
        const bool             bMajor     =  it->second;
        inserter = &weaponType.InstanciateWeapon( bMajor );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InstanciateSensors
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InstanciateSensors( std::back_insert_iterator < std::vector< PHY_Sensor* > > inserter ) const
{
    for( CIT_SensorTypeMap it = sensorTypes_.begin(); it != sensorTypes_.end(); ++it )
    {
        const PHY_SensorType&  sensorType = *it->first;
        const double         rHeight    =  it->second;
        inserter = &sensorType.InstanciateSensor( rHeight );
    }
}

void PHY_ComposanteTypePion::InstanciateProtections( std::back_insert_iterator < std::vector< PHY_HumanProtection* > > inserter ) const
{
    for( CIT_ListOfHumanProtection it = humanProtections_.begin(); it != humanProtections_.end(); ++it )
    {
        const PHY_HumanProtection&  humanProtection = *(*it);
        inserter = &humanProtection.InstanciateHumanProtection();
    }
}

// =============================================================================
// OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanConstruct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanConstruct( const MIL_ObjectType_ABC& object ) const
{
    if( objectData_.size() <= object.GetID() )
        return false;
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    return pObjectData && pObjectData->CanConstruct();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanDestroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanDestroy( const MIL_ObjectType_ABC& object ) const
{
    if( objectData_.size() <= object.GetID() )
        return false;
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    return pObjectData && pObjectData->CanDestroy();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanMine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanMine( const MIL_ObjectType_ABC& object ) const
{
    if( objectData_.size() <= object.GetID() )
        return false;
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    return pObjectData && pObjectData->CanMine();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanExtinguish
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanExtinguish( const MIL_ObjectType_ABC& object ) const
{
    if( objectData_.size() <= object.GetID() )
        return false;
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    return pObjectData && pObjectData->CanExtinguish();
}


// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanDemine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanDemine( const MIL_ObjectType_ABC& object ) const
{
    if( objectData_.size() <= object.GetID() )
        return false;
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    return pObjectData && pObjectData->CanDemine();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanBypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanBypass( const MIL_ObjectType_ABC& object, bool bObjectIsMined ) const
{
    if( objectData_.size() <= object.GetID() )
        return false;
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    return pObjectData && pObjectData->CanBypass( bObjectIsMined );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanRemoveFromPath
// Created: BCI 2011-06-20
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanRemoveFromPath( const MIL_ObjectType_ABC& object, bool isObjectMined ) const
{
    if( objectData_.size() <= object.GetID() )
        return false;
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    return pObjectData && pObjectData->CanRemoveFromPath( isObjectMined );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetConstructionTime
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetConstructionTime( const MIL_ObjectType_ABC& object, double rSizeCoef ) const
{
    if( objectData_.size() <= object.GetID() )
        return std::numeric_limits< double >::max();
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    if( pObjectData )
        return pObjectData->GetConstructionTime( rSizeCoef );
    return std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDestructionTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetDestructionTime( const MIL_ObjectType_ABC& object, double rSizeCoef ) const
{
    if( objectData_.size() <= object.GetID() )
        return std::numeric_limits< double >::max();
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    if( pObjectData )
        return pObjectData->GetDestructionTime( rSizeCoef );
    return std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMiningTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMiningTime( const MIL_ObjectType_ABC& object ) const
{
    if( objectData_.size() <= object.GetID() )
        return std::numeric_limits< double >::max();
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    if( pObjectData )
        return pObjectData->GetMiningTime();
    return std::numeric_limits< double >::max();
}
// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDeminingTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetDeminingTime( const MIL_ObjectType_ABC& object ) const
{
    if( objectData_.size() <= object.GetID() )
        return std::numeric_limits< double >::max();
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    if( pObjectData )
        return pObjectData->GetDeminingTime();
    return std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetExtinguishingTime
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetExtinguishingTime( const MIL_ObjectType_ABC& object ) const
{
    if( objectData_.size() <= object.GetID() )
        return std::numeric_limits< double >::max();
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    if( pObjectData )
        return pObjectData->GetExtinguishingTime();
    return std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetBypassTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetBypassTime( const MIL_ObjectType_ABC& object, double rSizeCoef, bool bObjectIsMined ) const
{
    if( objectData_.size() <= object.GetID() )
        return std::numeric_limits< double >::max();
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetID() ];
    if( pObjectData )
        return pObjectData->GetBypassTime( rSizeCoef, bObjectIsMined );
    return std::numeric_limits< double >::max();
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
            return pObjectData->GetMaxSpeed( object );
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

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetBreakdownType
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
const PHY_BreakdownType& PHY_ComposanteTypePion::GetBreakdownType( const T_BreakdownTypeProbabilityVector& probasVector ) const
{
    const double rRandomValue = 1. - MIL_Random::rand_ii( 0., 1., MIL_Random::eBreakdowns );

    for ( CIT_BreakdownTypeProbabilityVector it = probasVector.begin(); it != probasVector.end(); ++it )
    {
        if( rRandomValue <= it->rProbabilityBound_ )
            return *it->pBreakdownType_;
    }
    assert( false );
    return *(PHY_BreakdownType*)0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDangerosity
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& target, double rDistBtwSourceAndTarget ) const
{
    double rScore = 0.;
    for( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rScore = std::max( rScore, itWeapon->first->GetDangerosity( firer, target, rDistBtwSourceAndTarget ) );
    return rScore;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToFireOn
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH, const PHY_DotationCategory* dotation ) const
{
    double rRange = 0;
    for( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rRange = std::max( rRange, itWeapon->first->GetMaxRangeToFireOn( firer, targetComposanteType, rWantedPH, dotation ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMinRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    double rRange = std::numeric_limits< double >::max();
    for ( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rRange = std::min( rRange, itWeapon->first->GetMinRangeToFireOn( firer, targetComposanteType, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    double rRange = 0;
    for( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rRange = std::max( rRange, itWeapon->first->GetMaxRangeToFireOnWithPosture( firer, target, targetComposanteType, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMinRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    double rRange = std::numeric_limits< double >::max();
    for ( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rRange = std::min( rRange, itWeapon->first->GetMinRangeToFireOnWithPosture( firer, target, targetComposanteType, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMaxRangeToIndirectFire( const MIL_Agent_ABC& firer, const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const
{
    double rRange = -1.;

    for( CIT_WeaponTypeMap it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
    {
        if( it->first->GetDotationCategory() == dotationCategory )
            rRange = std::max( rRange, it->first->GetMaxRangeToIndirectFire( firer, bCheckDotationsAvailability ) );
    }
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetMinRangeToIndirectFire( const MIL_Agent_ABC& firer, const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const
{
    double rRange = std::numeric_limits< double >::max();

    for( CIT_WeaponTypeMap it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
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
    for( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rRange = std::max( rRange, itWeapon->first->GetMaxRangeToFire( pion, rWantedPH ) );
    return rRange;
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
    for( CIT_NTICapabilitySet it = ntiCapabilities_.begin(); it != ntiCapabilities_.end(); ++it )
        if( it->CanRepair( breakdown ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanHaul
// Created: NLD 2005-03-29
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanHaul( const PHY_ComposanteTypePion& type ) const
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
    if( rStockTransporterWeightCapacity_ <= 0. || rStockTransporterVolumeCapacity_ <= 0. )
        return false;

    if( pStockTransporterNature_ && *pStockTransporterNature_ != dotationCategory.GetNature() )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::UseAmmunition
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::UseAmmunition( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    for( CIT_ActiveProtectionVector it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
        (*it)->UseAmmunition( category, pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetPHModifier
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
double PHY_ComposanteTypePion::GetPHModifier( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    double result = 1.;
    for( CIT_ActiveProtectionVector it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
        result *= (*it)->GetPHModifier( category, pion );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CounterIndirectFire
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CounterIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    for( CIT_ActiveProtectionVector it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
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
    for( CIT_ActiveProtectionVector it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
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
    for( CIT_WeaponTypeMap it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rRange = std::max( rRange, it->first->GetDotationCategory().GetUrbanAttritionScore( material ) );
    return rRange;
}
