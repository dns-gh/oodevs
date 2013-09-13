// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __tools_test_pch_h_
#define __tools_test_pch_h_

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#pragma warning( disable : 4505 )
#include <turtle/mock.hpp>

#include <tools/Path.h>

tools::Path BOOST_RESOLVE( const tools::Path& filename );

extern unsigned short PORT;
extern tools::Path temp_directory;

void BOOST_CHECK_XML_EQUAL( const std::string& expected, const std::string& actual );

#include <tools/Exception.h>

#endif // __tools_test_pch_h_
