// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_test_pch_h
#define shield_test_pch_h

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#define MOCK_USE_CONVERSIONS
#include <turtle/mock.hpp>
#pragma warning( disable : 4505 )
#pragma warning( disable : 4355 ) // used in base member initializer list

std::string BOOST_RESOLVE( const std::string& filename );

extern unsigned short PORT;

#endif // shield_test_pch_h
