#include "simulation_kernel_test_pch.h"
#include "AutomateFactory.h"
#include "MissionController.h"
#include "simulation_kernel/Tools/MIL_IDManager.h"
#include "MockObjectTypeResolver.h"

// -----------------------------------------------------------------------------
// Name: VerifyAutomateFactoryPtr_Serialization
// Created: LDC 2010-02-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyAutomateFactoryPtr_Serialization )
{
    std::stringstream s;
    {
        MIL_IDManager manager;
        MissionController controller;
        MIL_CheckPointOutArchive out( s );
        AutomateFactory_ABC* factory = new AutomateFactory( manager, controller, 100, 100, false );
        out << factory;
        delete factory;
    }
    {
        MockObjectTypeResolver resolver;
        MIL_CheckPointInArchive in( s, resolver );
        AutomateFactory_ABC* factory = 0;
        in >> factory;
        BOOST_CHECK( factory );
        delete factory;
    }
}
