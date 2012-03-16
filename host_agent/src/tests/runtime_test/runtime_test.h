// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef runtime_test_h__
#define runtime_test_h__

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define MOCK_MAX_ARGS 10
#pragma warning( disable: 4505 )
#include <turtle/mock.hpp>

std::string BOOST_RESOLVE( const std::string& filename );

#endif
