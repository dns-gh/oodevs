#include "simulation_kernel_test_pch.h"

#include "AutomateFactory.h"
#include "Fixture.h"
#include "StubMIL_Automate.h"
#include "simulation_kernel/Decision/DEC_DataBase.h"
#include "simulation_kernel/Tools/MIL_IDManager.h"

// -----------------------------------------------------------------------------
// Name: VerifyAutomateFactoryPtr_Serialization
// Created: LDC 2010-02-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyAutomateFactoryPtr_Serialization )
{
    std::stringstream stringstream;
    {
        MIL_IDManager idManager;
        std::vector< std::string > libPath;
        std::vector< const std::string > stuff;
        DEC_DataBase database( libPath, stuff );
        MIL_CheckPointOutArchive outStream( stringstream );
        AutomateFactory_ABC* automateFactory = new AutomateFactory( idManager, database );
        outStream << automateFactory;
        delete automateFactory;
    }
    {
        MIL_CheckPointInArchive inStream( stringstream );
        AutomateFactory_ABC* automateFactory;
        inStream >> automateFactory;
    }
}
