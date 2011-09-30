// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AgentProxy.h"
#include "EventListener_ABC.h"
#include "ComponentTypes_ABC.h"
#include "ComponentTypeVisitor_ABC.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Equipment.h"
#include "clients_kernel/AgentType.h"
#include <spatialcontainer/TerrainData.h> // $$$$ _RC_ SLI 2011-02-07: dependency on pathfind!!!
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
    class ComponentVisitor : public ComponentTypeVisitor_ABC
    {
    public:
        ComponentVisitor( unsigned int componentTypeIdentifier, unsigned int available, const rpr::EntityTypeResolver_ABC& componentTypeResolver, EventListener_ABC& listener )
            : componentTypeIdentifier_( componentTypeIdentifier )
            , available_              ( available )
            , componentTypeResolver_  ( componentTypeResolver )
            , listener_               ( listener )
        {
            // NOTHING
        }
        virtual void NotifyEquipment( unsigned int typeIdentifier, const std::string& typeName, unsigned int /*number*/ )
        {
            if( typeIdentifier == componentTypeIdentifier_ )
            {
                const rpr::EntityType entityType = componentTypeResolver_.Find( typeName );
                listener_.EquipmentChanged( typeIdentifier, entityType, available_ );
            }
        }
    private:
        const unsigned int componentTypeIdentifier_;
        const unsigned int available_;
        const rpr::EntityTypeResolver_ABC& componentTypeResolver_;
        EventListener_ABC& listener_;
    };
    void NotifyEquipment( EventListener_ABC& listener, const dispatcher::Equipment& equipment, const ComponentTypes_ABC& componentTypes, const rpr::EntityTypeResolver_ABC& componentTypeResolver, unsigned long agentTypeIdentifier )
    {
        ComponentVisitor visitor( equipment.nEquipmentType_, equipment.nNbrAvailable_, componentTypeResolver, listener );
        componentTypes.Apply( agentTypeIdentifier, visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentProxy constructor
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
AgentProxy::AgentProxy( dispatcher::Agent_ABC& agent, const ComponentTypes_ABC& componentTypes, const rpr::EntityTypeResolver_ABC& componentTypeResolver )
    : dispatcher::Observer< sword::UnitAttributes >( agent )
    , dispatcher::Observer< sword::UnitEnvironmentType >( agent )
    , agent_                ( agent )
    , componentTypes_       ( componentTypes )
    , componentTypeResolver_( componentTypeResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentProxy destructor
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
AgentProxy::~AgentProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Register
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AgentProxy::Register( EventListener_ABC& listener )
{
    listeners_.push_back( &listener );
    listener.SpatialChanged( agent_.GetPosition().X(), agent_.GetPosition().Y(),
                             agent_.GetAltitude(), agent_.GetSpeed(), agent_.GetDirection() );
    agent_.Equipments().Apply( boost::bind( &NotifyEquipment, boost::ref( listener ), _1, boost::cref( componentTypes_ ), boost::cref( componentTypeResolver_ ), agent_.GetType().GetId() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Unregister
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AgentProxy::Unregister( EventListener_ABC& listener )
{
    listeners_.erase( std::remove( listeners_.begin(), listeners_.end(), &listener ), listeners_.end() );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Notify
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
void AgentProxy::Notify( const sword::UnitAttributes& attributes )
{
    if( attributes.has_position() || attributes.has_height() || attributes.has_speed() || attributes.has_direction() )
        BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
            listener->SpatialChanged( agent_.GetPosition().X(), agent_.GetPosition().Y(),
                                      agent_.GetAltitude(), agent_.GetSpeed(), agent_.GetDirection() );
    if( attributes.has_equipment_dotations() )
        BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
            agent_.Equipments().Apply( boost::bind( &NotifyEquipment, boost::ref( *listener ), _1, boost::cref( componentTypes_ ), boost::cref( componentTypeResolver_ ), agent_.GetType().GetId() ) );
    if( attributes.has_embarked() )
        BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
            listener->EmbarkmentChanged( agent_.IsMounted() );
}

namespace
{
    bool IsOnRoad( const sword::UnitEnvironmentType& message )
    {
        const unsigned int mask = TerrainData::motorway_  | TerrainData::largeroad_  | TerrainData::mediumroad_
                                | TerrainData::smallroad_ | TerrainData::bridge_;
        return ( message.linear() & mask ) != 0;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Notify
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
void AgentProxy::Notify( const sword::UnitEnvironmentType& message )
{
    const bool isOnRoad = IsOnRoad( message );
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->FormationChanged( isOnRoad );
}
