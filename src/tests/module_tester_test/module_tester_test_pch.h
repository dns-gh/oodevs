// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef module_tester_test_pch_h
#define module_tester_test_pch_h

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#pragma warning( disable: 4505 )
#define MOCK_MAX_ARGS 13
#include <turtle/mock.hpp>

std::string BOOST_RESOLVE( const std::string& filename );

void BOOST_CHECK_XML_EQUAL( const std::string& expected, const std::string& actual );

#undef max

#include <tools/Exception.h>

#endif // module_tester_test_pch_h
