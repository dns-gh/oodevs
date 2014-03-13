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
#include "ArmyFactory.h"
#include "AutomateFactory.h"
#include "FormationFactory.h"
#include "InhabitantFactory.h"
#include "MIL_Army.h"
#include "MIL_Formation.h"
#include "MIL_Time_ABC.h"
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
#include "Agents/Units/Humans/PHY_NbcSuit.h"
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
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "MT_Tools/MT_Logger.h"
#include "Objects/MIL_FireClass.h"
#include "Objects/MIL_NbcAgentType.h"
#include "Objects/MIL_DisasterType.h"
#include "Objects/MIL_ObjectFactory.h"
#include "Objects/MIL_ObjectManager.h"
#include "Objects/MIL_Object_ABC.h"
#include "Orders/MIL_LimaFunction.h"
#include "Orders/MIL_Report.h"
#include "Populations/MIL_PopulationType.h"
#include "Populations/MIL_PopulationAttitude.h"
#include "Populations/MIL_Population.h"
#include "Tools/MIL_IDManager.h"
#include "tools/PhyLoader.h"
#include "Urban/PHY_AccomodationType.h"
#include "Urban/PHY_InfrastructureType.h"
#include "Urban/PHY_MaterialCompositionType.h"
#include "Urban/PHY_ResourceNetworkType.h"
#include "Urban/PHY_RoofShapeType.h"
#include <tools/Path.h>

namespace
{
    // -----------------------------------------------------------------------------
    // Name: MIL_EntityManagerStaticMethods::InitializeType
    // Created: RPD 2010-02-07
    // -----------------------------------------------------------------------------
    template < typename T >
    void InitializeType( const tools::PhyLoader& loader, const std::string& strSection )
    {
        loader.LoadPhysicalFile( strSection.c_str(), &T::Initialize );
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
void MIL_EntityManagerStaticMethods::Initialize( const tools::PhyLoader& loader, const MIL_Time_ABC& time, MIL_ObjectFactory& objectFactory )
{
    // Static types
    PHY_ComposanteState          ::Initialize();
    PHY_HumanRank                ::Initialize();
    PHY_HumanWound               ::Initialize();
    PHY_NbcSuit                  ::Initialize();
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
    MIL_LimaFunction             ::Initialize();

    InitializeType< PHY_MaterialCompositionType    >( loader, "urban"              );
    InitializeType< PHY_AccomodationType           >( loader, "urban"              );
    InitializeType< PHY_InfrastructureType         >( loader, "urban"              );
    InitializeType< PHY_RoofShapeType              >( loader, "urban"              );
    InitializeType< MIL_Report                     >( loader, "reports"            );
    InitializeType< PHY_MaintenanceWorkRate        >( loader, "maintenance"        );
    InitializeType< PHY_MaintenanceResourcesAlarms >( loader, "maintenance"        );
    InitializeType< PHY_Experience                 >( loader, "human-factors"      );
    InitializeType< PHY_Stress                     >( loader, "human-factors"      );
    InitializeType< PHY_Tiredness                  >( loader, "human-factors"      );
    InitializeType< PHY_Volume                     >( loader, "volumes"            );
    InitializeType< PHY_Protection                 >( loader, "protections"        );
    InitializeType< PHY_DotationNature             >( loader, "resource-natures"   );
    InitializeType< PHY_DotationLogisticType       >( loader, "logistic-supply-classes" );
    InitializeType< PHY_DotationType               >( loader, "resources"          );
    InitializeType< PHY_ResourceNetworkType        >( loader, "resource-networks"  );
    InitializeType< MIL_FireClass                  >( loader, "fires"              );
    InitializeType< PHY_BreakdownType              >( loader, "breakdowns"         );
    InitializeType< PHY_LauncherType               >( loader, "launchers"          );
    InitializeType< MIL_DisasterType               >( loader, "disasters"          );
    InitializeType< PHY_ActiveProtection           >( loader, "active-protections" );
    InitializeObjects( loader, objectFactory );
    InitializeSensors( loader, time, objectFactory );
    InitializeWeapons( loader, time );
    InitializeComposantes( loader, time, objectFactory );
    InitializeType< MIL_AgentTypePion              >( loader, "units"              );
    InitializeType< MIL_AutomateType               >( loader, "automats"           );
    InitializeType< MIL_KnowledgeGroupType         >( loader, "knowledge-groups"   );
    InitializeType< MIL_NbcAgentType               >( loader, "nbc"                );
    InitializeType< PHY_SupplyResourcesAlarms      >( loader, "supply"             );
    InitializeType< logistic::SupplyConvoyConfig   >( loader, "supply"             );
    InitializeType< PHY_MedicalResourcesAlarms     >( loader, "health"             );
    InitializeType< PHY_RolePion_Communications    >( loader, "communications"     );
    InitializeType< MIL_PopulationType             >( loader, "populations"        );
    InitializeType< MIL_InhabitantType             >( loader, "inhabitants"        );

    InitializeMedical( loader );
    InitializeFuneral( loader, time );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeObjects
// Created: LGY 2012-09-14
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeObjects( const tools::PhyLoader& loader, MIL_ObjectFactory& objectFactory )
{
    loader.LoadPhysicalFile( "objects", boost::bind( &MIL_ObjectFactory::Initialize, &objectFactory, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeSensors
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeSensors( const tools::PhyLoader& loader, const MIL_Time_ABC& time, const ObjectTypeResolver_ABC& resolver )
{
    MT_LOG_INFO_MSG( "Initializing sensor types" );
    loader.LoadPhysicalFile( "sensors", boost::bind( &MIL_EntityManagerStaticMethods::LoadSensors, _1, boost::cref( time ), boost::cref( resolver ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeFuneral
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeFuneral( const tools::PhyLoader& loader, const MIL_Time_ABC& time )
{
    MT_LOG_INFO_MSG( "Initializing funeral" );
    loader.LoadPhysicalFile( "funeral", boost::bind( &logistic::FuneralConfig::Initialize, _1, time.GetTickDuration() ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::LoadSensors
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::LoadSensors( xml::xistream& xis, const MIL_Time_ABC& time, const ObjectTypeResolver_ABC& resolver )
{
    xis >> xml::start( "sensors" );
    PHY_PerceptionRecoSurveillance::Initialize( xis );
    PHY_PerceptionFlyingShell     ::Initialize( xis );
    PHY_SensorType                ::Initialize( xis, resolver );
    PHY_RadarType                 ::Initialize( xis, time );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeMedical
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeMedical( const tools::PhyLoader& loader )
{
    MT_LOG_INFO_MSG( "Initializing medical data" );
    loader.LoadPhysicalFile( "health", &MIL_EntityManagerStaticMethods::LoadMedical );
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
void MIL_EntityManagerStaticMethods::InitializeComposantes( const tools::PhyLoader& loader, const MIL_Time_ABC& time, const ObjectTypeResolver_ABC& resolver )
{
    loader.LoadPhysicalFile( "components", boost::bind( &PHY_ComposanteTypePion::Initialize, boost::cref( time ), _1, boost::cref( resolver ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManagerStaticMethods::InitializeWeapons
// Created: RPD 2010-02-07
// -----------------------------------------------------------------------------
void MIL_EntityManagerStaticMethods::InitializeWeapons( const tools::PhyLoader& loader, const MIL_Time_ABC& time )
{
    loader.LoadPhysicalFile( "weapon-systems", boost::bind( &PHY_WeaponType::Initialize, boost::cref( time ), _1 ) );
}
