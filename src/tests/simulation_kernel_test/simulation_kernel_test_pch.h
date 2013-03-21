// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __simulation_kernel_test_pch_h_
#define __simulation_kernel_test_pch_h_

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#pragma warning( disable: 4505 )
#include <turtle/mock.hpp>

#include <tools/Path.h>

tools::Path BOOST_RESOLVE( const tools::Path& filename );

#include <vector>
#include <list>
#include <map>
#include <set>

#undef max
#undef min

#include <tools/Exception.h>

#include <geometry/Types.h>

#endif // __simulation_kernel_test_pch_h_
