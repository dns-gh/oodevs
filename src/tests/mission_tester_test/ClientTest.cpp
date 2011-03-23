// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "mission_tester_test_pch.h"
#include "mission_tester/Exercise.h"

BOOST_AUTO_TEST_CASE( An_Exercise_is_created_loading_a_physical_database_with_no_crash )
{
    mission_tester::Exercise exercise( "tests/attrition", "../../data" );
}
