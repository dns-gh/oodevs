#include "simulation_kernel_test_pch.h"
#include "SingletonTerminator.h"
#include "simulation_kernel/Entities/Objects/MIL_MedicalTreatmentType.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroupType.h"
#include "meteo/PHY_Lighting.h"
#include "simulation_kernel/Entities/MIL_EntityManager.h"

SingletonTerminator::SingletonTerminator()
{
    // NOTHING
}

SingletonTerminator::~SingletonTerminator()
{
    TerminateMilitarySingletons();
    weather::PHY_Lighting::Terminate();
    TerminatePhysicalSingletons();
}
