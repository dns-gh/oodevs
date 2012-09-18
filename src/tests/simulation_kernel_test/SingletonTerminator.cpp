#include "simulation_kernel_test_pch.h"
#include "SingletonTerminator.h"
#include "StubMIL_Time_ABC.h"
#include "simulation_kernel/Entities/Agents/Units/Categories/PHY_Protection.h"
#include "simulation_kernel/Entities/Agents/Units/Categories/PHY_Volume.h"
#include "simulation_kernel/Entities/Agents/Units/Composantes/PHY_ComposanteState.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "simulation_kernel/Entities/Agents/Units/Weapons/PHY_LauncherType.h"
#include "simulation_kernel/Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "simulation_kernel/Entities/Objects/MIL_MedicalTreatmentType.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroupType.h"
#include "simulation_kernel/Urban/PHY_MaterialCompositionType.h"
#include "meteo/PHY_Lighting.h"
#include "simulation_kernel/MIL_Singletons.h"

SingletonTerminator::SingletonTerminator()
    : time_( 0 )
{
}

SingletonTerminator::SingletonTerminator( StubMIL_Time_ABC* time )
    : time_( time )
{
    if( time_ )
        MIL_Singletons::RegisterTime( *time_ );
}

SingletonTerminator::~SingletonTerminator()
{
    PHY_ComposanteState::Terminate();
    PHY_DotationType::Terminate();
    PHY_DotationLogisticType::Terminate();
    PHY_DotationNature::Terminate();
    PHY_Protection::Terminate();
    PHY_IndirectFireDotationClass::Terminate();
    PHY_MaterialCompositionType::Terminate();
    PHY_Volume::Terminate();
    PHY_LauncherType::Terminate();
    PHY_WeaponType::Terminate();
    PHY_ConsumptionType::Terminate();
    MIL_KnowledgeGroupType::Terminate();
    MIL_MedicalTreatmentType::Terminate();
    weather::PHY_Lighting::Terminate();

    if( time_ )
        MIL_Singletons::UnregisterTime( *time_ );
}
