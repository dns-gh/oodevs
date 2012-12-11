// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef movement_module_test_pch_h
#define movement_module_test_pch_h

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#pragma warning( disable: 4505 )
#define MOCK_MAX_ARGS 13
#include <turtle/mock.hpp>

std::string BOOST_RESOLVE( const std::string& filename );

#include <tools/Exception.h>

#include "ModuleFixture.h"

#endif // movement_module_test_pch_h
