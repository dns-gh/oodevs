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
#include "actions/Agent.h"
#include "actions/AgentList.h"
#include "actions/Automat.h"
#include "actions/AtlasNature.h"
#include "actions/Bool.h"
#include "actions/Datetime.h"
#include "actions/Direction.h"
#include "actions/DotationType.h"
#include "actions/Enumeration.h"
#include "actions/Formation.h"
#include "actions/Level.h"
#include "actions/Lima.h"
#include "actions/Limit.h"
#include "actions/Numeric.h"
#include "actions/ObjectKnowledge.h"
#include "actions/Path.h"
#include "actions/Polygon.h"
#include "actions/PopulationKnowledge.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/AtlasNatures.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Population_ABC.h"

#include "protocol/protocol.h"
#include "tools/Resolver.h"
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>
#include <xeumeuleu/xml.hpp>

namespace bl = boost::lambda;

namespace
{
    template< typename Functor >
    std::auto_ptr< Common::MsgMissionParameter > Serialize( const std::string& type, const std::string& xmlInput, Functor creator )
    {
        const kernel::OrderParameter definition( "test", type, false );
        xml::xistringstream xis( xmlInput ); xis >> xml::start( "parameter" );
        std::auto_ptr< actions::Parameter_ABC > parameter( creator( definition, xis ) );
        std::auto_ptr< Common::MsgMissionParameter > message( new Common::MsgMissionParameter() );
        parameter->CommitTo( *message );
        return message;
    }

    void CheckSet( const Common::MsgMissionParameter& message )
    {
        BOOST_CHECK_EQUAL( false, message.null_value() );
        BOOST_REQUIRE( message.has_value() );
    }

    MOCK_BASE_CLASS( MockTeam, kernel::Team_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );

        // $$$$ _RC_ FDS 2010-01-13: to be remove
        MOCK_METHOD( Accept, 1 );
        MOCK_METHOD( GetKarma, 0 );
        MOCK_METHOD_EXT( Register, 1, void( kernel::Formation_ABC& ), RegisterFormation );
        MOCK_METHOD_EXT( Remove, 1, void( kernel::Formation_ABC& ), RemoveFormation );
        MOCK_METHOD_EXT( Register, 1, void( kernel::Population_ABC& ), RegisterPopulation );
        MOCK_METHOD_EXT( Remove, 1, void( kernel::Population_ABC& ), RemovePopulation );
        MOCK_METHOD_EXT( Register, 1, void( kernel::Object_ABC& ), RegisterObject );
        MOCK_METHOD_EXT( Remove, 1, void( kernel::Object_ABC& ), RemoveObject );
        MOCK_METHOD_EXT( Register, 1, void( kernel::KnowledgeGroup_ABC& ), RegisterKnowledgeGroup );
        MOCK_METHOD_EXT( Remove, 1, void( kernel::KnowledgeGroup_ABC& ), RemoveKnowledgeGroup );

    };

    MOCK_BASE_CLASS( MockAgent, kernel::Agent_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetType, 0 );
        MOCK_METHOD( IsCommandPost, 0 );
        // $$$$ _RC_ FDS 2010-01-13: to be remove
        MOCK_METHOD( Accept, 1 );
        MOCK_METHOD( GetPosition, 0 );
    };

    MOCK_BASE_CLASS( MockAutomat, kernel::Automat_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetType, 0 );
        // $$$$ _RC_ FDS 2010-01-13: to be remove
        MOCK_METHOD( Accept, 1 );
        MOCK_METHOD( GetTeam, 0 );
        MOCK_METHOD( GetKnowledgeGroup, 0 );
        MOCK_METHOD_EXT( Register, 1, void ( kernel::Automat_ABC& ), RegisterAutomat );
        MOCK_METHOD_EXT( Remove, 1, void ( kernel::Automat_ABC& ), RemoveAutomat );
        MOCK_METHOD( GetAutomats, 0 );
        MOCK_METHOD_EXT( Register, 1, void ( kernel::Agent_ABC& ), RegisterAgent );
        MOCK_METHOD_EXT( Remove, 1, void ( kernel::Agent_ABC& ), RemoveAgent );
        MOCK_METHOD( GetAgents, 0 );
        MOCK_METHOD( GetParentAutomat, 0 );
        MOCK_METHOD( GetFormation, 0 );
    
    };

    MOCK_BASE_CLASS( MockPopulation, kernel::Population_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetType, 0 );
        MOCK_METHOD( GetLivingHumans, 0 );
        MOCK_METHOD( GetDeadHumans, 0 );
        MOCK_METHOD( Accept, 1 );      // $$$$ _RC_ FDS 2010-01-13: to be remove
    };

    MOCK_BASE_CLASS( MockPopulationKnowledge, kernel::PopulationKnowledge_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );

//        MOCK_METHOD_EXT( GetEntity, 0, const kernel::Entity_ABC*(), GetBaseEntity ); // $$$$ SBO 2009-10-30: refactor interface
        MOCK_METHOD( GetRecognizedEntity, 0 );
//        MOCK_METHOD_EXT( GetOwner, 0, const kernel::Entity_ABC&(), GetBaseOwner ); // $$$$ SBO 2009-10-30: refactor interface
        MOCK_METHOD_EXT( GetEntity, 0, const kernel::Population_ABC*(), GetPopulationEntity );
        MOCK_METHOD_EXT( GetOwner, 0, const kernel::KnowledgeGroup_ABC&(), GetKnowledgeGroup );
        MOCK_METHOD( Accept, 1 );      // $$$$ _RC_ FDS 2010-01-13: to be remove
        MOCK_METHOD( Display, 1 );
        MOCK_METHOD( DisplayInList, 1 );
    };

    MOCK_BASE_CLASS( MockAgentKnowledgeConverter, kernel::AgentKnowledgeConverter_ABC )
    {
        MOCK_METHOD( FindAgent, 2 );
        MOCK_METHOD( FindPopulation, 2 );
        MOCK_METHOD_EXT( Find, 2, const kernel::AgentKnowledge_ABC*( const kernel::AgentKnowledge_ABC&, const kernel::Entity_ABC& ), FindAgentKnowledgeFromKnowledge );
        MOCK_METHOD_EXT( Find, 2, const kernel::AgentKnowledge_ABC*( const kernel::Agent_ABC&, const kernel::Entity_ABC& ), FindAgentKnowledgeFromAgent );
        MOCK_METHOD_EXT( Find, 2, const kernel::PopulationKnowledge_ABC*( const kernel::PopulationKnowledge_ABC&, const kernel::Entity_ABC& ), FindPopulationKnowledgeFromKnowledge );
        MOCK_METHOD_EXT( Find, 2, const kernel::PopulationKnowledge_ABC*( const kernel::Population_ABC&, const kernel::Entity_ABC& ), FindPopulationKnowledgeFromPopulation );
    };

    MOCK_BASE_CLASS( MockObject, kernel::Object_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetType, 0 );
        MOCK_METHOD( Accept, 1 );      // $$$$ _RC_ FDS 2010-01-13: to be remove
        MOCK_METHOD( Display, 1 );
    };

    MOCK_BASE_CLASS( MockObjectKnowledge, kernel::ObjectKnowledge_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );

//        MOCK_METHOD_EXT( GetEntity, 0, const kernel::Entity_ABC*(), GetBaseEntity ); // $$$$ SBO 2009-10-30: refactor interface
        MOCK_METHOD( GetRecognizedEntity, 0 );
//        MOCK_METHOD_EXT( GetOwner, 0, const kernel::Entity_ABC&(), GetBaseOwner ); // $$$$ SBO 2009-10-30: refactor interface
        MOCK_METHOD_EXT( GetEntity, 0, const kernel::Object_ABC*(), GetObjectEntity );
        MOCK_METHOD_EXT( GetOwner, 0, const kernel::Team_ABC&(), GetTeam );
        MOCK_METHOD( Accept, 1 );      // $$$$ _RC_ FDS 2010-01-13: to be remove
        MOCK_METHOD( Display, 1 );
        MOCK_METHOD( DisplayInList, 1 );
    };

    MOCK_BASE_CLASS( MockObjectKnowledgeConverter, kernel::ObjectKnowledgeConverter_ABC )
    {
        MOCK_METHOD_EXT( Find, 2, const kernel::ObjectKnowledge_ABC*( unsigned long, const kernel::Team_ABC& ), FindObjectKnowledgeFromId );
        MOCK_METHOD_EXT( Find, 2, const kernel::ObjectKnowledge_ABC*( unsigned long, const kernel::Entity_ABC& ), FindObjectKnowledgeFromIdWithEntity );
        MOCK_METHOD_EXT( Find, 2, const kernel::ObjectKnowledge_ABC*( const kernel::ObjectKnowledge_ABC&, const kernel::Team_ABC& ), FindObjectKnowledgeFromKnowledge );
        MOCK_METHOD_EXT( Find, 2, const kernel::ObjectKnowledge_ABC*( const kernel::Object_ABC&, const kernel::Team_ABC& ), FindObjectKnowledgeFromObject );
        MOCK_METHOD_EXT( Find, 2, const kernel::ObjectKnowledge_ABC*( const kernel::Object_ABC&, const kernel::Entity_ABC& ), FindObjectKnowledgeFromObjectWithEntity );
    };

    MOCK_BASE_CLASS( MockCommunicationHierarchies, kernel::CommunicationHierarchies )
    {
        MOCK_METHOD( GetTop, 0 );
        MOCK_METHOD( GetSuperior, 0 );
        MOCK_METHOD( GetEntity, 0 );
        MOCK_METHOD( CreateSubordinateIterator, 0 );
        MOCK_METHOD( RegisterSubordinate, 1 );
        MOCK_METHOD( AddSubordinate, 1 );
        MOCK_METHOD( RemoveSubordinate, 1 );
        MOCK_METHOD( UnregisterSubordinate, 1 );
        MOCK_METHOD( UnregisterParent, 0 );
        MOCK_METHOD( CanCommunicate, 0 );
        MOCK_METHOD( RetrieveHierarchies, 1 );
    };
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Bool
// Created: SBO 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Bool )
{
    const std::string input( "<parameter name='test' type='bool' value='true'/>" );
    xml::xistringstream xis( input ); xis >> xml::start( "parameter" );
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "bool", input, bl::new_ptr< actions::parameters::Bool >() ) );
    CheckSet( *message );
    BOOST_CHECK_EQUAL( true, message->value().abool() );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Numeric
// Created: SBO 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Numeric )
{
    const std::string input( "<parameter name='test' type='numeric' value='1.5'/>" );
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "numeric", input, bl::new_ptr< actions::parameters::Numeric >() ) );
    CheckSet( *message );
    BOOST_CHECK_EQUAL( 1.5f, message->value().areal() );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Agent
// Created: SBO 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Agent )
{
    const std::string input( "<parameter name='test' type='agent' value='42'/>" );
    kernel::Controller controller;
    MockAgent agent;
    MOCK_EXPECT( agent, GetId ).returns( 42 );
    MockEntityResolver resolver;
    MOCK_EXPECT( resolver, GetAgent ).with( 42 ).returns( boost::ref( agent ) );
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "agent", input, 
        bl::bind( bl::new_ptr< actions::parameters::Agent >(), bl::_1, bl::_2, bl::var( resolver ), bl::var( controller ) ) ) );
    CheckSet( *message );
    BOOST_CHECK_EQUAL( 42, message->value().unit().oid() );
}

namespace
{
    void CheckCoordinate( const kernel::CoordinateConverter_ABC& converter, const std::string& utm, const Common::MsgCoordLatLong& wgs )
    {
        const geometry::Point2f expected = converter.ConvertToXY( utm );
        const geometry::Point2f actual   = converter.ConvertToXY( wgs );
        BOOST_CHECK_EQUAL( expected.X(), actual.X() );
        BOOST_CHECK_EQUAL( expected.Y(), actual.Y() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Polygon
// Created: SBO 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Polygon )
{
    const std::string input( 
        "<parameter name='Area' type='Polygon'>"
          "<location type='polygon'>"
            "<point coordinates='30TYS1037476379'/>"
            "<point coordinates='30TYS1136976125'/>"
            "<point coordinates='30TYS1143378131'/>"
            "<point coordinates='30TYS1066777949'/>"
            "<point coordinates='30TYS1037476379'/>"
          "</location>"
        "</parameter>" );
    kernel::CoordinateConverter converter;
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "polygon", input,
        bl::bind( bl::new_ptr< actions::parameters::Polygon >(), bl::_1, bl::var( converter ), bl::_2 ) ) );
    CheckSet( *message );
    const Common::MsgLocation& result = message->value().polygon().location();
    BOOST_CHECK_EQUAL( Common::MsgLocation::polygon, result.type() );
    BOOST_REQUIRE_EQUAL( 5, result.coordinates().elem_size() );
    CheckCoordinate( converter, "30TYS1037476379", result.coordinates().elem( 0 ) );
    CheckCoordinate( converter, "30TYS1136976125", result.coordinates().elem( 1 ) );
    CheckCoordinate( converter, "30TYS1143378131", result.coordinates().elem( 2 ) );
    CheckCoordinate( converter, "30TYS1066777949", result.coordinates().elem( 3 ) );
    CheckCoordinate( converter, "30TYS1037476379", result.coordinates().elem( 4 ) );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Path
// Created: SBO 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Path )
{
    const std::string input( 
        "<parameter name='Assault route' type='Path'>"
            "<parameter name='Pass point1' type='pathpoint'>"
                "<location type='point'>"
                    "<point coordinates='30TYS1037476379'/>"
                "</location>"
            "</parameter>"
            "<parameter name='Destination' type='pathpoint'>"
                "<location type='point'>"
                    "<point coordinates='30UXU3889827121'/>"
                "</location>"
            "</parameter>"
        "</parameter>" );
    kernel::CoordinateConverter converter;
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "path", input,
        bl::bind( bl::new_ptr< actions::parameters::Path >(), bl::_1, bl::var( converter ), bl::_2 ) ) );
    CheckSet( *message );
    const Common::MsgLocation& result = message->value().path().location();
    BOOST_CHECK_EQUAL( Common::MsgLocation::line, result.type() );
    BOOST_REQUIRE_EQUAL( 2, result.coordinates().elem_size() );
    CheckCoordinate( converter, "30TYS1037476379", result.coordinates().elem( 0 ) );
    CheckCoordinate( converter, "30UXU3889827121", result.coordinates().elem( 1 ) );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Direction 
// Created: FHD 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Direction )
{
    const std::string input( "<parameter name='test' type='direction' value='21'/>" );
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "direction", input,
        bl::bind( bl::new_ptr< actions::parameters::Direction >(), bl::_1, bl::_2 ) ) );
    BOOST_CHECK_EQUAL( 21, message->value().heading().heading() );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Automat
// Created: SBO 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Automat )
{
    const std::string input( "<parameter name='test' type='automate' value='42'/>" );
    kernel::Controller controller;
    MockAutomat automat;
    MOCK_EXPECT( automat, GetId ).returns( 42 );
    MockEntityResolver resolver;
    MOCK_EXPECT( resolver, GetAutomat ).with( 42 ).returns( boost::ref( automat ) );
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "automate", input, 
        bl::bind( bl::new_ptr< actions::parameters::Automat >(), bl::_1, bl::_2, bl::var( resolver ), bl::var( controller ) ) ) );
    CheckSet( *message );
    BOOST_CHECK_EQUAL( 42, message->value().automat().oid() );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Level
// Created: FHD 2009-10-29
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( ParametersSerialization_Level )
//{
//    // $$$$ FHD 2009-10-30: TODO find a right formatted level order
//    const std::string input( "<parameter name='test' type='level' value='42'/>" );     
//    kernel::FormationLevels levels;
//    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "level", input, 
//        bl::bind( bl::new_ptr< actions::parameters::Level >(), bl::_1, bl::_2, bl::var( levels ) ) ) );
//    CheckSet( *message );
//    //BOOST_CHECK_EQUAL( 42, message->value().oid() );
//}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Formation
// Created: SBO 2009-10-29
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( ParametersSerialization_Formation )
//{
//    const std::string input( "<parameter name='test' type='formation' value='42'/>" );
//    kernel::Controller controller;
//    tools::Resolver< kernel::Formation_ABC > resolver;
//    MockFormation formation;
//    resolver.Register( formation.GetId(), formation );
//    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "formation", input, 
//        bl::bind( bl::new_ptr< actions::parameters::Formation >(), bl::_1, bl::_2, bl::var( resolver ), bl::var( controller ) ) ) );
//    CheckSet( *message );
//    BOOST_CHECK_EQUAL( 42, message->value().unit().oid() );
//}
// -----------------------------------------------------------------------------
// Name: ParametersSerialization_DotationType 
// Created: SBO 2009-10-29
// -----------------------------------------------------------------------------
//BOOST_AUTO_TEST_CASE( ParametersSerialization_DotationType )
//{
//    const std::string input( "<parameter name='test' type='dotationtype' value='42'/>" );
//    xml::xistringstream xis( input ); xis >> xml::start( "parameter" );
//    tools::Resolver< kernel::DotationType > resolver;
//    actions::parameters::DotationType parameter( kernel::OrderParameter("test", "dotationtype", false), xis, resolver );
//    std::auto_ptr< Common::MsgMissionParameter > message( new Common::MsgMissionParameter() );
//    parameter.CommitTo( *message );
//    CheckSet( *message );
//    BOOST_CHECK_EQUAL( 42, message->value().dotationtype().oid() );
//}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Datetime 
// Created: FHD 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Datetime )
{
    const std::string input( "<parameter name='Horaire' type='datetime' value='20081211T190022'/>" );
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "datetime", input,
        bl::bind( bl::new_ptr< actions::parameters::DateTime >(), bl::_1, bl::_2 ) ) );
    BOOST_CHECK_EQUAL( "20081211T190022", message->value().datetime().data() );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_AtlasNature 
// Created: FHD 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_AtlasNature )
{
    const std::string input( "<parameter name='name' type='NatureAtlas' value='2113'/>" );
    kernel::AtlasNatures natures;
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "NatureAtlas", input,
        bl::bind( bl::new_ptr< actions::parameters::AtlasNature >(), bl::_1, bl::_2, bl::var( natures ) ) ) );
    BOOST_CHECK_EQUAL( Common::MsgAtlasNature::blinde,   message->value().atlasnature().nature() & Common::MsgAtlasNature::blinde );
    BOOST_CHECK_EQUAL( Common::MsgAtlasNature::vehicule, message->value().atlasnature().nature() & Common::MsgAtlasNature::vehicule );
    BOOST_CHECK_EQUAL( Common::MsgAtlasNature::none,     message->value().atlasnature().nature() & Common::MsgAtlasNature::none );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Enumeration 
// Created: SBO 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Enumeration )
{
    const std::string input( "<parameter name='name' optional='false' type='Enumeration' value='2'/>");
    xml::xistringstream xis( input ); xis >> xml::start( "parameter" );

    const std::string enumeration( 
        "<parameter name='my enumeration' optional='false' type='enumeration'>"
            "<value name='my value1' id='1'/>"
            "<value name='my value2' id='2'/>"
        "</parameter>" );
    xml::xistringstream xisEnumeration( enumeration ); xisEnumeration >> xml::start( "parameter" );
    kernel::OrderParameter definition( xisEnumeration );
    actions::parameters::Enumeration parameter( definition, xis );
    std::auto_ptr< Common::MsgMissionParameter > message( new Common::MsgMissionParameter() );
    parameter.CommitTo( *message );
    CheckSet( *message );
    BOOST_CHECK_EQUAL( 2, message->value().enumeration() );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Lima
// Created: FHD 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Lima )
{
    const std::string input( "<parameter name='Lima 1' type='lima' value='LDM'>"
        "<parameter name='Position' type='location'>"
          "<location type='line'>"
            "<point coordinates='31TCN7270936833'/>"
            "<point coordinates='31TCN7776344465'/>"
            "<point coordinates='31TCN8594043967'/>"
            "<point coordinates='31TCN9106845579'/>"
            "<point coordinates='31TCN9737753024'/>"
          "</location>"
        "</parameter>"
        "<parameter name='Horaire' type='datetime' value='20081211T190022'/>"
      "</parameter>" );
    xml::xistringstream xis( input ); xis >> xml::start( "parameter" );
    kernel::CoordinateConverter converter;
    actions::parameters::Lima parameter( converter, xis );
    std::auto_ptr< Common::MsgMissionParameter > message( new Common::MsgMissionParameter() );
    parameter.CommitTo( *message->mutable_value()->mutable_limasorder()->add_elem() );
    CheckSet( *message );
    const Common::MsgLimaOrder& lima = message->value().limasorder().elem(0);
    BOOST_CHECK_EQUAL( 2, lima.lima().location().type() );
    BOOST_CHECK_EQUAL( 5, lima.lima().location().coordinates().elem_size() );
    BOOST_CHECK_EQUAL( "20081211T190022", lima.horaire().data() );
    CheckCoordinate( converter, "31TCN7270936833", lima.lima().location().coordinates().elem( 0 ) );
    CheckCoordinate( converter, "31TCN7776344465", lima.lima().location().coordinates().elem( 1 ) );
    CheckCoordinate( converter, "31TCN8594043967", lima.lima().location().coordinates().elem( 2 ) );
    CheckCoordinate( converter, "31TCN9106845579", lima.lima().location().coordinates().elem( 3 ) );
    CheckCoordinate( converter, "31TCN9737753024", lima.lima().location().coordinates().elem( 4 ) );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_Lima
// Created: FHD 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_Limit )
{
    const std::string input( 
        "<parameter name='Boundary limit 1' type='Limit'>"
          "<location type='line'>"
            "<point coordinates='30TXS2657258333'/>"
            "<point coordinates='30TXS3446369735'/>"
            "<point coordinates='30TXS4042776985'/>"
            "<point coordinates='30TXS4297383178'/>"
          "</location>"
        "</parameter>" );
    kernel::CoordinateConverter converter;
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "limit", input,
        bl::bind( bl::new_ptr< actions::parameters::Limit >(), bl::_1, bl::var( converter ), bl::_2 ) ) );
    const Common::MsgLocation& loc = message->value().line().location();
    BOOST_CHECK_EQUAL( 2, loc.type() );
    BOOST_CHECK_EQUAL( 4, loc.coordinates().elem_size() );
    CheckCoordinate( converter, "30TXS2657258333", loc.coordinates().elem( 0 ) );
    CheckCoordinate( converter, "30TXS3446369735", loc.coordinates().elem( 1 ) );
    CheckCoordinate( converter, "30TXS4042776985", loc.coordinates().elem( 2 ) );
    CheckCoordinate( converter, "30TXS4297383178", loc.coordinates().elem( 3 ) );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_AgentList
// Created: FHD 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_AgentList )
{
//    const std::string input( "<parameter name='Unites a couvrir' type='AgentList'>"
//      "<parameter name='Agent 1:Peloton ERIAC OFF' type='agent' value='62'/>"
//      "<parameter name='Agent 2:Peloton ERIAC OFF' type='agent' value='63'/>"
//      "<parameter name='Agent 3:Peloton ERIAC S/OFF' type='agent' value='64'/>"
//      "<parameter name='Agent 4:PC CDT LOG ERIAC' type='agent' value='65'/>"
//      "<parameter name='Agent 5:Peloton ERIAC OFF' type='agent' value='66'/>"
//    "</parameter>" );
//    kernel::Controller controller;
//    tools::Resolver< kernel::Agent_ABC > resolver;
//    
//    
//    MockAgent agent;
//    MOCK_EXPECT( agent, GetId ).returns( 62 );
//    MOCK_EXPECT( agent, GetId ).returns( 63 );
//    MOCK_EXPECT( agent, GetId ).returns( 64 );
//    MOCK_EXPECT( agent, GetId ).returns( 65 );
//    MOCK_EXPECT( agent, GetId ).returns( 66 );
//    resolver.Register( agent.GetId(), agent );
//
//    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "agentlist", input, 
//        bl::bind( bl::new_ptr< actions::parameters::AgentList >(), bl::_1, bl::_2, bl::var( resolver ), bl::var( controller ) ) ) );
//    
//    CheckSet( *message );    
//    BOOST_CHECK_EQUAL( 62, message->value().unitlist().elem(0).oid() );
//    BOOST_CHECK_EQUAL( 63, message->value().unitlist().elem(1).oid() );
//    BOOST_CHECK_EQUAL( 64, message->value().unitlist().elem(2).oid() );
//    BOOST_CHECK_EQUAL( 65, message->value().unitlist().elem(3).oid() );
//    BOOST_CHECK_EQUAL( 66, message->value().unitlist().elem(4).oid() );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_PopulationKnowledge
// @brief Population real id is stored in XML, converter tries to find a matching knowledge 
// Created: FHD 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_PopulationKnowledge )
{
    const std::string input( "<parameter name='test' type='populationknowledge' value='42'/>" );

    MockPopulation population;
    MOCK_EXPECT( population, GetId ).returns( 42 );
    MockEntityResolver resolver;
    MOCK_EXPECT( resolver, GetPopulation ).with( 42 ).returns( boost::ref( population ) );
    MockAgent owner;
    MockPopulationKnowledge knowledge;
    MOCK_EXPECT( knowledge, GetId ).returns( 15 );
    MockAgentKnowledgeConverter converter;
    MOCK_EXPECT( converter, FindPopulationKnowledgeFromPopulation ).with( mock::same( population ), mock::same( owner ) ).returns( &knowledge );

    kernel::Controller controller;
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "populationknowledge", input, 
        bl::bind( bl::new_ptr< actions::parameters::PopulationKnowledge >(), bl::_1, bl::_2, bl::var( resolver ), bl::var( converter ), bl::var( owner ), bl::var( controller ) ) ) );
    BOOST_CHECK_EQUAL( 15, message->value().populationknowledge().oid() );
}

// -----------------------------------------------------------------------------
// Name: ParametersSerialization_ObjectKnowledge
// @brief Object real id is stored in XML, converter tries to find a matching knowledge 
// Created: SBO 2009-10-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ParametersSerialization_ObjectKnowledge )
{
    const std::string input( "<parameter name='test' type='objectknowledge' value='42'/>" );
    MockObject object;
    MOCK_EXPECT( object, GetId ).returns( 42 );
    MockEntityResolver resolver;
    MOCK_EXPECT( resolver, GetObject ).with( 42 ).returns( boost::ref( object ) );
    MockTeam owner;
    MockCommunicationHierarchies* pHierarchies = new MockCommunicationHierarchies;
    MOCK_EXPECT( *pHierarchies, GetSuperior ).returns( 0 );
    MOCK_EXPECT( *pHierarchies, GetEntity ).returns( boost::cref( owner ) );
    owner.Attach< kernel::CommunicationHierarchies >( *pHierarchies );
    MockObjectKnowledge knowledge;
    MOCK_EXPECT( knowledge, GetId ).returns( 15 );
    MockObjectKnowledgeConverter converter;
    MOCK_EXPECT( converter, FindObjectKnowledgeFromObjectWithEntity ).with( mock::same( object ), mock::same( owner ) ).returns( &knowledge );
    
    kernel::Controller controller;
    std::auto_ptr< Common::MsgMissionParameter > message( Serialize( "objectknowledge", input,
        bl::bind( bl::new_ptr< actions::parameters::ObjectKnowledge >(), bl::_1, bl::_2, bl::var( resolver ), bl::var( converter ), bl::var( owner ), bl::var( controller ) ) ) );
    BOOST_CHECK_EQUAL( 15, message->value().objectknowledge().oid() );
}
