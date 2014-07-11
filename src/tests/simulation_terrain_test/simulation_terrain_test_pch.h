// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef SIMULATION_KERNEL_TEST_PCH_H
#define SIMULATION_KERNEL_TEST_PCH_H

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#include <tools/Exception.h>
#include <tools/BoostTest.h>

extern tools::TestOptions testOptions;

#endif // SIMULATION_KERNEL_TEST_PCH_H
