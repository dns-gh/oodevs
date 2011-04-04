// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __mission_tester_test_pch_h_
#define __mission_tester_test_pch_h_

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#pragma warning( disable: 4505 )
#include <turtle/mock.hpp>

std::string BOOST_RESOLVE( const std::string& filename );

#include <boost/date_time/gregorian/gregorian.hpp>
#define BOOST_FAIL_ON_DATE( year, month, day )                                                                  \
    const boost::gregorian::date expected( year, month, day );                                                  \
    const boost::gregorian::date actual( boost::gregorian::day_clock::local_day() );                            \
    if( actual >= expected )                                                                                    \
        BOOST_FAIL( "expected date '" + boost::gregorian::to_simple_string( expected ) + "' has been reached" );

#endif // __mission_tester_test_pch_h_
