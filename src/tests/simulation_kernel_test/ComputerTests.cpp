#include "simulation_kernel_test_pch.h"

#include "simulation_kernel/DefaultTransportPermissionComputer.h"

// -----------------------------------------------------------------------------
// Name: TestDefaultTransportPermissionComputer
// Created: LDC 2009-11-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestDefaultTransportPermissionComputer )
{
    transport::DefaultTransportPermissionComputer computer;
    BOOST_CHECK_EQUAL( true, computer.CanBeLoaded() );
    computer.AllowLoading( true );
    BOOST_CHECK_EQUAL( true, computer.CanBeLoaded() );
    computer.AllowLoading( false );
    BOOST_CHECK_EQUAL( false, computer.CanBeLoaded() );      
}