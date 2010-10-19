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
#include "Agents/Units/Composantes/PHY_ActiveProtection.h"
#include "Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Agents/Units/Composantes/PHY_ComposanteState.h"
#include "Agents/Units/Logistic/PHY_MaintenanceWorkRate.h"
#include "Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Agents/Units/Logistic/PHY_MaintenanceLevel.h"
#include "Agents/Units/Logistic/PHY_BreakdownType.h"
#include "Agents/Roles/Logistic/PHY_Convoy_ABC.h"
#include "Agents/Roles/Logistic/PHY_MaintenanceResourcesAlarms.h"
#include "Agents/Roles/Logistic/PHY_MedicalResourcesAlarms.h"
#include "Agents/Roles/Logistic/PHY_SupplyResourcesAlarms.h"
#include "Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Agents/Perceptions/PHY_PerceptionRecoSurveillance.h"
#include "Agents/Perceptions/PHY_PerceptionFlyingShell.h"
#include "Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Agents/MIL_AgentTypePion.h"
#include "Agents/MIL_AgentPion.h"
#include "Automates/MIL_AutomateType.h"
#include "Automates/MIL_Automate.h"
#include "Effects/MIL_EffectManager.h"
#include "hla/HLA_Federate.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "MIL_Army.h"
#include "MIL_Formation.h"
#include "MIL_Singletons.h"
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
#include "simulation_kernel/Decision/DEC_Workspace.h"
#include "simulation_kernel/AgentFactory.h"
#include "simulation_kernel/ArmyFactory.h"
#include "simulation_kernel/AutomateFactory.h"
#include "simulation_kernel/FormationFactory.h"
#include "simulation_kernel/PopulationFactory.h"
#include "simulation_kernel/Knowledge/KnowledgeGroupFactory.h"
#include "simulation_kernel/UrbanType.h"
#include "simulation_kernel/PHY_ResourceNetworkType.h"
#include "tools/MIL_IDManager.h"
#include "tools/MIL_ProfilerMgr.h"
#include "tools/MIL_Tools.h"
#include <xeumeuleu/xml.hpp>

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

    xml::xifstream xis( config.GetPhysicalFile() );
    xis >> xml::start( "physical" );

    InitializeType< UrbanType                      >( xis, config, "urban"              );
    InitializeType< MIL_Report                     >( xis, config, "reports"            );
    InitializeType< PHY_MaintenanceWorkRate        >( xis, config, "maintenance"        );
    InitializeType< PHY_MaintenanceResourcesAlarms >( xis, config, "maintenance"        );
    InitializeType< PHY_Experience                 >( xis, config, "human-factors"      );
    InitializeType< PHY_Tiredness                  >( xis, config, "human-factors"      );
    InitializeType< PHY_Volume                     >( xis, config, "volumes"            );
    InitializeType< PHY_Protection                 >( xis, config, "protections"        );
    InitializeType< PHY_DotationNature             >( xis, config, "dotation-natures"   );
    InitializeType< PHY_DotationType               >( xis, config, "dotations"          );
    InitializeType< PHY_ResourceNetworkType        >( xis, config, "resource-networks"  );
    InitializeType< MIL_ObjectFactory              >( xis, config, "objects"            );
    InitializeType< PHY_BreakdownType              >( xis, config, "breakdowns"         );
    InitializeType< PHY_LauncherType               >( xis, config, "launchers"          );
    InitializeType< PHY_ActiveProtection           >( xis, config, "active-protections" );
    InitializeWeapons    ( xis, config, time );
    InitializeSensors    ( xis, config, time );
    InitializeComposantes( xis, config, time );
    InitializeType< MIL_AgentTypePion              >( xis, config, "units"              );
    InitializeType< MIL_AutomateType               >( xis, config, "automats"           );
    InitializeType< MIL_KnowledgeGroupType         >( xis, config, "knowledge-groups"   );
    InitializeType< MIL_NbcAgentType               >( xis, config, "nbc"                );
    InitializeType< MIL_FireClass                  >( xis, config, "fire"               );
    InitializeType< MIL_MedicalTreatmentType       >( xis, config, "medical-treatment"  );
    InitializeType< PHY_SupplyResourcesAlarms      >( xis, config, "supply"             );
    InitializeType< PHY_Convoy_ABC                 >( xis, config, "supply"             );
    InitializeType< PHY_MedicalResourcesAlarms     >( xis, config, "health"             );
    InitializeType< PHY_RolePion_Communications    >( xis, config, "communications"     );
    InitializeType< MIL_PopulationType             >( xis, config, "populations"        );
    InitializeMedical( xis, config );

    xis >> xml::end; // physical
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeSensors
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeSensors( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time )
{
    MT_LOG_INFO_MSG( "Initializing sensor types" );
    std::string strFile;
    xis >> xml::start( "sensors" )
            >> xml::attribute( "file", strFile )
        >> xml::end;
    strFile = config.BuildPhysicalChildFile( strFile );
    xml::xifstream xisSensors( strFile );
    config.AddFileToCRC( strFile );
    xisSensors >> xml::start( "sensors" );
    PHY_PerceptionRecoSurveillance::Initialize( xisSensors );
    PHY_PerceptionFlyingShell     ::Initialize( xisSensors );
    PHY_SensorType                ::Initialize( xisSensors );
    PHY_RadarType                 ::Initialize( xisSensors, time );
    xisSensors >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeMedical
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeMedical( xml::xistream& xis, MIL_Config& config )
{
    MT_LOG_INFO_MSG( "Initializing medical data" );
    std::string strFile;
    xis >> xml::start( "health" )
            >> xml::attribute( "file", strFile )
        >> xml::end;
    strFile = config.BuildPhysicalChildFile( strFile );
    xml::xifstream xisHealth( strFile );
    xisHealth >> xml::start( "health" );
    config.AddFileToCRC( strFile );
    PHY_HumanWound::InitializeMedicalData( xisHealth );
    xisHealth >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeComposantes
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeComposantes( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time )
{
    std::string strFile;
    xis >> xml::start( "components" )
            >> xml::attribute( "file", strFile )
        >> xml::end;
    strFile = config.BuildPhysicalChildFile( strFile );
    xml::xifstream xisComponents( strFile );
    config.AddFileToCRC( strFile );
    PHY_ComposanteTypePion::Initialize( time, xisComponents );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeWeapons
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeWeapons( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time )
{
    std::string strFile;
    xis >> xml::start( "weapon-systems" )
            >> xml::attribute( "file", strFile )
        >> xml::end;
    strFile = config.BuildPhysicalChildFile( strFile );
    xml::xifstream xisWeapons( strFile );
    config.AddFileToCRC( strFile );
    PHY_WeaponType::Initialize( time, xisWeapons, MIL_AgentServer::GetWorkspace().GetTimeStepDuration() );
}
