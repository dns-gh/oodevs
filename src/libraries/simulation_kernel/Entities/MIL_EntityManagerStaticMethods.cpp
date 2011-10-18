// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_EntityManagerStaticMethods.h"
#include "AgentFactory.h"
#include "ArmyFactory.h"
#include "AutomateFactory.h"
#include "FormationFactory.h"
#include "InhabitantFactory.h"
#include "MIL_Army.h"
#include "MIL_Formation.h"
#include "MIL_Singletons.h"
#include "PHY_AccomodationType.h"
#include "PHY_InfrastructureType.h"
#include "PHY_MaterialCompositionType.h"
#include "PHY_ResourceNetworkType.h"
#include "PHY_RoofShapeType.h"
#include "PopulationFactory.h"
#include "Agents/MIL_AgentTypePion.h"
#include "Agents/MIL_AgentPion.h"
#include "Agents/Perceptions/PHY_PerceptionRecoSurveillance.h"
#include "Agents/Perceptions/PHY_PerceptionFlyingShell.h"
#include "Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Agents/Roles/Logistic/SupplyConvoyConfig.h"
#include "Agents/Roles/Logistic/PHY_MaintenanceResourcesAlarms.h"
#include "Agents/Roles/Logistic/PHY_MedicalResourcesAlarms.h"
#include "Agents/Roles/Logistic/PHY_SupplyResourcesAlarms.h"
#include "Agents/Roles/Logistic/FuneralConfig.h"
#include "Agents/Units/Categories/PHY_NatureLevel.h"
#include "Agents/Units/Categories/PHY_NatureAtlas.h"
#include "Agents/Units/Categories/PHY_RoePopulation.h"
#include "Agents/Units/Categories/PHY_Volume.h"
#include "Agents/Units/Categories/PHY_Protection.h"
#include "Agents/Units/Composantes/PHY_ActiveProtection.h"
#include "Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Agents/Units/Composantes/PHY_ComposanteState.h"
#include "Agents/Units/Dotations/PHY_DotationType.h"
#include "Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Agents/Units/Dotations/PHY_DotationNature.h"
#include "Agents/Units/Humans/PHY_HumanRank.h"
#include "Agents/Units/Humans/PHY_HumanWound.h"
#include "Agents/Units/HumanFactors/PHY_Experience.h"
#include "Agents/Units/HumanFactors/PHY_Stress.h"
#include "Agents/Units/HumanFactors/PHY_Tiredness.h"
#include "Agents/Units/HumanFactors/PHY_Morale.h"
#include "Agents/Units/Logistic/PHY_MaintenanceWorkRate.h"
#include "Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Agents/Units/Logistic/PHY_MaintenanceLevel.h"
#include "Agents/Units/Logistic/PHY_BreakdownType.h"
#include "Agents/Units/Postures/PHY_Posture.h"
#include "Agents/Units/Radars/PHY_RadarType.h"
#include "Agents/Units/Radars/PHY_RadarClass.h"
#include "Agents/Units/Sensors/PHY_SensorType.h"
#include "Agents/Units/Weapons/PHY_LauncherType.h"
#include "Agents/Units/Weapons/PHY_WeaponType.h"
#include "Automates/MIL_AutomateType.h"
#include "Automates/MIL_Automate.h"
#include "Decision/DEC_Workspace.h"
#include "Effects/MIL_EffectManager.h"
#include "Inhabitants/MIL_InhabitantType.h"
#include "Knowledge/KnowledgeGroupFactory.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "MT_Tools/MT_Logger.h"
#include "Objects/MIL_FireClass.h"
#include "Objects/MIL_MedicalTreatmentType.h"
#include "Objects/MIL_NbcAgentType.h"
#include "Objects/MIL_ObjectManager.h"
#include "Objects/MIL_ObjectFactory.h"
#include "Objects/MIL_Object_ABC.h"
#include "Orders/MIL_LimaFunction.h"
#include "Orders/MIL_Report.h"
#include "Populations/MIL_PopulationType.h"
#include "Populations/MIL_PopulationAttitude.h"
#include "Populations/MIL_Population.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_ProfilerMgr.h"
#include "Tools/MIL_Tools.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    // -----------------------------------------------------------------------------
    // Name: MIL_EntityManagerStaticMethods::InitializeType
    // Created: RPD 2010-02-07
    // -----------------------------------------------------------------------------
    template < typename T >
    void InitializeType( MIL_Config& config, const std::string& strSection )
    {
        const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( strSection, &T::Initialize );
        config.AddFileToCRC( fileLoaded );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods constructor
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
MIL_EntityManagerStaticMethods::MIL_EntityManagerStaticMethods()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods destructor
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
MIL_EntityManagerStaticMethods::~MIL_EntityManagerStaticMethods()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::Initialize
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::Initialize( MIL_Config& config, const MIL_Time_ABC& time )
{
    // Static types
    PHY_ComposanteState          ::Initialize();
    PHY_HumanRank                ::Initialize();
    PHY_HumanWound               ::Initialize();
    PHY_ConsumptionType          ::Initialize();
    PHY_Posture                  ::Initialize();
    PHY_IndirectFireDotationClass::Initialize();
    PHY_AmmoDotationClass        ::Initialize();
    PHY_Morale                   ::Initialize();
    PHY_NatureLevel              ::Initialize();
    PHY_NatureAtlas              ::Initialize();
    PHY_RoePopulation            ::Initialize();
    PHY_MaintenanceLevel         ::Initialize();
    PHY_LogisticLevel            ::Initialize();
    PHY_LogisticLevel            ::Initialize();
    PHY_PerceptionLevel          ::Initialize();
    PHY_RadarClass               ::Initialize();
    MIL_PopulationAttitude       ::Initialize();
    PHY_DotationLogisticType     ::Initialize();
    MIL_LimaFunction             ::Initialize();

    InitializeType< PHY_MaterialCompositionType    >( config, "urban"              );
    InitializeType< PHY_AccomodationType           >( config, "urban"              );
    InitializeType< PHY_InfrastructureType         >( config, "urban"              );
    InitializeType< PHY_RoofShapeType              >( config, "urban"              );
    InitializeType< MIL_Report                     >( config, "reports"            );
    InitializeType< PHY_MaintenanceWorkRate        >( config, "maintenance"        );
    InitializeType< PHY_MaintenanceResourcesAlarms >( config, "maintenance"        );
    InitializeType< PHY_Experience                 >( config, "human-factors"      );
    InitializeType< PHY_Stress                     >( config, "human-factors"      );
    InitializeType< PHY_Tiredness                  >( config, "human-factors"      );
    InitializeType< PHY_Volume                     >( config, "volumes"            );
    InitializeType< PHY_Protection                 >( config, "protections"        );
    InitializeType< PHY_DotationNature             >( config, "resource-natures"   );
    InitializeType< PHY_DotationType               >( config, "resources"          );
    InitializeType< PHY_ResourceNetworkType        >( config, "resource-networks"  );
    InitializeType< MIL_FireClass                  >( config, "fires"              );
    InitializeType< MIL_ObjectFactory              >( config, "objects"            );
    InitializeType< PHY_BreakdownType              >( config, "breakdowns"         );
    InitializeType< PHY_LauncherType               >( config, "launchers"          );
    InitializeType< PHY_ActiveProtection           >( config, "active-protections" );
    InitializeWeapons    ( config, time );
    InitializeSensors    ( config, time );
    InitializeComposantes( config, time );
    InitializeType< MIL_AgentTypePion              >( config, "units"              );
    InitializeType< MIL_AutomateType               >( config, "automats"           );
    InitializeType< MIL_KnowledgeGroupType         >( config, "knowledge-groups"   );
    InitializeType< MIL_NbcAgentType               >( config, "nbc"                );
    InitializeMedicalTreatment( config, time );
    InitializeType< PHY_SupplyResourcesAlarms      >( config, "supply"             );
    InitializeType< logistic::SupplyConvoyConfig   >( config, "supply"             );
    InitializeType< PHY_MedicalResourcesAlarms     >( config, "health"             );
    InitializeType< PHY_RolePion_Communications    >( config, "communications"     );
    InitializeType< MIL_PopulationType             >( config, "populations"        );
    InitializeType< MIL_InhabitantType             >( config, "inhabitants"        );
    InitializeMedical( config );
    InitializeFuneral( config, time );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeSensors
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeSensors( MIL_Config& config, const MIL_Time_ABC& time )
{
    MT_LOG_INFO_MSG( "Initializing sensor types" );
    const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( "sensors", boost::bind( &MIL_EntityManagerStaticMethods::LoadSensors, _1, boost::cref( time ) ) );
    config.AddFileToCRC( fileLoaded );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeFuneral
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeFuneral( MIL_Config& config, const MIL_Time_ABC& time )
{
    //$$$ NLD - 20011017 - SCIPIO STUPID THALES ICD - TO BE REMOVED IN 4.3.4
    const std::string childFileName( config.BuildPhysicalChildFile( "Funeral.xml" ) );
    config.GetLoader().LoadFile( childFileName, boost::bind( &logistic::FuneralConfig::Initialize, _1, time.GetTickDuration() ) );
    config.AddFileToCRC( childFileName );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::LoadSensors
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::LoadSensors( xml::xistream& xis, const MIL_Time_ABC& time )
{
    xis >> xml::start( "sensors" );
    PHY_PerceptionRecoSurveillance::Initialize( xis );
    PHY_PerceptionFlyingShell     ::Initialize( xis );
    PHY_SensorType                ::Initialize( xis );
    PHY_RadarType                 ::Initialize( xis, time );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeMedical
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeMedical( MIL_Config& config )
{
    MT_LOG_INFO_MSG( "Initializing medical data" );
    const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( "health", &MIL_EntityManagerStaticMethods::LoadMedical );
    config.AddFileToCRC( fileLoaded );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::LoadMedical
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::LoadMedical( xml::xistream& xis )
{
    xis >> xml::start( "health" );
    PHY_HumanWound::InitializeMedicalData( xis );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeComposantes
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeComposantes( MIL_Config& config, const MIL_Time_ABC& time )
{
    const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( "components", boost::bind( &PHY_ComposanteTypePion::Initialize, boost::cref( time ), _1 ) );
    config.AddFileToCRC( fileLoaded );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeWeapons
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeWeapons( MIL_Config& config, const MIL_Time_ABC& time )
{
    const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( "weapon-systems", boost::bind( &PHY_WeaponType::Initialize, boost::cref( time ), _1 ) );
    config.AddFileToCRC( fileLoaded );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeMedicalTreatment
// Created: JCR 2010-10-26
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeMedicalTreatment( MIL_Config& config, const MIL_Time_ABC& time )
{
    const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( "medical-treatment", boost::bind( &MIL_MedicalTreatmentType::Initialize, _1, boost::cref( time ) ) );
    config.AddFileToCRC( fileLoaded );
}
