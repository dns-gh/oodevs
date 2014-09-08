// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_test_pch.h"

#include "MockEventTools.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_gui/Decisions_ABC.h"
#include "clients_kernel/FragOrder.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_gui/Event.h"
#include "clients_gui/EventOrderPresenter.h"
#include "clients_gui/EventOrderViewState.h"
#include "clients_gui/EventView_ABC.h"
#include "actions_gui/InterfaceBuilder_ABC.h"
#include "actions_gui/MissionInterface_ABC.h"
#include "clients_gui/TimelinePublisher.h"

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
        os << std::endl << "EventOrderViewState " << "{ "
           << state.target_ << ", "
           << state.types_ << ", "
           << state.currentType_ << ", "
           << state.missions_ << ", "
           << state.currentMission_ << ", "
           << state.disabledMissions_ << ", "
           << state.invalid_ << ", "
           << state.missionSelector_ << " }" << std::endl;
        return os;
    }
}

namespace
{
    bool operator==( const gui::EventOrderViewState& lhs, const gui::EventOrderViewState& rhs )
    {
        return lhs.target_ == rhs.target_ &&
               lhs.types_ == rhs.types_ &&
               lhs.currentType_ == rhs.currentType_ &&
               lhs.missions_ == rhs.missions_ &&
               lhs.currentMission_ == rhs.currentMission_ &&
               lhs.disabledMissions_ == rhs.disabledMissions_ &&
               lhs.invalid_ == rhs.invalid_ &&
               lhs.missionSelector_ == rhs.missionSelector_;
    }

    MOCK_BASE_CLASS( MockMissionInterface, actions::gui:: MissionInterface_ABC )
    {
        MOCK_METHOD( IsFilling, 0 );
        MOCK_METHOD( HasParameter, 1 );
        MOCK_METHOD( Draw, 2 );
        MOCK_METHOD( CheckValidity, 0 );
        MOCK_METHOD( Build, 3 );
        MOCK_METHOD( Rebuild, 1 );
        MOCK_METHOD( Purge, 0 );
        MOCK_METHOD( SetEntity, 1 );
        MOCK_METHOD( FillFrom, 1 );
        MOCK_METHOD( CommitTo, 1 );
        MOCK_METHOD( FixOrigin, 1 );
    };
    MOCK_BASE_CLASS( MockInterfaceBuilder, actions::gui::InterfaceBuilder_ABC )
    {
        MOCK_METHOD( BuildAll, 2 );
        MOCK_METHOD( BuildOne, 2 );
        MOCK_METHOD( GetParentObject, 0 );
        MOCK_METHOD( GetParamInterface, 0 );
        MOCK_METHOD( GetParameterLayer, 0 );
        MOCK_METHOD( GetControllers, 0 );
        MOCK_METHOD( GetObjectKnowledgeConverter, 0 );
        MOCK_METHOD( GetTacticalLineResolver, 0 );
        MOCK_METHOD( GetPathfindResolver, 0 );
        MOCK_METHOD( GetCurrentDateTime, 0 );
        MOCK_METHOD( GetStaticModel, 0 );
        MOCK_METHOD( GetConfig, 0 );
        MOCK_METHOD( SetParentObject, 1 );
        MOCK_METHOD( SetParamInterface, 1 );
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
    MOCK_BASE_CLASS( MockDecisions, gui::Decisions_ABC )
    {
        MOCK_METHOD( CanBeOrdered, 0 );
        MOCK_METHOD( IsDebugActivated, 0 );
        MOCK_METHOD( GetMissions, 0 );
        MOCK_METHOD( GetFragOrders, 0 );
        MOCK_METHOD( GetCurrentMission, 0 );
        MOCK_METHOD( GetAgent, 0 );
        MOCK_METHOD( GetModelName, 0 );
    };

    const xml::xistringstream xis( "<missions>"
                                   "  <units>"
                                   "    <mission dia-behavior='missionUnitZ' dia-type='missionUnitZ' id='1' name='missionUnitZ'/>"
                                   "    <mission dia-behavior='missionUnitA' dia-type='missionUnitA' id='2' name='missionUnitA'/>"
                                   "    <mission dia-behavior='missionUnit1' dia-type='missionUnit1' id='3' name='missionUnit1'/>"
                                   "    <mission dia-behavior='missionUnit2' dia-type='missionType2' id='4' name='missionUnit2'/>"
                                   "  </units>"
                                   "  <automats>"
                                   "    <mission dia-behavior='missionAutomat' dia-type='missionAutomat' id='5' name='missionAutomat'/>"
                                   "  </automats>"
                                   "  <populations>"
                                   "    <mission dia-behavior='missionPopulation' dia-type='missionPopulation' id='6' name='missionPopulation'/>"
                                   "  </populations>"
                                   "  <fragorders>"
                                   "    <fragorder available-without-mission='true' dia-type='fragorder1' id='7' name='fragorder1'/>"
                                   "    <fragorder available-without-mission='false' dia-type='fragorder2' id='8' name='fragorder2'/>"
                                   "    <fragorder available-without-mission='false' dia-type='fragorder3' id='9' name='fragorder3'/>"
                                   "  </fragorders>"
                                   "</missions>" );
    const std::vector< E_MissionType > types = boost::assign::list_of( eMissionType_Pawn )
                                                                     ( eMissionType_Automat )
                                                                     ( eMissionType_Population )
                                                                     ( eMissionType_FragOrder );
    const std::vector< E_MissionType > unitTypes =  boost::assign::list_of( eMissionType_Pawn )
                                                                          ( eMissionType_FragOrder );
    const std::vector< E_MissionType > automatTypes =  boost::assign::list_of( eMissionType_Automat )
                                                                             ( eMissionType_FragOrder );
    const std::vector< E_MissionType > crowdTypes =  boost::assign::list_of( eMissionType_Population )
                                                                           ( eMissionType_FragOrder );
    const std::string missionSelector = "<Select mission>";
    const std::string fragOrderSelector = "<Select frag order>";

    template< typename Type, typename Element >
    void FillResolver( const tools::Resolver< Type >& types, tools::Resolver< Element >& elements )
    {
        auto it = types.CreateIterator();
        while( it.HasMoreElements() )
        {
            const Type& type = it.NextElement();
            elements.Register( type.GetId(), *new Element( type ) );
        }
    }

    struct OrderPresenterFixture : public PresenterBaseFixture
    {
        OrderPresenterFixture()
            : orderPresenter( orderView, agentTypes, interfaceBuilder, missionInterface, actionsModel, actionFactory, timelinePublisher, controllers )
            , decisions( new MockDecisions() )
        {
            orderEvent.reset( eventFactory.Create( eEventTypes_Order ) ) ;
            agentTypes.ReadOrderTypes( xml::xisubstream( xis ) );
            FillResolver< kernel::MissionType >( agentTypes, allMissionResolver );
            FillResolver< kernel::FragOrderType >( agentTypes, allFragOrderResolver );
            allMissionResolver.Get( 3 ).Register( 8u, *new kernel::FragOrder( allFragOrderResolver.Get( 8 ).GetType() ) );
            allMissionResolver.Get( 4 ).Register( 9u, *new kernel::FragOrder( allFragOrderResolver.Get( 9 ).GetType() ) );
            unitMissionDecisionalModel.Register( 1u, allMissionResolver.Get( 3 ) );
            unitMissionDecisionalModel.Register( 2u, allMissionResolver.Get( 4 ) );
            unitFragOrderDecisionalModel.Register( 1u, allFragOrderResolver.Get( 7 ) );
            automatMissionDecisionalModel.Register( 1u, allMissionResolver.Get( 5 ) );
            crowdMissionDecisionalModel.Register( 1u, allMissionResolver.Get( 6 ) );
            entity.Attach< gui::Decisions_ABC >( *decisions );
        }
        ~OrderPresenterFixture()
        {
            unitMissionDecisionalModel.Clear();
            unitFragOrderDecisionalModel.Clear();
            automatMissionDecisionalModel.Clear();
            crowdMissionDecisionalModel.Clear();
            allMissionResolver.DeleteAll();
            allFragOrderResolver.DeleteAll();
        }

        kernel::AgentTypes agentTypes;
        gui::TimelinePublisher timelinePublisher;
        MockInterfaceBuilder interfaceBuilder;
        MockMissionInterface missionInterface;

        MockEventView< gui::EventOrderViewState > orderView;
        gui::EventOrderPresenter orderPresenter;
        gui::EventOrderViewState state;
        boost::shared_ptr< gui::Event > orderEvent;

        MockEntity entity;
        MockDecisions* decisions;

        tools::Resolver< kernel::Mission >   allMissionResolver;
        tools::Resolver< kernel::FragOrder > allFragOrderResolver;
        tools::Resolver< kernel::Mission >   unitMissionDecisionalModel;
        tools::Resolver< kernel::FragOrder > unitFragOrderDecisionalModel;
        tools::Resolver< kernel::Mission >   automatMissionDecisionalModel;
        tools::Resolver< kernel::Mission >   crowdMissionDecisionalModel;

        void ResetState( E_MissionType type = eMissionType_Pawn )
        {
            switch( type )
            {
            case eMissionType_Pawn:
                state = gui::EventOrderViewState( 0, types, eMissionType_Pawn,
                    boost::assign::list_of( missionSelector )( "missionUnit1" )( "missionUnit2" )( "missionUnitA" )("missionUnitZ" ),
                    missionSelector, std::vector< std::string >(), false, true );
                break;
            case eMissionType_Automat:
                state = gui::EventOrderViewState( 0, types, eMissionType_Automat,
                    boost::assign::list_of( missionSelector )( "missionAutomat" ),
                    missionSelector, std::vector< std::string >(), false, true );
                break;
            case eMissionType_Population:
                state = gui::EventOrderViewState( 0, types, eMissionType_Population,
                    boost::assign::list_of( missionSelector )( "missionPopulation" ),
                    missionSelector, std::vector< std::string >(), false, true );
                break;
            case eMissionType_FragOrder:
                state = gui::EventOrderViewState( 0, types, eMissionType_FragOrder,
                    boost::assign::list_of( fragOrderSelector )( "fragorder1" )( "fragorder2" )( "fragorder3" ),
                    fragOrderSelector, std::vector< std::string >(), false, true );
                break;
            default:
                assert( false );
                break;
            }
        }
        void CheckMissionInterfacePurge( MockEntity* target = 0 )
        {
            MOCK_EXPECT( missionInterface.Purge ).once();
            MOCK_EXPECT( missionInterface.SetEntity ).once().with( target );
        }
        void CheckMissionInterfacePurgeAndBuild( MockEntity* target = 0 )
        {
            CheckMissionInterfacePurge( target );
            MOCK_EXPECT( missionInterface.Build ).once();
        }
        void CheckBuild()
        {
            MOCK_EXPECT( orderView.BlockSignals ).once().with( true );
            MOCK_EXPECT( orderView.Build ).once().with( state );
            MOCK_EXPECT( orderView.BlockSignals ).once().with( false );
        }
        void CheckUnitBuild( const std::string& mission, bool invalid = false )
        {
            InternalCheckBuild( mission, invalid, unitMissionDecisionalModel, kernel::Agent_ABC::typeName_, unitTypes, eMissionType_Pawn );
        }
        void CheckAutomatBuild( const std::string& mission, bool invalid = false )
        {
            InternalCheckBuild( mission, invalid, automatMissionDecisionalModel, kernel::Automat_ABC::typeName_, automatTypes, eMissionType_Automat );
        }
        void CheckCrowdBuild( const std::string& mission, bool invalid = false )
        {
            InternalCheckBuild( mission, invalid, crowdMissionDecisionalModel, kernel::Population_ABC::typeName_, crowdTypes, eMissionType_Population );
        }
        void CheckFragOrderBuild( const kernel::Mission* currentMission = 0, const std::vector< std::string >& disabledMissions = boost::assign::list_of( "fragorder2" )( "fragorder3" ) )
        {
            MOCK_EXPECT( entity.GetId ).once().returns( 42 );
            MOCK_EXPECT( entity.GetTypeName ).once().returns( boost::cref( kernel::Agent_ABC::typeName_ ) );
            MOCK_EXPECT( decisions->GetMissions ).once().returns( unitMissionDecisionalModel.CreateIterator() );
            MOCK_EXPECT( decisions->GetMissions ).once().returns( unitMissionDecisionalModel.CreateIterator() );
            MOCK_EXPECT( decisions->GetFragOrders ).once().returns( unitFragOrderDecisionalModel.CreateIterator() );
            MOCK_EXPECT( decisions->GetFragOrders ).once().returns( unitFragOrderDecisionalModel.CreateIterator() );
            MOCK_EXPECT( decisions->GetCurrentMission ).once().returns( currentMission );
            state = gui::EventOrderViewState( 42, unitTypes, eMissionType_FragOrder,
                boost::assign::list_of( fragOrderSelector )( "fragorder1" )( "fragorder2" )( "fragorder3" ),
                fragOrderSelector, disabledMissions, false, true );
            CheckBuild();
        }

    private:
        void InternalCheckBuild( const std::string& mission,
                                 bool invalid,
                                 const tools::Resolver< kernel::Mission >& missionModel,
                                 const std::string& typeName,
                                 const std::vector< E_MissionType >& types,
                                 E_MissionType type )
        {
            std::vector< std::string > missions;
            for( auto it = missionModel.CreateIterator(); it.HasMoreElements(); )
                missions.push_back( it.NextElement().GetName() );

            bool selector = mission == missionSelector;
            if( selector )
                missions.insert( missions.begin(), missionSelector );
            MOCK_EXPECT( entity.GetId ).once().returns( 42 );
            MOCK_EXPECT( entity.GetTypeName ).once().returns( boost::cref( typeName ) );
            MOCK_EXPECT( decisions->GetMissions ).once().returns( missionModel.CreateIterator() );
            MOCK_EXPECT( decisions->GetMissions ).once().returns( missionModel.CreateIterator() );
            state = gui::EventOrderViewState( 42, types, type, missions, mission,
                std::vector< std::string>(), invalid, selector );
            CheckBuild();
        }
    };

} //! anonymous namespace

BOOST_FIXTURE_TEST_CASE( order_presenter_change_target, OrderPresenterFixture )
{
    // set agent as target
    CheckMissionInterfacePurge( &entity );
    CheckUnitBuild( missionSelector );
    orderPresenter.OnTargetChanged( &entity );

    // set automat as target
    CheckMissionInterfacePurge( &entity );
    CheckAutomatBuild( missionSelector );
    orderPresenter.OnTargetChanged( &entity );

    // set crowd as target
    CheckMissionInterfacePurge( &entity );
    CheckCrowdBuild( missionSelector );
    orderPresenter.OnTargetChanged( &entity );
}

BOOST_FIXTURE_TEST_CASE( order_presenter_set_target_then_clear_it, OrderPresenterFixture )
{
    // set agent as target
    CheckMissionInterfacePurge( &entity );
    CheckUnitBuild( missionSelector );
    orderPresenter.OnTargetChanged( &entity );

    // set nothing as target
    CheckMissionInterfacePurge();
    ResetState();
    CheckBuild();
    orderPresenter.OnTargetChanged( 0 );
}

BOOST_FIXTURE_TEST_CASE( order_presenter_change_type_without_target, OrderPresenterFixture )
{
    // set agent as mission type
    CheckMissionInterfacePurge();
    ResetState();
    CheckBuild();
    orderPresenter.OnMissionTypeChanged( eMissionType_Pawn );

    // set automat as mission type
    CheckMissionInterfacePurge();
    ResetState( eMissionType_Automat );
    CheckBuild();
    orderPresenter.OnMissionTypeChanged( eMissionType_Automat );

    // set crowd as mission type
    CheckMissionInterfacePurge();
    ResetState( eMissionType_Population );
    CheckBuild();
    orderPresenter.OnMissionTypeChanged( eMissionType_Population );

    // set frag order as mission type
    CheckMissionInterfacePurge();
    ResetState( eMissionType_FragOrder );
    CheckBuild();
    orderPresenter.OnMissionTypeChanged( eMissionType_FragOrder );
}

BOOST_FIXTURE_TEST_CASE( order_presenter_change_type_with_target, OrderPresenterFixture )
{
    // set agent as target
    CheckMissionInterfacePurge( &entity );
    CheckUnitBuild( missionSelector );
    orderPresenter.OnTargetChanged( &entity );

    // set frag order as mission type
    CheckMissionInterfacePurge( &entity );
    CheckFragOrderBuild();
    orderPresenter.OnMissionTypeChanged( eMissionType_FragOrder );

    // set unit as mission type
    CheckMissionInterfacePurge( &entity );
    CheckUnitBuild( missionSelector );
    orderPresenter.OnMissionTypeChanged( eMissionType_Pawn );
}

BOOST_FIXTURE_TEST_CASE( order_presenter_set_target_and_mission_then_clear_target, OrderPresenterFixture )
{
    // set entity
    CheckMissionInterfacePurge( &entity );
    CheckUnitBuild( missionSelector );
    orderPresenter.OnTargetChanged( &entity );

    // set "missionUnit1" mission
    CheckMissionInterfacePurgeAndBuild( &entity );
    CheckUnitBuild( "missionUnit1" );
    orderPresenter.OnMissionChanged( "missionUnit1" );

    // remove target and keeps the current mission and its parameters
    state.target_ = 0;
    state.types_ = types;
    state.missions_ = boost::assign::list_of( "missionUnit1" )( "missionUnit2" )( "missionUnitA" )("missionUnitZ" );
    MOCK_EXPECT( missionInterface.SetEntity ).once();
    CheckBuild();
    orderPresenter.OnTargetChanged( 0 );
}

BOOST_FIXTURE_TEST_CASE( order_presenter_set_mission_then_set_agent_with_and_without_this_mission, OrderPresenterFixture )
{
    // set agent as mission type
    CheckMissionInterfacePurge();
    ResetState();
    CheckBuild();
    orderPresenter.OnMissionTypeChanged( eMissionType_Pawn );

    // set "missionUnit1" mission
    CheckMissionInterfacePurgeAndBuild();
    state.missionSelector_ = false;
    state.currentMission_ = "missionUnit1";
    state.missions_ = boost::assign::list_of( "missionUnit1" )( "missionUnit2" )( "missionUnitA" )("missionUnitZ" );
    CheckBuild();
    orderPresenter.OnMissionChanged( "missionUnit1" );

    // set entity
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    CheckUnitBuild( "missionUnit1" );
    orderPresenter.OnTargetChanged( &entity );

    // Remove target and keeps the current mission and its parameters
    state.target_ = 0;
    state.types_ = types;
    state.missions_ = boost::assign::list_of( "missionUnit1" )( "missionUnit2" )( "missionUnitA" )("missionUnitZ" );
    MOCK_EXPECT( missionInterface.SetEntity ).once();
    CheckBuild();
    orderPresenter.OnTargetChanged( 0 );

    // set another mission that our entity doesn't have
    CheckMissionInterfacePurgeAndBuild();
    state.missionSelector_ = false;
    state.currentMission_ = "missionUnitA";
    CheckBuild();
    orderPresenter.OnMissionChanged( "missionUnitA" );

    // set this entity
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    MOCK_EXPECT( entity.GetId ).once().returns( 42 );
    MOCK_EXPECT( entity.GetTypeName ).once().returns( boost::cref( kernel::Agent_ABC::typeName_ ) );
    MOCK_EXPECT( decisions->GetMissions ).once().returns( unitMissionDecisionalModel.CreateIterator() );
    MOCK_EXPECT( decisions->GetMissions ).once().returns( unitMissionDecisionalModel.CreateIterator() );
    state = gui::EventOrderViewState( 42, unitTypes, eMissionType_Pawn,
        boost::assign::list_of( "missionUnitA" )( "missionUnit1" )( "missionUnit2" ),
        "missionUnitA", std::vector< std::string>(), true, false );
    CheckBuild();
    orderPresenter.OnTargetChanged( &entity );
}

BOOST_FIXTURE_TEST_CASE( order_presenter_switching_to_the_same_entity_type_keeps_the_parameters, OrderPresenterFixture )
{
    // set agent as target, purge but don't build
    CheckMissionInterfacePurge( &entity );
    CheckUnitBuild( missionSelector );
    orderPresenter.OnTargetChanged( &entity );

    // set "missionUnit1" mission, purge and build
    CheckMissionInterfacePurgeAndBuild( &entity );
    CheckUnitBuild( "missionUnit1" );
    orderPresenter.OnMissionChanged( "missionUnit1" );

    // set another agent with the same mission, keep parameters
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    CheckUnitBuild( "missionUnit1" );
    orderPresenter.OnTargetChanged( &entity );

    // set another mission, purge and build
    CheckMissionInterfacePurgeAndBuild( &entity );
    CheckUnitBuild( "missionUnit2" );
    orderPresenter.OnMissionChanged( "missionUnit2" );

    // set another agent with the same mission, keep parameters
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    CheckUnitBuild( "missionUnit2" );
    orderPresenter.OnTargetChanged( &entity );

    // set frag order mission type, purge
    CheckMissionInterfacePurge( &entity );
    CheckFragOrderBuild();
    orderPresenter.OnMissionTypeChanged( eMissionType_FragOrder );
}

BOOST_FIXTURE_TEST_CASE( order_presenter_display_frag_order_when_a_mission_starts, OrderPresenterFixture )
{
    // set an agent
    CheckMissionInterfacePurge( &entity );
    CheckUnitBuild( missionSelector );
    orderPresenter.OnTargetChanged( &entity );

    // set frag order mission type, display all fragorders with fragorder2 and fragorder3 unavailable
    CheckMissionInterfacePurge( &entity );
    CheckFragOrderBuild();
    orderPresenter.OnMissionTypeChanged( eMissionType_FragOrder );

    // set another entity with an active mission (missionUnit1), only fragorder3
    // is unavailable because fragorder2 is available in the current active mission missionUnit1
    MOCK_EXPECT( missionInterface.SetEntity ).once().with( &entity );
    CheckFragOrderBuild( &allMissionResolver.Get( 3 ), boost::assign::list_of( "fragorder3" ) );
    orderPresenter.OnTargetChanged( &entity );
}
