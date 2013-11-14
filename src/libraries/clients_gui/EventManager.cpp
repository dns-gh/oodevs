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
#include "clients_kernel/Tools.h"
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
            ? tools::translate( "EventManager", "<Select frag order>" ).toStdString()
            : tools::translate( "EventManager", "<Select mission>" ).toStdString();
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
    std::string lastMission = currentMission_;
    currentMissionType_ = type;

    // Retrieve types
    std::vector< E_MissionType > types;
    for( int i = 0; i < eNbrMissionTypes; ++i )
        types.push_back( static_cast< E_MissionType >( i ) );

    // Retrieve missions
    std::vector< std::string > result;
    if( currentMissionType_ == eMissionType_FragOrder )
        FillOrders< kernel::FragOrderType >( result, agentTypes_, currentMissionType_ );
    else
        FillOrders< kernel::MissionType >( result, agentTypes_, currentMissionType_ );

    // Reset current mission
    bool missionSelector = false;
    if( currentMission.empty() && !result.empty() )
    {
        result.insert( result.begin(), GetSelector( currentMissionType_ ) );
        currentMission = result[ 0 ];
        missionSelector = true;
    }

    // Build interface
    builder_.Build( types, currentMissionType_, result, currentMission, std::vector< std::string >(), false, missionSelector );

    // Retrieve current mission
    const kernel::OrderType* order = currentMissionType_ == eMissionType_FragOrder ?
        GetCurrentOrder< kernel::FragOrderType >( agentTypes_, currentMissionType_, currentMission ) :
        GetCurrentOrder< kernel::MissionType >( agentTypes_, currentMissionType_, currentMission );

    // Reset interface only for a different mission
    if( currentMission != lastMission || missionSelector )
    {
        // Build parameters interface
        missionInterface_.Purge();
        if( order )
            missionInterface_.Build( interfaceBuilder_, *order, currentMissionType_ );
        if( action )
            missionInterface_.FillFrom( *action );
    }
    missionInterface_.SetEntity( 0 );
    currentMission_ = currentMission;
    builder_.UpdateActions();
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
    Select( decisions, type, mission, 0 );
}

// -----------------------------------------------------------------------------
// Name: EventManager::Select
// Created: LGY 2013-10-03
// -----------------------------------------------------------------------------
void EventManager::Select( const Decisions_ABC& decisions, E_MissionType type, const std::string& mission,
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
    std::vector< std::string > disabledResult;
    if( currentMissionType_ == eMissionType_FragOrder )
    {
        // General frag order
        FillCompatibleOrders< kernel::FragOrder >( result, decisions.GetFragOrders() );
        // frag order relating to a mission
        FillCompatibleFragOrders( result, disabledResult, decisions.GetMissions(), decisions.GetCurrentMission(), planningMode_ );
    }
    else
        FillCompatibleOrders( result, decisions.GetMissions() );

    std::string currentMission = mission;
    // Entity type unchanged && current mission set && unknown mission => invalid mission
    bool invalidMission = currentMissionType_ == lastMissionType && !currentMission.empty() &&
        std::find( result.begin(), result.end(), mission ) == result.end();

    // Reset current mission
    // current mission is empty || (entity type changed(except frag oder) && unknown mission) => select first mission
    bool missionSelector = false;
    if( ( currentMission.empty() || // no mission selected
          lastMissionType != currentMissionType_ &&
          currentMissionType_ != eMissionType_FragOrder && lastMissionType != eMissionType_FragOrder &&
          std::find( result.begin(), result.end(), mission ) == result.end()
         ) && !result.empty() )
    {
        result.insert( result.begin(), GetSelector( currentMissionType_ ) );
        currentMission = result[ 0 ];
        missionSelector = true;
    }

    // Insert invalid mission
    if( invalidMission )
        result.insert( result.begin(), currentMission );

    // Build mission combobox
    builder_.Build( boost::assign::list_of( entityType )( eMissionType_FragOrder ),
                    currentMissionType_, result, currentMission, disabledResult, invalidMission, missionSelector );

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
        currentMission_ != currentMission || 
        action ) && !invalidMission )
    {
        // Purge parameters interface
        missionInterface_.Purge();
        // Build parameters interface
        if( order )
            missionInterface_.Build( interfaceBuilder_, *order, currentMissionType_ );
    }

    missionInterface_.SetEntity( &entity );
    // Fill parameters
    if( action )
        missionInterface_.FillFrom( *action );

    currentMission_ = currentMission;

    builder_.UpdateActions();
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
