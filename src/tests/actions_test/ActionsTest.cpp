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
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Team_ABC.h"
#include "ENT/ENT_Tr.h"
#include "protocol/Protocol.h"

#include <tools/Resolver.h>
#include <xeumeuleu/xml.hpp>

using namespace actions;

namespace
{
    MOCK_BASE_CLASS( MockTeam, kernel::Team_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
    };

    MOCK_BASE_CLASS( MockObject, kernel::Object_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( Display, 1 );
        MOCK_METHOD( GetType, 0 );
        MOCK_METHOD( GetSymbol, 0 );
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
            staticModel.types_.CreateMagicActionTypes();
            ENT_Tr::InitTranslations();
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
        MockAgent agent;
        MockTeam team;
        MockObject object;

        void Check( const std::string& input,
                    const std::function< void( const sword::ClientToSim& ) >& checkmagic,
                    bool hasAgent = true )
        {
            xml::xistringstream xis( input );
            if( hasAgent )
            {
                MOCK_EXPECT( entityResolver.FindAgent ).exactly( 3 ).with( 17u ).returns( &agent );
                MOCK_EXPECT( agent.GetId ).exactly( 2 ).returns( 17u );
            }
            std::unique_ptr< Action_ABC > fromXml( actionFactory.CreateAction( xis >> xml::start( "action" ) ) );
            BOOST_REQUIRE( fromXml.get() );
            const auto str1 = Serialize( *fromXml );
            CheckXml( input, str1 );
            const auto msg1 = Publish( *fromXml );
            checkmagic( msg1 );
            std::unique_ptr< Action_ABC > fromProto( actionFactory.CreateAction( msg1, false ) );
            BOOST_REQUIRE( fromProto.get() );
            const auto str2 = Serialize( *fromProto );
            CheckXml( input, str2 );
            const auto msg2 = Publish( *fromProto );
            BOOST_REQUIRE_EQUAL( msg1.DebugString(), msg2.DebugString() );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( serialize_null_parameter_when_type_attribute_missing, Fixture )
{
    xml::xistringstream xis( "<action id='44523' target='17' time='2009-11-26T17:26:08' type='mission'>"
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

    MOCK_EXPECT( entityResolver.FindEntity ).once().with( 17u ).returns( &agent );
    MOCK_EXPECT( entityResolver.FindAgent ).once().with( 17u ).returns( &agent );
    MOCK_EXPECT( agent.GetId ).once().returns( 17u );

    std::unique_ptr< Action_ABC > action( actionFactory.CreateAction( xis >> xml::start( "action" ) ) );
    BOOST_REQUIRE( action.get() );

    MockPublisher publisher;
    sword::ClientToSim msg;
    MOCK_EXPECT( publisher.ClientToSim ).once().with( mock::retrieve( msg ) );
    action->Publish( publisher, 42 );

    BOOST_CHECK( msg.message().has_unit_order() );
    const sword::UnitOrder& order = msg.message().unit_order();
    BOOST_CHECK( order.has_parameters() );
    // Parameter number is consistent
    const sword::MissionParameters& parameters = order.parameters();
    BOOST_CHECK_EQUAL( parameters.elem_size(), 5 );
    // second and fifth parameter are optional
    BOOST_CHECK( parameters.elem( 1 ).null_value() );
    BOOST_CHECK( parameters.elem( 4 ).null_value() );
}

// test whether action_abc factory gives same output
// with either xml or protobuf input
BOOST_FIXTURE_TEST_CASE( serializes_change_equipment_state, Fixture )
{
    const std::string input =
    "<action id='change_equipment_state' name='Change equipment state' target='17' time='2011-04-08T10:01:36' type='magicunit'>"
    "   <parameter name='EquipmentStates' type='list'>"
    "       <parameter name='Equipment (item 1)' type='list'>"
    "           <parameter name='ID' type='identifier' value='1'/>"
    "           <parameter name='NbAvailable' type='quantity' value='2'/>"
    "           <parameter name='NbDestroyed' type='quantity' value='3'/>"
    "           <parameter name='NbRepairableWithEvac' type='quantity' value='4'/>"
    "           <parameter name='NbOnSiteFixable' type='quantity' value='5'/>"
    "           <parameter name='NbInMaintenance' type='quantity' value='6'/>"
    "           <parameter name='NbPrisoner' type='quantity' value='7'/>"
    "           <parameter name='Breakdowns' type='list'>"
    "               <parameter name='Breakdown (item 1)' type='identifier' value='13'/>"
    "           </parameter>"
    "       </parameter>"
    "   </parameter>"
    "</action>";
    auto checkmagic = []( const sword::ClientToSim& msg ){
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
    Check( input, checkmagic );
}

BOOST_FIXTURE_TEST_CASE( serializes_change_human_state, Fixture )
{
    const std::string input =
    "<action id='change_human_state' name='Change human state' target='17' time='2011-04-08T10:01:36' type='magicunit'>"
    "  <parameter name='HumanStates' type='list'>"
    "    <parameter name='Human (item 1)' type='list'>"
    "      <parameter name='Quantity' type='quantity' value='1'/>"
    "      <parameter name='HumanRank' type='enumeration' value='1'/>"
    "      <parameter name='HumanState' type='enumeration' value='1'/>"
    "      <parameter name='Injuries' type='list'>"
    "        <parameter name='Injury (item 1)' type='list'>"
    "          <parameter name='ID' type='identifier' value='1'/>"
    "          <parameter name='InjurySeriousness' type='enumeration' value='1'/>"
    "        </parameter>"
    "        <parameter name='Injury (item 2)' type='list'>"
    "          <parameter name='ID' type='identifier' value='2'/>"
    "          <parameter name='InjurySeriousness' type='enumeration' value='2'/>"
    "        </parameter>"
    "      </parameter>"
    "      <parameter name='MentallyWounded' type='boolean' value='false'/>"
    "      <parameter name='Contaminated' type='boolean' value='false'/>"
    "    </parameter>"
    "    <parameter name='Human (item 2)' type='list'>"
    "      <parameter name='Quantity' type='quantity' value='1'/>"
    "      <parameter name='HumanRank' type='enumeration' value='1'/>"
    "      <parameter name='HumanState' type='enumeration' value='1'/>"
    "      <parameter name='Injuries' type='list'>"
    "        <parameter name='Injury (item 1)' type='list'>"
    "          <parameter name='ID' type='identifier' value='0'/>"
    "          <parameter name='InjurySeriousness' type='enumeration' value='0'/>"
    "        </parameter>"
    "      </parameter>"
    "      <parameter name='MentallyWounded' type='boolean' value='false'/>"
    "      <parameter name='Contaminated' type='boolean' value='false'/>"
    "    </parameter>"
    "  </parameter>"
    "</action>";
    auto checkmagic = []( const sword::ClientToSim& msg ){
        BOOST_REQUIRE( msg.message().has_unit_magic_action() );
        const auto& magic = msg.message().unit_magic_action();
        BOOST_REQUIRE( magic.has_parameters() );
        BOOST_REQUIRE_EQUAL( magic.parameters().elem_size(), 1 );
        const auto& states = magic.parameters().elem( 0 );
        BOOST_REQUIRE_EQUAL( states.value_size(), 2 );
        const auto& value = states.value( 0 );
        BOOST_REQUIRE_EQUAL( value.list_size(), 6 );
        const auto& quantity = value.list( 0 );
        BOOST_REQUIRE( quantity.has_quantity() );
        const auto& rank = value.list( 1 );
        BOOST_REQUIRE( rank.has_enumeration() );
        const auto& state = value.list( 2 );
        BOOST_REQUIRE( state.has_enumeration() );
        const auto& contaminated = value.list( 4 );
        BOOST_REQUIRE( contaminated.has_booleanvalue() );
        const auto& mentally = value.list( 5 );
        BOOST_REQUIRE( mentally.has_booleanvalue() );
    };
    Check( input, checkmagic );
}

#define CHECK_ATTRIBUTE_PARAMETER( attribute, index, type, value ) \
BOOST_REQUIRE( attribute##.list( index ).has_##type##() ); \
BOOST_REQUIRE_EQUAL( attribute##.list( index ).##type##(), value );

#define CHECK_ATTRIBUTE_LIST( attribute, index, size ) \
BOOST_REQUIRE_EQUAL( attribute##.list( index ).list_size(), size );

#define CHECK_ATTRIBUTE_DATETIME( attribute, index, value ) \
BOOST_REQUIRE( attribute##.list( index ).has_datetime() ); \
BOOST_REQUIRE_EQUAL( attribute##.list( index ).datetime().data(), value );

#define CHECK_ATTRIBUTE_SIZE_AND_ID( attribute, size ) \
BOOST_REQUIRE_EQUAL( attribute##.list_size(), size ); \
CHECK_ATTRIBUTE_PARAMETER( attribute##, 0, identifier, static_cast< unsigned int >( sword::ObjectMagicAction::##attribute ) );

BOOST_FIXTURE_TEST_CASE( serializes_object_creation, Fixture )
{
    const std::string input =
    "<action id='create_object' name='Object Creation' target='0' time='2011-04-08T10:01:36' type='magicobject'>"
    "   <parameter name='Type' type='string' value='abattis'/>"
    "   <parameter name='Location' type='location'>"
    "     <location type='point'>"
    "       <point coordinates='29TNF2675262470'/>"
    "     </location>"
    "   </parameter>"
    "   <parameter name='Name' type='string' value='test object'/>"
    "   <parameter name='Camp' type='army' value='1'/>"
    "   <parameter name='Attributes' type='list'>"
    "     <parameter name='Construction' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='0'/>"
    "       <parameter name='Type' type='identifier' value='100'/>"
    "       <parameter name='Number' type='quantity' value='100'/>"
    "       <parameter name='Density' type='numeric' value='100.1'/>"
    "       <parameter name='Percentage' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Obstacle' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='1'/>"
    "       <parameter name='TargetType' type='identifier' value='100'/>"
    "       <parameter name='Activation' type='boolean' value='true'/>"
    "       <parameter name='ActivationTime' type='quantity' value='100'/>"
    "       <parameter name='ActivityTime' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Mine' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='2'/>"
    "       <parameter name='Density' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Time limit' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='3'/>"
    "       <parameter name='Time' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Bypass' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='4'/>"
    "       <parameter name='Percentage' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Logistic' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='5'/>"
    "       <parameter name='TC2' type='identifier' value='100'/>"
    "     </parameter>"
    "     <parameter name='Nbc' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='6'/>"
    "       <parameter name='DangerLevel' type='quantity' value='100'/>"
    "       <parameter name='NBCAgents' type='list'>"
    "           <parameter name='ID (item 1)' type='identifier' value='1'/>"
    "           <parameter name='ID (item 2)' type='identifier' value='2'/>"
    "           <parameter name='ID (item 3)' type='identifier' value='3'/>"
    "       </parameter>"
    "       <parameter name='State' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Crossing site' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='7'/>"
    "       <parameter name='Width' type='quantity' value='100'/>"
    "       <parameter name='Depth' type='quantity' value='100'/>"
    "       <parameter name='FlowRate' type='quantity' value='100'/>"
    "       <parameter name='BanksRequireFitting' type='boolean' value='true'/>"
    "     </parameter>"
    "     <parameter name='Supply route' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='8'/>"
    "       <parameter name='Equipped' type='boolean' value='true'/>"
    "       <parameter name='MaxWeight' type='quantity' value='100'/>"
    "       <parameter name='Width' type='quantity' value='100'/>"
    "       <parameter name='Length' type='quantity' value='100'/>"
    "       <parameter name='FlowRate' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Fire' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='10'/>"
    "       <parameter name='FireClass' type='string' value='FireClass'/>"
    "       <parameter name='MaxCombustionEnergy' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Interaction height' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='12'/>"
    "       <parameter name='Height' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Stock' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='13'/>"
    "       <parameter name='Dotations' type='list'>"
    "         <parameter name='Dotation (item 1)' type='list'>"
    "           <parameter name='DotationId' type='identifier' value='1'/>"
    "           <parameter name='Stock' type='quantity' value='100'/>"
    "           <parameter name='MaxStock' type='quantity' value='100'/>"
    "         </parameter>"
    "         <parameter name='Dotation (item 2)' type='list'>"
    "           <parameter name='DotationId' type='identifier' value='2'/>"
    "           <parameter name='Stock' type='quantity' value='100'/>"
    "           <parameter name='MaxStock' type='quantity' value='100'/>"
    "         </parameter>"
    "       </parameter>"
    "     </parameter>"
    "     <parameter name='Effect delay' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='15'/>"
    "       <parameter name='Delay' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Flood' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='16'/>"
    "       <parameter name='Depth' type='quantity' value='100'/>"
    "       <parameter name='RefDist' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Structural state' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='18'/>"
    "       <parameter name='Value' type='numeric' value='100.1'/>"
    "     </parameter>"
    "     <parameter name='Infrastructure' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='19'/>"
    "       <parameter name='Threshold' type='numeric' value='100.1'/>"
    "     </parameter>"
    "     <parameter name='Lodging' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='21'/>"
    "       <parameter name='Capacity' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Trafficability' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='22'/>"
    "       <parameter name='Trafficability' type='numeric' value='100.1'/>"
    "     </parameter>"
    "     <parameter name='Underground' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='23'/>"
    "       <parameter name='Underground' type='boolean' value='true'/>"
    "     </parameter>"
    "     <parameter name='Confined' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='24'/>"
    "       <parameter name='Confined' type='boolean' value='true'/>"
    "     </parameter>"
    "     <parameter name='Alerted' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='25'/>"
    "       <parameter name='Alerted' type='boolean' value='true'/>"
    "     </parameter>"
    "     <parameter name='Evacuated' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='26'/>"
    "       <parameter name='Evacuated' type='boolean' value='true'/>"
    "     </parameter>"
    "     <parameter name='Resource network' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='27'/>"
    "     </parameter>"
    "     <parameter name='Altitude modifier' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='28'/>"
    "       <parameter name='Height' type='quantity' value='100'/>"
    "     </parameter>"
    "     <parameter name='Disaster' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='29'/>"
    "       <parameter name='Model' type='string' value='Model'/>"
    "       <parameter name='Date' type='datetime' value='20100901T151840'/>"
    "     </parameter>"
    "     <parameter name='Density' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='30'/>"
    "       <parameter name='Density' type='numeric' value='100.1'/>"
    "     </parameter>"
    "   </parameter>"
    "</action>";
    auto checkmagic = []( const sword::ClientToSim& msg ){
        BOOST_REQUIRE( msg.message().has_object_magic_action() );
        const auto& magic = msg.message().object_magic_action();
        BOOST_REQUIRE( magic.has_parameters() );
        BOOST_REQUIRE_EQUAL( magic.parameters().elem_size(), 5 );

        const auto& type = magic.parameters().elem( 0 );
        BOOST_REQUIRE_EQUAL( type.value_size(), 1 );
        BOOST_REQUIRE( type.value( 0 ).has_acharstr() );
        const auto& location = magic.parameters().elem( 1 );
        BOOST_REQUIRE_EQUAL( location.value_size(), 1 );
        BOOST_REQUIRE( location.value( 0 ).has_location() );
        const auto& name = magic.parameters().elem( 2 );
        BOOST_REQUIRE_EQUAL( name.value_size(), 1 );
        BOOST_REQUIRE( name.value( 0 ).has_acharstr() );
        const auto& camp = magic.parameters().elem( 3 );
        BOOST_REQUIRE_EQUAL( camp.value_size(), 1 );
        BOOST_REQUIRE( camp.value( 0 ).has_party() );
        const auto& attributes = magic.parameters().elem( 4 );
        BOOST_REQUIRE_EQUAL( attributes.value_size(), 26 );

        const auto& construction = attributes.value( 0 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( construction, 5 );                 // id
        CHECK_ATTRIBUTE_PARAMETER( construction, 1, identifier, 100u ); // type
        CHECK_ATTRIBUTE_PARAMETER( construction, 2, quantity, 100 );    // number
        CHECK_ATTRIBUTE_PARAMETER( construction, 3, areal, 100.1f );    // density
        CHECK_ATTRIBUTE_PARAMETER( construction, 4, quantity, 100 )     // percentage;
        const auto& obstacle = attributes.value( 1 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( obstacle, 5 );                     // id
        CHECK_ATTRIBUTE_PARAMETER( obstacle, 1, identifier, 100u );     // target type
        CHECK_ATTRIBUTE_PARAMETER( obstacle, 2, booleanvalue, true );   // Activation
        CHECK_ATTRIBUTE_PARAMETER( obstacle, 3, quantity, 100 );        // ActivationTime
        CHECK_ATTRIBUTE_PARAMETER( obstacle, 4, quantity, 100 )         // ActivityTime
        const auto& mine = attributes.value( 2 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( mine, 2 );              // id
        CHECK_ATTRIBUTE_PARAMETER( mine, 1, quantity, 100 ); // density
        const auto& time_limit = attributes.value( 3 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( time_limit, 2 );               // id
        CHECK_ATTRIBUTE_PARAMETER( time_limit, 1, quantity, 100 );  // time
        const auto& bypass = attributes.value( 4 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( bypass, 2 );              // id
        CHECK_ATTRIBUTE_PARAMETER( bypass, 1, quantity, 100 ); // percentage
        const auto& logistic = attributes.value( 5 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( logistic, 2 );                 // id
        CHECK_ATTRIBUTE_PARAMETER( logistic, 1, identifier, 100u ); // tc2
        const auto& nbc = attributes.value( 6 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( nbc, 4 );              // id
        CHECK_ATTRIBUTE_PARAMETER( nbc, 1, quantity, 100 ); // danger level
        CHECK_ATTRIBUTE_LIST( nbc, 2, 3 );                  // nbc agents
        CHECK_ATTRIBUTE_PARAMETER( nbc, 3, quantity, 100 ); // state
        const auto& crossing_site = attributes.value( 7 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( crossing_site, 5 );                    // id
        CHECK_ATTRIBUTE_PARAMETER( crossing_site, 1, quantity, 100 );       // width
        CHECK_ATTRIBUTE_PARAMETER( crossing_site, 2, quantity, 100 );       // depth
        CHECK_ATTRIBUTE_PARAMETER( crossing_site, 3, quantity, 100 );       // flowrate
        CHECK_ATTRIBUTE_PARAMETER( crossing_site, 4, booleanvalue, true );  // banks require fitting
        const auto& supply_route = attributes.value( 8 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( supply_route, 6 );                     // id
        CHECK_ATTRIBUTE_PARAMETER( supply_route, 1, booleanvalue, true );   // equipped
        CHECK_ATTRIBUTE_PARAMETER( supply_route, 2, quantity, 100 );        // max weight
        CHECK_ATTRIBUTE_PARAMETER( supply_route, 3, quantity, 100 );        // width
        CHECK_ATTRIBUTE_PARAMETER( supply_route, 4, quantity, 100 );        // length
        CHECK_ATTRIBUTE_PARAMETER( supply_route, 5, quantity, 100 );        // flow rate
        const auto& fire = attributes.value( 9 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( fire, 3 );                         // id
        CHECK_ATTRIBUTE_PARAMETER( fire, 1, acharstr, "FireClass" );    // fire class
        CHECK_ATTRIBUTE_PARAMETER( fire, 2, quantity, 100 );            // max combustion energy
        const auto& interaction_height = attributes.value( 10 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( interaction_height, 2 );              // id
        CHECK_ATTRIBUTE_PARAMETER( interaction_height, 1, quantity, 100 ); // height
        const auto& stock = attributes.value( 11 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( stock, 2 );    // id
        CHECK_ATTRIBUTE_LIST( stock, 1, 2 );        // dotations
        const auto& effect_delay = attributes.value( 12 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( effect_delay, 2 );              // id
        CHECK_ATTRIBUTE_PARAMETER( effect_delay, 1, quantity, 100 ); // delay
        const auto& flood = attributes.value( 13 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( flood, 3 );              // id
        CHECK_ATTRIBUTE_PARAMETER( flood, 1, quantity, 100 ); // depth
        CHECK_ATTRIBUTE_PARAMETER( flood, 2, quantity, 100 ); // ref dist
        const auto& structural_state = attributes.value( 14 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( structural_state, 2 );                 // id
        CHECK_ATTRIBUTE_PARAMETER( structural_state, 1, areal, 100.1f );    // value
        const auto& infrastructure = attributes.value( 15 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( infrastructure, 2 );               // id
        CHECK_ATTRIBUTE_PARAMETER( infrastructure, 1, areal, 100.1f );  // threshold
        const auto& lodging = attributes.value( 16 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( lodging, 2 );              // id
        CHECK_ATTRIBUTE_PARAMETER( lodging, 1, quantity, 100 ); // capacity
        const auto& trafficability = attributes.value( 17 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( trafficability, 2 );               // id
        CHECK_ATTRIBUTE_PARAMETER( trafficability, 1, areal, 100.1f );  // trafficability
        const auto& underground = attributes.value( 18 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( underground, 2 );                      // id
        CHECK_ATTRIBUTE_PARAMETER( underground, 1, booleanvalue, true );    // underground
        const auto& confined = attributes.value( 19 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( confined, 2 );                     // id
        CHECK_ATTRIBUTE_PARAMETER( confined, 1, booleanvalue, true );   // confined
        const auto& alerted = attributes.value( 20 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( alerted, 2 );                      // id
        CHECK_ATTRIBUTE_PARAMETER( alerted, 1, booleanvalue, true );    // alerted
        const auto& evacuated = attributes.value( 21 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( evacuated, 2 );                    // id
        CHECK_ATTRIBUTE_PARAMETER( evacuated, 1, booleanvalue, true );  // evactuated
        const auto& resource_network = attributes.value( 22 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( resource_network, 1 ); // id
        const auto& altitude_modifier = attributes.value( 23 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( altitude_modifier, 2 );              // id
        CHECK_ATTRIBUTE_PARAMETER( altitude_modifier, 1, quantity, 100 ); // height
        const auto& disaster = attributes.value( 24 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( disaster, 3 );                     // id
        CHECK_ATTRIBUTE_PARAMETER( disaster, 1, acharstr, "Model" );    // model
        CHECK_ATTRIBUTE_DATETIME( disaster, 2, "20100901T151840" );     // date
        const auto& density = attributes.value( 25 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( density, 2 );              // id
        CHECK_ATTRIBUTE_PARAMETER( density, 1, areal, 100.1f ); // density
    };
    MOCK_EXPECT( entityResolver.FindEntity ).exactly( 2 ).with( 0u ).returns( 0u );
    MOCK_EXPECT( entityResolver.FindTeam ).exactly( 2 ).with( 1u ).returns( &team );
    MOCK_EXPECT( team.GetId ).returns( 1u );
    Check( input, checkmagic, false );
}

BOOST_FIXTURE_TEST_CASE( serializes_object_update, Fixture )
{
    const std::string input =
    "<action id='update_object' name='Object Update' target='12' time='2011-04-08T10:01:36' type='magicobject'>"
    "   <parameter name='Attributes' type='list'>"
    "     <parameter name='Mine' type='list'>"
    "       <parameter name='AttributeId' type='identifier' value='2'/>"
    "       <parameter name='Type' type='identifier' value='100'/>"
    "       <parameter name='Number' type='quantity' value='100'/>"
    "       <parameter name='Density' type='numeric' value='100.1'/>"
    "       <parameter name='Percentage' type='quantity' value='100'/>"
    "     </parameter>"
    "   </parameter>"
    "</action>";
    auto checkmagic = []( const sword::ClientToSim& msg ){
        BOOST_REQUIRE( msg.message().has_object_magic_action() );
        const auto& magic = msg.message().object_magic_action();
        BOOST_REQUIRE( magic.has_parameters() );
        BOOST_REQUIRE_EQUAL( magic.parameters().elem_size(), 1 );

        const auto& attributes = magic.parameters().elem( 0 );
        BOOST_REQUIRE_EQUAL( attributes.value_size(), 1 );

        const auto& mine = attributes.value( 0 );
        CHECK_ATTRIBUTE_SIZE_AND_ID( mine, 5 );                 // id
        CHECK_ATTRIBUTE_PARAMETER( mine, 1, identifier, 100u ); // type
        CHECK_ATTRIBUTE_PARAMETER( mine, 2, quantity, 100 );    // number
        CHECK_ATTRIBUTE_PARAMETER( mine, 3, areal, 100.1f );    // density
        CHECK_ATTRIBUTE_PARAMETER( mine, 4, quantity, 100 )     // percentage;
    };
    MOCK_EXPECT( entityResolver.FindEntity ).exactly( 2 ).with( 12u ).returns( &object );
    MOCK_EXPECT( object.GetId ).exactly( 2 ).returns( 12u );
    Check( input, checkmagic, false );
}
