// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/MIL_AgentServer.h"
#include <tools/StdFileWrapper.h>
#include <tools/TemporaryDirectory.h>

BOOST_AUTO_TEST_CASE( FindMaxIdInFileTest )
{
    auto tempDir = tools::TemporaryDirectory( "sim_tests", testOptions.GetTempDir() );
    auto path = tempDir.Path() / "ids";

    // Missing file
    BOOST_CHECK_EQUAL( 0u, FindMaxIdInFile( path ));

    // No identifier
    tools::Ofstream( path ) << "one line\nanother line\nno EOL";
    BOOST_CHECK_EQUAL( 0u, FindMaxIdInFile( path ));

    // Several identifiers
    tools::Ofstream( path ) << "one line\nanother id=\"12\" line\nid=\"13\" and id=\"14\"\nno EOL";
    BOOST_CHECK_EQUAL( 14u, FindMaxIdInFile( path ));
}

