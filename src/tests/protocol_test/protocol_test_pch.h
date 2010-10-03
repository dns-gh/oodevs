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

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#include <turtle/mock.hpp>

std::string BOOST_RESOLVE( const std::string& filename );

#pragma warning( push, 0 )
#include <boost/date_time/gregorian/gregorian.hpp>
#pragma warning( pop )

#define BOOST_FAIL_ON_DATE( year, month, day )                                                                  \
    const boost::gregorian::date expected( year, month, day );                                                  \
    const boost::gregorian::date actual( boost::gregorian::day_clock::local_day() );                            \
    if( actual >= expected )                                                                                    \
        BOOST_FAIL( "expected date '" + boost::gregorian::to_simple_string( expected ) + "' has been reached" );

#endif // __protocol_test_pch_h_
