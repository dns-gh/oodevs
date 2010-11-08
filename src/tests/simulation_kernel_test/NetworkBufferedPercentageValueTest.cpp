// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "Network/NetworkBufferedPercentageValue.h"

BOOST_AUTO_TEST_CASE( NetworkBufferedPercentageValueTestDouble )
{
    NetworkBufferedPercentageValue< double > test1( 0.5, 0.05, 0., 1. );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );
    BOOST_CHECK_EQUAL( 0.5, test1.Get() );
    BOOST_CHECK_EQUAL( 0.5, test1.Send() );

    BOOST_CHECK_EQUAL( false, test1.Set( 0.51 ) );
    BOOST_CHECK_EQUAL( false, test1.NeedToBeSent() );
    BOOST_CHECK_EQUAL( 0.51, test1.Get() );

    BOOST_CHECK_EQUAL( true, test1.Set( 0.55 ) );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );
    BOOST_CHECK_EQUAL( 0.55, test1.Get() );
    BOOST_CHECK_EQUAL( 0.55, test1.Send() );
    BOOST_CHECK_EQUAL( false, test1.NeedToBeSent() );
       
    BOOST_CHECK_EQUAL( true, test1.Set( 0.75 ) );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );
    BOOST_CHECK_EQUAL( 0.75, test1.Get() );
    BOOST_CHECK_EQUAL( 0.75, test1.Send() );
    BOOST_CHECK_EQUAL( false, test1.NeedToBeSent() );

    BOOST_CHECK_EQUAL( false, test1.Set( 0.76 ) );
    BOOST_CHECK_EQUAL( false, test1.NeedToBeSent() );
    BOOST_CHECK_EQUAL( 0.76, test1.Get() );
    BOOST_CHECK_EQUAL( 0.76, test1.Send() );

    BOOST_CHECK_EQUAL( false, test1.Set( 0.80 ) );
    BOOST_CHECK_EQUAL( false, test1.NeedToBeSent() );

    BOOST_CHECK_EQUAL( true, test1.Set( 0.81 ) );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );

    test1.Set( 0.99 );
    test1.Send();
    BOOST_CHECK_EQUAL( true, test1.Set( 1. ) );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );

    test1.Set( 0.01 );
    test1.Send();
    BOOST_CHECK_EQUAL( true, test1.Set( 0. ) );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );
}

BOOST_AUTO_TEST_CASE( NetworkBufferedPercentageValueTestInt )
{
    NetworkBufferedPercentageValue< int > test1( 50, 5, 0, 100 );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );
    BOOST_CHECK_EQUAL( 50, test1.Get() );
    BOOST_CHECK_EQUAL( 50, test1.Send() );

    BOOST_CHECK_EQUAL( false, test1.Set( 51 ) );
    BOOST_CHECK_EQUAL( false, test1.NeedToBeSent() );
    BOOST_CHECK_EQUAL( 51, test1.Get() );

    BOOST_CHECK_EQUAL( true, test1.Set( 55 ) );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );
    BOOST_CHECK_EQUAL( 55, test1.Get() );
    BOOST_CHECK_EQUAL( 55, test1.Send() );
    BOOST_CHECK_EQUAL( false, test1.NeedToBeSent() );
       
    BOOST_CHECK_EQUAL( true, test1.Set( 75 ) );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );
    BOOST_CHECK_EQUAL( 75, test1.Get() );
    BOOST_CHECK_EQUAL( 75, test1.Send() );
    BOOST_CHECK_EQUAL( false, test1.NeedToBeSent() );

    BOOST_CHECK_EQUAL( false, test1.Set( 76 ) );
    BOOST_CHECK_EQUAL( false, test1.NeedToBeSent() );
    BOOST_CHECK_EQUAL( 76, test1.Get() );
    BOOST_CHECK_EQUAL( 76, test1.Send() );

    BOOST_CHECK_EQUAL( false, test1.Set( 80 ) );
    BOOST_CHECK_EQUAL( false, test1.NeedToBeSent() );

    BOOST_CHECK_EQUAL( true, test1.Set( 81 ) );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );

    test1.Set( 99 );
    test1.Send();
    BOOST_CHECK_EQUAL( true, test1.Set( 100 ) );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );

    test1.Set( 1 );
    test1.Send();
    BOOST_CHECK_EQUAL( true, test1.Set( 0 ) );
    BOOST_CHECK_EQUAL( true, test1.NeedToBeSent() );
}