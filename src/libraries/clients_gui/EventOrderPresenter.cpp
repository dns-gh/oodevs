// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventOrderPresenter.h"
#include "EventOrderView_ABC.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/FragOrder.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Tools.h"
#include "actions_gui/MissionInterface_ABC.h"
#include "gaming/Decisions_ABC.h"
#include <boost/assign.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter constructor
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
EventOrderPresenter::EventOrderPresenter( EventOrderView_ABC& view, const kernel::AgentTypes& agentTypes,
                                          actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                                          actions::gui::MissionInterface_ABC& missionInterface )
    : view_              ( view )
    , agentTypes_        ( agentTypes )
    , interfaceBuilder_  ( interfaceBuilder )
    , missionInterface_  ( missionInterface )
    , planningMode_      ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter destructor
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
EventOrderPresenter::~EventOrderPresenter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::Select()
{
    Select( eMissionType_Pawn );
}

namespace
{
    bool localAwareStringCompare( const std::string& string1, const std::string& string2 )
    {
        return QString::fromStdString( string1 ).localeAwareCompare( QString::fromStdString( string2 ) ) < 0;
    }
    template< typename T >
    void FillOrders( std::vector< std::string >& result, const kernel::AgentTypes& agentTypes,
                     const E_MissionType type )
    {
        auto it = agentTypes.tools::Resolver< T >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const T& mission = it.NextElement();
            if( mission.GetType() == type )
                result.push_back( mission.GetName() );
        }
        std::sort( result.begin(), result.end(), &localAwareStringCompare );
    }
    template< typename T >
    const kernel::OrderType* GetCurrentOrder( const kernel::AgentTypes& agentTypes,
                                              const E_MissionType type, const std::string& name )
    {
        auto it = agentTypes.tools::Resolver< T >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const T& mission = it.NextElement();
            if( mission.GetType() == type && name == mission.GetName() )
                return &mission;
        }
        return 0;
    }
    E_MissionType GetEntityType( const kernel::Entity_ABC& entity )
    {
        const std::string& typeName = entity.GetTypeName();
        if( typeName == kernel::Automat_ABC::typeName_ )
            return eMissionType_Automat;
        if( typeName == kernel::Population_ABC::typeName_ )
            return eMissionType_Population;
        return eMissionType_Pawn;
    }
    template< typename T >
    void FillCompatibleOrders( std::vector< std::string >& result, tools::Iterator< const T& > it )
    {
        while( it.HasMoreElements() )
            result.push_back( it.NextElement().GetName() );
        std::sort( result.begin(), result.end(), &localAwareStringCompare );
    }
    void FillCompatibleFragOrders( std::vector< std::string >& result,
                                   std::vector< std::string >& disabledResult,
                                   tools::Iterator< const kernel::Mission& > it,
                                   const kernel::Mission* currentMission,
                                   bool planningMode )
    {
        std::vector< std::string > enabledResult;
        while( it.HasMoreElements() )
        {
            const kernel::Mission& mission = it.NextElement();
            auto itFrag = mission.CreateIterator();
            while( itFrag.HasMoreElements() )
            {
                const kernel::OrderType& order = itFrag.NextElement().GetType();
                const std::string name = order.GetName();

                if( !planningMode )
                {
                    if( currentMission && currentMission->GetType().GetId() == mission.GetType().GetId() )
                        enabledResult.push_back( name );
                    else if( std::find( disabledResult.begin(), disabledResult.end(), name ) == disabledResult.end() )
                        disabledResult.push_back( name );
                }

                if( std::find( result.begin(), result.end(), name ) == result.end() )
                    result.push_back( name );
            }
        }
        for( auto it = enabledResult.begin(); it != enabledResult.end(); ++it )
            disabledResult.erase( std::remove( disabledResult.begin(), disabledResult.end(), *it ), disabledResult.end() );
        std::sort( result.begin(), result.end(), &localAwareStringCompare );
    }
    template< typename T >
    const kernel::OrderType* GetCurrentOrder( tools::Iterator< const T& > it, const std::string& name )
    {
        while( it.HasMoreElements() )
        {
            const T& order = it.NextElement();
            if( order.GetName() == name )
                return &order.GetType();
        }
        return 0;
    }
    const kernel::OrderType* GetCurrentFragOrder( tools::Iterator< const kernel::Mission& > it,
                                                  const std::string& name )
    {
        while( it.HasMoreElements() )
        {
            const kernel::Mission& mission = it.NextElement();
            auto itFrag = mission.CreateIterator();
            while( itFrag.HasMoreElements() )
            {
                const kernel::OrderType& order = itFrag.NextElement().GetType();
                if( order.GetName() == name )
                    return &order;
            }
        }
        return 0;
    }
    std::string GetSelector( E_MissionType type )
    {
        return type == eMissionType_FragOrder
            ? tools::translate( "EventOrderPresenter", "<Select frag order>" ).toStdString()
            : tools::translate( "EventOrderPresenter", "<Select mission>" ).toStdString();
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::Select( E_MissionType type )
{
    Select( type, "", 0 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::Select( E_MissionType type, const std::string& mission, const actions::Action_ABC* action )
{
    const std::string lastMission = state_.currentMission_;
    state_.Purge();
    state_.currentMission_ = mission;
    state_.currentType_ = type;

    // Retrieve types
    for( int i = 0; i < eNbrMissionTypes; ++i )
        state_.types_.push_back( static_cast< E_MissionType >( i ) );

    // Retrieve missions
    if( state_.currentType_ == eMissionType_FragOrder )
        FillOrders< kernel::FragOrderType >( state_.missions_, agentTypes_, state_.currentType_ );
    else
        FillOrders< kernel::MissionType >( state_.missions_, agentTypes_, state_.currentType_ );

    // Reset current mission
    if( state_.currentMission_.empty() && !state_.missions_.empty() )
    {
        state_.missions_.insert( state_.missions_.begin(), GetSelector( state_.currentType_ ) );
        state_.currentMission_ = state_.missions_[ 0 ];
        state_.missionSelector_ = true;
    }

    // Retrieve current mission
    const kernel::OrderType* order = state_.currentType_ == eMissionType_FragOrder ?
        GetCurrentOrder< kernel::FragOrderType >( agentTypes_, state_.currentType_, state_.currentMission_ ) :
        GetCurrentOrder< kernel::MissionType >( agentTypes_, state_.currentType_, state_.currentMission_ );

    // Reset interface only for a different mission
    if( state_.currentMission_ != lastMission || state_.missionSelector_ )
    {
        // Build parameters interface
        missionInterface_.Purge();
        if( order )
            missionInterface_.Build( interfaceBuilder_, *order, state_.currentType_ );
        if( action )
            missionInterface_.FillFrom( *action );
    }
    missionInterface_.SetEntity( 0 );

    // Update the view
    view_.Build( state_ );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::Select( E_MissionType type, const std::string& mission )
{
    Select( type, mission, 0 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::Select( const Decisions_ABC& decisions )
{
    const kernel::Entity_ABC& entity = decisions.GetAgent();
    E_MissionType type = GetEntityType( entity );
    Select( decisions, entity, type, type, "", 0 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::Select( const Decisions_ABC& decisions, E_MissionType type )
{
    Select( decisions, type, "" );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::Select( const Decisions_ABC& decisions, E_MissionType type, const std::string& mission )
{
    Select( decisions, type, mission, 0 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::Select( const Decisions_ABC& decisions, E_MissionType type, const std::string& mission,
                                  const actions::Action_ABC* action )
{
    const kernel::Entity_ABC& entity = decisions.GetAgent();
    auto entityType = GetEntityType( entity );

    // if type doesn't match with entity type
    if( type != eMissionType_FragOrder && type != entityType )
        type = entityType;

    Select( decisions, entity, type, entityType, mission, action );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::Select( const Decisions_ABC& decisions, const kernel::Entity_ABC& entity, E_MissionType type,
                                  E_MissionType entityType, const std::string& mission, const actions::Action_ABC* action )
{
    const std::string lastMission = state_.currentMission_;
    E_MissionType lastMissionType = state_.currentType_;
    state_.Purge();
    state_.currentType_ = type;
    state_.currentMission_ = mission;

    // Retrieve entity missions
    if( state_.currentType_ == eMissionType_FragOrder )
    {
        // General frag order
        FillCompatibleOrders< kernel::FragOrder >( state_.missions_, decisions.GetFragOrders() );
        // frag order relating to a mission
        FillCompatibleFragOrders( state_.missions_, state_.disabledMissions_, decisions.GetMissions(), decisions.GetCurrentMission(), planningMode_ );
    }
    else
        FillCompatibleOrders( state_.missions_, decisions.GetMissions() );

    std::string selectorText = GetSelector( state_.currentType_ );
    // Entity type unchanged && current mission set && unknown mission => invalid mission
    state_.invalid_ = state_.currentType_ == lastMissionType && !state_.currentMission_.empty() &&
                      state_.currentMission_ != selectorText &&  std::find( state_.missions_.begin(), state_.missions_.end(), mission ) == state_.missions_.end();

    // Reset current mission
    // current mission is empty || (entity type changed(except frag oder) && unknown mission) => select first mission
    if( ( state_.currentMission_.empty() || // no mission selected
          state_.currentMission_ == selectorText ||
          lastMissionType != state_.currentType_ &&
          state_.currentType_ != eMissionType_FragOrder && lastMissionType != eMissionType_FragOrder &&
          std::find( state_.missions_.begin(), state_.missions_.end(), mission ) == state_.missions_.end()
         ) && !state_.missions_.empty() )
    {
        state_.missions_.insert( state_.missions_.begin(), selectorText );
        state_.currentMission_ = state_.missions_[ 0 ];
        state_.missionSelector_ = true;
    }

    // Insert invalid mission
    if( state_.invalid_ )
        state_.missions_.insert( state_.missions_.begin(), state_.currentMission_ );

    // Fill types
    state_.types_ = boost::assign::list_of( entityType )( eMissionType_FragOrder );

    // Retrieve current order
    const kernel::OrderType* order = 0;
    if( state_.currentType_ == eMissionType_FragOrder )
    {
        // General frag order
        order = GetCurrentOrder< kernel::FragOrder >( decisions.GetFragOrders(), state_.currentMission_ );
        // Frag order with mission
        if( !order )
            order = GetCurrentFragOrder( decisions.GetMissions(), state_.currentMission_ );
    }
    else
        order = GetCurrentOrder< kernel::Mission >( decisions.GetMissions(), state_.currentMission_ );

    // no last mission || entity type changed || last mission != current mission => need to refresh parameters interface
    if( ( lastMission.empty() ||
        lastMissionType != state_.currentType_ ||
        lastMission != state_.currentMission_ || 
        action ) && !state_.invalid_ )
    {
        // Purge parameters interface
        missionInterface_.Purge();
        // Build parameters interface
        if( order )
            missionInterface_.Build( interfaceBuilder_, *order, state_.currentType_ );
    }

    missionInterface_.SetEntity( &entity );
    // Fill parameters
    if( action )
        missionInterface_.FillFrom( *action );

    // Update the view
    view_.Build( state_ );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Publish
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::Publish( actions::ActionsModel& model, timeline::Event* event, bool planned, int context ) const
{
    missionInterface_.SetPlanned( planned );
    if( state_.currentType_ == eMissionType_FragOrder )
        missionInterface_.PublishFragOrder( model, event, context );
    else
        missionInterface_.PublishMissionOrder( model, event, context );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::SetPlanningMode
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::SetPlanningMode( bool value )
{
    planningMode_ = value;
}
