// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "positions_plugin_test_pch.h"
#include "positions_plugin/PositionsPlugin.h"
#include "protocol/Protocol.h"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace bpt = boost::posix_time;

namespace
{
    const char* output = "positions_plugin_test.tmp";
    const int frequency = 42;

    const std::string load( const char* filename = output )
    {
        std::ifstream ifs( filename );
        if( ! ifs )
            throw std::runtime_error( std::string( "File " ) + filename + " not found" );
        return std::string( std::istreambuf_iterator< char >( ifs ), std::istreambuf_iterator< char >() );
    }
}

BOOST_AUTO_TEST_CASE( plugin_which_hasnt_received_any_message_outputs_headers_only )
{
    std::remove( output );
    {
        plugins::positions::PositionsPlugin plugin( output, frequency );
    }
    BOOST_CHECK_EQUAL( "Team (id);Unit (id)\n", load() );
    std::remove( output );
}

namespace
{
    sword::SimToClient MakeTimeMessage( int seconds )
    {
        sword::SimToClient message;
        message.mutable_message()->mutable_control_begin_tick()->mutable_date_time()->set_data( bpt::to_iso_string( bpt::from_time_t( seconds ) ) );
        return message;
    }
}

BOOST_AUTO_TEST_CASE( plugin_receiving_a_control_begin_tick_message_for_the_first_time_adds_time_to_output )
{
    std::remove( output );
    {
        plugins::positions::PositionsPlugin plugin( output, frequency );
        plugin.Receive( MakeTimeMessage( 0 ) );
        BOOST_CHECK_EQUAL( "Team (id);Unit (id);Thu 1. Jan 00:00:00 1970\n", load() );
    }
    std::remove( output );
}

BOOST_AUTO_TEST_CASE( plugin_being_destroyed_adds_last_received_time_even_if_already_output )
{
    std::remove( output );
    {
        plugins::positions::PositionsPlugin plugin( output, frequency );
        plugin.Receive( MakeTimeMessage( 0 ) );
        BOOST_CHECK_EQUAL( "Team (id);Unit (id);Thu 1. Jan 00:00:00 1970\n", load() );
        std::remove( output );
    }
    BOOST_CHECK_EQUAL( "Team (id);Unit (id);Thu 1. Jan 00:00:00 1970;Thu 1. Jan 00:00:00 1970\n", load() );
    std::remove( output );
}

BOOST_AUTO_TEST_CASE( plugin_being_destroyed_adds_last_received_time_to_output )
{
    std::remove( output );
    {
        plugins::positions::PositionsPlugin plugin( output, frequency );
        plugin.Receive( MakeTimeMessage( 0 ) );
        std::remove( output );
        plugin.Receive( MakeTimeMessage( frequency - 1 ) );
        BOOST_REQUIRE( ! std::ifstream( output ) );
    }
    BOOST_CHECK_EQUAL( "Team (id);Unit (id);Thu 1. Jan 00:00:00 1970;Thu 1. Jan 00:00:41 1970\n", load() );
    std::remove( output );
}

BOOST_AUTO_TEST_CASE( plugin_receiving_a_control_begin_tick_message_adds_time_to_output_if_frequency_threshold_is_reached )
{
    std::remove( output );
    {
        plugins::positions::PositionsPlugin plugin( output, frequency );
        plugin.Receive( MakeTimeMessage( 0 ) );
        std::remove( output );
        plugin.Receive( MakeTimeMessage( frequency - 1 ) );
        BOOST_REQUIRE( ! std::ifstream( output ) );
        plugin.Receive( MakeTimeMessage( frequency ) );
        BOOST_CHECK_EQUAL( "Team (id);Unit (id);Thu 1. Jan 00:00:00 1970;Thu 1. Jan 00:00:42 1970\n", load() );
    }
    std::remove( output );
}
