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

BOOST_CLASS_EXPORT_GUID( MIL_EntityManager, "MIL_EntityManager" )

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Initialize
// Created: JVT 2005-03-07
// -----------------------------------------------------------------------------
void MIL_EntityManager::Initialize( MIL_Config& config )
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

    MIL_InputArchive phyArchive;
    phyArchive.Open( config.GetPhysicalFile() );
    phyArchive.Section( "physical" );

    InitializeType< MIL_Report                     >( phyArchive, config, "ComptesRendus"       );
    InitializeType< PHY_MaintenanceWorkRate        >( phyArchive, config, "Maintenance"         );
    InitializeType< PHY_MaintenanceResourcesAlarms >( phyArchive, config, "Maintenance"         );
    InitializeType< PHY_Experience                 >( phyArchive, config, "FacteursHumains"     );
    InitializeType< PHY_Tiredness                  >( phyArchive, config, "FacteursHumains"     );    
    InitializeType< PHY_Volume                     >( phyArchive, config, "Volumes"             );
    InitializeType< PHY_Protection                 >( phyArchive, config, "Protections"         );
    InitializeType< PHY_DotationNature             >( phyArchive, config, "DotationNatures"     );
    InitializeType< PHY_DotationType               >( phyArchive, config, "Dotations"           );
    InitializeType< MIL_RealObjectType             >( phyArchive, config, "Objets"              );
    InitializeType< MIL_VirtualObjectType          >( phyArchive, config, "Objets"              );
    InitializeType< PHY_BreakdownType              >( phyArchive, config, "Pannes"              );
    InitializeType< PHY_LauncherType               >( phyArchive, config, "Lanceurs"            );
    InitializeType< PHY_WeaponType                 >( phyArchive, config, "Armements"           );
    InitializeSensors( phyArchive, config );
    InitializeType< PHY_ComposanteTypePion         >( phyArchive, config, "Composantes"         );
    InitializeType< MIL_AgentTypePion              >( phyArchive, config, "Pions"               );
    InitializeType< MIL_AutomateType               >( phyArchive, config, "Automates"           );
    InitializeType< MIL_KnowledgeGroupType         >( phyArchive, config, "GroupesConnaissance" );
    InitializeType< MIL_NbcAgentType               >( phyArchive, config, "NBC"                 );
    InitializeType< PHY_SupplyResourcesAlarms      >( phyArchive, config, "Ravitaillement"      );   
    InitializeType< PHY_Convoy_ABC                 >( phyArchive, config, "Ravitaillement"      );
    InitializeType< PHY_MedicalResourcesAlarms     >( phyArchive, config, "Sante"               );
    InitializeType< PHY_RolePion_Communications    >( phyArchive, config, "Communications"      );
    InitializeType< MIL_PopulationType             >( phyArchive, config, "Populations"         );
    InitializeMedical( phyArchive, config );

    phyArchive.EndSection(); // physical
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager constructor
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
MIL_EntityManager::MIL_EntityManager()
    : effectManager_                ( *new MIL_EffectManager() )
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
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeSensors
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeSensors( MIL_InputArchive& archive, MIL_Config& config )
{
    MT_LOG_INFO_MSG( "Initializing sensor types" );

    std::string strFile;
    archive.ReadField( "Capteurs", strFile );
    strFile = config.BuildPhysicalChildFile( strFile );

    MIL_InputArchive archiveType;
    archiveType.Open( strFile );
    config.AddFileToCRC( strFile );

    archiveType.Section( "Capteurs" );
    PHY_PerceptionRecoSurveillance::Initialize( archiveType );
    PHY_PerceptionFlyingShell     ::Initialize( archiveType );
    PHY_SensorType                ::Initialize( archiveType );
    PHY_RadarType                 ::Initialize( archiveType );
    archiveType.EndSection(); // Capteurs

    archiveType.Close();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeMedical
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeMedical( MIL_InputArchive& archive, MIL_Config& config )
{
    MT_LOG_INFO_MSG( "Initializing medical data" );

    std::string strFile;
    archive.ReadField( "Sante", strFile );
    strFile = config.BuildPhysicalChildFile( strFile );

    MIL_InputArchive archiveType;
    archiveType.Open( strFile );
    archiveType.Section( "Sante" );
    config.AddFileToCRC( strFile );

    PHY_HumanWound::InitializeMedicalData( archiveType );

    archiveType.EndSection(); // Sante
    archiveType.Close();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeType
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
template < typename T >
void MIL_EntityManager::InitializeType( MIL_InputArchive& archive, MIL_Config& config, const std::string& strSection )
{
    std::string strFile;
    archive.ReadField( strSection, strFile );
    strFile = config.BuildPhysicalChildFile( strFile );

    MIL_InputArchive archiveType;
    archiveType.Open( strFile );
    config.AddFileToCRC( strFile );

    T::Initialize( archiveType );

    archiveType.Close();
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
        
    delete &effectManager_;
    delete pObjectManager_;
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

    MIL_InputArchive odbArchive;
    odbArchive.Open( strOrbat );

    odbArchive.Section( "orbat" );

    InitializeArmies   ( odbArchive );
    InitializeDiplomacy( odbArchive );

    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automates_  .size() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , pions_      .size() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populations_.size() ) );

    odbArchive.EndSection(); // ODB
    odbArchive.Close();

    // Check automate composition
    if( MIL_AgentServer::GetWorkspace().GetConfig().CheckAutomateComposition() )
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
void MIL_EntityManager::InitializeDiplomacy( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing diplomacy" );

    archive.BeginList( "diplomacies" );
    while( archive.NextListElement() )
    {
        archive.BeginList( "side" );

        uint nID;
        archive.ReadAttribute( "id", nID );

        MIL_Army* pArmy = FindArmy( nID );
        if( !pArmy )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown side", archive.GetContext() );
        pArmy->InitializeDiplomacy( archive );

        archive.EndList(); // side
    }
    archive.EndList(); // diplomacies
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeArmies
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeArmies( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing armies" );    

    assert( armies_.empty() );

    archive.BeginList( "sides" );
    while( archive.NextListElement() )
    {
        archive.Section( "side" );

        uint nID;
        archive.ReadAttribute( "id", nID );

        MIL_Army*& pArmy = armies_[ nID ];
        if( pArmy )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Army already exists", archive.GetContext() );
        
        pArmy = new MIL_Army( nID, archive );
        archive.EndSection(); // side
    }
    archive.EndList(); // sides
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateFormation
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Formation& MIL_EntityManager::CreateFormation( uint nID, MIL_Army& army, MIL_InputArchive& archive, MIL_Formation* pParent )
{
    MIL_Formation*& pFormation = formations_[ nID ];
    if( pFormation )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Formation using this id already exists", archive.GetContext() );
    pFormation = new MIL_Formation( nID, army, archive, pParent );
    return *pFormation;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomate
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Automate& MIL_EntityManager::CreateAutomate( const MIL_AutomateType& type, uint nID, MIL_Formation& formation, MIL_InputArchive& archive )
{
    MIL_Automate*& pAutomate = automates_[ nID ];
    if( pAutomate )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automat using this id already exists", archive.GetContext() );
    
    pAutomate = &type.InstanciateAutomate( nID, formation, archive );
    pAutomate->ReadOverloading( archive );
    // $$$ Network
    return *pAutomate;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePion
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_EntityManager::CreatePion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, MIL_InputArchive& archive )
{
    MIL_AgentPion*& pPion = pions_[ nID ];
    if( pPion )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Pawn using this id already exists", archive.GetContext() );

    pPion = &type.InstanciatePion( nID, automate, archive );
    pPion->ReadOverloading( archive );

    if( MIL_AgentServer::GetWorkspace().GetHLAFederate() )
        MIL_AgentServer::GetWorkspace().GetHLAFederate()->Register( *pPion );
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

    if( MIL_AgentServer::GetWorkspace().GetHLAFederate() )
        MIL_AgentServer::GetWorkspace().GetHLAFederate()->Register( pion );

    pion.SendCreation ();
    pion.SendFullState();
    pion.SendKnowledge();
    return pion;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePopulation
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_Population& MIL_EntityManager::CreatePopulation( const MIL_PopulationType& type, uint nID, MIL_Army& army, MIL_InputArchive& archive )
{
    MIL_Population*& pPopulation = populations_[ nID ];
    if( pPopulation )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Population using this id already exists", archive.GetContext() );

    pPopulation = &type.InstanciatePopulation( nID, army, archive );
    return *pPopulation;
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
MIL_RealObject_ABC& MIL_EntityManager::CreateObject( const MIL_RealObjectType& type, uint nID, MIL_Army& army, MIL_InputArchive& archive )
{
    assert( pObjectManager_ );
    return pObjectManager_->CreateObject( type, nID, army, archive );
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
    if( MIL_AgentServer::GetWorkspace().GetProfilerManager().IsProfilingEnabled() )
    {
        MT_Profiler decisionUpdateProfiler;

        profiler_.Start();
        for( CIT_AutomateMap itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
        {
            decisionUpdateProfiler.Start();
            itAutomate->second->UpdateDecision();
            MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( *itAutomate->second, decisionUpdateProfiler.Stop() );
        }
        rAutomatesDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        for( CIT_PionMap itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        {
            decisionUpdateProfiler.Start();
            itPion->second->UpdateDecision();
            MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( *itPion->second, decisionUpdateProfiler.Stop() );
        }
        rPionsDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        for( CIT_PopulationMap itPopulation = populations_.begin(); itPopulation != populations_.end(); ++itPopulation )
        {
            decisionUpdateProfiler.Start();
            itPopulation->second->UpdateDecision();
            MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( *itPopulation->second, decisionUpdateProfiler.Stop() );
        }
        rPopulationsDecisionTime_ = profiler_.Stop();
    }
    else
    {
        profiler_.Start();
        for( CIT_AutomateMap itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
            itAutomate->second->UpdateDecision();
        rAutomatesDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        for( CIT_PionMap itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
            itPion->second->UpdateDecision();
        rPionsDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        for( CIT_PopulationMap itPopulation = populations_.begin(); itPopulation != populations_.end(); ++itPopulation )
            itPopulation->second->UpdateDecision();
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

    for( CIT_AutomateMap itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
        itAutomate->second->UpdateActions();

    for( CIT_PionMap itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        itPion->second->UpdateActions();

    for( CIT_PopulationMap itPopulation = populations_.begin(); itPopulation != populations_.end(); ++itPopulation )
        itPopulation->second->UpdateActions();

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
    const uint nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( nRandomBreakdownsNextTimeStep_ > nCurrentTimeStep )
        return;

    while( nRandomBreakdownsNextTimeStep_ <= nCurrentTimeStep )
        nRandomBreakdownsNextTimeStep_ += ( 3600 * 24 / MIL_AgentServer::GetWorkspace().GetTimeStepDuration() );

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
    ack().oid                     = asnMsg.oid;
    ack().oid_camp                = asnMsg.oid_camp;
    ack().oid_groupe_connaissance = asnMsg.oid_groupe_connaissance;
    ack().error_code              = EnumChangeHierarchyErrorCode::no_error;

    try
    {
        MIL_Automate* pAutomate = FindAutomate( asnMsg.oid );
        if( !pAutomate )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_automate );
        pAutomate->OnReceiveMsgChangeKnowledgeGroup( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgAutomateChangeLogisticLinks
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgAutomateChangeLogisticLinks( const ASN1T_MsgAutomatChangeLogisticLinks& asnMsg, uint nCtx )
{
    NET_ASN_MsgAutomatChangeLogisticLinksAck ack;
    ack().oid                         = asnMsg.oid;
    ack().m.oid_tc2Present            = asnMsg.m.oid_tc2Present;
    ack().m.oid_maintenancePresent    = asnMsg.m.oid_maintenancePresent;
    ack().m.oid_santePresent          = asnMsg.m.oid_santePresent;
    ack().m.oid_ravitaillementPresent = asnMsg.m.oid_ravitaillementPresent;
    ack().oid_tc2                     = asnMsg.oid_tc2;
    ack().oid_maintenance             = asnMsg.oid_maintenance;
    ack().oid_sante                   = asnMsg.oid_sante;
    ack().oid_ravitaillement          = asnMsg.oid_ravitaillement;
    ack().error_code                  = EnumChangeHierarchyErrorCode::no_error;

    try
    {
        MIL_Automate* pAutomate = FindAutomate( asnMsg.oid );
        if( !pAutomate )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_automate );
        pAutomate->OnReceiveMsgChangeLogisticLinks( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgUnitChangeSuperior
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgUnitChangeSuperior( const ASN1T_MsgUnitChangeSuperior& asnMsg, uint nCtx )
{
    NET_ASN_MsgUnitChangeSuperiorAck ack;
    ack().oid          = asnMsg.oid;
    ack().oid_automate = asnMsg.oid_automate;
    ack().error_code   = EnumChangeHierarchyErrorCode::no_error;

    try
    {
        MIL_AgentPion* pPion = FindAgentPion( asnMsg.oid );
        if( !pPion )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_pion );
        pPion->OnReceiveMsgChangeSuperior( asnMsg );
    }
    catch( NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >& e )
    {
        ack().error_code = e.GetErrorID();
    }
    ack.Send( nCtx );
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
// Name: MIL_EntityManager::RegisterLocalAgents
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
void MIL_EntityManager::RegisterLocalAgents( HLA_Federate& federate ) const
{
    for( CIT_PionMap itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
    {
        assert( itPion->second );
        federate.Register( *itPion->second );
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
void MIL_EntityManager::WriteODB( MT_XXmlOutputArchive& archive ) const
{    
    archive.Section( "orbat" );

    archive.Section( "sides" );
    for( CIT_ArmyMap it = armies_.begin(); it != armies_.end(); ++it )
        it->second->WriteODB( archive );
    archive.EndSection(); // sides

    archive.Section( "diplomacies" );
    for( CIT_ArmyMap it = armies_.begin(); it != armies_.end(); ++it )
        it->second->WriteDiplomacyODB( archive );
    archive.EndSection(); // diplomacies

    archive.EndSection(); // orbat
}
