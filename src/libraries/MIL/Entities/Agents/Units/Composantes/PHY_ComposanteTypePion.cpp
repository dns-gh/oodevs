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

#include "MIL_pch.h"

#include "PHY_ComposanteTypePion.h"

#include "PHY_ComposantePion.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationConsumptions.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Radars/PHY_RadarType.h"
#include "Entities/Agents/Units/Logistic/PHY_MaintenanceLevel.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "PHY_ComposanteTypeObjectData.h"

#include "Tools/MIL_Tools.h"

PHY_ComposanteTypePion::T_ComposanteTypeMap PHY_ComposanteTypePion::composantesTypes_;
MT_Random                                   PHY_ComposanteTypePion::randomGenerator_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

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

    if ( breakdown.GetTheoricRepairTime() > nMaxTime_ )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing composante types" );

    std::set< uint > ids_;
    archive.BeginList( "Composantes" );
    while ( archive.NextListElement() )
    {
        archive.Section( "Composante" );

        std::string strComposanteType;
        archive.ReadAttribute( "nom", strComposanteType );

        PHY_ComposanteTypePion*& pComposanteType = composantesTypes_[ strComposanteType ];
        if( pComposanteType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Composante type '%s' already registered", strComposanteType.c_str() ), archive.GetContext() );

        pComposanteType = new PHY_ComposanteTypePion( strComposanteType, archive );

        if( !ids_.insert( pComposanteType->GetMosID() ).second )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Composante ID already used", archive.GetContext() );

        archive.EndSection(); // Composante
    }
    archive.EndList(); // Composantes
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

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion constructor
// Created: NLD 2004-08-04
// Modified: JVT 2004-10-20
// -----------------------------------------------------------------------------
PHY_ComposanteTypePion::PHY_ComposanteTypePion( const std::string& strName, MIL_InputArchive& archive )
    : PHY_ComposanteType_ABC                     ( strName, archive )
    , speeds_                                    ( archive )
    , rMaxSlope_                                 ( 0.6 )
    , dotationCapacities_                        ( "Contenance", archive )
    , objectData_                                ( MIL_RealObjectType::GetObjectTypes().size(), 0 )
    , consumptions_                              ( PHY_ConsumptionType::GetConsumptionTypes().size(), 0 )
    , rNbrHumansLoadedPerTimeStep_               ( 0. )
    , rNbrHumansUnloadedPerTimeStep_             ( 0. )
    , rSensorRotationAngle_                      ( 0. )
    , rWeight_                                   ( 0. )
    , rPionTransporterWeightCapacity_            ( 0. )
    , rPionTransporterWeightLoadedPerTimeStep_   ( 0. )
    , rPionTransporterWeightUnloadedPerTimeStep_ ( 0. )
    , rHaulerWeightCapacity_                     ( 0. )
    , rHaulerLoadingTime_                        ( 0. )
    , rHaulerUnloadingTime_                      ( 0. )
    , nAmbulanceCollectionCapacity_              ( 0 )
    , rNbrHumansLoadedForCollectionPerTimeStep_  ( 0. )
    , rNbrHumansUnloadedForCollectionPerTimeStep_( 0. )
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
    archive.ReadField( "DeniveleMaximum", rMaxSlope_, CheckValueBound( 0., 1. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );

    InitializeWeapons                ( archive );
    InitializeSensors                ( archive );
    InitializeRadars                 ( archive );
    InitializeTransport              ( archive );
    InitializeObjects                ( archive );
    InitializeConsumptions           ( archive );
    InitializeLogistic               ( archive );
    InitializeBreakdownTypes         ( archive );

    archive.ReadField( "Poids", rWeight_, CheckValueGreater( 0. ) );
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
void PHY_ComposanteTypePion::InitializeBreakdownTypes( MIL_InputArchive& archive )
{
    if( GetProtection().IsHuman() )
    {
        if( archive.Section( "Pannes", MIL_InputArchive::eNothing ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "A human composante can't have breakdowns ( type='%s', protection='%s')", GetName().c_str(), GetProtection().GetName().c_str() ), archive.GetContext() );
        return;
    }

    archive.Section( "Pannes" );
    InitializeRandomBreakdownTypes   ( archive );
    InitializeAttritionBreakdownTypes( archive );    
    archive.EndSection(); // Pannes
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeRandomBreakdownTypes
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeRandomBreakdownTypes( MIL_InputArchive& archive )
{
    randomBreakdownTypeProbabilities_.clear();
    
    archive.BeginList( "PannesAleatoires" );
    
    MT_Float rTotalProb = 0.;
    while ( archive.NextListElement() )
    {
        archive.Section( "Panne" );
        
        std::string strBuf;
        
        archive.ReadAttribute( "nom", strBuf );
        
        const PHY_BreakdownType* pType = PHY_BreakdownType::Find( strBuf );
        
        if ( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown breakdown type '%s'", strBuf.c_str() ), archive.GetContext() );

        MT_Float rPercentage;
        
        archive.ReadAttribute( "pourcentage", rPercentage,  CheckValueBound( 0., 100. ) );
        rTotalProb  += rPercentage;
        
        if ( rTotalProb > 100. )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Total probability of breakdows is greater than 100%" ), archive.GetContext() );
        
        randomBreakdownTypeProbabilities_.push_back( sBreakdownTypeProbability( *pType, rTotalProb * 0.01 ) );
        
        archive.EndSection(); // Panne
    }
    
    if ( rTotalProb != 100. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Total probability of random breakdowns is less than 100%" ), archive.GetContext() );
    
    archive.EndList(); // PannesAleatoires
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeAttritionBreakdownTypes
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeAttritionBreakdownTypes( MIL_InputArchive& archive )
{
    attritionBreakdownTypeProbabilities_.clear();
    
    archive.BeginList( "PannesAttritions" );
    
    MT_Float rTotalProb = 0.;
    while ( archive.NextListElement() )
    {
        archive.Section( "Panne" );
        
        std::string strBuf;
        
        archive.ReadAttribute( "nom", strBuf );
        
        const PHY_BreakdownType* pType = PHY_BreakdownType::Find( strBuf );
        
        if ( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown breakdown type '%s'", strBuf.c_str() ), archive.GetContext() );

        MT_Float rPercentage;
        
        archive.ReadAttribute( "pourcentage", rPercentage,  CheckValueBound( 0., 100. ) );
        
        rTotalProb += rPercentage;
        
        if ( rTotalProb > 100. )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Total probability of breakdows is greater than 100%" ), archive.GetContext() );
            
        attritionBreakdownTypeProbabilities_.push_back( sBreakdownTypeProbability( *pType, rTotalProb * 0.01 ) );

        archive.EndSection(); // Panne
    }

    if ( rTotalProb != 100. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Total probability of attrition breakdowns is less than 100%" ), archive.GetContext() );

    archive.EndList(); // PannesAttritions
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeWeapons
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeWeapons( MIL_InputArchive& archive )
{
    archive.BeginList( "Armements" );
    while( archive.NextListElement() )
    {
        archive.Section( "Armement" );

        std::string strLauncher;
        std::string strAmmunition;
        bool        bMajor = false;

        archive.ReadAttribute( "lanceur"  , strLauncher );
        archive.ReadAttribute( "munition", strAmmunition );
        archive.ReadAttribute( "majeur"  , bMajor, MIL_InputArchive::eNothing );

        const PHY_WeaponType* pWeaponType = PHY_WeaponType::FindWeaponType( strLauncher, strAmmunition );
        if( !pWeaponType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown weapon type (%s, %s)", strLauncher.c_str(), strAmmunition.c_str() ), archive.GetContext() );

        if( weaponTypes_.find( pWeaponType ) != weaponTypes_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Weapon type %s,%s already initialized", strLauncher.c_str(), strAmmunition.c_str() ), archive.GetContext() );
            
        weaponTypes_[ pWeaponType ] = bMajor;

        archive.EndSection(); // Armement
    }

    archive.EndList(); // Armements
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeSensors
// Created: NLD 2004-08-09
// Modified: JVT 2004-10-20
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeSensors( MIL_InputArchive& archive )
{
    rSensorRotationAngle_ = std::numeric_limits< MT_Float >::max();

    archive.BeginList( "Senseurs" );
    while( archive.NextListElement() )
    {
        archive.Section( "Senseur" );

        std::string strSensor;
        archive.Read( strSensor );

        const PHY_SensorType* pSensorType = PHY_SensorType::FindSensorType( strSensor );
        if( !pSensorType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown sensor type '%s'", strSensor.c_str() ), archive.GetContext() );

        if( sensorTypes_.find( pSensorType ) != sensorTypes_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Sensor type '%s' already defined", strSensor.c_str() ), archive.GetContext() );

        archive.ReadAttribute( "hauteur", sensorTypes_[ pSensorType ] );

        if( pSensorType->GetTypeAgent() )
            rSensorRotationAngle_ = std::min( rSensorRotationAngle_, pSensorType->GetTypeAgent()->GetAngle() );

        archive.EndSection(); // Senseur
    }
    archive.EndList(); // Senseurs

    if ( rSensorRotationAngle_ == std::numeric_limits< MT_Float >::max() )
        rSensorRotationAngle_ = 0.;
    else
        rSensorRotationAngle_ -= std::min( rSensorRotationAngle_, 0.1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeRadars
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeRadars( MIL_InputArchive& archive )
{
    if( !archive.BeginList( "Radars", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        std::string strRadar;
        archive.Section( "Radar" );
        
        archive.Read( strRadar );

        const PHY_RadarType* pRadarType = PHY_RadarType::Find( strRadar );
        if( !pRadarType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown radar type", archive.GetContext() );

        if( radarTypes_.find( pRadarType ) != radarTypes_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Radar type already defined", archive.GetContext() );
        radarTypes_.insert( pRadarType );

        archive.EndSection(); // Radar
    }
    archive.EndList(); // Radars
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeTransport
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeTransport( MIL_InputArchive& archive )
{
    archive.Section( "Transport" );

    archive.Section( "Personnel" );

    if( archive.Section( "Temps", MIL_InputArchive::eNothing ) )
    {
        archive.ReadTimeField( "TempsEmbarquementParHomme", rNbrHumansLoadedPerTimeStep_  , CheckValueGreater( 0. ) );
        archive.ReadTimeField( "TempsDebarquementParHomme", rNbrHumansUnloadedPerTimeStep_, CheckValueGreater( 0. ) );

        rNbrHumansLoadedPerTimeStep_   = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansLoadedPerTimeStep_   );
        rNbrHumansUnloadedPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansUnloadedPerTimeStep_ );

        archive.EndSection(); // Temps
    }

    archive.EndSection(); // Personnel

    if( archive.Section( "Pion", MIL_InputArchive::eNothing ) )
    {
        archive.ReadField( "Capacite", rPionTransporterWeightCapacity_, CheckValueGreater( 0. ) );

        archive.Section( "Temps" );
        archive.ReadTimeField( "TempsChargementParTonne"  , rPionTransporterWeightLoadedPerTimeStep_  , CheckValueGreater( 0. ) );
        archive.ReadTimeField( "TempsDechargementParTonne", rPionTransporterWeightUnloadedPerTimeStep_, CheckValueGreater( 0. ) );

        rPionTransporterWeightLoadedPerTimeStep_   = 1. / MIL_Tools::ConvertSecondsToSim( rPionTransporterWeightLoadedPerTimeStep_   );
        rPionTransporterWeightUnloadedPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rPionTransporterWeightUnloadedPerTimeStep_ );

        archive.EndSection(); // Temps
        archive.EndSection(); // Pion
    }

    archive.EndSection(); // Transport
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeObjects
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeObjects( MIL_InputArchive& archive )
{
    archive.BeginList( "Objets" );

    while( archive.NextListElement() )
    {
        archive.Section( "Objet" );

        std::string strType;
        archive.ReadAttribute( "type", strType );

        const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( strType );
        if( !pObjectType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown object type '%s'", strType.c_str() ), archive.GetContext() );

        assert( objectData_.size() > pObjectType->GetID() );
        const PHY_ComposanteTypeObjectData*& pObject = objectData_[ pObjectType->GetID() ];
        if( pObject )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Object type '%s' already instanciated", strType.c_str() ), archive.GetContext() );

        pObject = new PHY_ComposanteTypeObjectData( *pObjectType, archive );
        
        archive.EndSection(); // Objet
    }
    archive.EndList(); // objets
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeConsumptions
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeConsumptions( MIL_InputArchive& archive )
{
    archive.Section( "Consommations" );

    const PHY_ConsumptionType::T_ConsumptionTypeMap& consumptionTypes = PHY_ConsumptionType::GetConsumptionTypes();
    for ( PHY_ConsumptionType::CIT_ConsumptionTypeMap it = consumptionTypes.begin(); it != consumptionTypes.end(); ++it )
    {
        const PHY_ConsumptionType& consumptionType = *it->second;
        consumptions_[ consumptionType.GetID() ] = new PHY_DotationConsumptions( consumptionType.GetName(), archive );
    }

    archive.EndSection(); // Consommations
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeLogisticMaintenance
// Created: NLD 2004-12-21
// Modified: JVT 2005-02-03
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeLogisticMaintenance( MIL_InputArchive& archive )
{
    rHaulerLoadingTime_    = 0.;
    rHaulerUnloadingTime_  = 0.;
    rHaulerWeightCapacity_ = 0.;

    if( !archive.Section( "Maintenance", MIL_InputArchive::eNothing ) )
        return;

    if( archive.Section( "Remorqueur", MIL_InputArchive::eNothing ) )
    {
        archive.ReadField    ( "Capacite"         , rHaulerWeightCapacity_ , CheckValueGreater       ( 0. ) );
        archive.ReadTimeField( "TempsChargement"  , rHaulerLoadingTime_    , CheckValueGreaterOrEqual( 0. ) );
        archive.ReadTimeField( "TempsDechargement", rHaulerUnloadingTime_  , CheckValueGreaterOrEqual( 0. ) );

        rHaulerLoadingTime_   = MIL_Tools::ConvertSecondsToSim( rHaulerLoadingTime_ );
        rHaulerUnloadingTime_ = MIL_Tools::ConvertSecondsToSim( rHaulerUnloadingTime_ );

        archive.EndSection(); // Remorqueur
    }

    ntiCapabilities_.clear();
    const PHY_MaintenanceLevel::T_MaintenanceLevelMap& maintenanceLevels = PHY_MaintenanceLevel::GetMaintenanceLevels();
    for( PHY_MaintenanceLevel::CIT_MaintenanceLevelMap it = maintenanceLevels.begin(); it != maintenanceLevels.end(); ++it )
    {
        const PHY_MaintenanceLevel& maintenanceLevel = *it->second;

        if( !archive.Section( maintenanceLevel.GetName(), MIL_InputArchive::eNothing ) )
            continue;

        sNTICapability ntiCapability( maintenanceLevel );

        MT_Float rTime;
        if ( archive.ReadTimeAttribute( "tempsMaxReparation", rTime, CheckValueGreater( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) )
            ntiCapability.nMaxTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rTime );

        if( archive.Section( "EA", MIL_InputArchive::eNothing ) )
        {
            ntiCapability.bElectronic_ = true;
            archive.EndSection(); // EA
        }

        if( archive.Section( "M", MIL_InputArchive::eNothing ) )
        {
            ntiCapability.bMobility_ = true;
            archive.EndSection(); // M
        }
        
        if( !ntiCapability.bMobility_ && !ntiCapability.bElectronic_ )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "NTI has not 'EA' nor 'M'", archive.GetContext() );

        if( !ntiCapabilities_.insert( ntiCapability ).second )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "NTI already defined", archive.GetContext() );

        archive.EndSection(); // NTIx
    }

    archive.EndSection(); // Maintenance
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeLogisticMedical
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeLogisticMedical( MIL_InputArchive& archive )
{
    if( !archive.Section( "Sante", MIL_InputArchive::eNothing ) )
        return;

    if( archive.Section( "Medecin", MIL_InputArchive::eNothing  ) )
    {
        bCanDiagnoseHumans_ = true;

        if( archive.Section( "Tri", MIL_InputArchive::eNothing  ) )
        {
            bCanSortHumans_ = true;
            archive.EndSection(); // MedecinBlessures
        }
        if( archive.Section( "SoinNBC", MIL_InputArchive::eNothing  ) )
        {
            bCanHealContaminated_ = true;
            archive.EndSection(); // MedecinNBC
        }
        if( archive.Section( "SoinReacMental", MIL_InputArchive::eNothing  ) )
        {
            bCanHealMentalDiseases_ = true;
            archive.EndSection(); // MedecinReacMental
        }
        if( archive.Section( "SoinBlessures", MIL_InputArchive::eNothing  ) )
        {
            const PHY_HumanWound::T_HumanWoundMap& wounds = PHY_HumanWound::GetHumanWounds();
            for( PHY_HumanWound::CIT_HumanWoundMap itWound = wounds.begin(); itWound != wounds.end(); ++itWound )
            {
                if( archive.Section( itWound->second->GetName(), MIL_InputArchive::eNothing ) )
                {
                    bCanHealWounds_ = true;
                    woundHealingCapabilities_[ itWound->second->GetID() ] = true;                    
                    archive.EndSection();
                }
            }
            archive.EndSection(); // MedecinBlessures
        }       
        archive.EndSection(); // Medecin
    }

    if( archive.Section( "AmbulanceRamassage", MIL_InputArchive::eNothing ) )
    {
        archive.ReadField    ( "Capacite"                  , nAmbulanceCollectionCapacity_              , CheckValueGreater( 0  ) );
        archive.ReadTimeField( "TempsChargementParHumain"  , rNbrHumansLoadedForCollectionPerTimeStep_  , CheckValueGreater( 0. ) );
        archive.ReadTimeField( "TempsDechargementParHumain", rNbrHumansUnloadedForCollectionPerTimeStep_, CheckValueGreater( 0. ) );

        rNbrHumansLoadedForCollectionPerTimeStep_   = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansLoadedForCollectionPerTimeStep_   );
        rNbrHumansUnloadedForCollectionPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansUnloadedForCollectionPerTimeStep_ );

        archive.EndSection(); // AmbulanceReleve
    }

    if( archive.Section( "AmbulanceReleve", MIL_InputArchive::eNothing ) )
    {
        archive.ReadField    ( "Capacite"                  , nAmbulanceEvacuationCapacity_              , CheckValueGreater( 0  ) );
        archive.ReadTimeField( "TempsChargementParHumain"  , rNbrHumansLoadedForEvacuationPerTimeStep_  , CheckValueGreater( 0. ) );
        archive.ReadTimeField( "TempsDechargementParHumain", rNbrHumansUnloadedForEvacuationPerTimeStep_, CheckValueGreater( 0. ) );

        rNbrHumansLoadedForEvacuationPerTimeStep_   = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansLoadedForEvacuationPerTimeStep_   );
        rNbrHumansUnloadedForEvacuationPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rNbrHumansUnloadedForEvacuationPerTimeStep_ );

        archive.EndSection(); // AmbulanceRamassage
    }

    archive.EndSection(); // Sante
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeLogisticSupply
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeLogisticSupply( MIL_InputArchive& archive )
{
    if( !archive.Section( "Ravitaillement", MIL_InputArchive::eNothing ) )
        return;

    if( archive.Section( "Transporteur", MIL_InputArchive::eNothing ) )
    {
        std::string strNature;
        archive.ReadField( "NatureTransportee", strNature );
        pStockTransporterNature_ = PHY_DotationNature::Find( strNature );
        if( !pStockTransporterNature_ )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unkown dotation nature", archive.GetContext() );

        archive.Section( "Capacite" );
        archive.ReadField( "Masse" , rStockTransporterWeightCapacity_, CheckValueGreater( 0. ) );
        archive.ReadField( "Volume", rStockTransporterVolumeCapacity_, CheckValueGreater( 0. ) );
        archive.EndSection(); // Capacite

        archive.EndSection(); // Transporteur
    }

    archive.EndSection(); // Ravitaillement
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InitializeLogistic
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
void PHY_ComposanteTypePion::InitializeLogistic( MIL_InputArchive& archive )
{
    if( !archive.Section( "FonctionsLogistiques", MIL_InputArchive::eNothing ) )
        return;

    InitializeLogisticMaintenance( archive );
    InitializeLogisticMedical    ( archive );
    InitializeLogisticSupply     ( archive );

    archive.EndSection(); // FonctionsLogistiques
}


// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::InstanciateComposante
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_ComposantePion& PHY_ComposanteTypePion::InstanciateComposante( PHY_RolePion_Composantes& role, uint nNbrHumanInCrew, bool bMajor, bool bLoadable, bool bCanBePartOfConvoy ) const
{
    return *new PHY_ComposantePion( *this, role, nNbrHumanInCrew, bMajor, bLoadable, bCanBePartOfConvoy );
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
        const MT_Float         rHeight    =  it->second;
        inserter = &sensorType.InstanciateSensor( rHeight );
    }
}

// =============================================================================
// OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanConstruct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanConstruct( const MIL_RealObjectType& objectType ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    return pObjectData && pObjectData->CanConstruct();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanDestroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanDestroy( const MIL_RealObjectType& objectType ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    return pObjectData && pObjectData->CanDestroy();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanMine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanMine( const MIL_RealObjectType& objectType ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    return pObjectData && pObjectData->CanMine();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanDemine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanDemine( const MIL_RealObjectType& objectType ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    return pObjectData && pObjectData->CanDemine();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanBypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypePion::CanBypass( const MIL_RealObjectType& objectType, bool bObjectIsMined ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    return pObjectData && pObjectData->CanBypass( bObjectIsMined );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetConstructionTime
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetConstructionTime( const MIL_RealObjectType& objectType, MT_Float rSizeCoef ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    if( pObjectData )
        return pObjectData->GetConstructionTime( rSizeCoef );
    return std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDestructionTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetDestructionTime( const MIL_RealObjectType& objectType, MT_Float rSizeCoef ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    if( pObjectData )
        return pObjectData->GetDestructionTime( rSizeCoef );
    return std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMiningTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetMiningTime( const MIL_RealObjectType& objectType ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    if( pObjectData )
        return pObjectData->GetMiningTime();
    return std::numeric_limits< MT_Float >::max();
}
// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDeminingTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetDeminingTime( const MIL_RealObjectType& objectType ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    if( pObjectData )
        return pObjectData->GetDeminingTime();
    return std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetBypassTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetBypassTime( const MIL_RealObjectType& objectType, MT_Float rSizeCoef, bool bObjectIsMined ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    if( pObjectData )
        return pObjectData->GetBypassTime( rSizeCoef, bObjectIsMined );
    return std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetMaxSpeed( const MIL_RealObject_ABC& object ) const
{
    assert( objectData_.size() > object.GetType().GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ object.GetType().GetID() ];
    if( pObjectData )
        return pObjectData->GetMaxSpeed( object );

    return object.GetDefaultMaxSpeed();
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
const PHY_ConsumptionType& PHY_ComposanteTypePion::GetConsumptionMode( const MIL_RealObjectType& objectType ) const
{
    assert( objectData_.size() > objectType.GetID() );
    const PHY_ComposanteTypeObjectData* pObjectData = objectData_[ objectType.GetID() ];
    if( pObjectData && pObjectData->GetConsumptionMode() )
        return *pObjectData->GetConsumptionMode();
    return objectType.GetDefaultConsumptionMode();
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
    const MT_Float rRandomValue = randomGenerator_.rand_ii( 0., 1. );
    
    for ( CIT_BreakdownTypeProbabilityVector it = probasVector.begin(); it != probasVector.end(); ++it )
    {
        if ( rRandomValue <= it->rProbabilityBound_ )
            return *it->pBreakdownType_;
    }
    assert( false );
    return *(PHY_BreakdownType*)0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDangerosity
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& target, MT_Float rDistBtwSourceAndTarget ) const
{
    MT_Float rScore = 0.;
    for( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rScore = std::max( rScore, itWeapon->first->GetDangerosity( firer, target, rDistBtwSourceAndTarget ) );
    return rScore;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToFireOn
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    MT_Float rRange = 0;
    for( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rRange = std::max( rRange, itWeapon->first->GetMaxRangeToFireOn( firer, targetComposanteType, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetMinRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    MT_Float rRange = std::numeric_limits< MT_Float >::max();
    for ( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rRange = std::min( rRange, itWeapon->first->GetMinRangeToFireOn( firer, targetComposanteType, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetMaxRangeToFireOnWithPosture( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    MT_Float rRange = 0;
    for( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rRange = std::max( rRange, itWeapon->first->GetMaxRangeToFireOnWithPosture( firer, target, targetComposanteType, rWantedPH ) );
    return rRange;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetMinRangeToFireOnWithPosture( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const
{
    MT_Float rRange = std::numeric_limits< MT_Float >::max();
    for ( CIT_WeaponTypeMap itWeapon = weaponTypes_.begin(); itWeapon != weaponTypes_.end(); ++itWeapon )
        rRange = std::min( rRange, itWeapon->first->GetMinRangeToFireOnWithPosture( firer, target, targetComposanteType, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetMaxRangeToIndirectFire( const MIL_Agent_ABC& firer, const PHY_IndirectFireDotationClass& ammoClass, bool bCheckDotationsAvailability ) const
{
    MT_Float rRange = -1.;
    
    for ( CIT_WeaponTypeMap it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rRange = std::max( rRange, it->first->GetMaxRangeToIndirectFire( firer, ammoClass, bCheckDotationsAvailability ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypePion::GetMinRangeToIndirectFire( const MIL_Agent_ABC& firer, const PHY_IndirectFireDotationClass& ammoClass, bool bCheckDotationsAvailability ) const
{
    MT_Float rRange = std::numeric_limits< MT_Float >::max();
    
    for ( CIT_WeaponTypeMap it = weaponTypes_.begin(); it != weaponTypes_.end(); ++it )
        rRange = std::min( rRange, it->first->GetMinRangeToIndirectFire( firer, ammoClass, bCheckDotationsAvailability ) );
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
bool PHY_ComposanteTypePion::CanHealHuman( const PHY_Human& human ) const
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
uint PHY_ComposanteTypePion::GetHealingTime( const PHY_Human& human ) const
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
    return std::numeric_limits< uint >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::Heal
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
uint PHY_ComposanteTypePion::Heal( PHY_Human& human ) const
{
    if( human.IsContaminated() )
    {
        assert( bCanHealContaminated_  );
        human.HealContamination();
        return PHY_HumanWound::GetContaminatedRestingTime();
    }

    if( human.IsWounded() )
    {
        assert( woundHealingCapabilities_[ human.GetWound().GetID() ] );
        const uint nRestingTime = human.GetWound().GetRestingTime();
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
