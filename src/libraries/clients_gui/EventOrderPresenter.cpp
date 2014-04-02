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
#include "moc_EventOrderPresenter.cpp"
#include "Event.h"
#include "EventAction.h"
#include "EventOrderViewState.h"
#include "EventPresenter.h"
#include "EventView_ABC.h"
#include "TimelinePublisher.h"

#include "actions/ActionsModel.h"
#include "actions/ActionFactory_ABC.h"
#include "actions/ActionTasker.h"

#include "actions_gui/MissionInterface_ABC.h"

#include "clients_gui/AutomatDecisions.h"
#include "clients_gui/Decisions_ABC.h"
#include "clients_gui/Tools.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/FragOrder.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/TimelineHelpers.h"
#include "clients_kernel/Tools.h"

#include <timeline/api.h>

#include <boost/assign.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter constructor
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
EventOrderPresenter::EventOrderPresenter( EventView_ABC< EventOrderViewState >& view,
                                          const kernel::AgentTypes& agentTypes,
                                          actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                                          actions::gui::MissionInterface_ABC& missionInterface,
                                          actions::ActionsModel& actionsModel,
                                          actions::ActionFactory_ABC& actionFactory,
                                          TimelinePublisher& timelinePublisher,
                                          kernel::Controllers& controllers )
    : EventSubPresenter_ABC< EventOrderViewState >( eEventTypes_Order, view )
    , agentTypes_( agentTypes )
    , interfaceBuilder_( interfaceBuilder )
    , missionInterface_( missionInterface )
    , actionsModel_( actionsModel )
    , actionFactory_( actionFactory )
    , timelinePublisher_( timelinePublisher )
    , entity_( controllers )
    , order_( 0 )
    , currentAction_( 0 )
    , lastContext_( 0 )
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

namespace
{
    E_MissionType ResolveMissionType( const kernel::Entity_ABC* entity, const boost::optional< const kernel::OrderType& >& type )
    {
        if( type )
            return type->GetType();
        else if( entity )
        {
            if( entity->GetTypeName() == kernel::Agent_ABC::typeName_ )
                return eMissionType_Pawn;
            else if( entity->GetTypeName() == kernel::Automat_ABC::typeName_ )
                return eMissionType_Automat;
            else if( entity->GetTypeName() == kernel::Population_ABC::typeName_ )
                return eMissionType_Population;
        }
        return eMissionType_Pawn;
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::FillFromAction
// Created: ABR 2013-11-22
// -----------------------------------------------------------------------------
void EventOrderPresenter::FillFrom( const Event& event )
{
    if( const actions::Action_ABC* action = event.GetAction() )
    {
        if( const actions::ActionTasker* tasker = action->Retrieve< actions::ActionTasker >() )
            entity_ = tasker->GetTasker();
        auto type = action->GetType();
        Select( ResolveMissionType( entity_, type ), type ? type->GetName() : "", action );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::OnTargetChanged
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderPresenter::OnTargetChanged( const kernel::Entity_ABC* entity )
{
    entity_ = entity;
    Select( state_->currentType_, state_->currentMission_ );
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::OnMissionTypeChanged
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderPresenter::OnMissionTypeChanged( E_MissionType missionType )
{
    Select( missionType );
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::OnMissionChanged
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderPresenter::OnMissionChanged( const QString& mission )
{
    if( currentAction_ )
        for( unsigned int i = currentAction_->Count(); i >= 4; --i ) // $$$$ SLI: Assuming the 4 first parameters are always the danger direction, limits and limas
            currentAction_->Remove( i );
    Select( state_->currentType_, mission.toStdString(), currentAction_ );
    BuildView();
}

namespace
{
    bool IsValidTarget( const kernel::Entity_ABC* entity )
    {
        if( !entity )
            return false;
        if( entity->GetTypeName() == kernel::Agent_ABC::typeName_ )
        {
            if( const kernel::Entity_ABC* superior = entity->Get< kernel::TacticalHierarchies >().GetSuperior() )
                return !tools::IsEngaged( *superior );
        }
        else if( entity->GetTypeName() == kernel::Automat_ABC::typeName_ )
            return tools::IsEngaged( *entity );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::ShouldEnableTrigger
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
bool EventOrderPresenter::ShouldEnableTrigger() const
{
    return IsValidTarget( entity_ ) &&          // check target validity
           missionInterface_.CheckValidity() && // check parameters validity
           !state_->invalid_ &&                 // check mission validity
           !state_->missionSelector_ &&
           std::find( state_->disabledMissions_.begin(), state_->disabledMissions_.end(), state_->currentMission_ ) == state_->disabledMissions_.end();
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::ShouldEnableClear
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
bool EventOrderPresenter::ShouldEnableClear() const
{
    return true; // return false if no param set
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::GetLastContext
// Created: ABR 2013-12-03
// -----------------------------------------------------------------------------
int EventOrderPresenter::GetLastContext() const
{
    return lastContext_;
}

namespace
{
    actions::Action_ABC* CreateAction( E_MissionType type,
                                       actions::ActionFactory_ABC& actionsFactory,
                                       const kernel::OrderType* orderType,
                                       const kernel::Entity_ABC* entity )
    {
        if( type == eNbrMissionType )
            throw MASA_EXCEPTION( "Can't create action with an invalid type" );
        if( !orderType )
            return actionsFactory.CreateAction( entity, type );
        if( type == eMissionType_FragOrder )
            return actionsFactory.CreateAction( entity, static_cast< const kernel::FragOrderType& >( *orderType ) );
        return actionsFactory.CreateAction( entity, static_cast< const kernel::MissionType& >( *orderType ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Trigger
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderPresenter::Trigger( const gui::Event& event )
{
    if( const actions::Action_ABC* action = event.GetAction() )
    {
        missionInterface_.FixOrigin( true );
        lastContext_ = actionsModel_.Publish( *action );
    }
    else
        throw MASA_EXCEPTION( "Invalid action" );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::CommitTo
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderPresenter::CommitTo( timeline::Event& event ) const
{
    event.name.clear();
    event.action.payload.clear();
    if( actions::Action_ABC* action = CreateAction( state_->currentType_, actionFactory_, order_, entity_ ) )
    {
        missionInterface_.CommitTo( *action );
        action->Publish( timelinePublisher_, 0 );
        event.name = action->GetName();
        event.action.payload = timelinePublisher_.GetPayload();
        delete currentAction_;
        currentAction_ = action;
    }
    event.action.apply = true;
    event.action.target = CREATE_EVENT_TARGET( EVENT_ORDER_PROTOCOL, EVENT_SIMULATION_SERVICE );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Purge
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderPresenter::Purge()
{
    EventSubPresenter_ABC< EventOrderViewState >::Purge();
    entity_ = 0;
    order_ = 0;
    Select();
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::Clear
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderPresenter::Clear()
{
    missionInterface_.Rebuild( interfaceBuilder_ );
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
                                   const kernel::Mission* currentMission )
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

                if( currentMission && currentMission->GetType().GetId() == mission.GetType().GetId() )
                    enabledResult.push_back( name );
                else if( std::find( disabledResult.begin(), disabledResult.end(), name ) == disabledResult.end() )
                    disabledResult.push_back( name );

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
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderPresenter::Select( E_MissionType type /*= eMissionType_Pawn*/,
                                  std::string mission /*= ""*/,                 // need to copy it here, because of the following state_.Purge()
                                  const actions::Action_ABC* action /*= 0*/ )
{
    if( type == eNbrMissionType )
        throw MASA_EXCEPTION( "Invalid type" );
    if( entity_ )
        SelectWithTarget( *entity_, type, mission, action );
    else
        SelectWithoutTarget( type, mission, action );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::SelectWithoutTarget
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::SelectWithoutTarget( E_MissionType type,
                                               const std::string& mission,
                                               const actions::Action_ABC* action )
{
    const std::string lastMission = state_->currentMission_;
    state_->Purge();
    state_->target_ = 0;
    state_->currentMission_ = mission;
    state_->currentType_ = type;

    // Retrieve types
    for( int i = 0; i < eNbrMissionType; ++i )
        state_->types_.push_back( static_cast< E_MissionType >( i ) );

    // Retrieve missions
    if( state_->currentType_ == eMissionType_FragOrder )
        FillOrders< kernel::FragOrderType >( state_->missions_, agentTypes_, state_->currentType_ );
    else
        FillOrders< kernel::MissionType >( state_->missions_, agentTypes_, state_->currentType_ );

    // Reset current mission
    const std::string selector = GetSelector( state_->currentType_ );
    if( state_->currentMission_.empty() && !state_->missions_.empty() ||
        state_->currentMission_ == selector )
    {
        if( std::find( state_->missions_.begin(), state_->missions_.end(), selector ) == state_->missions_.end() )
            state_->missions_.insert( state_->missions_.begin(), selector );
        state_->currentMission_ = selector;
    }
    state_->missionSelector_ = state_->currentMission_ == selector;

    // Retrieve current mission
    order_ = state_->currentType_ == eMissionType_FragOrder ?
        GetCurrentOrder< kernel::FragOrderType >( agentTypes_, state_->currentType_, state_->currentMission_ ) :
        GetCurrentOrder< kernel::MissionType >( agentTypes_, state_->currentType_, state_->currentMission_ );

    // Reset interface only for a different mission
    if( state_->currentMission_ != lastMission || state_->missionSelector_ )
    {
        // Build parameters interface
        missionInterface_.Purge();
        if( order_ )
            missionInterface_.Build( interfaceBuilder_, *order_, state_->currentType_ );
        if( action )
            missionInterface_.FillFrom( *action );
    }
    missionInterface_.SetEntity( 0 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderPresenter::SelectWithTarget
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventOrderPresenter::SelectWithTarget( const kernel::Entity_ABC& entity,
                                            E_MissionType type,
                                            const std::string& mission,
                                            const actions::Action_ABC* action )
{
    const gui::Decisions_ABC* decisions = entity.Retrieve< gui::Decisions_ABC >();
    if( !decisions )
        throw MASA_EXCEPTION( "Can't find Decisions_ABC for entity: " + entity.GetId() );

    E_MissionType entityType = GetEntityType( entity );
    // if type doesn't match with entity type
    if( type != eMissionType_FragOrder && type != entityType )
        type = entityType;

    const std::string lastMission = state_->currentMission_;
    E_MissionType lastMissionType = state_->currentType_;
    state_->Purge();
    state_->target_ = entity.GetId();
    state_->currentType_ = type;
    state_->currentMission_ = mission;

    // Retrieve entity missions
    if( state_->currentType_ == eMissionType_FragOrder )
    {
        // General frag order
        FillCompatibleOrders< kernel::FragOrder >( state_->missions_, decisions->GetFragOrders() );
        // frag order relating to a mission
        FillCompatibleFragOrders( state_->missions_, state_->disabledMissions_, decisions->GetMissions(), decisions->GetCurrentMission() );
    }
    else
        FillCompatibleOrders( state_->missions_, decisions->GetMissions() );

    std::string selectorText = GetSelector( state_->currentType_ );
    // Entity type unchanged && current mission set && unknown mission => invalid mission
    state_->invalid_ = state_->currentType_ == lastMissionType && !state_->currentMission_.empty() &&
                      state_->currentMission_ != selectorText &&  std::find( state_->missions_.begin(), state_->missions_.end(), mission ) == state_->missions_.end();

    // Reset current mission
    // current mission is empty || (entity type changed(except frag oder) && unknown mission) => select first mission
    if( ( state_->currentMission_.empty() || // no mission selected
          state_->currentMission_ == selectorText ||
          lastMissionType != state_->currentType_ &&
          state_->currentType_ != eMissionType_FragOrder && lastMissionType != eMissionType_FragOrder &&
          std::find( state_->missions_.begin(), state_->missions_.end(), mission ) == state_->missions_.end()
         ) && !state_->missions_.empty() )
    {
        state_->missions_.insert( state_->missions_.begin(), selectorText );
        state_->currentMission_ = state_->missions_[ 0 ];
        state_->missionSelector_ = true;
    }

    // Insert invalid mission
    if( state_->invalid_ )
        state_->missions_.insert( state_->missions_.begin(), state_->currentMission_ );

    // Fill types
    state_->types_ = boost::assign::list_of( entityType )( eMissionType_FragOrder );

    // Retrieve current order
    order_ = 0;
    if( state_->currentType_ == eMissionType_FragOrder )
    {
        // General frag order
        order_ = GetCurrentOrder< kernel::FragOrder >( decisions->GetFragOrders(), state_->currentMission_ );
        // Frag order with mission
        if( !order_ )
            order_ = GetCurrentFragOrder( decisions->GetMissions(), state_->currentMission_ );
    }
    else
        order_ = GetCurrentOrder< kernel::Mission >( decisions->GetMissions(), state_->currentMission_ );

    // no last mission || entity type changed || last mission != current mission => need to refresh parameters interface
    if( ( lastMission.empty() ||
        lastMissionType != state_->currentType_ ||
        lastMission != state_->currentMission_ ||
        action ) && !state_->invalid_ )
    {
        // Purge parameters interface
        missionInterface_.Purge();
        // Build parameters interface
        if( order_ )
            missionInterface_.Build( interfaceBuilder_, *order_, state_->currentType_ );
    }

    missionInterface_.SetEntity( &entity );
    // Fill parameters
    if( action )
        missionInterface_.FillFrom( *action );
}
