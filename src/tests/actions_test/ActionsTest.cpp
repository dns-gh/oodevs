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
        MOCK_METHOD( Register, 1 );
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

    BOOST_CHECK( msg.message().has_unit_order() );
    const sword::UnitOrder& order = msg.message().unit_order();
    BOOST_CHECK( order.has_parameters() );
    // Parameter number is consistent
    const sword::MissionParameters& parameters =order.parameters();
    BOOST_CHECK_EQUAL( parameters.elem_size(), 5 );
    // second and fifth parameter are optional
    BOOST_CHECK( parameters.elem( 1 ).null_value() );
    BOOST_CHECK( parameters.elem( 4 ).null_value() );
}
