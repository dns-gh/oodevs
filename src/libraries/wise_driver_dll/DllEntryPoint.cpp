// DllEntryPoint.cpp : Defines the entry point for the DLL application.

////////////////////////////////////////////////
// Includes
//
#include "wise_driver_dll_pch.h"
#include "Driver.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/WISEDLLDriverExports.h>
#pragma warning( pop )

DLLDRIVER_API long CreateMarshalledDriverInstance( IWISEGenericStub** ppDriver )
{
    IWISEDriver* pDriver = NULL;

    // Create driver instance...
    WISE_RESULT wResult = CreateDriverInstance( &pDriver );
    if( WISE_SUCCEEDED(wResult) )
    {
        // Create stub and attach it to the driver instance...
        *ppDriver = new CWISEDriverStub( pDriver );
    }
    return wResult;
}

DLLDRIVER_API long DestroyMarshalledDriverInstance( IWISEGenericStub* pDriver )
{
    // Make sure the pointer is actually pointing to a valid driver instance...
    CWISEDriverStub* pStub = dynamic_cast<CWISEDriverStub*>( pDriver );
    if( pStub == NULL )
    {
        return MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_DOES_NOT_EXIST );
    }

    // Extract the driver instance and destroy it
    WISE_RESULT wResult = DestroyDriverInstance( pStub->DetachDriver() );

    // Delete the stub instance...
    delete pStub;
    pStub = NULL;

    return wResult;
}

// Implementation of exported DLL function CreateDriverInstance
// for creating a new instance of the driver.
DLLDRIVER_API long CreateDriverInstance( IWISEDriver** ppDriver )
{
    // Validate the input pointer...
    CHECK_VALID_POINTER( ppDriver, WISE_FACILITY_COM_ADAPTER );

    // Create instance of the driver
    Driver* pSWORDDriver = new Driver();

    // Cast to IWISEDriver interface and return...
    *ppDriver = static_cast<IWISEDriver*>( pSWORDDriver );
    return WISE_OK;
}

// Implementation of exported DLL function DestroyDriverInstance for releasing a driver instance.
DLLDRIVER_API long DestroyDriverInstance( IWISEDriver* pDriver )
{
    // Make sure the pointer is actually pointing to a valid driver instance...
    Driver* pSWORDDriver = dynamic_cast<Driver*>( pDriver );
    if( pSWORDDriver == NULL )
    {
        return MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_DOES_NOT_EXIST );
    }

    // Delete the instance...
    delete pSWORDDriver;
    pSWORDDriver = NULL;

    return WISE_OK;
}
