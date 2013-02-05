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
#include "protocol/Serializers.h"

#include <xeumeuleu/xml.hpp>

using namespace protocol;
using namespace serializer;
using namespace sword;

namespace
{
    MOCK_BASE_CLASS( MockService, Service_ABC )
    {
        MOCK_METHOD( Convert, 1 );
        MOCK_METHOD( Resolve, 1 );
    };

    const Service_ABC::Point dummy = { 0.0, 0.0 };

    struct Fixture
    {
        Fixture()
            : reader( service )
        {
            xos << xml::start( "action" );
        }

        MockService service;
        Reader reader;
        xml::xostringstream xos;

        template< typename T >
        T Read( const std::string& input )
        {
            xml::xistringstream xis( input );
            xis >> xml::start( "action" );
            T dst;
            reader.Read( dst, xis );
            return dst;
        }

        template< typename T >
        T Read()
        {
            return Read< T >( xos.str() );
        }

        template< typename T >
        void AddParameterValue( const std::string& type, const T& value )
        {
            xos << xml::start( "parameter" )
                    << xml::attribute( "type", type )
                    << xml::attribute( "value", value )
                << xml::end;
        }
    };

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
    AddParameterValue( "boolean", true );
    xos << xml::start( "parameter" )
            << xml::attribute( "type", "boolean" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK( msg.elem( 0 ).value( 0 ).booleanvalue() );
    CheckNull( msg.elem( 1 ) );
}

BOOST_FIXTURE_TEST_CASE( read_numerics, Fixture )
{
    AddParameterValue( "integer", 47 );
    AddParameterValue( "numeric", 3.14f );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 2 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).intvalue(), 47 );
    BOOST_CHECK_EQUAL( msg.elem( 1 ).value( 0 ).areal(), 3.14f );
}

BOOST_FIXTURE_TEST_CASE( read_heading, Fixture )
{
    AddParameterValue( "heading", 47 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).heading().heading(), 47 );
}

BOOST_FIXTURE_TEST_CASE( read_enumeration, Fixture )
{
    AddParameterValue( "enumeration", 47 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).enumeration(), 47 );
}

BOOST_FIXTURE_TEST_CASE( read_datetime, Fixture )
{
    AddParameterValue( "datetime", "20121110T083917" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).datetime().data(), "20121110T083917" );
}

BOOST_FIXTURE_TEST_CASE( read_location, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='location'>"
    "    <location type='point'>"
    "      <point coordinates='29TNG4640002050'/>"
    "    </location>"
    "  </parameter>"
    "  <parameter type='location'>"
    "    <location type='line'>"
    "      <point coordinates='29TNG4476007950'/>"
    "      <point coordinates='29TNG4916008030'/>"
    "    </location>"
    "  </parameter>"
    "  <parameter type='location'>"
    "    <location type='polygon'>"
    "      <point coordinates='29TNF4459099545'/>"
    "      <point coordinates='29TNF4566099815'/>"
    "      <point coordinates='29TNG4916008030'/>"
    "    </location>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( service.Convert ).exactly( 6 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 3 );
    CheckLocation( msg.elem( 0 ).value( 0 ).location(), Location::point,   1 );
    CheckLocation( msg.elem( 1 ).value( 0 ).location(), Location::line,    2 );
    CheckLocation( msg.elem( 2 ).value( 0 ).location(), Location::polygon, 3 );
}

BOOST_FIXTURE_TEST_CASE( read_point, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter name='Point' type='point'>"
    "    <location type='point'>"
    "      <point coordinates='29TNF4813099675'/>"
    "    </location>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( service.Convert ).once().returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    CheckLocation( msg.elem( 0 ).value( 0 ).point().location(), Location::point, 1 );
}

BOOST_FIXTURE_TEST_CASE( read_polygon, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='polygon'>"
    "    <location type='polygon'>"
    "      <point coordinates='29TNF4619099340'/>"
    "      <point coordinates='29TNF4621099110'/>"
    "      <point coordinates='29TNF4682099300'/>"
    "      <point coordinates='29TNF4733099500'/>"
    "      <point coordinates='29TNF4729099640'/>"
    "    </location>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( service.Convert ).exactly( 5 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    CheckLocation( msg.elem( 0 ).value( 0 ).area().location(), Location::polygon, 5 );
}

BOOST_FIXTURE_TEST_CASE( read_limit, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='limit'>"
    "    <location type='line'>"
    "      <point coordinates='29TNG3941504520'/>"
    "      <point coordinates='29TNG4722506650'/>"
    "    </location>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( service.Convert ).exactly( 2 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    CheckLocation( msg.elem( 0 ).value( 0 ).limit().location(), Location::line, 2 );
}

BOOST_FIXTURE_TEST_CASE( read_path, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='path'>"
    "    <parameter type='pathpoint'>"
    "      <location type='point'>"
    "        <point coordinates='29TNG4738010820'/>"
    "      </location>"
    "    </parameter>"
    "    <parameter type='pathpoint'>"
    "      <location type='point'>"
    "        <point coordinates='29TNG4687011460'/>"
    "      </location>"
    "    </parameter>"
    "    <parameter type='pathpoint'>"
    "      <location type='point'>"
    "        <point coordinates='29TNG4251012740'/>"
    "      </location>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( service.Convert ).exactly( 3 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    CheckLocation( msg.elem( 0 ).value( 0 ).path().location(), Location::line, 3 );
}

BOOST_FIXTURE_TEST_CASE( read_phaseline, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter name='Limas' type='phaseline'>"
    "    <parameter name='Limas (item 1)' type='phaseline' value='LO'>"
    "      <parameter name='Location' type='location'>"
    "        <location type='line'>"
    "          <point coordinates='29TNH4274095890'/>"
    "          <point coordinates='29TNH4559095970'/>"
    "        </location>"
    "      </parameter>"
    "      <parameter name='Schedule' type='datetime' value='20121113T085130'/>"
    "    </parameter>"
    "    <parameter name='Limas (item 2)' type='phaseline' value='LC, LCA'>"
    "      <parameter name='Location' type='location'>"
    "        <location type='line'>"
    "          <point coordinates='29TNH4316097180'/>"
    "          <point coordinates='29TNH4673097100'/>"
    "          <point coordinates='29TNH4674097100'/>"
    "        </location>"
    "      </parameter>"
    "      <parameter name='Schedule' type='datetime' value='20121113T085132'/>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( service.Convert ).exactly( 5 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    const auto& phaseline = msg.elem( 0 ).value( 0 ).phaseline();
    BOOST_CHECK_EQUAL( phaseline.elem_size(), 2 );
    const auto& lima_1 = phaseline.elem( 0 );
    BOOST_CHECK_EQUAL( lima_1.fonctions_size(), 1 );
    BOOST_CHECK_EQUAL( lima_1.fonctions( 0 ), PhaseLineOrder::objective_line );
    CheckLocation( lima_1.line().location(), Location::line, 2 );
    BOOST_CHECK_EQUAL( lima_1.time().data(), "20121113T085130" );
    const auto& lima_2 = phaseline.elem( 1 );
    BOOST_CHECK_EQUAL( lima_2.fonctions_size(), 2 );
    BOOST_CHECK_EQUAL( lima_2.fonctions( 0 ), PhaseLineOrder::coordination_line );
    BOOST_CHECK_EQUAL( lima_2.fonctions( 1 ), PhaseLineOrder::attitude_change_line );
    CheckLocation( lima_2.line().location(), Location::line, 3 );
    BOOST_CHECK_EQUAL( lima_2.time().data(), "20121113T085132" );
}

BOOST_FIXTURE_TEST_CASE( read_automat_id, Fixture )
{
    AddParameterValue( "automat", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).automat().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_agent_id, Fixture )
{
    AddParameterValue( "agent", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).agent().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_agent_knowledge_id, Fixture )
{
    AddParameterValue( "agentknowledge", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).agentknowledge().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_crowd_knowledge_id, Fixture )
{
    AddParameterValue( "crowdknowledge", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).crowdknowledge().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_object_knowledge_id, Fixture )
{
    AddParameterValue( "objectknowledge", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).objectknowledge().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_urban_knowledge_id, Fixture )
{
    AddParameterValue( "urbanknowledge", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).urbanknowledge().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_planned_work, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter name='Chantiers (item 1)' type='plannedwork' value='barricade'>"
    "    <parameter type='obstacletype' value='reserved'/>"
    "    <parameter type='density' value='4.7'/>"
    "    <parameter type='integer' identifier='ActivityTime' value='38'/>"
    "    <parameter type='numeric' identifier='ActivationTime' value='39'/>"
    "    <parameter identifier='altitude_modifier' value='40'/>"
    "    <parameter type='quantity' identifier='time_limit' value='41'/>"
    "    <parameter identifier='obstacle_mining' name='Obstacle mining' type='bool' value='true'/>"
    "    <parameter name='TC2' type='automat' value='5169'/>"
    "    <parameter name='Name' type='string' value='gyhjkf'/>"
    "    <parameter identifier='lodging' name='Lodging' type='quantity' value='98'/>"
    "    <parameter name='Construction location' type='polygon'>"
    "      <location type='polygon'>"
    "        <point coordinates='35RPQ6669610168'/>"
    "        <point coordinates='35RPQ6699007546'/>"
    "        <point coordinates='35RPQ7177311788'/>"
    "      </location>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( service.Convert ).exactly( 3 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    const auto& work = msg.elem( 0 ).value( 0 ).plannedwork();
    BOOST_CHECK_EQUAL( work.type(), "barricade" );
    CheckLocation( work.position(), Location::polygon, 3 );
    BOOST_CHECK_EQUAL( work.type_obstacle(), ObstacleType::reserved );
    BOOST_CHECK_EQUAL( work.density(), 4.7f );
    BOOST_CHECK_EQUAL( work.combat_train().id(), 5169u );
    BOOST_CHECK_EQUAL( work.activity_time(), 38 );
    BOOST_CHECK_EQUAL( work.activation_time(), 39 );
    BOOST_CHECK_EQUAL( work.name(), "gyhjkf" );
    BOOST_CHECK_EQUAL( work.altitude_modifier(), 40 );
    BOOST_CHECK_EQUAL( work.time_limit(), 41 );
    BOOST_CHECK_EQUAL( work.mining(), true );
    BOOST_CHECK_EQUAL( work.lodging(), 98 );
}

BOOST_FIXTURE_TEST_CASE( read_nature_atlas, Fixture )
{
    AddParameterValue( "natureatlas", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).nature().flags(), 1337 );
}

BOOST_FIXTURE_TEST_CASE( read_resource_type, Fixture )
{
    AddParameterValue( "resourcetype", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).resourcetype().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_acharstr, Fixture )
{
    AddParameterValue( "string", "zomg" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).acharstr(), "zomg" );
}

BOOST_FIXTURE_TEST_CASE( read_object_id, Fixture )
{
    AddParameterValue( "object", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).object().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_party_id, Fixture )
{
    AddParameterValue( "army", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).party().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_indirect_fire_id, Fixture )
{
    AddParameterValue( "indirectfire", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).indirectfire().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_knowledge_group_id, Fixture )
{
    AddParameterValue( "knowledgegroup", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).knowledgegroup().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_formation_id, Fixture )
{
    AddParameterValue( "formation", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).formation().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_identifier, Fixture )
{
    AddParameterValue( "identifier", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).identifier(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_external_identifier, Fixture )
{
    AddParameterValue( "externalidentifier", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).external_identifier(), 1337 );
}

BOOST_FIXTURE_TEST_CASE( read_quantity, Fixture )
{
    AddParameterValue( "quantity", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).quantity(), 1337 );
}

BOOST_FIXTURE_TEST_CASE( read_equipment_type, Fixture )
{
    AddParameterValue( "equipmenttype", 1337 );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).equipmenttype().id(), 1337u );
}

BOOST_FIXTURE_TEST_CASE( read_resource_network_type, Fixture )
{
    AddParameterValue( "resourcenetworktype", "zomg" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).value( 0 ).resourcenetworktype().name(), "zomg" );
}

BOOST_FIXTURE_TEST_CASE( read_mission_objective, Fixture )
{
    AddParameterValue( "missionobjective", "test" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    BOOST_CHECK_EQUAL( msg.elem( 0 ).null_value(), true );
}

BOOST_FIXTURE_TEST_CASE( read_maintenance_priorities, Fixture )
{
    AddParameterValue( "maintenancepriorities", "7;46" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    auto& priorities = msg.elem( 0 ).value( 0 ).logmaintenancepriorities();
    BOOST_CHECK_EQUAL( priorities.elem_size(), 2 );
    BOOST_CHECK_EQUAL( priorities.elem( 0 ).id(), 7u );
    BOOST_CHECK_EQUAL( priorities.elem( 1 ).id(), 46u );
}

BOOST_FIXTURE_TEST_CASE( read_medical_priorities, Fixture )
{
    AddParameterValue( "medicalpriorities", "1;5" );
    const auto msg = Read< MissionParameters >();
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    auto& priorities = msg.elem( 0 ).value( 0 ).logmedicalpriorities();
    BOOST_CHECK_EQUAL( priorities.elem_size(), 2 );
    BOOST_CHECK_EQUAL( priorities.elem( 0 ), dead );
    BOOST_CHECK_EQUAL( priorities.elem( 1 ), wounded_extreme_urgency );
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
    auto& node = msg.elem( 0 ).value( 0 ).resourcenetworknode();
    BOOST_CHECK_EQUAL( node.object().id(), 1337u );
    BOOST_CHECK_EQUAL( node.resource().name(), "some_value" );
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
    auto& list = msg.elem( 0 ).value( 0 ).extensionlist();
    BOOST_CHECK_EQUAL( list.entries_size(), 2 );
    BOOST_CHECK_EQUAL( list.entries( 0 ).name(), "resource" );
    BOOST_CHECK_EQUAL( list.entries( 0 ).value(), "some_value" );
    BOOST_CHECK_EQUAL( list.entries( 1 ).name(), "cogito" );
    BOOST_CHECK_EQUAL( list.entries( 1 ).value(), "ergo sum" );
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
    "        <point coordinates='A'/>"
    "        <point coordinates='B'/>"
    "      </path>"
    "    </recipient>"
    "    <recipient id='6'/>"
    "    <transporter id='7' quantity='8'/>"
    "    <transporter id='9' quantity='10'/>"
    "    <wayBackPath>"
    "      <point coordinates='C'/>"
    "      <point coordinates='D'/>"
    "    </wayBackPath>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( service.Convert ).exactly( 4 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
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
    "    <wayOutPath>"
    "      <point coordinates='A'/>"
    "      <point coordinates='B'/>"
    "    </wayOutPath>"
    "    <wayBackPath>"
    "      <point coordinates='C'/>"
    "      <point coordinates='D'/>"
    "    </wayBackPath>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( service.Convert ).exactly( 4 ).returns( dummy );
    MOCK_EXPECT( service.Resolve ).once().with( 1u ).returns( Service_ABC::FORMATION );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
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
}

BOOST_FIXTURE_TEST_CASE( read_list, Fixture )
{
    const std::string input =
    "<action>"
    "  <parameter type='locationcomposite'>"
    "    <parameter type='polygon'>"
    "      <location type='polygon'>"
    "        <point coordinates='29TNF3862190625'/>"
    "        <point coordinates='29TNF4517591525'/>"
    "        <point coordinates='29TNF4590085350'/>"
    "      </location>"
    "    </parameter>"
    "    <parameter type='agent' value='1106'/>"
    "  </parameter>"
    "</action>";
    MOCK_EXPECT( service.Convert ).exactly( 3 ).returns( dummy );
    const auto msg = Read< MissionParameters >( input );
    BOOST_CHECK_EQUAL( msg.elem_size(), 1 );
    auto& val = msg.elem( 0 ).value( 0 );
    BOOST_CHECK_EQUAL( val.list_size(), 2 );
    CheckLocation( val.list( 0 ).area().location(), Location::polygon, 3 );
    BOOST_CHECK_EQUAL( val.list( 1 ).agent().id(), 1106u );
}

namespace
{
    template< typename T >
    void TestOrder( Fixture& fix )
    {
        fix.xos << xml::attribute( "target", 27u )
                << xml::attribute( "id", 17 );
        const auto msg = fix.Read< T >();
        BOOST_CHECK_EQUAL( msg.tasker().id(), 27u );
        BOOST_CHECK_EQUAL( msg.type().id(), 17u );
        BOOST_CHECK( !msg.has_parameters() );
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
        << xml::attribute( "id", 1 );
    MOCK_EXPECT( service.Resolve ).once().with( 27u ).returns( Service_ABC::TEAM );
    const auto msg = Read< FragOrder >();
    BOOST_CHECK_EQUAL( msg.tasker().party().id(), 27u );
    BOOST_CHECK_EQUAL( msg.type().id(), 1u );
    BOOST_CHECK( !msg.has_parameters() );
}

BOOST_FIXTURE_TEST_CASE( read_magic_action, Fixture )
{
    xos << xml::attribute( "id", mapping::MagicAction::data_[1].name );
    const auto msg = Read< MagicAction >();
    BOOST_CHECK_EQUAL( msg.type(), mapping::MagicAction::data_[1].type );
    BOOST_CHECK( !msg.has_parameters() );
}

BOOST_FIXTURE_TEST_CASE( read_unit_magic_action, Fixture )
{
    xos << xml::attribute( "target", 27u )
        << xml::attribute( "id", mapping::MagicUnitAction::data_[10].name );
    MOCK_EXPECT( service.Resolve ).once().with( 27u ).returns( Service_ABC::TEAM );
    const auto msg = Read< UnitMagicAction >();
    BOOST_CHECK_EQUAL( msg.tasker().party().id(), 27u );
    BOOST_CHECK_EQUAL( msg.type(), mapping::MagicUnitAction::data_[10].type );
    BOOST_CHECK( !msg.has_parameters() );
}

BOOST_FIXTURE_TEST_CASE( read_object_magic_action, Fixture )
{
    xos << xml::attribute( "target", 27u )
        << xml::attribute( "id", mapping::MagicObjectAction::data_[0].name );
    const auto msg = Read< ObjectMagicAction >();
    BOOST_CHECK_EQUAL( msg.object().id(), 27u );
    BOOST_CHECK_EQUAL( msg.type(), mapping::MagicObjectAction::data_[0].type );
    BOOST_CHECK( !msg.has_parameters() );
}

BOOST_FIXTURE_TEST_CASE( read_knowledge_magic_action, Fixture )
{
    xos << xml::attribute( "target", 27u )
        << xml::attribute( "id", mapping::MagicKnowledgeAction::data_[0].name );
    const auto msg = Read< KnowledgeMagicAction >();
    BOOST_CHECK_EQUAL( msg.knowledge_group().id(), 27u );
    BOOST_CHECK_EQUAL( msg.type(), mapping::MagicKnowledgeAction::data_[0].type );
    BOOST_CHECK( !msg.has_parameters() );
}

BOOST_FIXTURE_TEST_CASE( read_set_automat_mode, Fixture )
{
    xos << xml::attribute( "target", 27u )
        << xml::attribute( "engaged", true );
    const auto msg = Read< SetAutomatMode >();
    BOOST_CHECK_EQUAL( msg.automate().id(), 27u );
    BOOST_CHECK_EQUAL( msg.mode(), engaged );
}

BOOST_FIXTURE_TEST_CASE( read_client_to_sim, Fixture )
{
    xos << xml::attribute( "id", 2053 )
        << xml::attribute( "target", 8323 )
        << xml::attribute( "type", "mission" );
    MOCK_EXPECT( service.Resolve ).once().with( 8323u ).returns( Service_ABC::AUTOMAT );
    const auto msg = Read< ClientToSim_Content >();
    BOOST_CHECK( msg.has_automat_order() );
    BOOST_CHECK_EQUAL( msg.automat_order().tasker().id(), 8323u );
    BOOST_CHECK_EQUAL( msg.automat_order().type().id(), 2053u );
}

namespace
{
    typedef std::vector< ClientToSim_Content > T_Content;

    void ReadAction( T_Content& dst, xml::xistream& xis )
    {
        MockService service;
        Reader reader( service );
        ClientToSim_Content msg;
        MOCK_EXPECT( service.Convert ).returns( dummy );
        MOCK_EXPECT( service.Resolve ).returns( Service_ABC::AGENT );
        reader.Read( msg, xis );
        dst.push_back( msg );
    }
}

BOOST_AUTO_TEST_CASE( read_aurige_orders )
{
    xml::xifstream xis( BOOST_RESOLVE( "aurige.ord" ) );
    xis >> xml::start( "actions" );
    T_Content data;
    xis >> xml::list( "action", boost::bind( &ReadAction, boost::ref( data ), _1 ) );
    BOOST_CHECK_EQUAL( data.size(), 14795u );
}
