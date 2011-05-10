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
    const unsigned int frequency = 42;

    const std::string load( const char* filename = output )
    {
        std::ifstream ifs( filename );
        if( ! ifs )
            throw std::runtime_error( std::string( "File " ) + filename + " not found" );
        return std::string( std::istreambuf_iterator< char >( ifs ), std::istreambuf_iterator< char >() );
    }

    struct Fixture
    {
        Fixture()
        {
            std::remove( output );
        }
        ~Fixture()
        {
            std::remove( output );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( plugin_which_hasnt_received_any_message_outputs_headers_only_upon_destruction, Fixture )
{
    {
        plugins::positions::PositionsPlugin plugin( output, frequency );
    }
    BOOST_CHECK_EQUAL( "Team (id);Unit (id)\n", load() );
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

BOOST_FIXTURE_TEST_CASE( plugin_receiving_a_control_begin_tick_message_for_the_first_time_adds_time_to_output, Fixture )
{
    plugins::positions::PositionsPlugin plugin( output, frequency );
    plugin.Receive( MakeTimeMessage( 0 ) );
    BOOST_CHECK_EQUAL( "Team (id);Unit (id);1970-Jan-01 00:00:00\n", load() );
}

BOOST_FIXTURE_TEST_CASE( plugin_being_destroyed_adds_last_received_time_to_output_even_if_already_added, Fixture )
{
    {
        plugins::positions::PositionsPlugin plugin( output, frequency );
        plugin.Receive( MakeTimeMessage( 0 ) );
        std::remove( output );
    }
    BOOST_CHECK_EQUAL( "Team (id);Unit (id);1970-Jan-01 00:00:00;1970-Jan-01 00:00:00\n", load() );
}

BOOST_FIXTURE_TEST_CASE( plugin_being_destroyed_adds_last_received_time_to_output, Fixture )
{
    {
        plugins::positions::PositionsPlugin plugin( output, frequency );
        plugin.Receive( MakeTimeMessage( 0 ) );
        std::remove( output );
        plugin.Receive( MakeTimeMessage( frequency - 1 ) );
        BOOST_REQUIRE( ! std::ifstream( output ) );
    }
    BOOST_CHECK_EQUAL( "Team (id);Unit (id);1970-Jan-01 00:00:00;1970-Jan-01 00:00:41\n", load() );
}

BOOST_FIXTURE_TEST_CASE( plugin_receiving_a_control_begin_tick_message_adds_time_to_output_if_frequency_threshold_is_reached, Fixture )
{
    plugins::positions::PositionsPlugin plugin( output, frequency );
    plugin.Receive( MakeTimeMessage( 0 ) );
    std::remove( output );
    plugin.Receive( MakeTimeMessage( frequency - 1 ) );
    BOOST_REQUIRE( ! std::ifstream( output ) );
    plugin.Receive( MakeTimeMessage( frequency ) );
    BOOST_CHECK_EQUAL( "Team (id);Unit (id);1970-Jan-01 00:00:00;1970-Jan-01 00:00:42\n", load() );
}

namespace
{
    sword::SimToClient MakePartyMessage( unsigned int party, const std::string& name )
    {
        sword::SimToClient message;
        message.mutable_message()->mutable_party_creation()->mutable_party()->set_id( party );
        message.mutable_message()->mutable_party_creation()->set_name( name );
        return message;
    }
    sword::SimToClient MakeAutomatMessage( unsigned int party, unsigned int automat )
    {
        sword::SimToClient message;
        message.mutable_message()->mutable_automat_creation()->mutable_party()->set_id( party );
        message.mutable_message()->mutable_automat_creation()->mutable_automat()->set_id( automat );
        return message;
    }
    sword::SimToClient MakeUnitMessage( unsigned int automat, unsigned int unit, const std::string& name )
    {
        sword::SimToClient message;
        message.mutable_message()->mutable_unit_creation()->mutable_automat()->set_id( automat );
        message.mutable_message()->mutable_unit_creation()->mutable_unit()->set_id( unit );
        message.mutable_message()->mutable_unit_creation()->set_name( name );
        return message;
    }
}

BOOST_FIXTURE_TEST_CASE( received_units_are_added_to_output, Fixture )
{
    plugins::positions::PositionsPlugin plugin( output, frequency );
    plugin.Receive( MakePartyMessage( 11u, "party1" ) );
    plugin.Receive( MakeAutomatMessage( 11u, 21u ) );
    plugin.Receive( MakeUnitMessage( 21u, 31u, "unit1" ) );
    plugin.Receive( MakeUnitMessage( 21u, 32u, "unit2" ) );
    plugin.Receive( MakeTimeMessage( 0 ) );
    BOOST_CHECK_EQUAL( "Team (id);Unit (id);1970-Jan-01 00:00:00\n"
                       "party1 (11);unit1 (31);NA\n"
                       "party1 (11);unit2 (32);NA\n", load() );
}

namespace
{
    sword::SimToClient MakePositionMessage( unsigned int unit, double latitude, double longitude )
    {
        sword::SimToClient message;
        message.mutable_message()->mutable_unit_attributes()->mutable_unit()->set_id( unit );
        message.mutable_message()->mutable_unit_attributes()->mutable_position()->set_latitude( latitude );
        message.mutable_message()->mutable_unit_attributes()->mutable_position()->set_longitude( longitude );
        return message;
    }
}

BOOST_FIXTURE_TEST_CASE( received_units_with_positions_are_added_to_output_with_positions, Fixture )
{
    plugins::positions::PositionsPlugin plugin( output, frequency );
    plugin.Receive( MakePartyMessage( 11u, "party1" ) );
    plugin.Receive( MakeAutomatMessage( 11u, 21u ) );
    plugin.Receive( MakeUnitMessage( 21u, 31u, "unit1" ) );
    plugin.Receive( MakeUnitMessage( 21u, 32u, "unit2" ) );
    plugin.Receive( MakeTimeMessage( 0 ) );
    plugin.Receive( MakePositionMessage( 31u, 17, 42 ) );
    plugin.Receive( MakePositionMessage( 32u, 23, 51 ) );
    plugin.Receive( MakeTimeMessage( frequency ) );
    BOOST_CHECK_EQUAL( "Team (id);Unit (id);1970-Jan-01 00:00:00;1970-Jan-01 00:00:42\n"
                       "party1 (11);unit1 (31);NA;Lat:17 Long:42\n"
                       "party1 (11);unit2 (32);NA;Lat:23 Long:51\n", load() );
}
