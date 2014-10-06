// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "runtime_test_pch.h"

tools::TestOptions testOptions;

::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    testOptions = tools::ParseBoostTestOptions( argc, argv );
    return 0;
}

