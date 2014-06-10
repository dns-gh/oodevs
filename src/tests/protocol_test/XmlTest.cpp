// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "protocol_test_pch.h"

#include "protocol/Helpers.h"
#include "protocol/XmlReaders.h"
#include "protocol/XmlWriters.h"

#include "tools/MessageSender_ABC.h"

#pragma warning( push, 0 )
#include <google/protobuf/descriptor.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace protocol;
using namespace sword;

namespace
{
    MOCK_BASE_CLASS( MockReader, Reader_ABC )
    {
        MOCK_METHOD( Convert, 1 );
        MOCK_METHOD( Resolve, 1 );
    };

    MOCK_BASE_CLASS( MockWriter, Writer_ABC )
    {
        MOCK_METHOD( Convert, 2 );
    };

    const Reader_ABC::Point dummy = { 0.0, 0.0 };

    std::string Indent( const std::string& src )
    {
        xml::xostringstream output;
        output << xml::xistringstream( src );
        return output.str();
    }

    struct Fixture
    {
        Fixture()
        {
            xos << xml::start( "action" );
        }

        MockReader reader;
        xml::xostringstream xos;

        template< typename T >
        T Read( const std::string& input )
        {
            xml::xistringstream xis( input );
            xis >> xml::start( "action" );
            T dst;
            protocol::Read( reader, dst, xis );
            if( !dst.IsInitialized() )
                throw MASA_EXCEPTION( dst.GetDescriptor()->full_name()
                                    + " is missing field "
                                    + dst.InitializationErrorString() );
            return dst;
        }

        template< typename T >
        T Read()
        {
            return Read< T >( xos.str() );
        }

        template< typename T >
        void CheckCycle( const std::string& xml, const T& msg )
        {
            xml::xostringstream out;
            out << xml::start( "action" );
            MockWriter writer;
            MOCK_EXPECT( writer.Convert ).returns( "dummy" );
            protocol::Write( out, writer, msg );
            BOOST_CHECK_EQUAL( Indent( xml ), out.str() );
        }

        template< typename T >
        void CheckCycle( const T& msg )
        {
            CheckCycle< T >( xos.str(), msg );
        }
    };

    template< typename T >
    void AddParameterValue( xml::xosubstream xos, const std::string& type, const T& value )
    {
        xos << xml::start( "parameter" )
            << xml::attribute( "type", type )
            << xml::attribute( "value", value );
    }

    void CheckNull( const MissionParameter& src )
    {
        BOOST_CHECK( src.null_value() );
        BOOST_CHECK( !src.value_size() );
    }

    void CheckLocation( const Location& loc, const Location_Geometry type, int count )
    {
        BOOST_CHECK_EQUAL( loc.type(), type );
        BOOST_CHECK_EQUAL( loc.coordinates().elem_size(), count );
    }
}

BOOST_FIXTURE_TEST_CASE( read_boolean, Fixture )
{
    AddParameterValue( xos, "boolean", true );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK( msg.elem( 0 ).value( 0 ).booleanvalue() );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_numerics, Fixture )
{
    AddParameterValue( xos, "integer", 47 );
    AddParameterValue( xos, "numeric", 3.14f );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).intvalue(), 47 );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).value( 0 ).areal(), 3.14f );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_heading, Fixture )
{
    AddParameterValue( xos, "heading", 47 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).heading().heading(), 47 );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_enumeration, Fixture )
{
    AddParameterValue( xos, "enumeration", 47 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).enumeration(), 47 );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_datetime, Fixture )
{
    AddParameterValue( xos, "datetime", "20121110T083917" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).datetime().data(), "20121110T083917" );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_datetime_converts, Fixture )
{
    AddParameterValue( xos, "datetime", "2009-11-26T17:04:28" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).datetime().data(), "20091126T170428" );
}

BOOST_FIXTURE_TEST_CASE( read_location, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='location'>"
    "    <location type='point'>"
    "      <point coordinates='dummy'/>"
    "    </location>"
    "  </parameter>"
    "  <parameter type='location'>"
    "    <parameter type='location'>"
    "      <location type='line'>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "    <parameter type='location'>"
    "      <location type='polygon'>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 6 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    CheckLocation( msg.elem( 0 ).value( 0 ).location(), Location::point, 1 );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), false );
    const auto& list = msg.elem( 1 );
    BOOST_CHECK_EQUAL( list.value_size(), 2 );
    CheckLocation( list.value( 0 ).location(), Location::line,    2 );
    CheckLocation( list.value( 1 ).location(), Location::polygon, 3 );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_point, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='point'>"
    "    <location type='point'>"
    "      <point coordinates='dummy'/>"
    "    </location>"
    "  </parameter>"
    "  <parameter type='point'>"
    "    <parameter type='point'>"
    "      <location type='point'>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "    <parameter type='point'>"
    "      <location type='point'>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 3 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    CheckLocation( msg.elem( 0 ).value( 0 ).point().location(), Location::point, 1 );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), false );
    const auto& list = msg.elem( 1 );
    BOOST_CHECK_EQUAL( list.value_size(), 2 );
    CheckLocation( list.value( 0 ).point().location(), Location::point, 1 );
    CheckLocation( list.value( 1 ).point().location(), Location::point, 1 );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_polygon, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='polygon'>"
    "    <location type='polygon'>"
    "      <point coordinates='dummy'/>"
    "      <point coordinates='dummy'/>"
    "      <point coordinates='dummy'/>"
    "    </location>"
    "  </parameter>"
    "  <parameter type='polygon'>"
    "    <parameter type='polygon'>"
    "      <location type='polygon'>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "    <parameter type='polygon'>"
    "      <location type='polygon'>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 10 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    CheckLocation( msg.elem( 0 ).value( 0 ).area().location(), Location::polygon, 3 );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), false );
    const auto& list = msg.elem( 1 );
    BOOST_CHECK_EQUAL( list.value_size(), 2 );
    CheckLocation( list.value( 0 ).area().location(), Location::polygon, 3 );
    CheckLocation( list.value( 1 ).area().location(), Location::polygon, 4 );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_limit, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='limit'>"
    "    <location type='line'>"
    "      <point coordinates='dummy'/>"
    "      <point coordinates='dummy'/>"
    "    </location>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 2 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    CheckLocation( msg.elem( 0 ).value( 0 ).limit().location(), Location::line, 2 );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_path, Fixture )
{
    const std::string path =
    "  <parameter type='path'>"
    "    <parameter type='pathpoint'>"
    "      <location type='point'>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "    <parameter type='pathpoint'>"
    "      <location type='point'>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "    <parameter type='pathpoint'>"
    "      <location type='point'>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "  </parameter>";
    const std::string input =
    "<action>"
    + path +
    "  <parameter type='path'>"
    + path + path +
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 3*3 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    CheckLocation( msg.elem( 0 ).value( 0 ).path().location(), Location::line, 3 );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), false );
    const auto& list = msg.elem( 1 );
    BOOST_CHECK_EQUAL( list.value_size(), 2 );
    CheckLocation( list.value( 0 ).path().location(), Location::line, 3 );
    CheckLocation( list.value( 1 ).path().location(), Location::line, 3 );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_phaseline, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='phaseline'>"
    "    <parameter type='phaseline' value='LO'>"
    "      <parameter type='location'>"
    "        <location type='line'>"
    "          <point coordinates='dummy'/>"
    "          <point coordinates='dummy'/>"
    "        </location>"
    "      </parameter>"
    "      <parameter type='datetime'/>"
    "    </parameter>"
    "    <parameter type='phaseline' value='LC,LCA'>"
    "      <parameter type='location'>"
    "        <location type='line'>"
    "          <point coordinates='dummy'/>"
    "          <point coordinates='dummy'/>"
    "          <point coordinates='dummy'/>"
    "        </location>"
    "      </parameter>"
    "      <parameter type='datetime' value='20121113T085132'/>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 5 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    const auto& list = msg.elem( 0 ).value();
    BOOST_CHECK_EQUAL( list.size(), 2 );
    const auto& lima_1 = list.Get( 0 ).phaseline().elem( 0 );
    BOOST_CHECK_EQUAL( lima_1.fonctions_size(), 1 );
    BOOST_CHECK_EQUAL( lima_1.fonctions( 0 ), PhaseLineOrder::objective_line );
    CheckLocation( lima_1.line().location(), Location::line, 2 );
    BOOST_CHECK( lima_1.time().data().empty() );
    const auto& lima_2 = list.Get( 1 ).phaseline().elem( 0 );
    BOOST_CHECK_EQUAL( lima_2.fonctions_size(), 2 );
    BOOST_CHECK_EQUAL( lima_2.fonctions( 0 ), PhaseLineOrder::coordination_line );
    BOOST_CHECK_EQUAL( lima_2.fonctions( 1 ), PhaseLineOrder::attitude_change_line );
    CheckLocation( lima_2.line().location(), Location::line, 3 );
    BOOST_CHECK_EQUAL( lima_2.time().data(), "20121113T085132" );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_empty_phaseline, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='phaseline'/>"
    "</action>";
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), true );
}

BOOST_FIXTURE_TEST_CASE( read_empty_parameter, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter/>"
    "</action>";
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), true );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_automat_id, Fixture )
{
    AddParameterValue( xos, "automat", 1337 );
    xos << xml::start( "parameter" )
        << xml::attribute( "type", "automat" );
    AddParameterValue( xos, "automat", 1338 );
    AddParameterValue( xos, "automat", 1339 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).automat().id(), 1337u );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), false );
    const auto& list = msg.elem( 1 );
    BOOST_CHECK_EQUAL( list.value_size(), 2 );
    BOOST_CHECK_EQUAL( list.value( 0 ).automat().id(), 1338u );
    BOOST_CHECK_EQUAL( list.value( 1 ).automat().id(), 1339u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_agent_id, Fixture )
{
    AddParameterValue( xos, "agent", 1337 );
    xos << xml::start( "parameter" )
        << xml::attribute( "type", "agent" );
    AddParameterValue( xos, "agent", 1338 );
    AddParameterValue( xos, "agent", 1339 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).agent().id(), 1337u );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), false );
    const auto& list = msg.elem( 1 );
    BOOST_CHECK_EQUAL( list.value_size(), 2 );
    BOOST_CHECK_EQUAL( list.value( 0 ).agent().id(), 1338u );
    BOOST_CHECK_EQUAL( list.value( 1 ).agent().id(), 1339u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_agent_knowledge_id, Fixture )
{
    AddParameterValue( xos, "agentknowledge", 1337 );
    xos << xml::start( "parameter" )
        << xml::attribute( "type", "agentknowledge" );
    AddParameterValue( xos, "agentknowledge", 1338 );
    AddParameterValue( xos, "agentknowledge", 1339 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).agentknowledge().id(), 1337u );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), false );
    const auto& list = msg.elem( 1 );
    BOOST_CHECK_EQUAL( list.value_size(), 2 );
    BOOST_CHECK_EQUAL( list.value( 0 ).agentknowledge().id(), 1338u );
    BOOST_CHECK_EQUAL( list.value( 1 ).agentknowledge().id(), 1339u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_crowd_knowledge_id, Fixture )
{
    AddParameterValue( xos, "crowdknowledge", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).crowdknowledge().id(), 1337u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_object_knowledge_id, Fixture )
{
    AddParameterValue( xos, "objectknowledge", 1337 );
    xos << xml::start( "parameter" )
        << xml::attribute( "type", "objectknowledge" );
    AddParameterValue( xos, "objectknowledge", 1338 );
    AddParameterValue( xos, "objectknowledge", 1339 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).objectknowledge().id(), 1337u );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), false );
    const auto& list = msg.elem( 1 );
    BOOST_CHECK_EQUAL( list.value_size(), 2 );
    BOOST_CHECK_EQUAL( list.value( 0 ).objectknowledge().id(), 1338u );
    BOOST_CHECK_EQUAL( list.value( 1 ).objectknowledge().id(), 1339u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_urban_knowledge_id, Fixture )
{
    AddParameterValue( xos, "urbanknowledge", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).urbanknowledge().id(), 1337u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_phase_line_function, Fixture )
{
    AddParameterValue( xos, "limafunction", "LC" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).phase_line_function(), PhaseLineOrder::coordination_line );
    CheckCycle( msg );
}

namespace
{
    void CheckPlannedWork( const PlannedWork& work )
    {
        BOOST_CHECK_EQUAL( work.type(), "barricade" );
        CheckLocation( work.position(), Location::polygon, 3 );
        BOOST_CHECK_EQUAL( work.type_obstacle(), ObstacleType::reserved );
        BOOST_CHECK_EQUAL( work.density(), 4.2f );
        BOOST_CHECK_EQUAL( work.combat_train().id(), 5169u );
        BOOST_CHECK_EQUAL( work.activity_time(), 38 );
        BOOST_CHECK_EQUAL( work.activation_time(), 39 );
        BOOST_CHECK_EQUAL( work.name(), "gyhjkf" );
        BOOST_CHECK_EQUAL( work.altitude_modifier(), 40 );
        BOOST_CHECK_EQUAL( work.time_limit(), 41 );
        BOOST_CHECK_EQUAL( work.mining(), true );
        BOOST_CHECK_EQUAL( work.lodging(), 98 );
        BOOST_CHECK_EQUAL( work.fire_class(), "some fire" );
        BOOST_CHECK_EQUAL( work.max_combustion(), 42 );
    }
}

BOOST_FIXTURE_TEST_CASE( read_planned_work, Fixture )
{
    const std::string work =
    "  <parameter type='plannedwork' value='barricade'>"
    "    <parameter type='polygon'>"
    "      <location type='polygon'>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "    <parameter identifier='obstacletype' type='obstacletype' value='1'/>"
    "    <parameter identifier='density' type='numeric' value='4.2'/>"
    "    <parameter identifier='tc2' type='automat' value='5169'/>"
    "    <parameter identifier='activitytime' type='quantity' value='38'/>"
    "    <parameter identifier='activationtime' type='quantity' value='39'/>"
    "    <parameter identifier='name' type='string' value='gyhjkf'/>"
    "    <parameter identifier='altitude_modifier' type='quantity' value='40'/>"
    "    <parameter identifier='time_limit' type='quantity' value='41'/>"
    "    <parameter identifier='obstacle_mining' type='bool' value='true'/>"
    "    <parameter identifier='lodging' type='quantity' value='98'/>"
    "    <parameter identifier='fire_class' type='fireclass' value='some fire'/>"
    "    <parameter identifier='max_combustion_energy' type='quantity' value='42'/>"
    "  </parameter>";
    const std::string input =
    "<action>"
    + work +
    "  <parameter type='plannedwork'>"
    + work + work +
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 3*3 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    CheckPlannedWork( msg.elem( 0 ).value( 0 ).plannedwork() );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), false );
    const auto& next = msg.elem( 1 );
    BOOST_CHECK_EQUAL( next.value_size(), 2 );
    CheckPlannedWork( next.value( 0 ).plannedwork() );
    CheckPlannedWork( next.value( 1 ).plannedwork() );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_nature_atlas, Fixture )
{
    AddParameterValue( xos, "natureatlas", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).nature().flags(), 1337 );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_resource_type, Fixture )
{
    AddParameterValue( xos, "resourcetype", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).resourcetype().id(), 1337u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_resource_type_list, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='resourcetype'>"
    "    <parameter type='resourcetype' value='100'/>"
    "    <parameter type='resourcetype' value='20'/>"
    "  </parameter>"
    "</action>";
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).resourcetype().id(), 100u );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 1 ).resourcetype().id(), 20u );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_acharstr, Fixture )
{
    AddParameterValue( xos, "string", "zomg" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).acharstr(), "zomg" );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_stage, Fixture )
{
    AddParameterValue( xos, "stage", "zomg" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).stage(), "zomg" );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_object_id, Fixture )
{
    AddParameterValue( xos, "object", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).object().id(), 1337u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_party_id, Fixture )
{
    AddParameterValue( xos, "army", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).party().id(), 1337u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_indirect_fire_id, Fixture )
{
    AddParameterValue( xos, "indirectfire", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).indirectfire().id(), 1337u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_knowledge_group_id, Fixture )
{
    AddParameterValue( xos, "knowledgegroup", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).knowledgegroup().id(), 1337u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_formation_id, Fixture )
{
    AddParameterValue( xos, "formation", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).formation().id(), 1337u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_identifier, Fixture )
{
    AddParameterValue( xos, "identifier", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).identifier(), 1337u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_external_identifier, Fixture )
{
    AddParameterValue( xos, "externalidentifier", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).external_identifier(), 1337 );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_quantity, Fixture )
{
    AddParameterValue( xos, "quantity", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).quantity(), 1337 );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_equipment_type, Fixture )
{
    AddParameterValue( xos, "equipmenttype", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).equipmenttype().id(), 1337u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_resource_network_type, Fixture )
{
    AddParameterValue( xos, "resourcenetworktype", "zomg" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).resourcenetworktype().name(), "zomg" );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_mission_objective, Fixture )
{
    AddParameterValue( xos, "missionobjective", "test" );
    xos << xml::start( "parameter" )
        << xml::attribute( "type", "missionobjective" )
            << xml::start( "parameter" )
            << xml::attribute( "type", "missionobjective" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), true );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).null_value(), true );
}

BOOST_FIXTURE_TEST_CASE( read_maintenance_priorities, Fixture )
{
    AddParameterValue( xos, "maintenancepriorities", "7;46" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    auto& priorities = msg.elem( 0 ).value( 0 ).logmaintenancepriorities();
    BOOST_CHECK_EQUAL( priorities.elem_size(), 2 );
    BOOST_CHECK_EQUAL( priorities.elem( 0 ).id(), 7u );
    BOOST_CHECK_EQUAL( priorities.elem( 1 ).id(), 46u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_medical_priorities, Fixture )
{
    AddParameterValue( xos, "medicalpriorities", "1;5" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    auto& priorities = msg.elem( 0 ).value( 0 ).logmedicalpriorities();
    BOOST_CHECK_EQUAL( priorities.elem_size(), 2 );
    BOOST_CHECK_EQUAL( priorities.elem( 0 ), dead );
    BOOST_CHECK_EQUAL( priorities.elem( 1 ), wounded_extreme_urgency );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_resource_network_node, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='resourcenetworknode' value='1337'>"
    "    <parameter type='resource' value='some_value'/>"
    "  </parameter>"
    "</action>";
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    auto& node = msg.elem( 0 ).value( 0 ).resourcenetworknode();
    BOOST_CHECK_EQUAL( node.object().id(), 1337u );
    BOOST_CHECK_EQUAL( node.resource().name(), "some_value" );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_resource_network_node_scipio, Fixture )
{
    std::string input =
    "<action>"
    "  <parameter type='resourcenetwork' value='1337'>"
    "    <parameter type='resource' value='some_value'/>"
    "  </parameter>"
    "</action>";
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    auto& node = msg.elem( 0 ).value( 0 ).resourcenetworknode();
    BOOST_CHECK_EQUAL( node.object().id(), 1337u );
    BOOST_CHECK_EQUAL( node.resource().name(), "some_value" );
    boost::replace_all( input, "resourcenetwork", "resourcenetworknode" );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_extension_list, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='extensionlist'>"
    "    <parameter type='resource' value='some_value'/>"
    "    <parameter type='cogito' value='ergo sum'/>"
    "  </parameter>"
    "</action>";
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    auto& list = msg.elem( 0 ).value( 0 ).extensionlist();
    BOOST_CHECK_EQUAL( list.entries_size(), 2 );
    BOOST_CHECK_EQUAL( list.entries( 0 ).name(), "resource" );
    BOOST_CHECK_EQUAL( list.entries( 0 ).value(), "some_value" );
    BOOST_CHECK_EQUAL( list.entries( 1 ).name(), "cogito" );
    BOOST_CHECK_EQUAL( list.entries( 1 ).value(), "ergo sum" );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_push_flow_parameters, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='pushflowparameters'>"
    "    <recipient id='1'>"
    "      <resource id='2' quantity='3'/>"
    "      <resource id='4' quantity='5'/>"
    "      <path>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "      </path>"
    "    </recipient>"
    "    <recipient id='6'/>"
    "    <transporter id='7' quantity='8'/>"
    "    <transporter id='9' quantity='10'/>"
    "    <waybackpath>"
    "      <point coordinates='dummy'/>"
    "      <point coordinates='dummy'/>"
    "    </waybackpath>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 4 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    auto& push = msg.elem( 0 ).value( 0 ).push_flow_parameters();
    BOOST_CHECK_EQUAL( push.recipients_size(), 2 );
    auto& recipient = push.recipients( 0 );
    BOOST_CHECK_EQUAL( recipient.receiver().id(), 1u );
    BOOST_CHECK_EQUAL( recipient.resources_size(), 2 );
    BOOST_CHECK_EQUAL( recipient.resources( 0 ).resourcetype().id(), 2u );
    BOOST_CHECK_EQUAL( recipient.resources( 0 ).quantity(), 3u );
    BOOST_CHECK_EQUAL( recipient.resources( 1 ).resourcetype().id(), 4u );
    BOOST_CHECK_EQUAL( recipient.resources( 1 ).quantity(), 5u );
    BOOST_CHECK_EQUAL( recipient.path().elem_size(), 2 );
    CheckLocation( recipient.path().elem( 0 ).location(), Location::point, 1 );
    CheckLocation( recipient.path().elem( 1 ).location(), Location::point, 1 );
    BOOST_CHECK_EQUAL( push.recipients( 1 ).receiver().id(), 6u );
    BOOST_CHECK_EQUAL( push.transporters_size(), 2 );
    BOOST_CHECK_EQUAL( push.transporters( 0 ).equipmenttype().id(), 7u );
    BOOST_CHECK_EQUAL( push.transporters( 0 ).quantity(), 8u );
    BOOST_CHECK_EQUAL( push.transporters( 1 ).equipmenttype().id(), 9u );
    BOOST_CHECK_EQUAL( push.transporters( 1 ).quantity(), 10u );
    BOOST_CHECK_EQUAL( push.waybackpath().elem_size(), 2 );
    CheckLocation( push.waybackpath().elem( 0 ).location(), Location::point, 1 );
    CheckLocation( push.waybackpath().elem( 1 ).location(), Location::point, 1 );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_pull_flow_parameters, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='pullflowparameters'>"
    "    <supplier id='1'/>"
    "    <resource id='2' quantity='3'/>"
    "    <resource id='4' quantity='5'/>"
    "    <transporter id='6' quantity='7'/>"
    "    <transporter id='8' quantity='9'/>"
    "    <wayoutpath>"
    "      <point coordinates='dummy'/>"
    "      <point coordinates='dummy'/>"
    "    </wayoutpath>"
    "    <waybackpath>"
    "      <point coordinates='dummy'/>"
    "      <point coordinates='dummy'/>"
    "    </waybackpath>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 4 ).returns( dummy );
    MOCK_EXPECT( reader.Resolve ).once().with( 1u ).returns( Reader_ABC::FORMATION );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    auto& pull = msg.elem( 0 ).value( 0 ).pull_flow_parameters();
    BOOST_CHECK_EQUAL( pull.supplier().formation().id(), 1u );
    BOOST_CHECK_EQUAL( pull.resources_size(), 2 );
    BOOST_CHECK_EQUAL( pull.resources( 0 ).resourcetype().id(), 2u );
    BOOST_CHECK_EQUAL( pull.resources( 0 ).quantity(), 3u );
    BOOST_CHECK_EQUAL( pull.resources( 1 ).resourcetype().id(), 4u );
    BOOST_CHECK_EQUAL( pull.resources( 1 ).quantity(), 5u );
    BOOST_CHECK_EQUAL( pull.transporters_size(), 2 );
    BOOST_CHECK_EQUAL( pull.transporters( 0 ).equipmenttype().id(), 6u );
    BOOST_CHECK_EQUAL( pull.transporters( 0 ).quantity(), 7u );
    BOOST_CHECK_EQUAL( pull.transporters( 1 ).equipmenttype().id(), 8u );
    BOOST_CHECK_EQUAL( pull.transporters( 1 ).quantity(), 9u );
    BOOST_CHECK_EQUAL( pull.wayoutpath().elem_size(), 2 );
    CheckLocation( pull.wayoutpath().elem( 0 ).location(), Location::point, 1 );
    CheckLocation( pull.wayoutpath().elem( 1 ).location(), Location::point, 1 );
    BOOST_CHECK_EQUAL( pull.waybackpath().elem_size(), 2 );
    CheckLocation( pull.waybackpath().elem( 0 ).location(), Location::point, 1 );
    CheckLocation( pull.waybackpath().elem( 1 ).location(), Location::point, 1 );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( break_location_reader, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='list'>"
    "    <parameter type='polygon'>"
    "      <location type='polygon'>"
    "        <point/>"
    "      </location>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    Read< MissionParameters >( input );
}

BOOST_FIXTURE_TEST_CASE( read_list, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='list'>"
    "    <parameter type='list'>"
    "      <parameter type='polygon'>"
    "        <location type='polygon'>"
    "          <point coordinates='dummy'/>"
    "          <point coordinates='dummy'/>"
    "          <point coordinates='dummy'/>"
    "        </location>"
    "      </parameter>"
    "      <parameter type='agent' value='1106'/>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 3 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    auto& val = msg.elem( 0 ).value( 0 );
    BOOST_CHECK_EQUAL( val.list_size(), 2 );
    CheckLocation( val.list( 0 ).area().location(), Location::polygon, 3 );
    BOOST_CHECK_EQUAL( val.list( 1 ).agent().id(), 1106u );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_change_quota_links, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='list'>"
    "    <parameter type='list'>"
    "      <parameter type='identifier' value='3'/>"
    "      <parameter type='quantity' value='4'/>"
    "    </parameter>"
    "    <parameter type='list'>"
    "      <parameter type='identifier' value='5'/>"
    "      <parameter type='quantity' value='6'/>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).list_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 1 ).list_size(), 2 );
    auto& first = msg.elem( 0 ).value( 0 );
    BOOST_CHECK_EQUAL( first.list( 0 ).identifier(), 3u );
    BOOST_CHECK_EQUAL( first.list( 1 ).quantity(), 4 );
    auto& second = msg.elem( 0 ).value( 1 );
    BOOST_CHECK_EQUAL( second.list( 0 ).identifier(), 5u );
    BOOST_CHECK_EQUAL( second.list( 1 ).quantity(), 6 );
    CheckCycle( input, msg );
}

BOOST_FIXTURE_TEST_CASE( read_locationcomposite, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='locationcomposite'>"
    "    <parameter type='polygon'>"
    "      <location type='polygon'>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "    <parameter type='point'>"
    "      <location type='point'>"
    "        <point coordinates='dummy'/>"
    "      </location>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( reader.Convert ).exactly( 4 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    const auto& list = msg.elem( 0 ).value();
    BOOST_CHECK_EQUAL( list.size(), 2 );
    CheckLocation( list.Get( 0 ).area().location(), Location::polygon, 3 );
    CheckLocation( list.Get( 1 ).point().location(), Location::point, 1 );
    CheckCycle( input, msg );
}

namespace
{
    template< typename T >
    void TestOrder( Fixture& fix )
    {
        fix.xos << xml::attribute( "target", 27u )
                << xml::attribute( "id", 17 )
                << xml::attribute( "type", "mission" )
                << xml::attribute( "time", "2011-04-08T10:01:36" );
        const auto msg = fix.Read< T >();
        BOOST_CHECK_EQUAL( msg.tasker().id(), 27u );
        BOOST_CHECK_EQUAL( msg.type().id(), 17u );
        BOOST_CHECK( msg.has_parameters() );
        BOOST_CHECK_EQUAL( msg.parameters().elem_size(), 0 );
        BOOST_CHECK_EQUAL( msg.start_time().data(), "2011-04-08T10:01:36" );
        fix.CheckCycle( msg );
    }
}

BOOST_FIXTURE_TEST_CASE( read_unit_order, Fixture )
{
    TestOrder< UnitOrder >( *this );
}

BOOST_FIXTURE_TEST_CASE( read_automat_order, Fixture )
{
    TestOrder< UnitOrder >( *this );
}

BOOST_FIXTURE_TEST_CASE( read_crowd_order, Fixture )
{
    TestOrder< CrowdOrder >( *this );
}

BOOST_FIXTURE_TEST_CASE( read_frag_order, Fixture )
{
    xos << xml::attribute( "target", 27u )
        << xml::attribute( "id", 1 )
        << xml::attribute( "type", "fragorder" )
        << xml::attribute( "time", "2011-04-08T10:01:36" );
    MOCK_EXPECT( reader.Resolve ).once().with( 27u ).returns( Reader_ABC::PARTY );
    const auto msg = Read< FragOrder >();
    BOOST_CHECK_EQUAL( msg.tasker().party().id(), 27u );
    BOOST_CHECK_EQUAL( msg.type().id(), 1u );
    BOOST_CHECK( msg.has_parameters() );
    BOOST_CHECK_EQUAL( msg.parameters().elem_size(), 0 );
    BOOST_CHECK_EQUAL( msg.start_time().data(), "2011-04-08T10:01:36" );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_magic_action, Fixture )
{
    xos << xml::attribute( "id", mapping::MagicAction::data_[1].name )
        << xml::attribute( "type", "magic" )
        << xml::attribute( "time", "2011-04-08T10:01:36" );
    const auto msg = Read< MagicAction >();
    BOOST_CHECK_EQUAL( msg.type(), mapping::MagicAction::data_[1].type );
    BOOST_CHECK( msg.has_parameters() );
    BOOST_CHECK_EQUAL( msg.parameters().elem_size(), 0 );
    BOOST_CHECK_EQUAL( msg.start_time().data(), "2011-04-08T10:01:36" );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_unit_magic_action, Fixture )
{
    xos << xml::attribute( "target", 27u )
        << xml::attribute( "id", mapping::MagicUnitAction::data_[10].name )
        << xml::attribute( "type", "magicunit" )
        << xml::attribute( "time", "2011-04-08T10:01:36" );
    MOCK_EXPECT( reader.Resolve ).once().with( 27u ).returns( Reader_ABC::PARTY );
    const auto msg = Read< UnitMagicAction >();
    BOOST_CHECK_EQUAL( msg.tasker().party().id(), 27u );
    BOOST_CHECK_EQUAL( msg.type(), mapping::MagicUnitAction::data_[10].type );
    BOOST_CHECK( msg.has_parameters() );
    BOOST_CHECK_EQUAL( msg.parameters().elem_size(), 0 );
    BOOST_CHECK_EQUAL( msg.start_time().data(), "2011-04-08T10:01:36" );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_object_magic_action, Fixture )
{
    xos << xml::attribute( "target", 27u )
        << xml::attribute( "id", mapping::MagicObjectAction::data_[0].name )
        << xml::attribute( "type", "magicobject" )
        << xml::attribute( "time", "2011-04-08T10:01:36" );
    const auto msg = Read< ObjectMagicAction >();
    BOOST_CHECK_EQUAL( msg.object().id(), 27u );
    BOOST_CHECK_EQUAL( msg.type(), mapping::MagicObjectAction::data_[0].type );
    BOOST_CHECK( msg.has_parameters() );
    BOOST_CHECK_EQUAL( msg.parameters().elem_size(), 0 );
    BOOST_CHECK_EQUAL( msg.start_time().data(), "2011-04-08T10:01:36" );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_knowledge_magic_action, Fixture )
{
    xos << xml::attribute( "target", 27u )
        << xml::attribute( "id", mapping::MagicKnowledgeAction::data_[0].name )
        << xml::attribute( "type", "magicknowledge" )
        << xml::attribute( "time", "2011-04-08T10:01:36" );
    const auto msg = Read< KnowledgeMagicAction >();
    BOOST_CHECK_EQUAL( msg.knowledge_group().id(), 27u );
    BOOST_CHECK_EQUAL( msg.type(), mapping::MagicKnowledgeAction::data_[0].type );
    BOOST_CHECK( msg.has_parameters() );
    BOOST_CHECK_EQUAL( msg.parameters().elem_size(), 0 );
    BOOST_CHECK_EQUAL( msg.start_time().data(), "2011-04-08T10:01:36" );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_set_automat_mode, Fixture )
{
    xos << xml::attribute( "target", 27u )
        << xml::attribute( "engaged", true )
        << xml::attribute( "type", "change_mode" )
        << xml::attribute( "time", "2011-04-08T10:01:36" );
    const auto msg = Read< SetAutomatMode >();
    BOOST_CHECK_EQUAL( msg.automate().id(), 27u );
    BOOST_CHECK_EQUAL( msg.mode(), engaged );
    BOOST_CHECK_EQUAL( msg.start_time().data(), "2011-04-08T10:01:36" );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_client_to_sim, Fixture )
{
    xos << xml::attribute( "id", 2053 )
        << xml::attribute( "target", 8323 )
        << xml::attribute( "type", "mission" );
    MOCK_EXPECT( reader.Resolve ).once().with( 8323u ).returns( Reader_ABC::AUTOMAT );
    const auto msg = Read< ClientToSim_Content >();
    BOOST_CHECK( msg.has_automat_order() );
    BOOST_CHECK_EQUAL( msg.automat_order().tasker().id(), 8323u );
    BOOST_CHECK_EQUAL( msg.automat_order().type().id(), 2053u );
    CheckCycle( msg );
}

BOOST_FIXTURE_TEST_CASE( read_complex_list_of_list, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='list'>"
    "    <parameter type='list'>"
    "      <parameter type='quantity' value='1'/>"
    "      <parameter type='enumeration' value='1'/>"
    "      <parameter type='enumeration' value='1'/>"
    "      <parameter type='list'>"
    "        <parameter type='list'>"
    "          <parameter type='identifier' value='1'/>"
    "          <parameter type='enumeration' value='1'/>"
    "        </parameter>"
    "        <parameter type='list'>"
    "          <parameter type='identifier' value='2'/>"
    "          <parameter type='enumeration' value='2'/>"
    "        </parameter>"
    "      </parameter>"
    "      <parameter type='boolean' value='false'/>"
    "      <parameter type='boolean' value='false'/>"
    "    </parameter>"
    "    <parameter type='list'>"
    "      <parameter type='quantity' value='1'/>"
    "      <parameter type='enumeration' value='1'/>"
    "      <parameter type='enumeration' value='1'/>"
    "      <parameter type='list'>"
    "        <parameter type='list'>"
    "          <parameter type='identifier' value='0'/>"
    "          <parameter type='enumeration' value='0'/>"
    "        </parameter>"
    "      </parameter>"
    "      <parameter type='boolean' value='false'/>"
    "      <parameter type='boolean' value='false'/>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), false );
    const auto& list = msg.elem( 0 ).value();
    BOOST_CHECK_EQUAL( list.size(), 2 );
    CheckCycle( input, msg );
}

#if 0
namespace
{
    typedef std::vector< ClientToSim_Content > T_Content;

    void ReadAction( size_t& count, xml::xostream& xos, xml::xistream& xis )
    {
        ClientToSim_Content msg;
        {
            MockReader reader;
            MOCK_EXPECT( reader.Convert ).returns( dummy );
            MOCK_EXPECT( reader.Resolve ).returns( Reader_ABC::AUTOMAT );
            Read( reader, msg, xis );
        }
        {
            MockWriter writer;
            MOCK_EXPECT( writer.Convert ).returns( "fixme" );
            Write( xml::xosubstream( xos ) << xml::start( "action" ), writer, msg );
        }
        ++count;
    }
}

BOOST_AUTO_TEST_CASE( read_aurige_orders )
{
    const std::string input = "orders.ord";
    tools::Xifstream xis( BOOST_RESOLVE( input ) );
    tools::Xofstream xos( BOOST_RESOLVE( input + ".clone" ) );
    xis >> xml::start( "actions" );
    xos << xml::start( "actions" );
    size_t count = 0;
    xis >> xml::list( "action", boost::bind( &ReadAction, boost::ref( count ), boost::ref( xos ), _1 ) );
    BOOST_CHECK_EQUAL( count, 5u );
}
#endif
