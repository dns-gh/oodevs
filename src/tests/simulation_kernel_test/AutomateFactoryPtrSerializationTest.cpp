#include "simulation_kernel_test_pch.h"
#include "AutomateFactory.h"
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
        MIL_CheckPointOutArchive* out = new MIL_CheckPointOutArchive( s );
        AutomateFactory_ABC* factory = new AutomateFactory( manager, 100, 100, false );
        ( *out ) << factory;
        delete factory;
#ifndef _DEBUG //$$$$ boost + nedmalloc + binary_ioarchive + std::locale = crash
        delete out;
#endif
    }
    {
        MockObjectTypeResolver resolver;
        MIL_CheckPointInArchive* in = new MIL_CheckPointInArchive( s, resolver );
        AutomateFactory_ABC* factory = 0;
        ( *in ) >> factory;
        BOOST_CHECK( factory );
        delete factory;
#ifndef _DEBUG //$$$$ boost + nedmalloc + binary_ioarchive + std::locale = crash
        delete in;
#endif
    }
}
