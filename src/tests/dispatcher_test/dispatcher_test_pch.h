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

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#define MOCK_USE_CONVERSIONS
#include <turtle/mock.hpp>
#pragma warning( disable : 4505 )

#include <tools/Path.h>

tools::Path BOOST_RESOLVE( const tools::Path& filename );

#include <tools/Exception.h>

#endif // __dispatcher_kernel_test_pch_h_
