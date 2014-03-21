// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __xeuseudeu_test_pch_h_
#define __xeuseudeu_test_pch_h_

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable: 4505 4503 4996 )
#elif defined __GNUC__
#   pragma GCC system_header
#endif

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#include <turtle/mock.hpp>

std::string BOOST_RESOLVE( const std::string& filename );

#include <boost/algorithm/string/predicate.hpp>
#include <boost/bind.hpp>

#define BOOST_CHECK_EXCEPTION_CONTENT( expr, str ) \
    BOOST_CHECK_EXCEPTION( expr, std::exception, \
        boost::bind( &boost::algorithm::contains< const char*, std::string >, \
                     boost::bind( &std::exception::what, _1 ), \
                     str ) );

#ifdef _MSC_VER
#   pragma warning( push )
#endif

#endif // __xeuseudeu_test_pch_h_
