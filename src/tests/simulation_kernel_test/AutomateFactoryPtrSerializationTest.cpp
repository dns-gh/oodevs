#include "simulation_kernel_test_pch.h"
#include "AutomateFactory.h"
#include "ActionManager.h"
#include "MissionController.h"
#include "simulation_kernel/Tools/MIL_IDManager.h"
#include "MockObjectTypeResolver.h"

// -----------------------------------------------------------------------------
// Name: VerifyAutomateFactoryPtr_Serialization
// Created: LDC 2010-02-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyAutomateFactoryPtr_Serialization )
{
    const auto world = boost::shared_ptr< TER_World >();
    std::stringstream s;
    {
        MIL_IDManager manager;
        ActionManager actions;
        MissionController controller( actions );
        MIL_CheckPointOutArchive out( s, world );
        AutomateFactory_ABC* factory = new AutomateFactory( manager, controller, 100, 100, false );
        out << factory;
        delete factory;
    }
    {
        MockObjectTypeResolver resolver;
        MIL_CheckPointInArchive in( s, resolver, world );
        AutomateFactory_ABC* factory = 0;
        in >> factory;
        BOOST_CHECK( factory );
        delete factory;
    }
}
