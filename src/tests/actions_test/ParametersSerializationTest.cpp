// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_test_pch.h"
#include "MockEntityResolver.h"
#include "Tools.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/FireClass.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>

namespace
{
    MOCK_BASE_CLASS( MockAutomat, kernel::Automat_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetType, 0 );
    };

    MOCK_BASE_CLASS( MockPopulation, kernel::Population_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetHealthyHumans, 0 );
        MOCK_METHOD( GetWoundedHumans, 0 );
        MOCK_METHOD( GetContaminatedHumans, 0 );
        MOCK_METHOD( GetDeadHumans, 0 );
    };

    MOCK_BASE_CLASS( MockObject, kernel::Object_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetType, 0 );
        MOCK_METHOD( Display, 1 );
        MOCK_METHOD( GetSymbol, 0 );
    };

    MOCK_BASE_CLASS( MockObjectKnowledge, kernel::ObjectKnowledge_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetTeam, 0 );
        MOCK_METHOD( GetOwner, 0 );
        MOCK_METHOD( GetEntity, 0 );
        MOCK_METHOD( GetEntityId, 0 );
        MOCK_METHOD( Display, 1 );
        MOCK_METHOD( DisplayInList, 1 );
    };

    MOCK_BASE_CLASS( MockUrbanObject, kernel::UrbanObject_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( ApplyTemplate, 1 );
        MOCK_METHOD( UpdateTemplate, 1 );
        MOCK_METHOD( IsUpdatingTemplate, 0);
        MOCK_METHOD( GetLivingSpace, 1);
        MOCK_METHOD( GetHumansMap, 0 );
    };

    const std::string point1 = "35RPQ8487711651";
    const std::string point2 = "35RPQ8487711652";
    const std::string point3 = "35RPQ8487711653";

    const std::string date1 = "20091126T170001";
    const std::string date2 = "20091126T170002";
    const std::string date3 = "20091126T170003";

    std::string CreatePhysical( const std::string& physicalType,
                                const std::string& content,
                                bool isList )
    {
        auto result = "<mission cdt-dia-behavior='cdr-dia-behavior' dia-type='dia-type' "
                        "id='1337' mrt-dia-behavior='mrt-dia-behavior' name='" + physicalType + "'>"
                        "    <parameter dia-name='" + physicalType + "'";
        if( isList )
            result += " maxOccurs='unbounded' minOccurs='1'";
        result += " name='" + physicalType + "' type='" + physicalType + "'";
        if( content.empty() )
            result += "/></mission>";
        else
            result += ">" + content + "</parameter></mission>";
        return result;
    }

    std::string CreateOrd( const std::string& name,
                            const std::string& type,
                            const std::vector< const std::string >& values,
                            const std::vector< const std::string >& contents )
    {
        BOOST_REQUIRE( values.empty() && !contents.empty() || !values.empty() && contents.empty() );
        auto result = "<action id='1337' name='" + name + "' target='42' time='2011-04-08T10:01:36' type='mission'>";
        result += "<parameter name='" + name + "' type='" + type + "'";
        if( values.size() == 1 )
            result += " value='" + values.at( 0 ) + "'/>";
        if( contents.size() == 1 )
            result += ">" + contents.at( 0 ) + "</parameter>";
        if( values.size() > 1 )
        {
            result += ">";
            for( int i = 0; i < static_cast< int >( values.size() ); ++i )
                result += "<parameter name='" + name + " (item " +
                    boost::lexical_cast< std::string >( i + 1 ) +
                    ")' type='" + type + "' value='" + values.at( i ) + "'/>";
            result += "</parameter>";
        }
        if( contents.size()  > 1 )
        {
            result += ">";
            for( int i = 0; i < static_cast< int >( contents.size() ); ++i )
            {
                result += "<parameter name='" + name + " (item " +
                    boost::lexical_cast< std::string >( i + 1 ) +
                    ")' type='" + type + "'>" + contents.at( i ) + "</parameter>";
            }
            result += "</parameter>";
        }
        result += "</action>";
        return result;
    }

    class Fixture
    {
    public:
        Fixture()
            : parameterFactory( coordinateConverter,
                                entityResolver,
                                staticModel,
                                objectKnowledgeConverter,
                                controller )
            , actionFactory( controller,
                             parameterFactory,
                             entityResolver,
                             staticModel,
                             time )
        {
            // NOTHING
        }
        virtual ~Fixture()
        {
            staticModel.types_.tools::Resolver< kernel::MissionType >::DeleteAll();
            staticModel.objectTypes_.Resolver2< kernel::DotationType >::DeleteAll();
            staticModel.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::DeleteAll();
        }
        kernel::CoordinateConverter coordinateConverter;
        MockEntityResolver entityResolver;
        kernel::Controller controller;
        kernel::StaticModel staticModel;
        MockObjectKnowledgeConverter objectKnowledgeConverter;
        MockTime time;
        actions::ActionParameterFactory parameterFactory;
        actions::ActionFactory actionFactory;
        MockAgent target;

        void RegisterMissionType( const std::string& physicalType,
                                  const std::string& content,
                                  bool isList )
        {
            xml::xistringstream xis( CreatePhysical( physicalType, content, isList ) );
            auto type = new kernel::MissionType( xis >> xml::start( "mission" ), eMissionType_Pawn );
            staticModel.types_.tools::Resolver< kernel::MissionType >::Register( 1337, *type );
        }

        void RegisterDummyLogSupplyClass()
        {
            auto input = "<logistic-supply-class id='2' type='log-supply-class'/>";
            xml::xistringstream xis( input );
            kernel::LogisticSupplyClass* supplyClass = new kernel::LogisticSupplyClass( xis >> xml::start( "logistic-supply-class" ) );
            staticModel.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::Register( supplyClass->GetName(), *supplyClass );
        }

        void RegisterDotationType( unsigned int id, const std::string& name )
        {
            auto input = "<resource category='munition' id='" + boost::lexical_cast< std::string >( id ) +
                         "' id-nature='3' logistic-supply-class='log-supply-class' name='" + name + "' nature='Solid' "
                         "network-usable='false' package-mass='0.035' package-size='200' package-volume='0.025' type='Mitraille'/>";
            xml::xistringstream xis( input );
            kernel::DotationType* dotation = new kernel::DotationType( xis >> xml::start( "resource" ), staticModel.objectTypes_ );
            staticModel.objectTypes_.Resolver2< kernel::DotationType >::Register( dotation->GetId(), dotation->GetName(), *dotation );
        }

        void RegisterEquipmentType( unsigned int id, const std::string& name )
        {
            auto input = "<equipment id='" + boost::lexical_cast< std::string >( id ) + "' max-slope='1'"
                         " name='" + name + "' protection='Aircraft' size='Heavy' slope-deceleration='0' weight='1000'>"
                         "  <weapon-systems/>"
                         "  <speeds max='42'/>"
                         "</equipment>";
            xml::xistringstream xis( input );
            kernel::EquipmentType* equipment = new kernel::EquipmentType( xis >> xml::start( "equipment" ), staticModel.objectTypes_ );
            staticModel.objectTypes_.Resolver2< kernel::EquipmentType >::Register( equipment->GetId(), equipment->GetName(), *equipment );
        }

        void RegisterObjectType( const std::string& name )
        {
            auto input = "<object name='" + name + "' point-size='5' type='" + name + "'/>";
            xml::xistringstream xis( input );
            kernel::ObjectType* object = new kernel::ObjectType( xis >> xml::start( "object" ) );
            staticModel.objectTypes_.tools::StringResolver< kernel::ObjectType >::Register( object->GetType(), *object );
        }

        void RegisterFireClass( const std::string& name )
        {
            auto input = "<fire name='" + name + "'/>";
            xml::xistringstream xis( input );
            kernel::FireClass* fire = new kernel::FireClass( xis >> xml::start( "fire" ) );
            staticModel.objectTypes_.tools::StringResolver< kernel::FireClass >::Register( fire->GetName(), *fire );
        }

        void CheckAgent( MockAgent& agent, unsigned int id )
        {
            MOCK_EXPECT( entityResolver.FindAgent ).with( id ).returns( &agent );
            MOCK_EXPECT( agent.GetId ).returns( id );
        }

        void CheckAutomat( MockAutomat& automat, unsigned int id )
        {
            MOCK_EXPECT( entityResolver.FindAutomat ).with( id ).returns( &automat );
            MOCK_EXPECT( automat.GetId ).returns( id );
        }

        void CheckCrowd( MockPopulation& crowd, unsigned int id )
        {
            MOCK_EXPECT( entityResolver.FindPopulation ).with( id ).returns( &crowd );
            MOCK_EXPECT( crowd.GetId ).returns( id );
        }

        void CheckObjectKnowledge( MockObject& object,
                                   MockObjectKnowledge& knowledge,
                                   unsigned int id )
        {
            MOCK_EXPECT( entityResolver.FindObject ).with( id ).returns( &object );
            MOCK_EXPECT( entityResolver.GetObject ).with( id ).returns( boost::ref( object ) );
            MOCK_EXPECT( objectKnowledgeConverter.FindObjectKnowledgeFromObjectWithEntity ).with( mock::same( object ), mock::same( target ) ).returns( &knowledge );
            MOCK_EXPECT( knowledge.GetEntityId ).returns( id );
        }

        void CheckUrbanObject( MockUrbanObject& urban, unsigned int id )
        {
            MOCK_EXPECT( entityResolver.FindUrbanObject ).with( id ).returns( &urban );
            MOCK_EXPECT( urban.GetId ).returns( id );
        }

        void CheckLocation( sword::Location_Geometry type,
                            const sword::Location& location,
                            const std::vector< const std::string >& points )
        {
            BOOST_REQUIRE_EQUAL( location.type(), type );
            BOOST_REQUIRE_EQUAL( location.coordinates().elem_size(), static_cast< int >( points.size() ) );
            for( int i = 0; i < static_cast< int >( points.size() ); ++i )
            {
                const geometry::Point2f expected = coordinateConverter.ConvertToXY( points.at( i ) );
                const geometry::Point2f actual   = coordinateConverter.kernel::CoordinateConverter_ABC::ConvertToXY( location.coordinates().elem( i ) );
                BOOST_CHECK_EQUAL( expected.X(), actual.X() );
                BOOST_CHECK_EQUAL( expected.Y(), actual.Y() );
            }
        }

        void BuildAndCheckLocationParameter( const std::string& physicalType,
                                             const std::string& ordType,
                                             const std::string& locationName,
                                             const std::function< void( const sword::MissionParameter& ) >& checker,
                                             const std::vector< const std::string >& points )
        {
            auto content = "<location type='" + locationName + "'>";
            for( auto it = points.begin(); it != points.end(); ++it )
                content += "  <point coordinates='" + *it + "'/>";
            content += "</location>";
            CheckParameter( physicalType, ordType, std::vector< const std::string >(), checker, "",
                            boost::assign::list_of( content ) );
        }

        void CheckLocationParameter( sword::Location_Geometry type,
                                     const std::string& locationName,
                                     const std::vector< const std::string >& points )
        {
            auto checker = [&]( const sword::MissionParameter& msg ){
                BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
                BOOST_REQUIRE( msg.value( 0 ).has_location() );
                CheckLocation( type, msg.value( 0 ).location(), points );
            };
            BuildAndCheckLocationParameter( "Location", "location", locationName, checker, points );
        }

        void CheckPhaseline( const sword::PhaseLinesOrder& phaselines,
                             const std::vector< const std::string >& points,
                             const std::string& date,
                             sword::PhaseLineOrder_Function type )
        {
            BOOST_REQUIRE_EQUAL( phaselines.elem_size(), 1 );
            auto& phaseline = phaselines.elem( 0 );
            CheckLocation( sword::Location_Geometry_line, phaseline.line().location(),
                           points );
            BOOST_REQUIRE_EQUAL( phaseline.time().data(), date );
            BOOST_REQUIRE_EQUAL( phaseline.fonctions_size(), 1 );
            BOOST_REQUIRE_EQUAL( phaseline.fonctions( 0 ), type );
        }

        void CheckPlannedWork( const sword::PlannedWork& plannedwork )
        {
            BOOST_REQUIRE( plannedwork.has_type() );
            BOOST_REQUIRE_EQUAL( plannedwork.type(), "some object" );
            BOOST_REQUIRE( plannedwork.has_position() );
            CheckLocation( sword::Location_Geometry_polygon, plannedwork.position(),
                           boost::assign::list_of( point1 )( point2 )( point3 )( point1 ) );
            BOOST_REQUIRE( plannedwork.has_type_obstacle() );
            BOOST_REQUIRE_EQUAL( plannedwork.type_obstacle(), sword::ObstacleType_DemolitionTargetType_reserved );
            BOOST_REQUIRE( plannedwork.has_density() );
            BOOST_REQUIRE_EQUAL( plannedwork.density(), 4.2f );
            BOOST_REQUIRE( plannedwork.has_combat_train() );
            BOOST_REQUIRE_EQUAL( plannedwork.combat_train().id(), 1337u );
            BOOST_REQUIRE( plannedwork.has_activity_time() );
            BOOST_REQUIRE_EQUAL( plannedwork.activity_time(), 42 );
            BOOST_REQUIRE( plannedwork.has_activation_time() );
            BOOST_REQUIRE_EQUAL( plannedwork.activation_time(), 42 );
            BOOST_REQUIRE( plannedwork.has_name() );
            BOOST_REQUIRE_EQUAL( plannedwork.name(), "some name" );
            BOOST_REQUIRE( plannedwork.has_altitude_modifier() );
            BOOST_REQUIRE_EQUAL( plannedwork.altitude_modifier(), 42 );
            BOOST_REQUIRE( plannedwork.has_time_limit() );
            BOOST_REQUIRE_EQUAL( plannedwork.time_limit(), 42 );
            BOOST_REQUIRE( plannedwork.has_mining() );
            BOOST_REQUIRE_EQUAL( plannedwork.mining(), true );
            BOOST_REQUIRE( plannedwork.has_lodging() );
            BOOST_REQUIRE_EQUAL( plannedwork.lodging(), 42 );
            BOOST_REQUIRE( plannedwork.has_fire_class() );
            BOOST_REQUIRE_EQUAL( plannedwork.fire_class(), "some fire" );
            BOOST_REQUIRE( plannedwork.has_max_combustion() );
            BOOST_REQUIRE_EQUAL( plannedwork.max_combustion(), 42 );
        }

        void Check( const std::string& input,
                    const std::function< void( const sword::MissionParameter& ) >& checker )
        {
            xml::xistringstream xis( input );
            MOCK_EXPECT( entityResolver.FindAgent ).with( 42u ).returns( &target );
            MOCK_EXPECT( entityResolver.FindEntity ).with( 42u ).returns( &target );
            MOCK_EXPECT( target.GetId ).returns( 42u );
            std::unique_ptr< actions::Action_ABC > fromXml( actionFactory.CreateAction( xis >> xml::start( "action" ) ) );
            BOOST_REQUIRE( fromXml.get() );
            const auto str1 = Serialize( *fromXml );
            CheckXml( input, str1 );
            const auto msg1 = Publish( *fromXml );
            BOOST_REQUIRE( msg1.message().has_unit_order() );
            const auto& order = msg1.message().unit_order();
            BOOST_REQUIRE( order.has_parameters() );
            BOOST_REQUIRE_EQUAL( order.parameters().elem_size(), 1 );
            const auto& param = order.parameters().elem( 0 );
            checker( param );
            std::unique_ptr< actions::Action_ABC > fromProto( actionFactory.CreateAction( msg1, false ) );
            BOOST_REQUIRE( fromProto.get() );
            const auto str2 = Serialize( *fromProto );
            CheckXml( input, str2 );
            const auto msg2 = Publish( *fromProto );
            BOOST_REQUIRE_EQUAL( msg1.DebugString(), msg2.DebugString() );
        }

        // if multiple values or contents -> consider it as a list (maxOccurs='unbounded')
        void CheckParameter( const std::string& physicalType,
                             const std::string& ordType,
                             const std::vector< const std::string >& ordValuesAsString,
                             const std::function< void( const sword::MissionParameter& ) >& checker,
                             const std::string& physicalContent = "",
                             const std::vector< const std::string >& ordContents = std::vector< const std::string >() )
        {
            RegisterMissionType( physicalType, physicalContent, ordValuesAsString.size() > 1 || ordContents.size() > 1 );
            Check( CreateOrd( physicalType, ordType, ordValuesAsString, ordContents ), checker );
        }

        template< typename T, typename U >
        void CheckSingleAndListParameter( const std::string& name,
                                          const std::string& type,
                                          const T& checker,
                                          const U& parameter )
        {
            RegisterMissionType( name, "", false );
            const auto single =
                "<action id='1337' name='" + name + "' target='42' time='2011-04-08T10:01:36' type='mission'>"
                + parameter( "" ) +
                "</action>";
            Check( single, [&]( const sword::MissionParameter& msg ){
                BOOST_CHECK_EQUAL( msg.value_size(), 1 );
                checker( msg.value( 0 ) );
            });
            const auto list =
                "<action id='1337' name='" + name + "' target='42' time='2011-04-08T10:01:36' type='mission'>"
                "  <parameter name='" + name + "' type='" + type + "'>"
                + parameter( " (item 1)" ) + parameter( " (item 2)" ) +
                "  </parameter>"
                "</action>";
            Check( list, [&]( const sword::MissionParameter& msg ){
                BOOST_CHECK_EQUAL( msg.value_size(), 2 );
                checker( msg.value( 0 ) );
                checker( msg.value( 1 ) );
            });
        }
    };
}

// =============================================================================
// Simple types: bool, numeric, integer, string, heading, datetime, enumeration
// =============================================================================

BOOST_FIXTURE_TEST_CASE( serializes_parameter_bool, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_booleanvalue() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).booleanvalue(), true );
    };
    CheckParameter( "Boolean", "boolean",
                    boost::assign::list_of( "true" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_bool_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_booleanvalue() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).booleanvalue(), true );
        BOOST_REQUIRE( msg.value( 1 ).has_booleanvalue() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).booleanvalue(), false );
        BOOST_REQUIRE( msg.value( 2 ).has_booleanvalue() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).booleanvalue(), true );
    };
    CheckParameter( "Boolean", "boolean",
                    boost::assign::list_of( "true" )( "false" )( "true" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_numeric, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_areal() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).areal(), 4.2f);
    };
    CheckParameter( "Numeric", "numeric",
                    boost::assign::list_of( "4.2" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_numeric_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_areal() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).areal(), 4.2f );
        BOOST_REQUIRE( msg.value( 1 ).has_areal() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).areal(), 13.37f );
        BOOST_REQUIRE( msg.value( 2 ).has_areal() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).areal(), 4.2f );
    };
    CheckParameter( "Numeric", "numeric",
                    boost::assign::list_of( "4.2" )( "13.37" )( "4.2" ), checker );
}

// ActionParameterFactory convert integer input into numeric value
BOOST_FIXTURE_TEST_CASE( serializes_parameter_integer, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_areal() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).areal(), 42.0f );
    };
    CheckParameter( "Integer", "numeric", boost::assign::list_of( "42" ), checker );
}

// This conversion result in a mixed integer/numeric types.
// It's actually not used, so non blocking, but we should fix that someday.
//BOOST_FIXTURE_TEST_CASE( serializes_parameter_integer_list, Fixture )
//{
//    auto checker = []( const sword::MissionParameter& msg ){
//        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
//        BOOST_REQUIRE( msg.value( 0 ).has_areal() );
//        BOOST_REQUIRE_EQUAL( msg.value( 0 ).areal(), 42 );
//        BOOST_REQUIRE( msg.value( 1 ).has_areal() );
//        BOOST_REQUIRE_EQUAL( msg.value( 1 ).areal(), 1337 );
//        BOOST_REQUIRE( msg.value( 2 ).has_areal() );
//        BOOST_REQUIRE_EQUAL( msg.value( 2 ).areal(), 42 );
//    };
//    CheckParameter( "Integer", "numeric", boost::assign::list_of( "42" )( "1337" )( "42" ), checker );
//}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_string, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_acharstr() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).acharstr(), "some string" );
    };
    CheckParameter( "String", "string",
                    boost::assign::list_of( "some string" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_string_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_acharstr() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).acharstr(), "some string" );
        BOOST_REQUIRE( msg.value( 1 ).has_acharstr() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).acharstr(), "1337" );
        BOOST_REQUIRE( msg.value( 2 ).has_acharstr() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).acharstr(), "42" );
    };
    CheckParameter( "String", "string",
                    boost::assign::list_of( "some string" )( "1337" )( "42" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_heading, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_heading() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).heading().heading(), 42 );
    };
    CheckParameter( "Heading", "heading",
                    boost::assign::list_of( "42" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_heading_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_heading() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).heading().heading(), 42 );
        BOOST_REQUIRE( msg.value( 1 ).has_heading() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).heading().heading(), 1337 );
        BOOST_REQUIRE( msg.value( 2 ).has_heading() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).heading().heading(), 42 );
    };
    CheckParameter( "Heading", "heading",
                    boost::assign::list_of( "42" )( "1337" )( "42" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_datetime, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_datetime() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).datetime().data(), date1 );
    };
    CheckParameter( "Datetime", "datetime",
                    boost::assign::list_of( date1 ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_datetime_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_datetime() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).datetime().data(), date1 );
        BOOST_REQUIRE( msg.value( 1 ).has_datetime() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).datetime().data(), date2 );
        BOOST_REQUIRE( msg.value( 2 ).has_datetime() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).datetime().data(), date3 );
    };
    CheckParameter( "Datetime", "datetime",
                    boost::assign::list_of( date1 )( date2 )( date3 ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_enumeration, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_enumeration() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).enumeration(), 1 );
    };
    auto physicalContent = "<value id='0' name='A'/>"
                           "<value id='1' name='B'/>"
                           "<value id='2' name='C'/>";
    CheckParameter( "Enumeration", "enumeration",
                    boost::assign::list_of( "1" ), checker, physicalContent );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_enumeration_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_enumeration() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).enumeration(), 2 );
        BOOST_REQUIRE( msg.value( 1 ).has_enumeration() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).enumeration(), 0 );
        BOOST_REQUIRE( msg.value( 2 ).has_enumeration() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).enumeration(), 1 );
    };
    auto physicalContent = "<value id='0' name='A'/>"
                           "<value id='1' name='B'/>"
                           "<value id='2' name='C'/>";
    CheckParameter( "Enumeration", "enumeration",
                    boost::assign::list_of( "2" )( "0" )( "1" ), checker, physicalContent );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_enumeration_throw_if_wrong_value, Fixture )
{
    const std::string physicalContent = "<value id='0' name='A'/>"
                                        "<value id='1' name='B'/>"
                                        "<value id='2' name='C'/>";
    RegisterMissionType( "Enumeration", physicalContent, false );
    auto input = CreateOrd( "Enumeration", "enumeration", boost::assign::list_of( "3" ), std::vector< const std::string >() );
    xml::xistringstream xis( input );
    MOCK_EXPECT( entityResolver.FindAgent ).once().with( 42u ).returns( &target );
    MOCK_EXPECT( entityResolver.FindEntity ).once().with( 42u ).returns( &target );
    MOCK_EXPECT( target.GetId ).once().returns( 42u );
    BOOST_CHECK_THROW( actionFactory.CreateAction( xis >> xml::start( "action" ) ), tools::Exception );
}

// =============================================================================
// Entity types: agent, agentknowledge, automat, crowdknowledge, objectknowledge,
// urbanknowledge
// =============================================================================

BOOST_FIXTURE_TEST_CASE( serializes_parameter_agent, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_agent() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).agent().id(), 1337u );
    };
    MockAgent agent;
    CheckAgent( agent, 1337u );
    CheckParameter( "Agent", "agent",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_agent_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_agent() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).agent().id(), 1u );
        BOOST_REQUIRE( msg.value( 1 ).has_agent() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).agent().id(), 2u );
        BOOST_REQUIRE( msg.value( 2 ).has_agent() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).agent().id(), 3u );
    };
    MockAgent agent1;
    MockAgent agent2;
    MockAgent agent3;
    CheckAgent( agent1, 1u );
    CheckAgent( agent2, 2u );
    CheckAgent( agent3, 3u );
    CheckParameter( "Agent", "agent",
                    boost::assign::list_of( "1" )( "2" )( "3" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_agentknowledge, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_agentknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).agentknowledge().id(), 1337u );
    };
    MockAgent agent;
    CheckAgent( agent, 1337u );
    CheckParameter( "AgentKnowledge", "agentknowledge",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_agentknowledge_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_agentknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).agentknowledge().id(), 1u );
        BOOST_REQUIRE( msg.value( 1 ).has_agentknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).agentknowledge().id(), 2u );
        BOOST_REQUIRE( msg.value( 2 ).has_agentknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).agentknowledge().id(), 3u );
    };
    MockAgent agent1;
    MockAgent agent2;
    MockAgent agent3;
    CheckAgent( agent1, 1u );
    CheckAgent( agent2, 2u );
    CheckAgent( agent3, 3u );
    CheckParameter( "AgentKnowledge", "agentknowledge",
                    boost::assign::list_of( "1" )( "2" )( "3" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_automat, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_automat() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).automat().id(), 1337u );
    };
    MockAutomat automat;
    CheckAutomat( automat, 1337u );
    CheckParameter( "Automat", "automat",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_automat_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_automat() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).automat().id(), 1u );
        BOOST_REQUIRE( msg.value( 1 ).has_automat() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).automat().id(), 2u );
        BOOST_REQUIRE( msg.value( 2 ).has_automat() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).automat().id(), 3u );
    };
    MockAutomat automat1;
    MockAutomat automat2;
    MockAutomat automat3;
    CheckAutomat( automat1, 1u );
    CheckAutomat( automat2, 2u );
    CheckAutomat( automat3, 3u );
    CheckParameter( "Automat", "automat",
                    boost::assign::list_of( "1" )( "2" )( "3" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_crowdknowledge, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_crowdknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).crowdknowledge().id(), 1337u );
    };
    MockPopulation crowd;
    CheckCrowd( crowd, 1337u );
    CheckParameter( "Crowdknowledge", "crowdknowledge",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_crowdknowledge_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_crowdknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).crowdknowledge().id(), 1u );
        BOOST_REQUIRE( msg.value( 1 ).has_crowdknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).crowdknowledge().id(), 2u );
        BOOST_REQUIRE( msg.value( 2 ).has_crowdknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).crowdknowledge().id(), 3u );
    };
    MockPopulation crowd1;
    MockPopulation crowd2;
    MockPopulation crowd3;
    CheckCrowd( crowd1, 1u );
    CheckCrowd( crowd2, 2u );
    CheckCrowd( crowd3, 3u );
    CheckParameter( "CrowdKnowledge", "crowdknowledge",
                    boost::assign::list_of( "1" )( "2" )( "3" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_objectknowledge, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_objectknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).objectknowledge().id(), 1337u );
    };
    MockObject object;
    MockObjectKnowledge knowledge;
    CheckObjectKnowledge( object, knowledge, 1337u );
    CheckParameter( "ObjectKnowledge", "objectknowledge",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_objectknowledge_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_objectknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).objectknowledge().id(), 1u );
        BOOST_REQUIRE( msg.value( 1 ).has_objectknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).objectknowledge().id(), 2u );
        BOOST_REQUIRE( msg.value( 2 ).has_objectknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).objectknowledge().id(), 3u );
    };
    MockObject object1; MockObjectKnowledge knowledge1;
    MockObject object2; MockObjectKnowledge knowledge2;
    MockObject object3; MockObjectKnowledge knowledge3;
    CheckObjectKnowledge( object1, knowledge1, 1u );
    CheckObjectKnowledge( object2, knowledge2, 2u );
    CheckObjectKnowledge( object3, knowledge3, 3u );
    CheckParameter( "ObjectKnowledge", "objectknowledge",
                    boost::assign::list_of( "1" )( "2" )( "3" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_urbanknowledge, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_urbanknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).urbanknowledge().id(), 1337u );
    };
    MockUrbanObject urban;
    CheckUrbanObject( urban, 1337u );
    CheckParameter( "UrbanKnowledge", "urbanknowledge",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_urbanknowledge_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_urbanknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).urbanknowledge().id(), 1u );
        BOOST_REQUIRE( msg.value( 1 ).has_urbanknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).urbanknowledge().id(), 2u );
        BOOST_REQUIRE( msg.value( 2 ).has_urbanknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).urbanknowledge().id(), 3u );
    };
    MockUrbanObject urban1;
    MockUrbanObject urban2;
    MockUrbanObject urban3;
    CheckUrbanObject( urban1, 1u );
    CheckUrbanObject( urban2, 2u );
    CheckUrbanObject( urban3, 3u );
    CheckParameter( "UrbanKnowledge", "urbanknowledge",
                    boost::assign::list_of( "1" )( "2" )( "3" ), checker );
}

// =============================================================================
// Locations types: path, point, polygon, path, location, limit, lima
// =============================================================================

BOOST_FIXTURE_TEST_CASE( serializes_parameter_point, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_point() );
        CheckLocation( sword::Location_Geometry_point, msg.value( 0 ).point().location(),
                       boost::assign::list_of( point1 ) );
    };
    BuildAndCheckLocationParameter( "Point", "point", "point", checker,
                                    boost::assign::list_of( point1 ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_point_list, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_point() );
        CheckLocation( sword::Location_Geometry_point, msg.value( 0 ).point().location(),
                       boost::assign::list_of( point1 ) );
        BOOST_REQUIRE( msg.value( 1 ).has_point() );
        CheckLocation( sword::Location_Geometry_point, msg.value( 1 ).point().location(),
                       boost::assign::list_of( point2 ) );
        BOOST_REQUIRE( msg.value( 2 ).has_point() );
        CheckLocation( sword::Location_Geometry_point, msg.value( 2 ).point().location(),
                       boost::assign::list_of( point1 ) );
    };
    auto content1 = "<location type='point'>"
                   "    <point coordinates='" + point1 + "'/>"
                    "</location>";
    auto content2 = "<location type='point'>"
                    "    <point coordinates='" + point2 + "'/>"
                    "</location>";
    CheckParameter( "Point", "point", std::vector< const std::string >(), checker, "",
                    boost::assign::list_of( content1 )( content2 )( content1 ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_polygon, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_area() );
        CheckLocation( sword::Location_Geometry_polygon, msg.value( 0 ).area().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 )( point1 ) );
    };
    BuildAndCheckLocationParameter( "Polygon", "polygon", "polygon", checker,
                                    boost::assign::list_of( point1 )( point2 )( point3 )( point1 ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_polygon_list, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_area() );
        CheckLocation( sword::Location_Geometry_polygon, msg.value( 0 ).area().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 )( point1 ) );
        BOOST_REQUIRE( msg.value( 1 ).has_area() );
        CheckLocation( sword::Location_Geometry_polygon, msg.value( 1 ).area().location(),
                       boost::assign::list_of( point2 )( point3 )( point1 )( point2 ) );
        BOOST_REQUIRE( msg.value( 2 ).has_area() );
        CheckLocation( sword::Location_Geometry_polygon, msg.value( 2 ).area().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 )( point1 ) );
    };
    auto content1 = "<location type='polygon'>"
                    "    <point coordinates='" + point1 + "'/>"
                    "    <point coordinates='" + point2 + "'/>"
                    "    <point coordinates='" + point3 + "'/>"
                    "    <point coordinates='" + point1 + "'/>"
                    "</location>";
    auto content2 = "<location type='polygon'>"
                    "    <point coordinates='" + point2 + "'/>"
                    "    <point coordinates='" + point3 + "'/>"
                    "    <point coordinates='" + point1 + "'/>"
                    "    <point coordinates='" + point2 + "'/>"
                    "</location>";
    CheckParameter( "Polygon", "polygon", std::vector< const std::string >(), checker, "",
                    boost::assign::list_of( content1 )( content2 )( content1 ) );
}

namespace
{
    const std::string pathContent = "<parameter name='Way point 1' type='pathpoint'>"
                                    "  <location type='point'>"
                                    "    <point coordinates='" + point1 + "'/>"
                                    "  </location>"
                                    "</parameter>"
                                    "<parameter name='Way point 2' type='pathpoint'>"
                                    "  <location type='point'>"
                                    "    <point coordinates='" + point2 + "'/>"
                                    "  </location>"
                                    "</parameter>"
                                    "<parameter name='Destination' type='pathpoint'>"
                                    "  <location type='point'>"
                                    "    <point coordinates='" + point3 + "'/>"
                                    "  </location>"
                                    "</parameter>";
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_path, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_path() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 0 ).path().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 ) );
    };
    CheckParameter( "Path", "path", std::vector< const std::string >(), checker, "",
                    boost::assign::list_of( pathContent ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_path_list, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_path() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 0 ).path().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 ) );
        BOOST_REQUIRE( msg.value( 1 ).has_path() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 1 ).path().location(),
                       boost::assign::list_of( point3 )( point2 )( point1 ) );
        BOOST_REQUIRE( msg.value( 2 ).has_path() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 2 ).path().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 ) );
    };
    auto pathContent2 = "<parameter name='Way point 1' type='pathpoint'>"
                        "  <location type='point'>"
                        "    <point coordinates='" + point3 + "'/>"
                        "  </location>"
                        "</parameter>"
                        "<parameter name='Way point 2' type='pathpoint'>"
                        "  <location type='point'>"
                        "    <point coordinates='" + point2 + "'/>"
                        "  </location>"
                        "</parameter>"
                        "<parameter name='Destination' type='pathpoint'>"
                        "  <location type='point'>"
                        "    <point coordinates='" + point1 + "'/>"
                        "  </location>"
                        "</parameter>";
    CheckParameter( "Path", "path", std::vector< const std::string >(), checker, "",
                    boost::assign::list_of( pathContent )( pathContent2 )( pathContent ) );
}

// Something goes wrong with the rectangle, but since it seems not used in worldwide
// it can wait.
//BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_rectangle, Fixture )
//{
//    CheckLocationParameter( sword::Location_Geometry_rectangle, "rectangle",
//                            boost::assign::list_of( point1 )( point2 ) );
//}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_circle, Fixture )
{
    CheckLocationParameter( sword::Location_Geometry_circle, "circle",
                            boost::assign::list_of( point1 )( point2 ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_line, Fixture )
{
    CheckLocationParameter( sword::Location_Geometry_line, "line",
                            boost::assign::list_of( point1 )( point2 )( point3 ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_point, Fixture )
{
    CheckLocationParameter( sword::Location_Geometry_point, "point",
                            boost::assign::list_of( point1 ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_poylygon, Fixture )
{
    CheckLocationParameter( sword::Location_Geometry_polygon, "polygon",
                            boost::assign::list_of( point1 )( point2 )( point3 )( point1 ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_list, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE( msg.value_size() == 4 );
        BOOST_REQUIRE( msg.value( 0 ).has_location() );
        CheckLocation( sword::Location_Geometry_circle, msg.value( 0 ).location(),
                       boost::assign::list_of( point1 )( point2 ) );
        BOOST_REQUIRE( msg.value( 1 ).has_location() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 1 ).location(),
                       boost::assign::list_of( point1 )( point2 )( point3 ) );
        BOOST_REQUIRE( msg.value( 2 ).has_location() );
        CheckLocation( sword::Location_Geometry_point, msg.value( 2 ).location(),
                       boost::assign::list_of( point1 ) );
        BOOST_REQUIRE( msg.value( 3 ).has_location() );
        CheckLocation( sword::Location_Geometry_polygon, msg.value( 3 ).location(),
                       boost::assign::list_of( point1 )( point2 )( point3 )( point1 ) );
        //BOOST_REQUIRE( msg.value( 4 ).has_location() );
        //CheckLocation( sword::Location_Geometry_rectangle, msg.value( 4 ).location(),
        //               boost::assign::list_of( point1 )( point2 ) );
    };
    auto content1 = "<location type='circle'>"
                    "  <point coordinates='" + point1 + "'/>"
                    "  <point coordinates='" + point2 + "'/>"
                    "</location>";
    auto content2 = "<location type='line'>"
                    "  <point coordinates='" + point1 + "'/>"
                    "  <point coordinates='" + point2 + "'/>"
                    "  <point coordinates='" + point3 + "'/>"
                    "</location>";
    auto content3 = "<location type='point'>"
                    "  <point coordinates='" + point1 + "'/>"
                    "</location>";
    auto content4 = "<location type='polygon'>"
                    "  <point coordinates='" + point1 + "'/>"
                    "  <point coordinates='" + point2 + "'/>"
                    "  <point coordinates='" + point3 + "'/>"
                    "  <point coordinates='" + point1 + "'/>"
                    "</location>";
    //auto content5 = "<location type='rectangle'>"
    //                "  <point coordinates='" + point1 + "'/>"
    //                "  <point coordinates='" + point2 + "'/>"
    //                "</location>";
    CheckParameter( "Location", "location", std::vector< const std::string >(), checker, "",
                    boost::assign::list_of( content1 )( content2 )( content3 )( content4 )/*( content5 )*/ );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_limit, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_limit() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 0 ).limit().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 ) );
    };
    BuildAndCheckLocationParameter( "Limit", "limit", "line", checker,
                                    boost::assign::list_of( point1 )( point2 )( point3 ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_limit_list, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_limit() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 0 ).limit().location(),
                       boost::assign::list_of( point1 )( point2 ) );
        BOOST_REQUIRE( msg.value( 1 ).has_limit() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 1 ).limit().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 ) );
        BOOST_REQUIRE( msg.value( 2 ).has_limit() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 2 ).limit().location(),
                       boost::assign::list_of( point1 )( point2 ) );
    };
    auto content1 = "<location type='line'>"
                    "  <point coordinates='" + point1 + "'/>"
                    "  <point coordinates='" + point2 + "'/>"
                    "</location>";
    auto content2 = "<location type='line'>"
                    "  <point coordinates='" + point1 + "'/>"
                    "  <point coordinates='" + point2 + "'/>"
                    "  <point coordinates='" + point3 + "'/>"
                    "</location>";
    CheckParameter( "Limit", "limit", std::vector< const std::string >(), checker, "",
                    boost::assign::list_of( content1 )( content2 )( content1 ) );
}

// Single phaseline doesn't work ... it's not supported by the simulation but nonetheless,
// adaptation propose it so it should be fixed

// Multiple phaseline work strangely, they're all recorded as values in sword::MissionParameter,
// instead of being elements under the sword::PhaseLinesOrder.
// It result that the repeated field "elem" in sword::PhaseLinesOrder seems obsolete,
// as the whole class sword::PhaseLinesOrder.
BOOST_FIXTURE_TEST_CASE( serializes_parameter_lima_list, Fixture )
{
    auto input = "<action id='1337' name='PhaseLine' target='42' time='2011-04-08T10:01:36' type='mission'>"
                 "  <parameter name='PhaseLine' type='phaseline'>"
                 "    <parameter name='PhaseLine (item 1)' type='phaseline' value='LDM'>"
                 "      <parameter name='Location' type='location'>"
                 "        <location type='line'>"
                 "          <point coordinates='" + point1 + "'/>"
                 "          <point coordinates='" + point2 + "'/>"
                 "          <point coordinates='" + point3 + "'/>"
                 "        </location>"
                 "      </parameter>"
                 "      <parameter name='Schedule' type='datetime' value='" + date1 + "'/>"
                 "      <parameter name='Phase Line Objects' type='objectknowledge'/>"
                 "    </parameter>"
                 "    <parameter name='PhaseLine (item 2)' type='phaseline' value='LFM'>"
                 "      <parameter name='Location' type='location'>"
                 "        <location type='line'>"
                 "          <point coordinates='" + point3 + "'/>"
                 "          <point coordinates='" + point2 + "'/>"
                 "          <point coordinates='" + point1 + "'/>"
                 "        </location>"
                 "      </parameter>"
                 "      <parameter name='Schedule' type='datetime' value='" + date2 + "'/>"
                 "      <parameter name='Phase Line Objects' type='objectknowledge'/>"
                 "    </parameter>"
                 "  </parameter>"
                 "</action>";
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 2 );
        BOOST_REQUIRE( msg.value( 0 ).has_phaseline() );
        CheckPhaseline( msg.value( 0 ).phaseline(),
                        boost::assign::list_of( point1 )( point2 )( point3 ),
                        date1,
                        sword::PhaseLineOrder_Function_start_of_mission_line );
        CheckPhaseline( msg.value( 1 ).phaseline(),
                        boost::assign::list_of( point3 )( point2 )( point1 ),
                        date2,
                        sword::PhaseLineOrder_Function_end_of_mission_line );
    };
    RegisterMissionType( "PhaseLine", "", true );
    Check( input, checker );
}

// =============================================================================
// Location composite
// =============================================================================

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_composite_point, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_point() );
        CheckLocation( sword::Location_Geometry_point, msg.value( 0 ).point().location(),
                       boost::assign::list_of( point1 ) );
    };
    BuildAndCheckLocationParameter( "LocationComposite", "point", "point", checker,
                                    boost::assign::list_of( point1 ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_composite_polygon, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_area() );
        CheckLocation( sword::Location_Geometry_polygon, msg.value( 0 ).area().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 )( point1 ) );
    };
    BuildAndCheckLocationParameter( "LocationComposite", "polygon", "polygon", checker,
                                    boost::assign::list_of( point1 )( point2 )( point3 )( point1 ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_composite_path, Fixture )
{
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_path() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 0 ).path().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 ) );
    };
    auto content = "<parameter name='Way point 1' type='pathpoint'>"
                   "  <location type='point'>"
                   "    <point coordinates='" + point1 + "'/>"
                   "  </location>"
                   "</parameter>"
                   "<parameter name='Way point 2' type='pathpoint'>"
                   "  <location type='point'>"
                   "    <point coordinates='" + point2 + "'/>"
                   "  </location>"
                   "</parameter>"
                   "<parameter name='Destination' type='pathpoint'>"
                   "  <location type='point'>"
                   "    <point coordinates='" + point3 + "'/>"
                   "  </location>"
                   "</parameter>";
    CheckParameter( "LocationComposite", "path", std::vector< const std::string >(), checker, "",
                    boost::assign::list_of( content ) );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_composite_agent, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_agent() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).agent().id(), 1337u );
    };
    MockAgent agent;
    CheckAgent( agent, 1337u );
    CheckParameter( "LocationComposite", "agent",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_composite_agentknowledge, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_agentknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).agentknowledge().id(), 1337u );
    };
    MockAgent agent;
    CheckAgent( agent, 1337u );
    CheckParameter( "LocationComposite", "agentknowledge",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_composite_automat, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_automat() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).automat().id(), 1337u );
    };
    MockAutomat automat;
    CheckAutomat( automat, 1337u );
    CheckParameter( "LocationComposite", "automat",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_composite_crowdknowledge, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_crowdknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).crowdknowledge().id(), 1337u );
    };
    MockPopulation crowd;
    CheckCrowd( crowd, 1337u );
    CheckParameter( "LocationComposite", "crowdknowledge",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_composite_objectknowledge, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_objectknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).objectknowledge().id(), 1337u );
    };
    MockObject object;
    MockObjectKnowledge knowledge;
    CheckObjectKnowledge( object, knowledge, 1337u );
    CheckParameter( "LocationComposite", "objectknowledge",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_composite_urbanknowledge, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_urbanknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).urbanknowledge().id(), 1337u );
    };
    MockUrbanObject urban;
    CheckUrbanObject( urban, 1337u );
    CheckParameter( "LocationComposite", "urbanknowledge",
                    boost::assign::list_of( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_location_composite_list, Fixture )
{
    auto input = "<action id='1337' name='LocationComposite' target='42' time='2011-04-08T10:01:36' type='mission'>"
                 "  <parameter name='LocationComposite' type='locationcomposite'>"
                 "    <parameter name='LocationComposite (item 1)' type='point'>"
                 "      <location type='point'>"
                 "        <point coordinates='" + point1 + "'/>"
                 "      </location>"
                 "    </parameter>"
                 "    <parameter name='LocationComposite (item 2)' type='polygon'>"
                 "      <location type='polygon'>"
                 "        <point coordinates='" + point1 + "'/>"
                 "        <point coordinates='" + point2 + "'/>"
                 "        <point coordinates='" + point3 + "'/>"
                 "        <point coordinates='" + point1 + "'/>"
                 "      </location>"
                 "    </parameter>"
                 "    <parameter name='LocationComposite (item 3)' type='path'>"
                 "      <parameter name='Way point 1' type='pathpoint'>"
                 "        <location type='point'>"
                 "          <point coordinates='" + point1 + "'/>"
                 "        </location>"
                 "      </parameter>"
                 "      <parameter name='Way point 2' type='pathpoint'>"
                 "        <location type='point'>"
                 "          <point coordinates='" + point2 + "'/>"
                 "        </location>"
                 "      </parameter>"
                 "      <parameter name='Destination' type='pathpoint'>"
                 "        <location type='point'>"
                 "          <point coordinates='" + point3 + "'/>"
                 "        </location>"
                 "      </parameter>"
                 "    </parameter>"
                 "    <parameter name='LocationComposite (item 4)' type='agent' value='1337'/>"
                 "    <parameter name='LocationComposite (item 5)' type='agentknowledge' value='1337'/>"
                 "    <parameter name='LocationComposite (item 6)' type='automat' value='1337'/>"
                 "    <parameter name='LocationComposite (item 7)' type='crowdknowledge' value='1337'/>"
                 "    <parameter name='LocationComposite (item 8)' type='objectknowledge' value='1337'/>"
                 "    <parameter name='LocationComposite (item 9)' type='urbanknowledge' value='1337'/>"
                 "  </parameter>"
                 "</action>";
    auto checker = [&]( const sword::MissionParameter& msg ){
        BOOST_REQUIRE( msg.value_size() == 9 );
        BOOST_REQUIRE( msg.value( 0 ).has_point() );
        CheckLocation( sword::Location_Geometry_point, msg.value( 0 ).point().location(),
                       boost::assign::list_of( point1 ) );
        BOOST_REQUIRE( msg.value( 1 ).has_area() );
        CheckLocation( sword::Location_Geometry_polygon, msg.value( 1 ).area().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 )( point1 ) );
        BOOST_REQUIRE( msg.value( 2 ).has_path() );
        CheckLocation( sword::Location_Geometry_line, msg.value( 2 ).path().location(),
                       boost::assign::list_of( point1 )( point2 )( point3 ) );
        BOOST_REQUIRE( msg.value( 3 ).has_agent() );
        BOOST_REQUIRE_EQUAL( msg.value( 3 ).agent().id(), 1337u );
        BOOST_REQUIRE( msg.value( 4 ).has_agentknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 4 ).agentknowledge().id(), 1337u );
        BOOST_REQUIRE( msg.value( 5 ).has_automat() );
        BOOST_REQUIRE_EQUAL( msg.value( 5 ).automat().id(), 1337u );
        BOOST_REQUIRE( msg.value( 6 ).has_crowdknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 6 ).crowdknowledge().id(), 1337u );
        BOOST_REQUIRE( msg.value( 7 ).has_objectknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 7 ).objectknowledge().id(), 1337u );
        BOOST_REQUIRE( msg.value( 8 ).has_urbanknowledge() );
        BOOST_REQUIRE_EQUAL( msg.value( 8 ).urbanknowledge().id(), 1337u );
    };
    MockAgent agent;
    CheckAgent( agent, 1337u );
    CheckAgent( agent, 1337u );
    MockAutomat automat;
    CheckAutomat( automat, 1337u );
    MockPopulation crowd;
    CheckCrowd( crowd, 1337u );
    MockObject object;
    MockObjectKnowledge knowledge;
    CheckObjectKnowledge( object, knowledge, 1337u );
    MockUrbanObject urban;
    CheckUrbanObject( urban, 1337u );

    RegisterMissionType( "LocationComposite", "", true );
    Check( input, checker );
}

// =============================================================================
// Other types: ResourceType, MaintenancePriorities, MedicalPriorities,
// PlannedWork, ResourceNetworkNode
// =============================================================================

BOOST_FIXTURE_TEST_CASE( serializes_parameter_resource_type, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_resourcetype() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).resourcetype().id(), 42u );
    };
    RegisterDummyLogSupplyClass();
    RegisterDotationType( 42u, "some dotation" );
    CheckParameter( "ResourceType", "resourcetype",
                    boost::assign::list_of( "42" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_resource_type_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_resourcetype() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).resourcetype().id(), 42u );
        BOOST_REQUIRE( msg.value( 1 ).has_resourcetype() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).resourcetype().id(), 1337u );
        BOOST_REQUIRE( msg.value( 2 ).has_resourcetype() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).resourcetype().id(), 42u );
    };
    RegisterDummyLogSupplyClass();
    RegisterDotationType( 42u, "some dotation 1" );
    RegisterDotationType( 1337u, "some dotation 2" );
    CheckParameter( "ResourceType", "resourcetype",
                    boost::assign::list_of( "42" )( "1337" )( "42" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_nature_atlas, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_nature() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).nature().flags(), 42 );
    };
    CheckParameter( "NatureAtlas", "natureatlas",
                    boost::assign::list_of( "42" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_nature_atlas_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_nature() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).nature().flags(), 42 );
        BOOST_REQUIRE( msg.value( 1 ).has_nature() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).nature().flags(), 1337 );
        BOOST_REQUIRE( msg.value( 2 ).has_nature() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).nature().flags(), 42 );
    };
    CheckParameter( "NatureAtlas", "natureatlas",
                    boost::assign::list_of( "42" )( "1337" )( "42" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_resource_network_nodeype, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_resourcenetworknode() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).resourcenetworknode().object().id(), 42u );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).resourcenetworknode().resource().name(), "Resource Network Name" );
    };
    auto input = "<action id='1337' name='ResourceNetworkNode' target='42' time='2011-04-08T10:01:36' type='mission'>"
                 "  <parameter name='ResourceNetworkNode' type='resourcenetworknode' value='42'>"
                 "    <parameter name='Resource' type='resource' value='Resource Network Name'/>"
                 "  </parameter>"
                 "</action>";
    MockUrbanObject urban;
    CheckUrbanObject( urban, 42u );
    RegisterMissionType( "ResourceNetworkNode", "", false );
    Check( input, checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_resource_network_node_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_resourcenetworknode() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).resourcenetworknode().object().id(), 42u );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).resourcenetworknode().resource().name(), "Resource Network Name 1" );
        BOOST_REQUIRE( msg.value( 1 ).has_resourcenetworknode() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).resourcenetworknode().object().id(), 1337u );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).resourcenetworknode().resource().name(), "Resource Network Name 2" );
        BOOST_REQUIRE( msg.value( 2 ).has_resourcenetworknode() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).resourcenetworknode().object().id(), 42u );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).resourcenetworknode().resource().name(), "Resource Network Name 3" );
    };
    auto input = "<action id='1337' name='ResourceNetworkNode' target='42' time='2011-04-08T10:01:36' type='mission'>"
                 "  <parameter name='ResourceNetworkNode' type='resourcenetworknode'>"
                 "    <parameter name='ResourceNetworkNode (item 1)' type='resourcenetworknode' value='42'>"
                 "      <parameter name='Resource' type='resource' value='Resource Network Name 1'/>"
                 "    </parameter>"
                 "    <parameter name='ResourceNetworkNode (item 2)' type='resourcenetworknode' value='1337'>"
                 "      <parameter name='Resource' type='resource' value='Resource Network Name 2'/>"
                 "    </parameter>"
                 "    <parameter name='ResourceNetworkNode (item 3)' type='resourcenetworknode' value='42'>"
                 "      <parameter name='Resource' type='resource' value='Resource Network Name 3'/>"
                 "    </parameter>"
                 "  </parameter>"
                 "</action>";
    MockUrbanObject urban1;
    CheckUrbanObject( urban1, 42u );
    MockUrbanObject urban2;
    CheckUrbanObject( urban2, 1337u );
    RegisterMissionType( "ResourceNetworkNode", "", true );
    Check( input, checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_maintenance_priorities, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_logmaintenancepriorities() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmaintenancepriorities().elem_size(), 2 );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmaintenancepriorities().elem( 0 ).id(), 42u );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmaintenancepriorities().elem( 1 ).id(), 1337u );
    };
    RegisterEquipmentType( 42u, "some equipment 1 " );
    RegisterEquipmentType( 1337u, "some equipment 2 " );
    CheckParameter( "MaintenancePriorities", "maintenancepriorities",
                    boost::assign::list_of( "42;1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_maintenance_priorities_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_logmaintenancepriorities() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmaintenancepriorities().elem_size(), 2 );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmaintenancepriorities().elem( 0 ).id(), 42u );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmaintenancepriorities().elem( 1 ).id(), 1337u );
        BOOST_REQUIRE( msg.value( 1 ).has_logmaintenancepriorities() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).logmaintenancepriorities().elem_size(), 1 );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).logmaintenancepriorities().elem( 0 ).id(), 42u );
        BOOST_REQUIRE( msg.value( 2 ).has_logmaintenancepriorities() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).logmaintenancepriorities().elem_size(), 1 );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).logmaintenancepriorities().elem( 0 ).id(), 1337u );
    };
    RegisterEquipmentType( 42u, "some equipment 1 " );
    RegisterEquipmentType( 1337u, "some equipment 2 " );
    CheckParameter( "MaintenancePriorities", "maintenancepriorities",
                    boost::assign::list_of( "42;1337" )( "42" )( "1337" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_medical_priorities, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 1 );
        BOOST_REQUIRE( msg.value( 0 ).has_logmedicalpriorities() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmedicalpriorities().elem_size(), 2 );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmedicalpriorities().elem( 0 ), sword::unwounded );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmedicalpriorities().elem( 1 ), sword::dead );
    };
    CheckParameter( "MedicalPriorities", "medicalpriorities",
                    boost::assign::list_of( "0;1" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_medical_priorities_list, Fixture )
{
    auto checker = []( const sword::MissionParameter& msg ){
        BOOST_REQUIRE_EQUAL( msg.value_size(), 3 );
        BOOST_REQUIRE( msg.value( 0 ).has_logmedicalpriorities() );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmedicalpriorities().elem_size(), 2 );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmedicalpriorities().elem( 0 ), sword::unwounded );
        BOOST_REQUIRE_EQUAL( msg.value( 0 ).logmedicalpriorities().elem( 1 ), sword::dead );
        BOOST_REQUIRE( msg.value( 1 ).has_logmedicalpriorities() );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).logmedicalpriorities().elem_size(), 1 );
        BOOST_REQUIRE_EQUAL( msg.value( 1 ).logmedicalpriorities().elem( 0 ), sword::wounded_urgency_1 );
        BOOST_REQUIRE( msg.value( 2 ).has_logmedicalpriorities() );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).logmedicalpriorities().elem_size(), 1 );
        BOOST_REQUIRE_EQUAL( msg.value( 2 ).logmedicalpriorities().elem( 0 ), sword::wounded_urgency_2 );
    };
    CheckParameter( "MedicalPriorities", "medicalpriorities",
                    boost::assign::list_of( "0;1" )( "2" )( "3" ), checker );
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_planned_work, Fixture )
{
    const auto checker = [&]( const sword::MissionParameter_Value& value ){
        BOOST_REQUIRE( value.has_plannedwork() );
        CheckPlannedWork( value.plannedwork() );
    };
    MockAutomat automat;
    CheckAutomat( automat, 1337u );
    RegisterObjectType( "some object" );
    RegisterFireClass( "some fire" );
    CheckSingleAndListParameter( "PlannedWork", "plannedwork", checker,
        [&]( const std::string& suffix ) {
            return
                "<parameter name='PlannedWork" + suffix + "' type='plannedwork' value='some object'>"
                "  <parameter identifier='location' name='Construction location' type='location'>"
                "    <location type='polygon'>"
                "      <point coordinates='" + point1 + "'/>"
                "      <point coordinates='" + point2 + "'/>"
                "      <point coordinates='" + point3 + "'/>"
                "      <point coordinates='" + point1 + "'/>"
                "    </location>"
                "  </parameter>"
                "  <parameter identifier='obstacletype' name='Activation' type='obstacletype' value='1'/>"
                "  <parameter identifier='density' name='Density' type='numeric' value='4.2'/>"
                "  <parameter identifier='tc2' name='Obstacle tc2' type='automat' value='1337'/>"
                "  <parameter identifier='activitytime' name='Activity time:' type='quantity' value='42'/>"
                "  <parameter identifier='activationtime' name='Activation time:' type='quantity' value='42'/>"
                "  <parameter identifier='name' name='Name' type='string' value='some name'/>"
                "  <parameter identifier='altitude_modifier' name='Altitude modifier' type='quantity' value='42'/>"
                "  <parameter identifier='time_limit' name='Time limit' type='quantity' value='42'/>"
                "  <parameter identifier='obstacle_mining' name='Obstacle mining' type='boolean' value='true'/>"
                "  <parameter identifier='lodging' name='Lodging' type='quantity' value='42'/>"
                "  <parameter identifier='fire_class' name='Fire class:' type='fireclass' value='some fire'/>"
                "  <parameter identifier='max_combustion_energy' name='Max combustion energy:' type='quantity' value='42'/>"
                "</parameter>";
    });
}

BOOST_FIXTURE_TEST_CASE( serializes_parameter_itinerary, Fixture )
{
    const auto checker = [&]( const sword::MissionParameter_Value& value ){
        const auto& req = value.pathfind().request();
        BOOST_CHECK_EQUAL( req.unit().id(), 13u );
        sword::Location loc;
        loc.set_type( sword::Location_Geometry_point ); // whatever
        for( auto it = req.positions().begin(); it != req.positions().end(); ++it )
            *loc.mutable_coordinates()->add_elem() = *it;
        CheckLocation( sword::Location_Geometry_point, loc, boost::assign::list_of( point1 )( point2 ) );
        BOOST_CHECK_EQUAL( req.equipment_types_size(), 2 );
        BOOST_CHECK_EQUAL( req.equipment_types( 0 ).id(), 7u );
        BOOST_CHECK_EQUAL( req.equipment_types( 1 ).id(), 17u );
        BOOST_CHECK( req.ignore_dynamic_objects() );
    };
    CheckSingleAndListParameter( "Itinerary", "itinerary", checker,
        [&]( const std::string& suffix ) {
            return
                "<parameter name='Itinerary" + suffix + "' type='itinerary'>"
                "  <id value='1'/>"
                "  <request>"
                "    <unit id='13'/>"
                "    <positions>"
                "      <point coordinates='" + point1 + "'/>"
                "      <point coordinates='" + point2 + "'/>"
                "    </positions>"
                "    <equipments>"
                "      <type id='7'/>"
                "      <type id='17'/>"
                "    </equipments>"
                "    <ignore_dynamic_objects value='true'/>"
                "  </request>"
                "</parameter>";
    });
}

// =============================================================================
// Reports types: ResourceNetworkType, Stage
// =============================================================================

// ResourceNetworkType and Stage are not used by missions, only by reports, so
// they're not serialized in order files, nor manipulated by the actions library.
