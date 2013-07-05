// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "SingletonTerminator.h"
#include "simulation_kernel/Entities/Objects/MIL_MedicalTreatmentType.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroupType.h"
#include "meteo/PHY_Lighting.h"
#include "simulation_kernel/Entities/MIL_EntityManager.h"

SingletonTerminator::~SingletonTerminator()
{
    TerminateMilitarySingletons();
    weather::PHY_Lighting::Terminate();
    TerminatePhysicalSingletons();
}

BOOST_GLOBAL_FIXTURE( SingletonTerminator )
