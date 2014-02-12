// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "actions_test_pch.h"
#include "MockEntityResolver.h"
#include "Tools.h"
#include "actions/ActionParameterFactory.h"
#include "actions/ActionFactory.h"
#include "actions/Action_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "protocol/Protocol.h"
#include <tools/Resolver.h>
#include <protocol/ServerPublisher_ABC.h>
#include <xeumeuleu/xml.hpp>

using namespace actions;

namespace
{
    MOCK_BASE_CLASS( MockTime, kernel::Time_ABC )
    {
        MOCK_METHOD( GetDateTime, 0 );
        MOCK_METHOD( GetInitialDateTime, 0 );
        MOCK_METHOD( GetTickDuration, 0 );
        MOCK_METHOD( GetTimeAsString, 0 );
    };

    MOCK_BASE_CLASS( MockPublisher, Publisher_ABC )
    {
        MOCK_METHOD( Send, 1, void( const sword::ClientToSim& ),            ClientToSim );
        MOCK_METHOD( Send, 1, void( const sword::ClientToAuthentication& ), ClientToAuthentication );
        MOCK_METHOD( Send, 1, void( const sword::ClientToReplay& ),         ClientToReplay );
        MOCK_METHOD( Send, 1, void( const sword::ClientToAar& ),            ClientToAar );
        MOCK_METHOD( Send, 1, void( const sword::ClientToMessenger& ),      ClientToMessenger );
        MOCK_METHOD( Register, 1, void( T_SimHandler ),    SimRegister );
        MOCK_METHOD( Register, 1, void( T_ReplayHandler ), ReplayRegister );
    };

    class Fixture
    {
    public:
        Fixture()
            : parameterFactory( coordinateConverter, entityResolver, staticModel, agentKnowledgeConverter, objectKnowledgeConverter, controller )
            , actionFactory( controller, parameterFactory, entityResolver, staticModel, time )
        {
            xml::xistringstream xis( "<mission cdt-dia-behavior='worldwide.commander.tasks.Attack' dia-type='worldwide.commander.tasks.Attack' id='44523' mrt-dia-behavior='worldwide.commander.tasks.Attack' name='COMBAT - Attack'>"
                                     "  <parameter dia-name='dangerDirection_' is-context='true' name='Danger direction' optional='false' type='Heading'/>"
                                     "  <parameter dia-name='phaseLines_' is-context='true' max-occurs='unbounded' min-occurs='1' name='Phase lines' optional='true' type='PhaseLine'/>"
                                     "  <parameter dia-name='boundaryLimit1_' is-context='true' name='Limit 1' optional='false' type='Limit'/>"
                                     "  <parameter dia-name='boundaryLimit2_' is-context='true' name='Limit 2' optional='false' type='Limit'/>"
                                     "  <parameter dia-name='echelonNumber' max-value='2' min-value='1' name='Echelon number' optional='true' type='Integer'/>"
                                    "</mission>" );
            kernel::MissionType* type = new kernel::MissionType( xis >> xml::start( "mission" ), eMissionType_Automat );
            staticModel.types_.tools::Resolver< kernel::MissionType >::Register( 44523, *type );
        }
        virtual ~Fixture()
        {
            staticModel.types_.tools::Resolver< kernel::MissionType >::DeleteAll();
        }
        kernel::CoordinateConverter coordinateConverter;
        MockEntityResolver entityResolver;
        kernel::Controller controller;
        kernel::StaticModel staticModel;
        MockAgentKnowledgeConverter agentKnowledgeConverter;
        MockObjectKnowledgeConverter objectKnowledgeConverter;
        ActionParameterFactory parameterFactory;
        MockTime time;
        ActionFactory actionFactory;
    };
}

BOOST_FIXTURE_TEST_CASE( serialize_null_parameter_when_type_attribute_missing, Fixture )
{
    xml::xistringstream xis( "<action id='44523' start_time='20091126T172608' target='146' time='2009-11-26T17:26:08' type='mission'>"
                             "  <parameter type='heading' value='360'/>"
                             "  <parameter/>"
                             "  <parameter type='limit'>"
                             "    <location type='line'>"
                             "      <point coordinates='35RPP6184187366'/>"
                             "    </location>"
                             "  </parameter>"
                             "  <parameter type='limit'>"
                             "    <location type='line'>"
                             "      <point coordinates='35RPP8069481729'/>"
                             "    </location>"
                             "  </parameter>"
                             "  <parameter/>"
                            "</action>" );

    MockAgent agent;
    MOCK_EXPECT( entityResolver.FindAgent ).with( 146u ).returns( &agent );
    MOCK_EXPECT( agent.GetId ).returns( 146u );
    MOCK_EXPECT( agent.GetName ).returns( "agent" );

    std::auto_ptr< Action_ABC > action( actionFactory.CreateAction( xis >> xml::start( "action" ) ) );
    BOOST_REQUIRE( action.get() );
    MockPublisher publicher;

    sword::ClientToSim msg;
    MOCK_EXPECT( publicher.ClientToSim ).once().with( mock::retrieve( msg ) );
    action->Publish( publicher, 42 );

    BOOST_CHECK( msg.message().has_automat_order() );
    const sword::AutomatOrder& order = msg.message().automat_order();
    BOOST_CHECK( order.has_parameters() );
    // Parameter number is consistent
    const sword::MissionParameters& parameters =order.parameters();
    BOOST_CHECK_EQUAL( parameters.elem_size(), 5 );
    // second and fifth parameter are optional
    BOOST_CHECK( parameters.elem( 1 ).null_value() );
    BOOST_CHECK( parameters.elem( 4 ).null_value() );
}

namespace
{
    void Filter( xml::xostream& xos, xml::xistream& xis )
    {
        xis >> xml::list([&]( const std::string&, const std::string& name, xml::xistream& sub ){
            xos << xml::start( name );
            Filter( xos, sub );
            xos << xml::end;
        });
        xis >> xml::attributes([&]( const std::string&, const std::string& name, xml::xistream& sub ){
            if( name != "name" )
                xos << xml::attribute( name, sub );
        });
    }
}

// test whether action_abc factory gives same output
// with either xml or protobuf input
BOOST_FIXTURE_TEST_CASE( serializes_change_equipment_state, Fixture )
{
    const QDateTime now = QDateTime::fromString( "2010-09-01T15:18:40", Qt::ISODate );
    const std::string input =
    "<action id='change_equipment_state' name='NO TRANSLATION' target='17' time='" + now.toString( Qt::ISODate ).toStdString() + "' type='magicunit'>"
    "   <parameter name='EquipmentStates' type='list'>"
    "       <parameter name='EquipmentStates' type='list'>"
    "           <parameter name='EquipmentStates' type='identifier' value='1'/>"
    "           <parameter name='EquipmentStates' type='quantity' value='2'/>"
    "           <parameter name='EquipmentStates' type='quantity' value='3'/>"
    "           <parameter name='EquipmentStates' type='quantity' value='4'/>"
    "           <parameter name='EquipmentStates' type='quantity' value='5'/>"
    "           <parameter name='EquipmentStates' type='quantity' value='6'/>"
    "           <parameter name='EquipmentStates' type='quantity' value='7'/>"
    "           <parameter name='EquipmentStates' type='list'>"
    "               <parameter name='EquipmentStates' type='identifier' value='13'/>"
    "           </parameter>"
    "       </parameter>"
    "   </parameter>"
    "</action>";
    xml::xistringstream xis( input );
    MockAgent agent;
    MOCK_EXPECT( entityResolver.FindAgent ).with( 17u ).returns( &agent );
    MOCK_EXPECT( agent.GetId ).returns( 17u );
    MOCK_EXPECT( agent.GetName ).returns( "agent" );
    staticModel.types_.tools::StringResolver< kernel::MagicActionType >::Register(
        "change_equipment_state", *new kernel::MagicActionType( "change_equipment_state", sword::UnitMagicAction::change_equipment_state ) );
    std::auto_ptr< Action_ABC > fromXml( actionFactory.CreateAction( xis >> xml::start( "action" ) ) );
    BOOST_REQUIRE( fromXml.get() );
    auto serialize = [&]( const Action_ABC& action ) -> std::string {
        xml::xostringstream xos;
        action.Serialize( xos << xml::start( "action" ) );
        return xos.str();
    };
    auto checkxml = [&]( const std::string expected, const std::string& obtained ){
        xml::xistringstream xis1( expected ), xis2( obtained );
        xml::xostringstream xos1, xos2;
        Filter( xos1, xis1 );
        Filter( xos2, xis2 );
        BOOST_REQUIRE_EQUAL( xos1.str(), xos2.str() );
    };
    auto publish = [&]( const Action_ABC& action ) -> sword::ClientToSim {
        sword::ClientToSim msg;
        MockPublisher publisher;
        MOCK_EXPECT( publisher.ClientToSim ).once().with( mock::retrieve( msg ) );
        action.Publish( publisher, 42 );
        return msg;
    };
    auto checkmagic = [&]( const sword::ClientToSim& msg ){
        BOOST_REQUIRE( msg.message().has_unit_magic_action() );
        const auto& magic = msg.message().unit_magic_action();
        BOOST_REQUIRE( magic.has_parameters() );
        BOOST_REQUIRE_EQUAL( magic.parameters().elem_size(), 1 );
        const auto& states = magic.parameters().elem( 0 );
        BOOST_REQUIRE_EQUAL( states.value_size(), 1 );
        const auto& value = states.value( 0 );
        BOOST_REQUIRE_EQUAL( value.list_size(), 8 );
        const auto& breaks = value.list( 7 );
        BOOST_REQUIRE_EQUAL( breaks.list_size(), 1 );
        const auto& list = breaks.list( 0 );
        BOOST_REQUIRE( list.has_identifier() );
        BOOST_REQUIRE_EQUAL( list.identifier(), 13u );
    };
    const auto str1 = serialize( *fromXml );
    checkxml( input, str1 );
    const auto msg1 = publish( *fromXml );
    checkmagic( msg1 );
    // check proto factory now
    MOCK_EXPECT( time.GetDateTime ).returns( now );
    MOCK_EXPECT( entityResolver.FindEntity ).with( 17u ).returns( &agent );
    std::auto_ptr< Action_ABC > fromProto( actionFactory.CreateAction( msg1, false ) );
    BOOST_REQUIRE( fromProto.get() );
    const auto str2 = serialize( *fromProto );
    checkxml( input, str2 );
    const auto msg2 = publish( *fromProto );
    BOOST_REQUIRE_EQUAL( msg1.DebugString(), msg2.DebugString() );
}
