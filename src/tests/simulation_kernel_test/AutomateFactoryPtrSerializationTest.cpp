#include "simulation_kernel_test_pch.h"
#include "AutomateFactory.h"
#include "simulation_kernel/Tools/MIL_IDManager.h"

// -----------------------------------------------------------------------------
// Name: VerifyAutomateFactoryPtr_Serialization
// Created: LDC 2010-02-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyAutomateFactoryPtr_Serialization )
{
    std::stringstream s;
    {
        MIL_IDManager manager;
        MIL_CheckPointOutArchive out( s );
        AutomateFactory_ABC* factory = new AutomateFactory( manager, 100, 100 );
        out << factory;
        delete factory;
    }
    {
        MIL_CheckPointInArchive in( s );
        AutomateFactory_ABC* factory = 0;
        in >> factory;
        BOOST_CHECK( factory );
        delete factory;
    }
}
