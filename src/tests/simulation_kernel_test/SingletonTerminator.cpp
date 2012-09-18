#include "simulation_kernel_test_pch.h"
#include "SingletonTerminator.h"
#include "StubMIL_Time_ABC.h"
#include "simulation_kernel/Entities/Agents/Units/Categories/PHY_Protection.h"
#include "simulation_kernel/Entities/Agents/Units/Composantes/PHY_ComposanteState.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "simulation_kernel/MIL_Singletons.h"

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
    if( time_ )
        MIL_Singletons::UnregisterTime( *time_ );
}
