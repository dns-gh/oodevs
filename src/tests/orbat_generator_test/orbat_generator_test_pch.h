// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef orbat_generator_test_pch_h
#define orbat_generator_test_pch_h

#include <boost/test/auto_unit_test.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

void BOOST_CHECK_XML_EQUAL( const std::string& expected, const std::string& actual );

#pragma warning( disable : 4505 ) // unreferenced local function has been removed
#pragma warning( disable : 4355 ) // used in base member initializer list

#include "tools/MASA_Exception.h"

#endif // orbat_generator_test_pch_h
