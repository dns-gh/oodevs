// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __propagation_test_pch_h_
#define __propagation_test_pch_h_

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#pragma warning( disable : 4505 )
#include <turtle/mock.hpp>

#include "tools/Path.h"

tools::Path BOOST_RESOLVE( const tools::Path& filename );

#include <tools/Exception.h>

#endif // __propagation_test_pch_h_
