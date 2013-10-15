// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventManager.h"
#include "EventBuilder_ABC.h"
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
#include "actions_gui/MissionInterface_ABC.h"
#include "gaming/Decisions_ABC.h"
#include <boost/assign.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventManager constructor
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
EventManager::EventManager( EventBuilder_ABC& builder, const kernel::AgentTypes& agentTypes,
                            actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                            actions::gui::MissionInterface_ABC& missionInterface )
    : builder_           ( builder )
    , agentTypes_        ( agentTypes )
    , currentMissionType_( eMissionType_Pawn )
    , interfaceBuilder_  ( interfaceBuilder )
    , missionInterface_  ( missionInterface )
    , planningMode_      ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventManager destructor
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
EventManager::~EventManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventManager::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Select()
{
    Select( eMissionType_Pawn );
}

namespace
{
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
        std::sort( result.begin(), result.end() );
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
        std::sort( result.begin(), result.end() );
    }
    void FillCompatibleFragOrders( std::vector< std::string >& result, tools::Iterator< const kernel::Mission& > it,
                                   const kernel::Mission* currentMission, bool planningMode )
    {
        while( it.HasMoreElements() )
        {
            const kernel::Mission& mission = it.NextElement();
            auto itFrag = mission.CreateIterator();
            while( itFrag.HasMoreElements() )
            {
                const kernel::OrderType& order = itFrag.NextElement().GetType();
                if( ( planningMode || ( currentMission && currentMission->GetType().GetId() == mission.GetType().GetId() ) ) &&
                    std::find( result.begin(), result.end(), order.GetName() ) == result.end() )
                    result.push_back( order.GetName() );
            }
        }
        std::sort( result.begin(), result.end() );
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
}

// -----------------------------------------------------------------------------
// Name: EventManager::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Select( E_MissionType type )
{
    Select( type, "", 0 );
}

// -----------------------------------------------------------------------------
// Name: EventManager::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Select( E_MissionType type, const std::string& mission, const actions::Action_ABC* action )
{
    std::string currentMission = mission;

    // Retrieve types
    std::vector< E_MissionType > types;
    for( int i = 0; i < eNbrMissionTypes; ++i )
        types.push_back( static_cast< E_MissionType >( i ) );

    // Retrieve missions
    std::vector< std::string > result;
    if( type == eMissionType_FragOrder )
        FillOrders< kernel::FragOrderType >( result, agentTypes_, type );
    else
        FillOrders< kernel::MissionType >( result, agentTypes_, type );

    // Reset current mission
    if( currentMission.empty() && !result.empty() )
        currentMission = result[ 0 ];

    // Build interface
    builder_.Build( types, type, result, currentMission );

    currentMissionType_ = type;

    // Retrieve current mission
    const kernel::OrderType* order = type == eMissionType_FragOrder ?
        GetCurrentOrder< kernel::FragOrderType >( agentTypes_, type, currentMission ) :
        GetCurrentOrder< kernel::MissionType >( agentTypes_, type, currentMission );

    // Build parameters interface
    missionInterface_.Purge();
    if( order )
        missionInterface_.Build( interfaceBuilder_, *order, currentMissionType_ );
    if( action )
        missionInterface_.FillFrom( *action );
    missionInterface_.SetEntity( 0 );
    currentMission_ = currentMission;
}

// -----------------------------------------------------------------------------
// Name: EventManager::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Select( E_MissionType type, const std::string& mission )
{
    Select( type, mission, 0 );
}

// -----------------------------------------------------------------------------
// Name: EventManager::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Select( const Decisions_ABC& decisions )
{
    const kernel::Entity_ABC& entity = decisions.GetAgent();
    E_MissionType type = GetEntityType( entity );
    Select( decisions, entity, type, type, "", 0 );
}

// -----------------------------------------------------------------------------
// Name: EventManager::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Select( const Decisions_ABC& decisions, E_MissionType type )
{
    Select( decisions, type, "" );
}

// -----------------------------------------------------------------------------
// Name: EventManager::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Select( const Decisions_ABC& decisions, E_MissionType type, const std::string& mission )
{
    // Get entity
    const kernel::Entity_ABC& entity = decisions.GetAgent();
    // Get entity type
    auto entityType = GetEntityType( entity );

    // if type doesn't match with entity type
    if( type != eMissionType_FragOrder && type != entityType )
        type = entityType;

    Select( decisions, entity, type, entityType, mission, 0 );
}

// -----------------------------------------------------------------------------
// Name: EventManager::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Select( const Decisions_ABC& decisions, E_MissionType type, const std::string& mission,
                           const actions::Action_ABC* action )
{
    // Get entity
    const kernel::Entity_ABC& entity = decisions.GetAgent();
    // Get entity type
    auto entityType = GetEntityType( entity );

    // if type doesn't match with entity type
    if( type != eMissionType_FragOrder && type != entityType )
        type = entityType;

    Select( decisions, entity, type, entityType, mission, action );
}

// -----------------------------------------------------------------------------
// Name: EventManager::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Select( const Decisions_ABC& decisions, const kernel::Entity_ABC& entity, E_MissionType type,
                           E_MissionType entityType, const std::string& mission, const actions::Action_ABC* action )
{
    E_MissionType lastMissionType = currentMissionType_;
    currentMissionType_ = type;

    // Retrieve entity missions
    std::vector< std::string > result;
    if( currentMissionType_ == eMissionType_FragOrder )
    {
        // General frag order
        FillCompatibleOrders< kernel::FragOrder >( result, decisions.GetFragOrders() );
        // frag order relating to a mission
        FillCompatibleFragOrders( result, decisions.GetMissions(), decisions.GetCurrentMission(), planningMode_ );
    }
    else
        FillCompatibleOrders< kernel::Mission >( result, decisions.GetMissions() );

    std::string currentMission = mission;
    // Entity type unchanged && current mission set && unknown mission => invalid mission
    bool invalidMission = currentMissionType_ == lastMissionType && !currentMission.empty() &&
        std::find( result.begin(), result.end(), mission ) == result.end();

    // Reset current mission
    // current mission is empty || (entity type changed(except frag oder) && unknown mission) => select first mission
    if( ( currentMission.empty() || // no mission selected
          ( lastMissionType != currentMissionType_ &&
            currentMissionType_ != eMissionType_FragOrder && lastMissionType != eMissionType_FragOrder &&
            std::find( result.begin(), result.end(), mission ) == result.end() )
          ) && !result.empty() )
        currentMission = result[ 0 ];

    // Insert invalid mission
    if( invalidMission )
        result.insert( result.begin(), currentMission );

    // Build mission combobox
    builder_.Build( boost::assign::list_of( entityType )( eMissionType_FragOrder ),
                    currentMissionType_, result, currentMission, invalidMission );

    // Retrieve current order
    const kernel::OrderType* order = 0;
    if( currentMissionType_ == eMissionType_FragOrder )
    {
        // General frag order
        order = GetCurrentOrder< kernel::FragOrder >( decisions.GetFragOrders(), currentMission );
        // Frag order with mission
        if( !order )
            order = GetCurrentFragOrder( decisions.GetMissions(), currentMission );
    }
    else
        order = GetCurrentOrder< kernel::Mission >( decisions.GetMissions(), currentMission );

    // no last mission || entity type changed || last mission != current mission => need to refresh parameters interface
    if( ( currentMission_.empty() ||
        lastMissionType != currentMissionType_ ||
        currentMission_ != currentMission ) && !invalidMission )
    {
        // Purge parameters interface
        missionInterface_.Purge();
        // Build parameters interface
        if( order )
            missionInterface_.Build( interfaceBuilder_, *order, currentMissionType_ );
        // Fill parameters
        if( action )
            missionInterface_.FillFrom( *action );
    }
    missionInterface_.SetEntity( &entity );
    currentMission_ = currentMission;
}

// -----------------------------------------------------------------------------
// Name: EventManager::Publish
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Publish( actions::ActionsModel& model, timeline::Event* event, bool planned ) const
{
    missionInterface_.SetPlanned( planned );
    if( currentMissionType_ == eMissionType_FragOrder )
        missionInterface_.PublishFragOrder( model, event );
    else
        missionInterface_.PublishMissionOrder( model, event );
}

// -----------------------------------------------------------------------------
// Name: EventManager::SetPlanningMode
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::SetPlanningMode( bool value )
{
    planningMode_ = value;
}
