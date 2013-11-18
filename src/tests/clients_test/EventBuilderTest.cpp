// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/FragOrder.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/EventOrderView_ABC.h"
#include "clients_gui/EventOrderPresenter.h"
#include "actions_gui/MissionInterface_ABC.h"
#include "actions_gui/InterfaceBuilder_ABC.h"
#include "gaming/Decisions_ABC.h"
#include <boost/assign.hpp>
#include "ENT/ENT_Tr.h"

namespace gui
{
    template< typename T >
    std::ostream& operator<<( std::ostream& os, const std::vector< T >& vector )
    {
        os << "[";
        for( auto it = vector.begin(); it != vector.end(); ++it )
            if( it == vector.begin() )
                os << *it;
            else
                os << ", " << *it;
        os << "]";
        return os;
    }

    std::ostream& operator<<( std::ostream& os, const gui::EventOrderViewState& state )
    {
        os << state.types_
           << state.currentType_
           << state.missions_
           << state.currentMission_
           << state.disabledMissions_
           << state.invalid_
           << state.missionSelector_;
        return os;
    }
}

namespace
{
    const xml::xistringstream xis( "<missions>"
                                   "  <units>"
                                   "    <mission dia-behavior='missionUnitZ' dia-type='missionUnitZ' id='1' name='missionUnitZ'/>"
                                   "    <mission dia-behavior='missionUnitA' dia-type='missionUnitA' id='2' name='missionUnitA'/>"
                                   "    <mission dia-behavior='missionUnitA' dia-type='missionUnitA' id='3' name='missionType1'/>"
                                   "  </units>"
                                   "  <automats>"
                                   "    <mission dia-behavior='missionAutomat' dia-type='missionAutomat' id='4' name='missionAutomat'/>"
                                   "  </automats>"
                                   "  <populations>"
                                   "    <mission dia-behavior='missionPopulation' dia-type='missionPopulation' id='5' name='missionPopulation'/>"
                                   "  </populations>"
                                   "  <fragorders>"
                                   "    <fragorder available-without-mission='true' dia-type='fragorder' id='6' name='fragorder'/>"
                                   "    <fragorder available-without-mission='false' dia-type='fragorder2' id='7' name='fragorder2'/>"
                                   "  </fragorders>"
                                   "</missions>" );

    const std::vector< E_MissionType > types = boost::assign::list_of( eMissionType_Pawn )
                                                                     ( eMissionType_Automat )
                                                                     ( eMissionType_Population )
                                                                     ( eMissionType_FragOrder );
    const std::vector< E_MissionType > pawnTypes =  boost::assign::list_of( eMissionType_Pawn )
                                                                           ( eMissionType_FragOrder );
    const std::vector< E_MissionType > automatTypes =  boost::assign::list_of( eMissionType_Automat )
                                                                             ( eMissionType_FragOrder );
    const std::vector< E_MissionType > populationTypes =  boost::assign::list_of( eMissionType_Population )
                                                                                ( eMissionType_FragOrder );

    const std::string missionSelector = "<Select mission>";
    const std::string fragOrderSelector = "<Select frag order>";

    bool operator==( const gui::EventOrderViewState& lhs, const gui::EventOrderViewState& rhs )
    {
        return lhs.types_ == rhs.types_ &&
               lhs.currentType_ == rhs.currentType_ &&
               lhs.missions_ == rhs.missions_ &&
               lhs.currentMission_ == rhs.currentMission_ &&
               lhs.disabledMissions_ == rhs.disabledMissions_ &&
               lhs.invalid_ == rhs.invalid_ &&
               lhs.missionSelector_ == rhs.missionSelector_;
    }

    MOCK_BASE_CLASS( MockMissionInterface, actions::gui:: MissionInterface_ABC )
    {
        MOCK_METHOD( Build, 3 );
        MOCK_METHOD( Purge, 0 );
        MOCK_METHOD( SetEntity, 1 );
        MOCK_METHOD( SetPlanned, 1 );
        MOCK_METHOD( PublishFragOrder, 3 );
        MOCK_METHOD( PublishMissionOrder, 3 );
        MOCK_METHOD( FillFrom, 1 );
    };

    MOCK_BASE_CLASS( MockInterfaceBuilder, actions::gui::InterfaceBuilder_ABC )
    {
        MOCK_METHOD( BuildAll, 2 );
        MOCK_METHOD( BuildOne, 2 );
        MOCK_METHOD( GetParentObject, 0 );
        MOCK_METHOD( GetParamInterface, 0 );
        MOCK_METHOD( GetParameterLayer, 0 );
        MOCK_METHOD( GetControllers, 0 );
        MOCK_METHOD( GetAgentKnowledgeConverter, 0 );
        MOCK_METHOD( GetObjectKnowledgeConverter, 0 );
        MOCK_METHOD( GetTacticalLineResolver, 0 );
        MOCK_METHOD( GetCurrentDateTime, 0 );
        MOCK_METHOD( GetStaticModel, 0 );
        MOCK_METHOD( SetParentObject, 1 );
        MOCK_METHOD( SetParamInterface, 1 );
    };

    MOCK_BASE_CLASS( MockEventBuilder, gui::EventOrderView_ABC )
    {
        MOCK_METHOD( Build, 1 );
    };

    MOCK_BASE_CLASS( MockEntity, kernel::Entity_ABC )
    {
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( Select, 1 );
        MOCK_METHOD( MultipleSelect, 2 );
        MOCK_METHOD( ContextMenu, 2 );
        MOCK_METHOD( Activate, 1 );
        MOCK_METHOD( GetTypeName, 0 );
    };

    MOCK_BASE_CLASS( MockDecisions, Decisions_ABC )
    {
        MOCK_METHOD( CanBeOrdered, 0 );
        MOCK_METHOD( GetMissions, 0 );
        MOCK_METHOD( GetFragOrders, 0 );
        MOCK_METHOD( GetCurrentMission, 0 );
        MOCK_METHOD( GetAgent, 0 );
    };

    class Fixture
    {
    public:
        Fixture()
            : manager( builder, agentTypes, interfaceBuilder, missionInterface )
            , xisMissionType1( "<mission dia-behavior='missionType1' dia-type='missionType1' id='1' name='missionType1'/>" )
            , xisMissionType2( "<mission dia-behavior='missionType2' dia-type='missionType2' id='2' name='missionType2'/>" )
            , type1( xisMissionType1 >> xml::start( "mission" ), eMissionType_Pawn )
            , type2( xisMissionType2 >> xml::start( "mission" ), eMissionType_Pawn )
            , mission1( type1 )
            , mission2( type2 )
            , xisFragOrderType1( "<fragorder available-without-mission='true' dia-type='fragorder' id='5' name='fragorder'/>" )
            , xisFragOrderType2( "<fragorder available-without-mission='false' dia-type='fragorder2' id='6' name='fragorder2'/>" )
            , xisFragOrderType3( "<fragorder available-without-mission='false' dia-type='fragorder2' id='7' name='fragorder2'/>" )
            , type3( xisFragOrderType1 >> xml::start( "fragorder" ) )
            , type4( xisFragOrderType2 >> xml::start( "fragorder" ) )
            , type5( xisFragOrderType3 >> xml::start( "fragorder" ) )
            , fragOrder( type3 )
        {
            mission1.Register( 7u, *new kernel::FragOrder( type5 ) );
            mission2.Register( 6u, *new kernel::FragOrder( type4 ) );
            agentTypes.ReadOrderTypes( xml::xisubstream( xis ) );
            missionResolver.Register( 1u, mission1 );
            missionResolver.Register( 2u, mission2 );
            fragOrderResolver.Register( 1u, fragOrder );
        }
    kernel::AgentTypes agentTypes;
    MockEventBuilder builder;
    MockInterfaceBuilder interfaceBuilder;
    MockMissionInterface missionInterface;
    gui::EventOrderPresenter manager;
    xml::xistringstream xisMissionType1;
    xml::xistringstream xisMissionType2;
    xml::xistringstream xisFragOrderType1;
    xml::xistringstream xisFragOrderType2;
    xml::xistringstream xisFragOrderType3;
    kernel::MissionType type1;
    kernel::MissionType type2;
    kernel::Mission mission1;
    kernel::Mission mission2;
    kernel::FragOrderType type3;
    kernel::FragOrderType type4;
    kernel::FragOrderType type5;
    kernel::FragOrder fragOrder;
    MockEntity entity;
    MockDecisions decisions;
    tools::Resolver< kernel::Mission > missionResolver;
    tools::Resolver< kernel::FragOrder > fragOrderResolver;

    void CheckPurge( MockEntity* target = 0 )
    {
        MOCK_EXPECT( missionInterface.Purge ).once();
        MOCK_EXPECT( missionInterface.SetEntity ).once().with( target );
    }

    void CheckPurgeAndBuild( MockEntity* target = 0 )
    {
        CheckPurge( target );
        MOCK_EXPECT( missionInterface.Build ).once();
    }

    void CheckMission( const std::string& type, const std::vector< E_MissionType >& missionTypes,
                       E_MissionType current, const std::string& currentMission, bool invalid = false )
    {
        std::vector< std::string > missions = boost::assign::list_of( "missionType1" )( "missionType2" );
        bool selector = currentMission == missionSelector;
        if( selector )
            missions.insert( missions.begin(), currentMission );
        MOCK_EXPECT( entity.GetTypeName ).once().returns( boost::cref( type ) );
        MOCK_EXPECT( decisions.GetAgent ).once().returns( boost::cref( entity ) );
        MOCK_EXPECT( decisions.GetMissions ).once().returns( missionResolver.CreateIterator() );
        MOCK_EXPECT( decisions.GetMissions ).once().returns( missionResolver.CreateIterator() );
        MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState( missionTypes, current,
            missions, currentMission, std::vector< std::string>(), invalid, selector ) );
    }

    void CheckFragOrder( const std::vector< std::string >& disabledMissions = std::vector< std::string >() )
    {
        MOCK_EXPECT( entity.GetTypeName ).once().returns( boost::cref( kernel::Agent_ABC::typeName_ ) );
        MOCK_EXPECT( decisions.GetAgent ).once().returns( boost::cref( entity ) );
        MOCK_EXPECT( decisions.GetFragOrders ).once().returns( fragOrderResolver.CreateIterator() );
        MOCK_EXPECT( decisions.GetFragOrders ).once().returns( fragOrderResolver.CreateIterator() );
        MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState( pawnTypes, eMissionType_FragOrder,
            boost::assign::list_of( fragOrderSelector )( "fragorder" )( "fragorder2" ),
            fragOrderSelector, disabledMissions, false, true ) );
    }
    };
}

BOOST_FIXTURE_TEST_CASE( no_target_is_selected, Fixture )
{
    // Select agent
    CheckPurge();
    MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState( types,
        eMissionType_Pawn, boost::assign::list_of( missionSelector )( "missionType1" )( "missionUnitA" )("missionUnitZ" ), missionSelector,
        std::vector< std::string >(), false, true ) );
    manager.Select();

    // Select automat
    CheckPurge();
    MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState( types,
        eMissionType_Automat, boost::assign::list_of( missionSelector )( "missionAutomat" ), missionSelector,
        std::vector< std::string >(), false, true ) );
    manager.Select( eMissionType_Automat );

    // Select automat
    CheckPurge();
    MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState( types,
        eMissionType_Population, boost::assign::list_of( missionSelector )( "missionPopulation" ), missionSelector,
        std::vector< std::string >(), false, true ) );
    manager.Select( eMissionType_Population );

   // Select frag order
    CheckPurge();
    MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState( types,
        eMissionType_FragOrder, boost::assign::list_of( fragOrderSelector )( "fragorder" )( "fragorder2" ), fragOrderSelector,
        std::vector< std::string >(), false, true ) );
    manager.Select( eMissionType_FragOrder );
}

BOOST_FIXTURE_TEST_CASE( select_agent, Fixture )
{
    CheckPurge( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, missionSelector );
    manager.Select( decisions );
}

BOOST_FIXTURE_TEST_CASE( select_population, Fixture )
{
    CheckPurge( &entity );
    CheckMission( kernel::Population_ABC::typeName_, populationTypes, eMissionType_Population, missionSelector );
    manager.Select( decisions );
}

BOOST_FIXTURE_TEST_CASE( select_automat, Fixture )
{
    CheckPurge( &entity );
    CheckMission( kernel::Automat_ABC::typeName_, automatTypes, eMissionType_Automat, missionSelector );
    manager.Select( decisions );
}

BOOST_FIXTURE_TEST_CASE( select_target_then_change_mission_type, Fixture )
{
    // Select missions
    CheckPurge( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, missionSelector );
    manager.Select( decisions, eMissionType_Pawn );

    // Select frag orders
    CheckPurge( &entity );
    MOCK_EXPECT( decisions.GetMissions ).exactly( 2 ).returns( missionResolver.CreateIterator() );
    MOCK_EXPECT( decisions.GetCurrentMission ).once().returns( 0 );
    CheckFragOrder( boost::assign::list_of( "fragorder2" ) );
    manager.Select( decisions, eMissionType_FragOrder );

    // Select missions
    CheckPurge( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, missionSelector );
    manager.Select( decisions, eMissionType_Pawn );
}

BOOST_FIXTURE_TEST_CASE( select_target_then_clear_it, Fixture )
{
    // Select missions
    CheckPurge( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, missionSelector );
    manager.Select( decisions, eMissionType_Pawn );

    // Remove target
    CheckPurge();
    MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState( types, eMissionType_Pawn,
        boost::assign::list_of( missionSelector )( "missionType1" )( "missionUnitA" )("missionUnitZ"),
        missionSelector, std::vector< std::string >(), false, true ) );
    manager.Select();
}

BOOST_FIXTURE_TEST_CASE( select_target_and_mission_then_clear_target, Fixture )
{
    // Select entity
    CheckPurge( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, missionSelector );
    manager.Select( decisions, eMissionType_Pawn );

    // Select "missionType1" mission
    CheckPurgeAndBuild( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, "missionType1" );
    manager.Select( decisions, eMissionType_Pawn, "missionType1" );

    // Remove target and keeps the current mission and its parameters
    MOCK_EXPECT( missionInterface.SetEntity ).once();
    MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState(types, eMissionType_Pawn,
        boost::assign::list_of( "missionType1" )( "missionUnitA" )("missionUnitZ"),
        "missionType1", std::vector< std::string >(), false, false ) );
    manager.Select( eMissionType_Pawn, "missionType1" );
}

BOOST_FIXTURE_TEST_CASE( select_mission_then_select_agent_with_and_without_this_mission, Fixture )
{
    // Select "missionType1" mission
    CheckPurgeAndBuild( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, "missionType1" );
    manager.Select( decisions, eMissionType_Pawn, "missionType1" );

    // Select "invalid" mission, first mission of the agent is selected
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    MOCK_EXPECT( entity.GetTypeName ).once().returns( boost::cref( kernel::Agent_ABC::typeName_ ) );
    MOCK_EXPECT( decisions.GetAgent ).once().returns( boost::cref( entity ) );
    MOCK_EXPECT( decisions.GetMissions ).once().returns( missionResolver.CreateIterator() );
    MOCK_EXPECT( decisions.GetMissions ).once().returns( missionResolver.CreateIterator() );
    MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState(pawnTypes, eMissionType_Pawn,
        boost::assign::list_of( "invalid" )( "missionType1" )( "missionType2" ),
        "invalid", std::vector< std::string >(), true, false ) );
    manager.Select( decisions, eMissionType_Pawn, "invalid" );
}

BOOST_FIXTURE_TEST_CASE( change_entity_type_clears_selection, Fixture )
{
    // Select agent mission
    CheckPurgeAndBuild( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, "missionType2" );
    manager.Select( decisions, eMissionType_Pawn, "missionType2" );

    // Select Population mission
    CheckPurgeAndBuild( &entity );
    CheckMission( kernel::Population_ABC::typeName_, populationTypes, eMissionType_Population, "missionType2" );
    manager.Select( decisions, eMissionType_Population, "missionType2" );

    // Select automat mission
    CheckPurgeAndBuild( &entity );
    CheckMission( kernel::Automat_ABC::typeName_, automatTypes, eMissionType_Automat, "missionType2" );
    manager.Select( decisions, eMissionType_Automat, "missionType2" );
}

BOOST_FIXTURE_TEST_CASE( select_automat_mission_with_an_agent, Fixture )
{
    // Select automat mission with an agent
    CheckPurgeAndBuild( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, "missionType1" );
    manager.Select( decisions, eMissionType_Automat, "missionType1" );
}

BOOST_FIXTURE_TEST_CASE( switching_to_the_same_entity_type_keeps_the_parameters, Fixture )
{
    // Select agent mission
    CheckPurgeAndBuild( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, "missionType1" );
    manager.Select( decisions, eMissionType_Pawn, "missionType1" );

    // Select an other agent with the same mission
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, "missionType1" );
    manager.Select( decisions, eMissionType_Pawn, "missionType1" );

    // Select an other mission
    CheckPurgeAndBuild( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, "missionType2" );
    manager.Select( decisions, eMissionType_Pawn, "missionType2" );

    // Select an other agent with the same mission
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, "missionType2" );
    manager.Select( decisions, eMissionType_Pawn, "missionType2" );

    // Select automat mission
    CheckPurgeAndBuild( &entity );
    CheckMission( kernel::Automat_ABC::typeName_, automatTypes, eMissionType_Automat, "missionType2" );
    manager.Select( decisions, eMissionType_Automat, "missionType2" );
}

BOOST_FIXTURE_TEST_CASE( the_parameters_are_filled_without_a_target_then_we_select_a_target, Fixture )
{
    // Select agent mission
    CheckPurgeAndBuild();
    MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState(types,
        eMissionType_Pawn, boost::assign::list_of( "missionType1" )( "missionUnitA" )("missionUnitZ" ),
        "missionType1", std::vector< std::string >(), false, false ) );
    manager.Select( eMissionType_Pawn, "missionType1" );

    // Keep parameters
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    CheckMission( kernel::Agent_ABC::typeName_, pawnTypes, eMissionType_Pawn, "missionType1" );
    manager.Select( decisions, eMissionType_Pawn, "missionType1" );
}

BOOST_FIXTURE_TEST_CASE( display_all_frag_orders_if_no_target_is_selected, Fixture )
{
    // Set normal mode
    manager.SetPlanningMode( false );

    // No target is selected
    CheckPurge();
    MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState(types,
        eMissionType_FragOrder, boost::assign::list_of( fragOrderSelector )( "fragorder" )( "fragorder2" ),
        fragOrderSelector, std::vector< std::string >(), false, true ) );
    manager.Select( eMissionType_FragOrder );

    // Set planning mode
    manager.SetPlanningMode( true );

    //// No target is selected
    CheckPurge();
    MOCK_EXPECT( builder.Build ).once().with( gui::EventOrderViewState(types,
        eMissionType_FragOrder, boost::assign::list_of( fragOrderSelector )( "fragorder" )( "fragorder2" ),
        fragOrderSelector, std::vector< std::string >(), false, true ) );
    manager.Select( eMissionType_FragOrder );
}

BOOST_FIXTURE_TEST_CASE( in_planning_mode_display_all_frag_orders, Fixture )
{
    // Set normal mode
    manager.SetPlanningMode( false );

    // display only general frag order
    CheckPurge( &entity );
    MOCK_EXPECT( decisions.GetMissions ).exactly( 2 ).returns( missionResolver.CreateIterator() );
    MOCK_EXPECT( decisions.GetCurrentMission ).once().returns( 0 );
    CheckFragOrder( boost::assign::list_of( "fragorder2" ) );
    manager.Select( decisions, eMissionType_FragOrder );

    // Set planning mode
    manager.SetPlanningMode( true );

    // display all frag oders
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    MOCK_EXPECT( decisions.GetMissions ).exactly( 2 ).returns( missionResolver.CreateIterator() );
    MOCK_EXPECT( decisions.GetCurrentMission ).once().returns( 0 );
    CheckFragOrder();
    manager.Select( decisions, eMissionType_FragOrder );
}

BOOST_FIXTURE_TEST_CASE( display_frag_order_when_a_mission_starts, Fixture )
{
    // display only general frag order
    CheckPurge( &entity );
    MOCK_EXPECT( decisions.GetMissions ).exactly( 2 ).returns( missionResolver.CreateIterator() );
    MOCK_EXPECT( decisions.GetCurrentMission ).once().returns( 0 );
    CheckFragOrder( boost::assign::list_of( "fragorder2" ) );
    manager.Select( decisions, eMissionType_FragOrder );

    // display fragorder2 because mission2 is activated
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    MOCK_EXPECT( decisions.GetMissions ).exactly( 2 ).returns( missionResolver.CreateIterator() );
    MOCK_EXPECT( decisions.GetCurrentMission ).once().returns( &mission2 );
    CheckFragOrder( std::vector< std::string >() );
    manager.Select( decisions, eMissionType_FragOrder );
}
