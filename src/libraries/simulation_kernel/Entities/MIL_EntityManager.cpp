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
#include "Objects/MIL_ObjectFactory.h"
#include "Objects/MIL_Object_ABC.h"
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
#include "simulation_kernel/Decision/DEC_Workspace.h"

#include "simulation_kernel/AgentFactory.h"
#include "simulation_kernel/ArmyFactory.h"
#include "simulation_kernel/AutomateFactory.h"
#include "simulation_kernel/FormationFactory.h"
#include "simulation_kernel/PopulationFactory.h"


#include "Tools/MIL_IDManager.h"

#include "Objects/MIL_FireClass.h"
#include "Objects/MIL_NbcAgentType.h"
#include "Objects/MIL_MedicalTreatmentType.h"

#include "Populations/MIL_PopulationType.h"
#include "Populations/MIL_PopulationAttitude.h"
#include "Populations/MIL_Population.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "Orders/MIL_LimaFunction.h"
#include "Orders/MIL_Report.h"
#include "Tools/MIL_ProfilerMgr.h"
#include "MIL_Army.h"
#include "MIL_Formation.h"
#include "Network/NET_ASN_Messages.h"
#include "HLA/HLA_Federate.h"
#include "Network/NET_AsnException.h"
#include "MIL_Singletons.h"
#include <xeumeuleu/xml.h>



BOOST_CLASS_EXPORT_GUID( MIL_EntityManager, "MIL_EntityManager" )

MIL_EntityManager* MIL_EntityManager::singleton_ = 0;

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_EntityManager* role, const unsigned int /*version*/ )
{
    //@TODO MGD work on serialization to avoid singleton and add test for all entities
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_EntityManager* role, const unsigned int /*version*/ )
{
    DEC_DataBase* database;
    archive >> database;
    ::new( role )MIL_EntityManager( MIL_Singletons::GetTime(), MIL_Singletons::GetEffectManager(), MIL_Singletons::GetProfiler(), MIL_Singletons::GetHla(), MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().GetDatabase() );
}

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
    xis >> xml::start( "physical" );

    InitializeType< MIL_Report                     >( xis, config, "reports"             );
    InitializeType< PHY_MaintenanceWorkRate        >( xis, config, "maintenance"         );
    InitializeType< PHY_MaintenanceResourcesAlarms >( xis, config, "maintenance"         );
    InitializeType< PHY_Experience                 >( xis, config, "human-factors"       );
    InitializeType< PHY_Tiredness                  >( xis, config, "human-factors"       );    
    InitializeType< PHY_Volume                     >( xis, config, "volumes"             );
    InitializeType< PHY_Protection                 >( xis, config, "protections"         );
    InitializeType< PHY_DotationNature             >( xis, config, "dotation-natures"    );
    InitializeType< PHY_DotationType               >( xis, config, "dotations"           );
    InitializeType< MIL_ObjectFactory              >( xis, config, "objects"             );
    InitializeType< PHY_BreakdownType              >( xis, config, "breakdowns"          );
    InitializeType< PHY_LauncherType               >( xis, config, "launchers"           );
    InitializeWeapons    ( xis, config, time, effects );
    InitializeSensors    ( xis, config, time );
    InitializeComposantes( xis, config, time );
    InitializeType< MIL_AgentTypePion              >( xis, config, "units"               );
    InitializeType< MIL_AutomateType               >( xis, config, "automats"            );
    InitializeType< MIL_KnowledgeGroupType         >( xis, config, "knowledge-groups"    );
    InitializeType< MIL_NbcAgentType               >( xis, config, "nbc"                 );
    InitializeType< MIL_FireClass                  >( xis, config, "fire"                );
    InitializeType< MIL_MedicalTreatmentType       >( xis, config, "medical-treatment"   );
    InitializeType< PHY_SupplyResourcesAlarms      >( xis, config, "supply"              );   
    InitializeType< PHY_Convoy_ABC                 >( xis, config, "supply"              );
    InitializeType< PHY_MedicalResourcesAlarms     >( xis, config, "health"              );
    InitializeType< PHY_RolePion_Communications    >( xis, config, "communications"      );
    InitializeType< MIL_PopulationType             >( xis, config, "populations"         );
    InitializeMedical( xis, config );

    xis >> xml::end(); // physical  
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager constructor
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
MIL_EntityManager::MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, MIL_ProfilerMgr& profiler, HLA_Federate* hla, DEC_DataBase& database )
    : time_                         ( time )
    , profilerManager_              ( profiler )
    , hla_                          ( hla )
    , effectManager_                ( effects )
    , nRandomBreakdownsNextTimeStep_( 0  )
    , rKnowledgesTime_              ( 0. )
    , rAutomatesDecisionTime_       ( 0. )
    , rPionsDecisionTime_           ( 0. )
    , rPopulationsDecisionTime_     ( 0. )
    , rActionsTime_                 ( 0. )
    , rEffectsTime_                 ( 0. )
    , rStatesTime_                  ( 0. )
    , idManager_                    ( new MIL_IDManager() )
    , pObjectManager_               ( new MIL_ObjectManager() )
    , populationFactory_            ( new PopulationFactory( database ) )          
    , agentFactory_                 ( new AgentFactory( *idManager_, database ) )
    , automateFactory_              ( new AutomateFactory( *idManager_, database ) )
    , formationFactory_             ( new FormationFactory( *automateFactory_ ) )
    , armyFactory_                  ( new ArmyFactory( *automateFactory_, *formationFactory_, *pObjectManager_, *populationFactory_) )
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
    xis >> xml::start( "sensors" )
            >> xml::attribute( "file", strFile )
        >> xml::end();
    strFile = config.BuildPhysicalChildFile( strFile );


    xml::xifstream xisSensors( strFile );
    config.AddFileToCRC( strFile );

    xisSensors >> xml::start( "sensors" );
    PHY_PerceptionRecoSurveillance::Initialize( xisSensors );
    PHY_PerceptionFlyingShell     ::Initialize( xisSensors );
    PHY_SensorType                ::Initialize( xisSensors );
    PHY_RadarType                 ::Initialize( xisSensors, time );
    xisSensors >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeMedical
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeMedical( xml::xistream& xis, MIL_Config& config )
{
    MT_LOG_INFO_MSG( "Initializing medical data" );

    std::string strFile;
    xis >> xml::start( "health" )
            >> xml::attribute( "file", strFile )
        >> xml::end();
    strFile = config.BuildPhysicalChildFile( strFile );

    xml::xifstream xisHealth( strFile );

    xisHealth >> xml::start( "health" );
    config.AddFileToCRC( strFile );

    PHY_HumanWound::InitializeMedicalData( xisHealth );

    xisHealth >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeType
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
template < typename T >
void MIL_EntityManager::InitializeType( xml::xistream& xis, MIL_Config& config, const std::string& strSection )
{
    std::string strFile;
    xis >> xml::start( strSection )
            >> xml::attribute( "file", strFile )
        >> xml::end();

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
    xis >> xml::start( "components" )
            >> xml::attribute( "file", strFile )
        >> xml::end();
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
    xis >> xml::start( "weapon-systems" )
            >> xml::attribute( "file", strFile )
        >> xml::end();
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
    tools::Resolver< MIL_AgentPion >::DeleteAll();//@TODO move resolver in factory
    automateFactory_->DeleteAll();
    formationFactory_->DeleteAll(); //@TODO test if can be done in destructor or maintain this
    armyFactory_->DeleteAll();
    populationFactory_->DeleteAll();

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
    MIL_NbcAgentType              ::Terminate();
    MIL_FireClass                 ::Terminate();
    MIL_MedicalTreatmentType      ::Terminate();
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
    xis >> xml::start( "orbat" );

    InitializeArmies   ( xis );
    InitializeDiplomacy( xis );

    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automateFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , tools::Resolver< MIL_AgentPion >::Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populationFactory_->Count() ) );//@TODO MGD maybe add more informations
    
    xis >> xml::end();

    // Check automate composition
    if( config.CheckAutomateComposition() )
    {//@TODO MGD move exception in called function
        for( tools::Iterator< const MIL_Automate& > it = automateFactory_->CreateIterator(); it.HasMoreElements(); )
        {
            const MIL_Automate& automate = it.NextElement();
            if( !automate.CheckComposition() )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "The effective composition of the automate '%d' ('%s') is not consistent with the composition described in the type '%s'", automate.GetID(), automate.GetName().c_str(), automate.GetType().GetName().c_str() ) );
        }
    }

    // Disengage automata for frozen mode
    if( config.IsFrozenMode() )
    {
        automateFactory_->Apply( boost::bind( &MIL_Automate::Disengage, _1 ) );
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

    xis >> xml::start( "diplomacies" )
            >> xml::list( "side", *this, &MIL_EntityManager::ReadDiplomacy )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadDiplomacy
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadDiplomacy( xml::xistream& xis )
{
    uint id;
    xis >> xml::attribute( "id", id );

    MIL_Army* pArmy = armyFactory_->Find( id );
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

    assert( armyFactory_->Count() == 0 );

    xis >> xml::start( "sides" )
        >> xml::list( "side", boost::bind( &ArmyFactory_ABC::Create, boost::ref( *armyFactory_ ), _1 ) )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomat
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateAutomat( xml::xistream& xis, MIL_Formation& formation )
{
    //@TODO MGD Remove
    automateFactory_->Create( xis, formation );
    // $$$ Network
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomat
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateAutomat( xml::xistream& xis, MIL_Automate& parent )
{
    //@TODO MGD Remove
    automateFactory_->Create( xis, parent );
    // $$$ Network
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePion
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_EntityManager::CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis )
{
    MIL_AgentPion* pPion = agentFactory_->Create( type, automate, xis );
    tools::Resolver< MIL_AgentPion >::Register( pPion->GetID(), *pPion );

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
    MIL_AgentPion* pPion = agentFactory_->Create( type, automate, vPosition );
    tools::Resolver< MIL_AgentPion >::Register( pPion->GetID(), *pPion );

    if( hla_ )
        hla_->Register( *pPion );

    pPion->SendCreation ();
    pPion->SendFullState();
    pPion->SendKnowledge();
    return *pPion;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateObject( xml::xistream& xis, MIL_Army_ABC& army )
{
    //@TODO MGD REMOVE
    assert( pObjectManager_ );
    pObjectManager_->CreateObject( xis, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, ASN1T_EnumDemolitionTargetType obstacleType )
{
    assert( pObjectManager_ );
    return pObjectManager_->CreateObject( army, type, pLocalisation, obstacleType );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation )
{
    assert( pObjectManager_ );
    return pObjectManager_->CreateObject( type, army, localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( MIL_Army_ABC& army, const MIL_ObjectBuilder_ABC& builder )
{
    return pObjectManager_->CreateObject( army, builder );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( const std::string& /*type*/, MIL_Army_ABC& /*army*/, const TER_Localisation& /*localisation*/, const std::string& /*strOption*/, const std::string& /*strExtra*/, double /*rCompletion*/, double /*rMining*/, double /*rBypass*/ )
{
    throw std::exception( __FUNCTION__ " not implemented" );    
    // return pObjectManager_->CreateObject( type, army, localisation, strOption, strExtra, rCompletion, rMining, rBypass );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::FindObject( uint nID ) const
{
    assert( pObjectManager_ );
    return pObjectManager_->Find( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindObjectType
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_EntityManager::FindObjectType( const std::string& type ) const
{
    assert( pObjectManager_ );
    return pObjectManager_->FindType( type );
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

    armyFactory_->Apply( boost::bind( &MIL_Army::UpdateKnowledges, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateKnowledges, _1 ) );

    armyFactory_->Apply( boost::bind( &MIL_Army::CleanKnowledges, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::CleanKnowledges, _1 ) );

    rKnowledgesTime_ = profiler_.Stop();
}

namespace
{
    void UpdateAutomate( MT_Profiler& profiler, float duration, MIL_Automate& automate )
    {
        profiler.Start();
        automate.UpdateDecision( duration );
        MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( automate, profiler.Stop() );
    }
    void UpdatePion( MT_Profiler& profiler, float duration, MIL_AgentPion& pion )
    {
        profiler.Start();
        pion.UpdateDecision( duration );
        MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( pion, profiler.Stop() );
    }
    void UpdatePopulation( MT_Profiler& profiler, float duration, MIL_Population& population )
    {
        profiler.Start();
        population.UpdateDecision( duration );
        MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( population, profiler.Stop() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateDecisions
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateDecisions()
{
    float duration = (float)MIL_AgentServer::GetWorkspace().GetTimeStepDuration();
    if( profilerManager_.IsProfilingEnabled() )
    {
        MT_Profiler decisionUpdateProfiler;

        profiler_.Start();
        automateFactory_->Apply( boost::bind( &UpdateAutomate, boost::ref(decisionUpdateProfiler), duration, _1 ) );
        rAutomatesDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        tools::Resolver< MIL_AgentPion >::Apply( boost::bind( &UpdatePion, boost::ref(decisionUpdateProfiler), duration, _1 ) );
        rPionsDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        populationFactory_->Apply( boost::bind( &UpdatePopulation, boost::ref(decisionUpdateProfiler), duration, _1 ) );
        rPopulationsDecisionTime_ = profiler_.Stop();
    }
    else
    {
        profiler_.Start();
        automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateDecision, _1, duration ) );
        rAutomatesDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        tools::Resolver< MIL_AgentPion >::Apply( boost::bind( &MIL_AgentPion::UpdateDecision, _1, duration ) );
        rPionsDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        populationFactory_->Apply( boost::bind( &MIL_Population::UpdateDecision, _1, duration ) );
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

    automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateActions, _1 ) );
    tools::Resolver< MIL_AgentPion >::Apply( boost::bind( &MIL_AgentPion::UpdateActions, _1 ) );

    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateActions, _1 ) );

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
    automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateState, _1 ) );
    tools::Resolver< MIL_AgentPion >::Apply( boost::bind( &MIL_AgentPion::UpdateState, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateState, _1 ) );

    automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateNetwork, _1 ) );
    tools::Resolver< MIL_AgentPion >::Apply( boost::bind( &MIL_AgentPion::UpdateNetwork, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateNetwork, _1 ) );

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

    tools::Resolver< MIL_AgentPion >::Apply( boost::bind( &MIL_AgentPion::PreprocessRandomBreakdowns, _1, nRandomBreakdownsNextTimeStep_ ) );
    MT_LOG_INFO_MSG( "Breakdowns preprocessed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Update
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::Update()
{
    PreprocessRandomBreakdowns();
    if( !MIL_AgentServer::GetWorkspace().GetConfig().IsFrozenMode() )
    {
        UpdateKnowledges();
        UpdateDecisions ();
    }
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
    tools::Resolver< MIL_AgentPion >::Apply( boost::bind( &MIL_AgentPion::Clean, _1 ) );
    automateFactory_->Apply( boost::bind( &MIL_Automate::Clean, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::Clean, _1 ) );
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
    armyFactory_->Apply( boost::bind( &MIL_Army::SendCreation, _1 ) );
    armyFactory_->Apply( boost::bind( &MIL_Army::SendFullState, _1 ) );
    armyFactory_->Apply( boost::bind( &MIL_Army::SendKnowledge, _1 ) );
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
            pAutomate->OnReceiveMsgUnitMagicAction( asnMsg, *armyFactory_ );
        else if( MIL_AgentPion* pPion = FindAgentPion( asnMsg.oid ) )
            pPion->OnReceiveMsgUnitMagicAction( asnMsg, *armyFactory_ );
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
        MIL_Population* pPopulation = populationFactory_->Find( asnMsg.oid );
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
        else if( MIL_Population* pPopulation = populationFactory_->Find( asnMsg.oid ) )
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
    pObjectManager_->OnReceiveMsgObjectMagicAction( asnMsg, nCtx, *armyFactory_ );
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
        MIL_Population* pPopulation = populationFactory_->Find( asnMsg.oid );
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
        MIL_Army* pArmy1 = armyFactory_->Find( asnMsg.oid_camp1 );
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
        pAutomate->OnReceiveMsgChangeKnowledgeGroup( asnMsg, *armyFactory_ );
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
        pAutomate->OnReceiveMsgChangeSuperior( asnMsg, *formationFactory_ ); 
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
// Name: MIL_EntityManager::ChannelPopulations
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
void MIL_EntityManager::ChannelPopulations( const TER_Localisation& localisation )
{
    populationFactory_->Apply( boost::bind( &MIL_Population::NotifyChanneled, _1, localisation ) );
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

    ArmyFactory * armyFactory;
    FormationFactory * formationFactory;
    AutomateFactory * automateFactory;
    PopulationFactory * populationFactory;
    file //>> effectManager_  // Effets liés aux actions qui ne sont pas sauvegardés
         >> armyFactory
         >> formationFactory//@TODO MGD serialize
         >> tools::Resolver< MIL_AgentPion >::elements_
         >> automateFactory
         >> populationFactory
         >> pObjectManager_
         >> rKnowledgesTime_
         >> rAutomatesDecisionTime_
         >> rPionsDecisionTime_
         >> rPopulationsDecisionTime_
         >> rActionsTime_
         >> rEffectsTime_
         >> rStatesTime_
         >> nRandomBreakdownsNextTimeStep_;

    armyFactory_.reset( armyFactory );
    formationFactory_.reset( formationFactory );
    automateFactory_.reset( automateFactory );
    populationFactory_.reset( populationFactory );

    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automateFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , tools::Resolver< MIL_AgentPion >::Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populationFactory_->Count() ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    const ArmyFactory_ABC * const tempArmy = armyFactory_.get();
    const FormationFactory_ABC * const tempFormationFactory = formationFactory_.get();//@TODO MGD See to move std::auto_ptr serialization in an include file in tools
    const AutomateFactory_ABC * const tempAutomateFactory = automateFactory_.get();
    const PopulationFactory_ABC * const populationFactory = populationFactory_.get();

    file //<< effectManager_  // Effets liés aux actions qui ne sont pas sauvegardés
         << tempArmy
         << tempFormationFactory
         << tools::Resolver< MIL_AgentPion >::elements_
         << tempAutomateFactory
         << populationFactory
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
    xos << xml::start( "orbat" )
            << xml::start( "sides" );
                armyFactory_->Apply( boost::bind( &MIL_Army::WriteODB, _1, boost::ref( xos ) ) );
    xos     << xml::end();

    xos     << xml::start( "diplomacies" );
                armyFactory_->Apply( boost::bind( &MIL_Army::WriteDiplomacyODB, _1, boost::ref( xos ) ) );
    xos     << xml::end();

    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindAutomate
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_Automate* MIL_EntityManager::FindAutomate( uint nID ) const
{//@TODO MGD Remove
    return automateFactory_->Find( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindAgentPion
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_EntityManager::FindAgentPion( uint nID ) const
{
    return tools::Resolver< MIL_AgentPion >::Find( nID );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetArmies
// Created: NLD 2004-09-01 //@TODO MGD just use on time in object to destroy knowledge, find a way to remove this function
// -----------------------------------------------------------------------------
const tools::Resolver< MIL_Army >& MIL_EntityManager::GetArmies() const
{
    return *armyFactory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetKnowledgesTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetKnowledgesTime() const
{
    return rKnowledgesTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetDecisionsTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetDecisionsTime() const
{
    return rAutomatesDecisionTime_ + rPionsDecisionTime_ + rPopulationsDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetAutomatesDecisionTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetAutomatesDecisionTime() const
{
    return rAutomatesDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetPionsDecisionTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetPionsDecisionTime() const
{
    return rPionsDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetPopulationsDecisionTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetPopulationsDecisionTime() const
{
    return rPopulationsDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetActionsTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetActionsTime() const
{
    return rActionsTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetEffectsTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetEffectsTime() const
{
    return rEffectsTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetStatesTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetStatesTime() const
{
    return rStatesTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetEffectManager
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
MIL_EffectManager& MIL_EntityManager::GetEffectManager() const
{
    return effectManager_;
}