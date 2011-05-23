// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_test_pch_h_
#define __protocol_test_pch_h_

#pragma warning( disable : 4996 ) // Function call with parameters that may be unsafe

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#include <turtle/mock.hpp>
#pragma warning( disable: 4505 )

std::string BOOST_RESOLVE( const std::string& filename );

extern unsigned short PORT;

#endif // __protocol_test_pch_h_
