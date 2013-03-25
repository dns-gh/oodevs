// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __dispatcher_kernel_test_pch_h_
#define __dispatcher_kernel_test_pch_h_

#include <boost/test/floating_point_comparison.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#define MOCK_USE_CONVERSIONS
#include <turtle/mock.hpp>
#pragma warning( disable : 4505 4714 )

#include <sqlite/sqlite3.h>
#include <spatialite.h>

#include <tools/Path.h>

extern tools::Path temp_directory;

tools::Path BOOST_RESOLVE( const tools::Path& filename );

#endif // __dispatcher_kernel_test_pch_h_
