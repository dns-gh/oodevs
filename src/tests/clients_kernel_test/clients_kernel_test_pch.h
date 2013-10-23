// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_KERNEL_TEST_PCH_H__
#define CLIENTS_KERNEL_TEST_PCH_H__

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#include <tools/Exception.h>
#include <tools/BoostTest.h>

extern tools::TestOptions testOptions;

#endif // __clients_test_pch_h_
