// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/MIL_EntityManager.cpp $
// $Author: Nld $
// $Modtime: 10/06/05 10:19 $
// $Revision: 45 $
// $Workfile: MIL_EntityManager.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_EntityManager.h"

#include "Effects/MIL_EffectManager.h"
#include "Objects/MIL_ObjectManager.h"
#include "Agents/Units/Categories/PHY_NatureLevel.h"
#include "Agents/Units/Categories/PHY_NatureAtlas.h"
#include "Agents/Units/Categories/PHY_RoePopulation.h"
#include "Agents/Units/Categories/PHY_Volume.h"
#include "Agents/Units/Categories/PHY_Protection.h"
#include "Agents/Units/HumanFactors/PHY_Experience.h"
#include "Agents/Units/HumanFactors/PHY_Tiredness.h"
#include "Agents/Units/HumanFactors/PHY_Morale.h"
#include "Agents/Units/Postures/PHY_Posture.h"
#include "Agents/Units/Dotations/PHY_DotationType.h"
#include "Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Agents/Units/Dotations/PHY_DotationNature.h"
#include "Agents/Units/Weapons/PHY_LauncherType.h"
#include "Agents/Units/Weapons/PHY_WeaponType.h"
#include "Agents/Units/Sensors/PHY_SensorType.h"
#include "Agents/Units/Radars/PHY_RadarType.h"
#include "Agents/Units/Radars/PHY_RadarClass.h"
#include "Agents/Units/Humans/PHY_HumanRank.h"
#include "Agents/Units/Humans/PHY_HumanWound.h"
#include "Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Agents/Units/Composantes/PHY_ComposanteState.h"
#include "Agents/Units/Logistic/PHY_MaintenanceWorkRate.h"
#include "Agents/Units/Logistic/PHY_MaintenanceLevel.h"
#include "Agents/Units/Logistic/PHY_BreakdownType.h"
#include "Agents/Roles/Logistic/Supply/PHY_Convoy_ABC.h"
#include "Agents/Roles/Logistic/Maintenance/PHY_MaintenanceResourcesAlarms.h"
#include "Agents/Roles/Logistic/Medical/PHY_MedicalResourcesAlarms.h"
#include "Agents/Roles/Logistic/Supply/PHY_SupplyResourcesAlarms.h"
#include "Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Agents/Perceptions/PHY_PerceptionRecoSurveillance.h"
#include "Agents/Perceptions/PHY_PerceptionFlyingShell.h"
#include "Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Agents/MIL_AgentTypePion.h"
#include "Agents/MIL_AgentPion.h"
#include "Automates/MIL_AutomateType.h"
#include "Automates/MIL_Automate.h"
#include "Objects/MIL_RealObjectType.h"
#include "Objects/MIL_VirtualObjectType.h"
#include "Objects/MIL_NbcAgentType.h"
#include "Populations/MIL_PopulationType.h"
#include "Populations/MIL_PopulationAttitude.h"
#include "Populations/MIL_Population.h"
#include "Entities/MIL_Intelligence.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "Orders/MIL_LimaFunction.h"
#include "Orders/MIL_Report.h"
#include "Tools/MIL_ProfilerMgr.h"
#include "Tools/MIL_IDManager.h"
#include "MIL_Army.h"
#include "MIL_Formation.h"
#include "Network/NET_ASN_Messages.h"
#include "HLA/HLA_Federate.h"
#include "Network/NET_AsnException.h"
#include "MIL_Singletons.h"
#include "xeumeuleu/xml.h"

using namespace xml;

BOOST_CLASS_EXPORT_GUID( MIL_EntityManager, "MIL_EntityManager" )

MIL_EntityManager* MIL_EntityManager::singleton_ = 0;

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Initialize
// Created: JVT 2005-03-07
// -----------------------------------------------------------------------------
void MIL_EntityManager::Initialize( MIL_Config& config, const MIL_Time_ABC& time, MIL_EffectManager& effects )
{
    // Static types
    PHY_ComposanteState          ::Initialize();
    PHY_HumanRank                ::Initialize();
    PHY_HumanWound               ::Initialize();
    MIL_Army                     ::Initialize();
    PHY_ConsumptionType          ::Initialize();
    PHY_Posture                  ::Initialize();
    PHY_IndirectFireDotationClass::Initialize();
    PHY_AmmoDotationClass        ::Initialize();
    PHY_Morale                   ::Initialize();
    PHY_NatureLevel              ::Initialize();
    PHY_NatureAtlas              ::Initialize();
    PHY_RoePopulation            ::Initialize();
    PHY_MaintenanceLevel         ::Initialize();
    PHY_PerceptionLevel          ::Initialize();
    PHY_RadarClass               ::Initialize();
    MIL_PopulationAttitude       ::Initialize();
    PHY_DotationLogisticType     ::Initialize();
    MIL_LimaFunction             ::Initialize();

    xml::xifstream xis( config.GetPhysicalFile() );
    xis >> start( "physical" );

    InitializeType< MIL_Report                     >( xis, config, "reports"             );
    InitializeType< PHY_MaintenanceWorkRate        >( xis, config, "maintenance"         );
    InitializeType< PHY_MaintenanceResourcesAlarms >( xis, config, "maintenance"         );
    InitializeType< PHY_Experience                 >( xis, config, "human-factors"       );
    InitializeType< PHY_Tiredness                  >( xis, config, "human-factors"       );    
    InitializeType< PHY_Volume                     >( xis, config, "volumes"             );
    InitializeType< PHY_Protection                 >( xis, config, "protections"         );
    InitializeType< PHY_DotationNature             >( xis, config, "dotation-natures"    );
    InitializeType< PHY_DotationType               >( xis, config, "dotations"           );
    InitializeType< MIL_RealObjectType             >( xis, config, "objects"             );
    InitializeType< MIL_VirtualObjectType          >( xis, config, "objects"             );
    InitializeType< PHY_BreakdownType              >( xis, config, "breakdowns"          );
    InitializeType< PHY_LauncherType               >( xis, config, "launchers"           );
    InitializeWeapons    ( xis, config, time, effects );
    InitializeSensors    ( xis, config, time );
    InitializeComposantes( xis, config, time );
    InitializeType< MIL_AgentTypePion              >( xis, config, "units"               );
    InitializeType< MIL_AutomateType               >( xis, config, "automats"            );
    InitializeType< MIL_KnowledgeGroupType         >( xis, config, "knowledge-groups"    );
    InitializeType< MIL_NbcAgentType               >( xis, config, "nbc"                 );
    InitializeType< PHY_SupplyResourcesAlarms      >( xis, config, "supply"              );   
    InitializeType< PHY_Convoy_ABC                 >( xis, config, "supply"              );
    InitializeType< PHY_MedicalResourcesAlarms     >( xis, config, "health"              );
    InitializeType< PHY_RolePion_Communications    >( xis, config, "communications"      );
    InitializeType< MIL_PopulationType             >( xis, config, "populations"         );
    InitializeMedical( xis, config );

    xis >> end(); // physical
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager constructor
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
MIL_EntityManager::MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, MIL_ProfilerMgr& profiler, HLA_Federate* hla )
    : time_                         ( time )
    , profilerManager_              ( profiler )
    , hla_                          ( hla )
    , effectManager_                ( effects )
    , pObjectManager_               (  new MIL_ObjectManager() )
    , nRandomBreakdownsNextTimeStep_( 0  )
    , rKnowledgesTime_              ( 0. )
    , rAutomatesDecisionTime_       ( 0. )
    , rPionsDecisionTime_           ( 0. )
    , rPopulationsDecisionTime_     ( 0. )
    , rActionsTime_                 ( 0. )
    , rEffectsTime_                 ( 0. )
    , rStatesTime_                  ( 0. )
{
    if( !singleton_ )
        singleton_ = this;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager constructor
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
MIL_EntityManager::MIL_EntityManager()
    : time_                         ( MIL_Singletons::GetTime() )
    , profilerManager_              ( MIL_Singletons::GetProfiler() )
    , hla_                          ( MIL_Singletons::GetHla() )
    , effectManager_                ( MIL_Singletons::GetEffectManager() )
    , pObjectManager_               (  new MIL_ObjectManager() )
    , nRandomBreakdownsNextTimeStep_( 0  )
    , rKnowledgesTime_              ( 0. )
    , rAutomatesDecisionTime_       ( 0. )
    , rPionsDecisionTime_           ( 0. )
    , rPopulationsDecisionTime_     ( 0. )
    , rActionsTime_                 ( 0. )
    , rEffectsTime_                 ( 0. )
    , rStatesTime_                  ( 0. )
{
    if( !singleton_ )
        singleton_ = this;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetSingleton
// Created: SBO 2007-11-06
// -----------------------------------------------------------------------------
MIL_EntityManager& MIL_EntityManager::GetSingleton()
{
    if( !singleton_ )
        throw std::runtime_error( "Entity manager singleton not initialized." );
    return *singleton_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeSensors
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeSensors( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time )
{
    MT_LOG_INFO_MSG( "Initializing sensor types" );

    std::string strFile;
    xis >> start( "sensors" )
            >> attribute( "file", strFile )
        >> end();
    strFile = config.BuildPhysicalChildFile( strFile );


    xml::xifstream xisSensors( strFile );
    config.AddFileToCRC( strFile );

    xisSensors >> start( "sensors" );
    PHY_PerceptionRecoSurveillance::Initialize( xisSensors );
    PHY_PerceptionFlyingShell     ::Initialize( xisSensors );
    PHY_SensorType                ::Initialize( xisSensors );
    PHY_RadarType                 ::Initialize( xisSensors, time );
    xisSensors >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeMedical
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeMedical( xml::xistream& xis, MIL_Config& config )
{
    MT_LOG_INFO_MSG( "Initializing medical data" );

    std::string strFile;
    xis >> start( "health" )
            >> attribute( "file", strFile )
        >> end();
    strFile = config.BuildPhysicalChildFile( strFile );

    xml::xifstream xisHealth( strFile );

    xisHealth >> start( "health" );
    config.AddFileToCRC( strFile );

    PHY_HumanWound::InitializeMedicalData( xisHealth );

    xisHealth >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeType
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
template < typename T >
void MIL_EntityManager::InitializeType( xml::xistream& xis, MIL_Config& config, const std::string& strSection )
{
    std::string strFile;
    xis >> start( strSection )
            >> attribute( "file", strFile )
        >> end();

    strFile = config.BuildPhysicalChildFile( strFile );

    xml::xifstream xisType( strFile );
    config.AddFileToCRC( strFile );

    T::Initialize( xisType );//verfifier tous les initialize
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeComposantes
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeComposantes( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time )
{
    std::string strFile;
    xis >> start( "components" )
            >> attribute( "file", strFile )
        >> end();
    strFile = config.BuildPhysicalChildFile( strFile );

    xml::xifstream xisComponents( strFile );
    config.AddFileToCRC( strFile );

    PHY_ComposanteTypePion::Initialize( time, xisComponents );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeWeapons
// Created: AGE 2007-08-13
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeWeapons( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time, MIL_EffectManager& effects )
{
    std::string strFile;
    xis >> start( "weapon-systems" )
            >> attribute( "file", strFile )
        >> end();
    strFile = config.BuildPhysicalChildFile( strFile );

    xml::xifstream xisWeapons( strFile );
    config.AddFileToCRC( strFile );

    PHY_WeaponType::Initialize( effects, time, xisWeapons );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_EntityManager::~MIL_EntityManager()
{
    // ODB - $$$ Virer ça ... utiliser des factories
    for( CIT_PionMap itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        delete itPion->second;

    for( CIT_AutomateMap itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
        delete itAutomate->second;

    for( CIT_PopulationMap itPopulation = populations_.begin(); itPopulation != populations_.end(); ++itPopulation )
        delete itPopulation->second;

    for( CIT_ArmyMap itArmy = armies_.begin(); itArmy != armies_.end(); ++itArmy )
        delete itArmy->second;

    // Types
    MIL_PopulationAttitude        ::Terminate();
    MIL_AutomateType              ::Terminate();
    MIL_AgentTypePion             ::Terminate();
    PHY_SensorType                ::Terminate();
    PHY_ComposanteTypePion        ::Terminate();
    PHY_WeaponType                ::Terminate();
    PHY_LauncherType              ::Terminate();
    PHY_DotationType              ::Terminate();
    PHY_DotationNature            ::Terminate();
    MIL_RealObjectType            ::Terminate();
    MIL_VirtualObjectType         ::Terminate();
    MIL_NbcAgentType              ::Terminate();
    PHY_Protection                ::Terminate();
    PHY_Volume                    ::Terminate();
    PHY_HumanRank                 ::Terminate();
    PHY_HumanWound                ::Terminate();
    PHY_ComposanteState           ::Terminate();
    PHY_IndirectFireDotationClass ::Terminate();
    PHY_AmmoDotationClass         ::Terminate();
    MIL_KnowledgeGroupType        ::Terminate();
    PHY_Experience                ::Terminate();
    PHY_Tiredness                 ::Terminate();
    PHY_Morale                    ::Terminate();
    PHY_MaintenanceLevel          ::Terminate();
    PHY_PerceptionLevel           ::Terminate();
    PHY_RadarClass                ::Terminate();
    PHY_NatureLevel               ::Terminate();
    PHY_NatureAtlas               ::Terminate();
    PHY_RoePopulation             ::Terminate();
    PHY_MaintenanceWorkRate       ::Terminate();
    PHY_Convoy_ABC                ::Terminate();
    PHY_RadarType                 ::Terminate();
    MIL_PopulationType            ::Terminate();
    PHY_DotationLogisticType      ::Terminate();
    PHY_SupplyResourcesAlarms     ::Terminate();
    PHY_MaintenanceResourcesAlarms::Terminate();
    PHY_MedicalResourcesAlarms    ::Terminate();
    MIL_LimaFunction              ::Terminate();

    delete pObjectManager_;

    if( this == singleton_ )
        singleton_ = 0;
}

// =============================================================================
// ODB INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadODB
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadODB( const MIL_Config& config )
{
    MT_LOG_STARTUP_MESSAGE( "-------------------------" );
    MT_LOG_STARTUP_MESSAGE( "----  Loading ODB    ----" );
    MT_LOG_STARTUP_MESSAGE( "-------------------------" );

    const std::string strOrbat = config.GetOrbatFile();
    MT_LOG_INFO_MSG( MT_FormatString( "ODB file name : '%s'", strOrbat.c_str() ) );

    xml::xifstream xis( strOrbat );
    xis >> start( "orbat" );

    InitializeArmies   ( xis );
    InitializeDiplomacy( xis );

    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automates_  .size() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , pions_      .size() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populations_.size() ) );

    xis >> end();

    // Check automate composition
    if( config.CheckAutomateComposition() )
    {
        for( CIT_AutomateMap it = automates_.begin(); it != automates_.end(); ++it )
        {
            const MIL_Automate& automate = *it->second;
            if( !automate.CheckComposition() )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "The effective composition of the automate '%d' ('%s') is not consistent with the composition described in the type '%s'", automate.GetID(), automate.GetName().c_str(), automate.GetType().GetName().c_str() ) );
        }
    }
    UpdateStates();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeDiplomacy
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeDiplomacy( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing diplomacy" );

    xis >> start( "diplomacies" )
            >> xml::list( "side", *this, MIL_EntityManager::ReadDiplomacy )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadDiplomacy
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadDiplomacy( xml::xistream& xis )
{
    uint id;
    xis >> attribute( "id", id );

    MIL_Army* pArmy = FindArmy( id );
    if( !pArmy )
        xis.error( "Unknown side" );
    pArmy->InitializeDiplomacy( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeArmies
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeArmies( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing armies" );

    assert( armies_.empty() );

    xis >> start( "sides" )
            >> xml::list( "side", *this, &MIL_EntityManager::ReadArmy )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadArmy
// Created: ABL 2007-07-09
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadArmy( xml::xistream& xis )
{
    uint id;
    xis >> attribute( "id", id );

    MIL_Army*& pArmy = armies_[ id ];
    if( pArmy )
        xis.error( "Army already exists" );
    pArmy = new MIL_Army( *this, id, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateFormation
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateFormation( xml::xistream& xis, MIL_Army& army, MIL_Formation* parent /*=0*/ )
{
    uint id;
    xis >> attribute( "id", id );
    MIL_Formation*& pFormation = formations_[ id ];
    if( pFormation )
        xis.error( "Formation using this id already exists" );
    pFormation = new MIL_Formation( *this, MIL_AgentServer::GetWorkspace().GetTacticalLineManager(), id, army, xis, parent );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomat
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateAutomat( xml::xistream& xis, MIL_Formation& formation )
{
    uint id;
    std::string strType;

    xis >> attribute( "id", id )
        >> attribute( "type", strType );

    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( strType );
    if( !pType )
        xis.error( "Unknown automat type" );

    MIL_Automate*& pAutomate = automates_[ id ];
    if( pAutomate )
        xis.error( "Automat using this id already exists" );
    pAutomate = (MIL_Automate*)0xdead;  // $$$$ NLD 2007-04-04: N'importe quoi
    
    pAutomate = &pType->InstanciateAutomate( id, formation, xis );
    pAutomate->ReadOverloading( xis );
    // $$$ Network
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomat
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateAutomat( xml::xistream& xis, MIL_Automate& parent )
{
    uint id;
    std::string strType;

    xis >> attribute( "id", id )
        >> attribute( "type", strType );

    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( strType );
    if( !pType )
        xis.error( "Unknown automat type" );
            
    MIL_Automate*& pAutomate = automates_[ id ];
    if( pAutomate )
        xis.error( "Automat using this id already exists" );
    pAutomate = (MIL_Automate*)0xdead; // $$$$ NLD 2007-04-04: N'importe quoi
    
    pAutomate = &pType->InstanciateAutomate( id, parent, xis );
    pAutomate->ReadOverloading( xis );
    // $$$ Network
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePion
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_EntityManager::CreatePion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, xml::xistream& xis )
{
    MIL_AgentPion*& pPion = pions_[ nID ];
    if( pPion )
        xis.error( "Pawn using this id already exists" );

    pPion = &type.InstanciatePion( nID, automate, xis );
    pPion->ReadOverloading( xis );

    if( hla_ )
        hla_->Register( *pPion );
    // $$$ Network
    return *pPion;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_EntityManager::CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition )
{
    MIL_AgentPion& pion = type.InstanciatePion( MIL_IDManager::units_.GetFreeSimID(), automate, vPosition );
    assert( pions_[ pion.GetID() ] == 0 );
    pions_[ pion.GetID() ] = &pion;

    if( hla_ )
        hla_->Register( pion );

    pion.SendCreation ();
    pion.SendFullState();
    pion.SendKnowledge();
    return pion;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePopulation
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreatePopulation( xml::xistream& xis, MIL_Army& army )
{
    uint id;
    std::string strType;
    xis >> attribute( "id", id )
        >> attribute( "type", strType );

    const MIL_PopulationType* pType = MIL_PopulationType::Find( strType );
    if( !pType )
        xis.error( "Unknown population type" );

    MIL_Population*& pPopulation = populations_[ id ];
    if( pPopulation )
        xis.error( "Population using this id already exists" );

    pPopulation = &pType->InstanciatePopulation( id, army, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateIntelligence
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateIntelligence( xml::xistream& xis, MIL_Formation& formation )
{
    std::auto_ptr< MIL_Intelligence > intelligence( new MIL_Intelligence( xis, formation ) );
    intelligences_[intelligence->GetId()] = intelligence.get();
    intelligence.release();
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindArmy
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Army* MIL_EntityManager::FindArmy( const std::string& strName ) const
{
    for( CIT_ArmyMap it = armies_.begin(); it != armies_.end(); ++it )
        if( sCaseInsensitiveLess()( it->second->GetName(), strName ) )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateObject( xml::xistream& xis, MIL_Army& army )
{
    assert( pObjectManager_ );
    pObjectManager_->CreateObject( xis, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_RealObject_ABC* MIL_EntityManager::CreateObject( MIL_Army& army, const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint nCurrentParamIdx )
{
    assert( pObjectManager_ );
    return pObjectManager_->CreateObject( army, obstacleType, diaParameters, nCurrentParamIdx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_RealObject_ABC* MIL_EntityManager::CreateObject( const MIL_RealObjectType& type, MIL_Army& army, const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    assert( pObjectManager_ );
    return pObjectManager_->CreateObject( type, army, localisation, strOption, strExtra, rCompletion, rMining, rBypass );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObjectNuageNBC
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_NuageNBC& MIL_EntityManager::CreateObjectNuageNBC( MIL_Army& army, const TER_Localisation& localisation, const MIL_NbcAgentType& nbcAgentType )
{
    assert( pObjectManager_ );
    return pObjectManager_->CreateObjectNuageNBC( army, localisation, nbcAgentType );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObjectZoneeMineeParDispersion
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_ZoneMineeParDispersion& MIL_EntityManager::CreateObjectZoneeMineeParDispersion( MIL_Army& army, const TER_Localisation& localisation, uint nNbrMines )
{
    assert( pObjectManager_ );
    return pObjectManager_->CreateObjectZoneeMineeParDispersion( army, localisation, nNbrMines );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObjectControlZone
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_ControlZone& MIL_EntityManager::CreateObjectControlZone( MIL_Army& army, const TER_Localisation& localisation, MT_Float rRadius )
{
    assert( pObjectManager_ );
    return pObjectManager_->CreateObjectControlZone( army, localisation, rRadius );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_RealObject_ABC* MIL_EntityManager::FindObject( uint nID ) const
{
    assert( pObjectManager_ );
    return pObjectManager_->FindRealObject( nID );
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateKnowledges()
{
    profiler_.Start();

    for( CIT_ArmyMap it = armies_.begin(); it != armies_.end(); ++it )
        it->second->UpdateKnowledges();
    for( CIT_PopulationMap it = populations_.begin(); it != populations_.end(); ++it )
        it->second->UpdateKnowledges();

    for( CIT_ArmyMap it = armies_.begin(); it != armies_.end(); ++it )
        it->second->CleanKnowledges();
    for( CIT_PopulationMap it = populations_.begin(); it != populations_.end(); ++it )
        it->second->CleanKnowledges();

    rKnowledgesTime_ = profiler_.Stop();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateDecisions
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateDecisions()
{
    if( profilerManager_.IsProfilingEnabled() )
    {
        MT_Profiler decisionUpdateProfiler;

        profiler_.Start();
        for( CIT_AutomateMap it = automates_.begin(); it != automates_.end(); ++it )
        {
            decisionUpdateProfiler.Start();
            it->second->UpdateDecision();
            MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( *it->second, decisionUpdateProfiler.Stop() );
        }
        rAutomatesDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        for( CIT_PionMap it = pions_.begin(); it != pions_.end(); ++it )
        {
            decisionUpdateProfiler.Start();
            it->second->UpdateDecision();
            MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( *it->second, decisionUpdateProfiler.Stop() );
        }
        rPionsDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        for( CIT_PopulationMap it = populations_.begin(); it != populations_.end(); ++it )
        {
            decisionUpdateProfiler.Start();
            it->second->UpdateDecision();
            MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( *it->second, decisionUpdateProfiler.Stop() );
        }
        rPopulationsDecisionTime_ = profiler_.Stop();
    }
    else
    {
        profiler_.Start();
        for( CIT_AutomateMap it = automates_.begin(); it != automates_.end(); ++it )
            it->second->UpdateDecision();
        rAutomatesDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        for( CIT_PionMap it = pions_.begin(); it != pions_.end(); ++it )
            it->second->UpdateDecision();
        rPionsDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        for( CIT_PopulationMap it = populations_.begin(); it != populations_.end(); ++it )
            it->second->UpdateDecision();
        rPopulationsDecisionTime_ = profiler_.Stop();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateActions
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateActions()
{
    profiler_.Start();

    for( CIT_AutomateMap it = automates_.begin(); it != automates_.end(); ++it )
        it->second->UpdateActions();

    for( CIT_PionMap it = pions_.begin(); it != pions_.end(); ++it )
        it->second->UpdateActions();

    for( CIT_PopulationMap it = populations_.begin(); it != populations_.end(); ++it )
        it->second->UpdateActions();

    rActionsTime_ = profiler_.Stop();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateEffects
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateEffects()
{
    assert( pObjectManager_ );

    profiler_.Start();
    pObjectManager_->ProcessEvents();
    effectManager_.Update();
    rEffectsTime_ = profiler_.Stop();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateStates
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateStates()
{
    profiler_.Start();

    // !! Automate avant Pions (?? => LOG ??)
    for( CIT_AutomateMap itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
        itAutomate->second->UpdateState();
    for( CIT_PionMap itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        itPion->second->UpdateState();
    for( CIT_PopulationMap itPopulation = populations_.begin(); itPopulation != populations_.end(); ++itPopulation )
        itPopulation->second->UpdateState();

    for( itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        itPion->second->UpdateNetwork();
    for( CIT_AutomateMap itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
        itAutomate->second->UpdateNetwork();
    for( CIT_PopulationMap itPopulation = populations_.begin(); itPopulation != populations_.end(); ++itPopulation )
        itPopulation->second->UpdateNetwork();

    assert( pObjectManager_ );
    pObjectManager_->UpdateStates();

    rStatesTime_ = profiler_.Stop();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::PreprocessRandomBreakdowns()
{
    const uint nCurrentTimeStep = time_.GetCurrentTick();
    if( nRandomBreakdownsNextTimeStep_ > nCurrentTimeStep )
        return;

    while( nRandomBreakdownsNextTimeStep_ <= nCurrentTimeStep )
        nRandomBreakdownsNextTimeStep_ += ( 3600 * 24 / time_.GetTickDuration() );

    for( CIT_PionMap itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        itPion->second->PreprocessRandomBreakdowns( nRandomBreakdownsNextTimeStep_ );
    MT_LOG_INFO_MSG( "Breakdowns preprocessed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Update
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::Update()
{
    PreprocessRandomBreakdowns();

    UpdateKnowledges();
    UpdateDecisions ();
    UpdateActions   ();
    UpdateEffects   ();
    UpdateStates    ();
};

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Clean
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::Clean()
{
    for( CIT_PionMap itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        itPion->second->Clean();

    for( CIT_AutomateMap itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
        itAutomate->second->Clean();

    for( CIT_PopulationMap itPopulation = populations_.begin(); itPopulation != populations_.end(); ++itPopulation )
        itPopulation->second->Clean();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MIL_EntityManager::SendStateToNewClient() const
{
    for( CIT_ArmyMap itArmy = armies_.begin(); itArmy != armies_.end(); ++itArmy )
        itArmy->second->SendCreation();

    for( CIT_ArmyMap itArmy = armies_.begin(); itArmy != armies_.end(); ++itArmy )
        itArmy->second->SendFullState();

    // Knowledge
    for( CIT_ArmyMap itArmy = armies_.begin(); itArmy != armies_.end(); ++itArmy )
        itArmy->second->SendKnowledge();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgUnitOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgUnitOrder( const ASN1T_MsgUnitOrder& asnMsg, uint nCtx )
{
    NET_ASN_MsgUnitOrderAck ack;
    ack().oid        = asnMsg.oid;
    ack().error_code = EnumOrderErrorCode::no_error;

    try
    {
        MIL_AgentPion* pPion = FindAgentPion( asnMsg.oid );
        if( !pPion )
            throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_unit );
        pPion->OnReceiveMsgOrder( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumOrderErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgAutomatOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgAutomatOrder( const ASN1T_MsgAutomatOrder& asnMsg, uint nCtx )
{
    NET_ASN_MsgAutomatOrderAck ack;
    ack().oid        = asnMsg.oid;
    ack().error_code = EnumOrderErrorCode::no_error;

    try
    {
        MIL_Automate* pAutomate = FindAutomate( asnMsg.oid );
        if( !pAutomate )
            throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_unit );
        pAutomate->OnReceiveMsgOrder( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumOrderErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgUnitMagicAction
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgUnitMagicAction( const ASN1T_MsgUnitMagicAction& asnMsg, uint nCtx )
{
    NET_ASN_MsgUnitMagicActionAck ack;
    ack().oid        = asnMsg.oid;
    ack().error_code = EnumUnitErrorCode::no_error;

    try
    {
        if( MIL_Automate*  pAutomate = FindAutomate ( asnMsg.oid ) )
            pAutomate->OnReceiveMsgUnitMagicAction( asnMsg );
        else if( MIL_AgentPion* pPion = FindAgentPion( asnMsg.oid ) )
            pPion->OnReceiveMsgUnitMagicAction( asnMsg );
        else
            throw NET_AsnException< ASN1T_EnumUnitErrorCode >( EnumUnitErrorCode::error_invalid_unit );
    }
    catch( NET_AsnException< ASN1T_EnumUnitErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgPopulationOrder
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgPopulationOrder( const ASN1T_MsgPopulationOrder& asnMsg, uint nCtx )
{
    NET_ASN_MsgPopulationOrderAck ack;
    ack().oid        = asnMsg.oid;
    ack().error_code = EnumOrderErrorCode::no_error;

    try
    {
        MIL_Population* pPopulation = FindPopulation( asnMsg.oid );
        if( !pPopulation )
            throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_unit );
        pPopulation->OnReceiveMsgOrder( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumOrderErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgFragOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgFragOrder( const ASN1T_MsgFragOrder& asnMsg, uint nCtx )
{
    NET_ASN_MsgFragOrderAck ack;
    ack().oid        = asnMsg.oid;
    ack().error_code = EnumOrderErrorCode::no_error;

    try
    {
        if( MIL_Automate* pAutomate = FindAutomate  ( asnMsg.oid ) )
            pAutomate->OnReceiveMsgFragOrder( asnMsg );
        else if( MIL_Population* pPopulation = FindPopulation( asnMsg.oid ) )
            pPopulation->OnReceiveMsgFragOrder( asnMsg );
        else if( MIL_AgentPion* pPion = FindAgentPion ( asnMsg.oid ) )
            pPion->OnReceiveMsgFragOrder( asnMsg );
        else
            throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_unit );
    }
    catch( NET_AsnException< ASN1T_EnumOrderErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgSetAutomateMode
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgSetAutomateMode( const ASN1T_MsgSetAutomatMode& asnMsg, uint nCtx )
{
    NET_ASN_MsgSetAutomatModeAck ack;
    ack().oid        = asnMsg.oid;
    ack().error_code = EnumSetAutomatModeErrorCode::no_error;

    try
    {
        MIL_Automate* pAutomate = FindAutomate( asnMsg.oid );
        if( !pAutomate )
            throw NET_AsnException< ASN1T_EnumSetAutomatModeErrorCode >( EnumSetAutomatModeErrorCode::error_invalid_unit );
        pAutomate->OnReceiveMsgSetAutomateMode( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumSetAutomatModeErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgUnitCreationRequest
// Created: AGE 2007-06-18
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgUnitCreationRequest( const ASN1T_MsgUnitCreationRequest& msg, uint nCtx )
{
    NET_ASN_MsgUnitCreationRequestAck ack;
    ack() = EnumUnitErrorCode::no_error;
    try
    {
        MIL_Automate* pAutomate = FindAutomate( msg.oid_automate );
        if( !pAutomate )
            throw NET_AsnException< ASN1T_EnumUnitErrorCode >( EnumUnitErrorCode::error_invalid_unit );
        pAutomate->OnReceiveMsgUnitCreationRequest( msg );
    }
    catch( NET_AsnException< ASN1T_EnumUnitErrorCode >& e )
    {
        ack() = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgObjectMagicAction
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgObjectMagicAction( const ASN1T_MsgObjectMagicAction& asnMsg, uint nCtx )
{
    assert( pObjectManager_ );
    pObjectManager_->OnReceiveMsgObjectMagicAction( asnMsg, nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgPopulationMagicAction
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgPopulationMagicAction( const ASN1T_MsgPopulationMagicAction& asnMsg, uint nCtx )
{
    NET_ASN_MsgPopulationMagicActionAck ack;
    ack().oid        = asnMsg.oid;
    ack().error_code = EnumPopulationErrorCode::no_error;

    try
    {
        MIL_Population* pPopulation = FindPopulation( asnMsg.oid );
        if( !pPopulation )
            throw NET_AsnException< ASN1T_EnumPopulationErrorCode >( EnumPopulationErrorCode::error_invalid_unit );
        pPopulation->OnReceiveMsgPopulationMagicAction( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumPopulationErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgChangeDiplomacy
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgChangeDiplomacy( const ASN1T_MsgChangeDiplomacy& asnMsg, uint nCtx )
{
    NET_ASN_MsgChangeDiplomacyAck ack;
    ack().oid_camp1   = asnMsg.oid_camp1;
    ack().oid_camp2   = asnMsg.oid_camp2;
    ack().error_code  = EnumChangeDiplomacyErrorCode::no_error;

    try
    {
        MIL_Army* pArmy1 = FindArmy( asnMsg.oid_camp1 );
        if( !pArmy1 )
            throw NET_AsnException< ASN1T_EnumChangeDiplomacyErrorCode >( EnumChangeDiplomacyErrorCode::error_invalid_camp );
        pArmy1->OnReceiveMsgChangeDiplomacy( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumChangeDiplomacyErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgAutomateChangeKnowledgeGroup
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgAutomateChangeKnowledgeGroup( const ASN1T_MsgAutomatChangeKnowledgeGroup& asnMsg, uint nCtx )
{
    NET_ASN_MsgAutomatChangeKnowledgeGroupAck ack;
    ack() = EnumChangeHierarchyErrorCode::no_error;
    try
    {
        MIL_Automate* pAutomate = FindAutomate( asnMsg.oid );
        if( !pAutomate )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_automate );
        pAutomate->OnReceiveMsgChangeKnowledgeGroup( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >& e )
    {
        ack() = e.GetErrorID();
    }
    ack.Send( nCtx );

    if( ack() == EnumChangeHierarchyErrorCode::no_error )
    {
        NET_ASN_MsgAutomatChangeKnowledgeGroup msg;
        msg().oid                     = asnMsg.oid;
        msg().oid_camp                = asnMsg.oid_camp;
        msg().oid_groupe_connaissance = asnMsg.oid_groupe_connaissance;
        msg.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgAutomateChangeLogisticLinks
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgAutomateChangeLogisticLinks( const ASN1T_MsgAutomatChangeLogisticLinks& asnMsg, uint nCtx )
{
    NET_ASN_MsgAutomatChangeLogisticLinksAck ack;
    ack() = EnumChangeHierarchyErrorCode::no_error;
    try
    {
        MIL_Automate* pAutomate = FindAutomate( asnMsg.oid );
        if( !pAutomate )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_automate );
        pAutomate->OnReceiveMsgChangeLogisticLinks( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >& e )
    {
        ack() = e.GetErrorID();
    }
    ack.Send( nCtx );

    if( ack() == EnumChangeHierarchyErrorCode::no_error )
    {
        NET_ASN_MsgAutomatChangeLogisticLinks msg;
        msg().oid                         = asnMsg.oid;
        msg().m.oid_tc2Present            = asnMsg.m.oid_tc2Present;
        msg().m.oid_maintenancePresent    = asnMsg.m.oid_maintenancePresent;
        msg().m.oid_santePresent          = asnMsg.m.oid_santePresent;
        msg().m.oid_ravitaillementPresent = asnMsg.m.oid_ravitaillementPresent;
        msg().oid_tc2                     = asnMsg.oid_tc2;
        msg().oid_maintenance             = asnMsg.oid_maintenance;
        msg().oid_sante                   = asnMsg.oid_sante;
        msg().oid_ravitaillement          = asnMsg.oid_ravitaillement;
        msg.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgAutomateChangeSuperior
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgAutomateChangeSuperior( const ASN1T_MsgAutomatChangeSuperior& asnMsg, uint nCtx )
{
    NET_ASN_MsgAutomatChangeSuperiorAck ack;
    ack() = EnumChangeHierarchyErrorCode::no_error;
    try
    {
        MIL_Automate* pAutomate = FindAutomate( asnMsg.oid );
        if( !pAutomate )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_automate );
        pAutomate->OnReceiveMsgChangeSuperior( asnMsg ); 
    }
    catch( NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >& e )
    {
        ack() = e.GetErrorID();
    }
    ack.Send( nCtx );

    if( ack() == EnumChangeHierarchyErrorCode::no_error )
    {
        NET_ASN_MsgAutomatChangeSuperior msg;
        msg().oid            = asnMsg.oid;
        msg().oid_superior.t = asnMsg.oid_superior.t;
        if( asnMsg.oid_superior.t == T_MsgAutomatChangeSuperior_oid_superior_formation )
            msg().oid_superior.u.formation = asnMsg.oid_superior.u.formation;
        else if( asnMsg.oid_superior.t == T_MsgAutomatChangeSuperior_oid_superior_automate )
            msg().oid_superior.u.automate = asnMsg.oid_superior.u.automate;
        msg.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgUnitChangeSuperior
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgUnitChangeSuperior( const ASN1T_MsgUnitChangeSuperior& asnMsg, uint nCtx )
{
    NET_ASN_MsgUnitChangeSuperiorAck ack;
    ack() = EnumChangeHierarchyErrorCode::no_error;
    try
    {
        MIL_AgentPion* pPion = FindAgentPion( asnMsg.oid );
        if( !pPion )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_pion );
        pPion->OnReceiveMsgChangeSuperior( *this, asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >& e )
    {
        ack() = e.GetErrorID();
    }
    ack.Send( nCtx );

    if( ack() == EnumChangeHierarchyErrorCode::no_error )
    {
        NET_ASN_MsgUnitChangeSuperior msg;
        msg().oid          = asnMsg.oid;
        msg().oid_automate = asnMsg.oid_automate;
        msg.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgLogSupplyChangeQuotas
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgLogSupplyChangeQuotas( const ASN1T_MsgLogSupplyChangeQuotas& asnMsg, uint nCtx )
{
    NET_ASN_MsgLogSupplyChangeQuotasAck ack;
    ack() = MsgLogSupplyChangeQuotasAck::no_error;

    try
    {
        MIL_Automate* pReceiver = FindAutomate( asnMsg.oid_receveur );
        if( !pReceiver )
            throw NET_AsnException< ASN1T_MsgLogSupplyChangeQuotasAck >( MsgLogSupplyChangeQuotasAck::error_invalid_receveur );
        pReceiver->OnReceiveMsgLogSupplyChangeQuotas( asnMsg );
    }
    catch( NET_AsnException< ASN1T_MsgLogSupplyChangeQuotasAck >& e )
    {
        ack() = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgLogSupplyPushFlow
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgLogSupplyPushFlow( const ASN1T_MsgLogSupplyPushFlow&  asnMsg, uint nCtx )
{
    NET_ASN_MsgLogSupplyPushFlowAck ack;
    ack() = MsgLogSupplyPushFlowAck::no_error;

    try
    {
        MIL_Automate* pReceiver = FindAutomate( asnMsg.oid_receveur );
        if( !pReceiver )
            throw NET_AsnException< ASN1T_MsgLogSupplyPushFlowAck >( MsgLogSupplyPushFlowAck::error_invalid_receveur );
        pReceiver->OnReceiveMsgLogSupplyPushFlow( asnMsg );
    }
    catch( NET_AsnException< ASN1T_MsgLogSupplyPushFlowAck >& e )
    {
        ack() = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgIntelligenceCreationRequest
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgIntelligenceCreationRequest( const ASN1T_MsgIntelligenceCreationRequest& message )
{
    MIL_Formation* formation = FindFormation( message.intelligence.formation );
    if( !formation )
    {
        NET_ASN_MsgIntelligenceCreationRequestAck ack;
        ack().error_code = EnumIntelligenceErrorCode::error_invalid_formation;
        ack.Send();
    }
    else
    {
        std::auto_ptr< MIL_Intelligence > intelligence( new MIL_Intelligence( message, *formation ) );
        intelligences_[intelligence->GetId()] = intelligence.get();
        intelligence->Update( message );
        intelligence.release();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgIntelligenceUpdateRequest
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgIntelligenceUpdateRequest( const ASN1T_MsgIntelligenceUpdateRequest& message )
{
    CIT_IntelligenceMap it = intelligences_.find( message.oid );
    if( it != intelligences_.end() )
        it->second->Update( message );
    else
    {
        NET_ASN_MsgIntelligenceCreationRequestAck ack;
        ack().error_code = EnumIntelligenceErrorCode::error_invalid_oid;
        ack.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgIntelligenceDestructionRequest
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgIntelligenceDestructionRequest( const ASN1T_MsgIntelligenceDestructionRequest& message )
{
    T_IntelligenceMap::iterator it = intelligences_.find( message.oid );
    if( it != intelligences_.end() )
    {
        it->second->Update( message );
        delete it->second;
        intelligences_.erase( it );
    }
    else
    {
        NET_ASN_MsgIntelligenceCreationRequestAck ack;
        ack().error_code = EnumIntelligenceErrorCode::error_invalid_oid;
        ack.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ChannelPopulations
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
void MIL_EntityManager::ChannelPopulations( const TER_Localisation& localisation )
{
    for( CIT_PopulationMap it = populations_.begin(); it != populations_.end(); ++it )
        it->second->NotifyChanneled( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::load( MIL_CheckPointInArchive& file, const uint )
{
    // $$$$ NLD 2007-01-11: A voir
    delete pObjectManager_;
    pObjectManager_ = 0;

    file //>> effectManager_  // Effets liés aux actions qui ne sont pas sauvegardés
         >> armies_
         >> formations_
         >> pions_
         >> automates_
         >> populations_
         >> pObjectManager_
         >> rKnowledgesTime_
         >> rAutomatesDecisionTime_
         >> rPionsDecisionTime_
         >> rPopulationsDecisionTime_
         >> rActionsTime_
         >> rEffectsTime_
         >> rStatesTime_
         >> nRandomBreakdownsNextTimeStep_;

    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automates_  .size() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , pions_      .size() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populations_.size() ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file //<< effectManager_  // Effets liés aux actions qui ne sont pas sauvegardés
         << armies_
         << formations_
         << pions_
         << automates_
         << populations_
         << pObjectManager_
         << rKnowledgesTime_
         << rAutomatesDecisionTime_
         << rPionsDecisionTime_
         << rPopulationsDecisionTime_
         << rActionsTime_
         << rEffectsTime_
         << rStatesTime_
         << nRandomBreakdownsNextTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_EntityManager::WriteODB( xml::xostream& xos ) const
{
    xos << start( "orbat" )
            << start( "sides" );
    for( CIT_ArmyMap it = armies_.begin(); it != armies_.end(); ++it )
        it->second->WriteODB( xos );
    xos     << end();

    xos     << start( "diplomacies" );
    for( CIT_ArmyMap it = armies_.begin(); it != armies_.end(); ++it )
        it->second->WriteDiplomacyODB( xos );
    xos     << end();

    xos << end();
}
