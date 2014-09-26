// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/CallsignResolver.h"
#include "hla_plugin/MarkingFactory.h"
#include <xeumeuleu/streams/xistringstream.hpp>

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( marking_with_id )
{
    xml::xistringstream xis( "<hla marking-with-identifier='1'/>" );
    xis >> xml::start( "hla" );
    const MarkingFactory factory( xis );
    const Marking marking = factory.CreateMarking("unit",42);
    BOOST_CHECK_EQUAL( marking.str(), "unit42" );
}

BOOST_AUTO_TEST_CASE( marking_without_id )
{
    xml::xistringstream xis( "<hla marking-with-identifier='0'/>" );
    xis >> xml::start( "hla" );
    const MarkingFactory factory( xis );
    const Marking marking = factory.CreateMarking("unit",42);
    BOOST_CHECK_EQUAL( marking.str(), "unit" );
}

BOOST_AUTO_TEST_CASE( truncated_marking_with_id )
{
    xml::xistringstream xis( "<hla marking-with-identifier='1'/>" );
    xis >> xml::start( "hla" );
    const MarkingFactory factory( xis );
    const Marking marking = factory.CreateMarking("averylongunitmarking",42);
    BOOST_CHECK_EQUAL( marking.str(), "averylong42" );
    BOOST_CHECK_EQUAL( marking.str().size(), 11u );
}

BOOST_AUTO_TEST_CASE( truncated_marking_without_id )
{
    xml::xistringstream xis( "<hla marking-with-identifier='0'/>" );
    xis >> xml::start( "hla" );
    const MarkingFactory factory( xis );
    const Marking marking = factory.CreateMarking("averylongunitmarking",42);
    BOOST_CHECK_EQUAL( marking.str(), "averylongun" );
    BOOST_CHECK_EQUAL( marking.str().size(), 11u );
}

BOOST_AUTO_TEST_CASE( aggregate_marking_with_id )
{
    xml::xistringstream xis( "<hla marking-with-identifier='1'/>" );
    xis >> xml::start( "hla" );
    const MarkingFactory factory( xis );
    const AggregateMarking marking = factory.CreateAggregateMarking("aggregate",42);
    BOOST_CHECK_EQUAL( marking.str(), "aggregate42" );
}

BOOST_AUTO_TEST_CASE( aggregate_marking_without_id )
{
    xml::xistringstream xis( "<hla marking-with-identifier='0'/>" );
    xis >> xml::start( "hla" );
    MarkingFactory factory( xis );
    AggregateMarking marking = factory.CreateAggregateMarking("aggregate",42);
    BOOST_CHECK_EQUAL( marking.str(), "aggregate" );
}

BOOST_AUTO_TEST_CASE( truncated_aggregate_marking_with_id )
{
    xml::xistringstream xis( "<hla marking-with-identifier='1'/>" );
    xis >> xml::start( "hla" );
    const MarkingFactory factory( xis );
    const AggregateMarking marking = factory.CreateAggregateMarking("averylongaggregatemarkingthatshouldbetruncated",42);
    BOOST_CHECK_EQUAL( marking.str(), "averylongaggregatemarkingthat42" );
    BOOST_CHECK_EQUAL( marking.str().size(), 31u );
}

BOOST_AUTO_TEST_CASE( truncated_aggregate_marking_without_id )
{
    xml::xistringstream xis( "<hla marking-with-identifier='0'/>" );
    xis >> xml::start( "hla" );
    const MarkingFactory factory( xis );
    const AggregateMarking marking = factory.CreateAggregateMarking("averylongaggregatemarkingthatshouldbetruncated",42);
    BOOST_CHECK_EQUAL( marking.str(), "averylongaggregatemarkingthatsh" );
    BOOST_CHECK_EQUAL( marking.str().size(), 31u );
}

BOOST_AUTO_TEST_CASE( callsign_with_id )
{
    xml::xistringstream xis( "<hla marking-with-identifier='1'/>" );
    xis >> xml::start( "hla" );
    const CallsignResolver factory( xis );
    const std::string callsign = factory.Generate( "unit", 42 );
    BOOST_CHECK_EQUAL( callsign, "unit42" );
}

BOOST_AUTO_TEST_CASE( callsign_without_id )
{
    xml::xistringstream xis( "<hla marking-with-identifier='0'/>" );
    xis >> xml::start( "hla" );
    const CallsignResolver factory( xis );
    const std::string callsign = factory.Generate( "unit", 42 );
    BOOST_CHECK_EQUAL( callsign, "unit" );
}
