#include "simulation_kernel_test_pch.h"
#include "SingletonTerminator.h"
#include "StubMIL_Time_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_MedicalTreatmentType.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroupType.h"
#include "meteo/PHY_Lighting.h"
#include "simulation_kernel/Entities/MIL_EntityManager.h"
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
    TerminateMilitarySingletons();
    weather::PHY_Lighting::Terminate();
    TerminatePhysicalSingletons();

    if( time_ )
        MIL_Singletons::UnregisterTime( *time_ );
}
