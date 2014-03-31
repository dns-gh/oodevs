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
#include "LocalAgentResolver_ABC.h"
#include "clients_kernel/AgentType.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include "dispatcher/Equipment.h"
#include "protocol/Protocol.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include <spatialcontainer/TerrainData.h> // $$$$ _RC_ SLI 2011-02-07: dependency on pathfind!!!
#include <algorithm>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
    class ComponentVisitor : public ComponentTypeVisitor_ABC
    {
    public:
        ComponentVisitor( unsigned int componentTypeIdentifier, unsigned int available, unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages,
                const rpr::EntityTypeResolver_ABC& componentTypeResolver, EventListener_ABC& listener, dispatcher::Logger_ABC& logger )
            : componentTypeIdentifier_( componentTypeIdentifier )
            , available_              ( available )
            , dead_                   ( dead )
            , lightDamages_           ( lightDamages )
            , heavyDamages_           ( heavyDamages )
            , componentTypeResolver_  ( componentTypeResolver )
            , listener_               ( listener )
            , logger_                 ( logger )
        {
            // NOTHING
        }
        virtual void NotifyEquipment( unsigned int typeIdentifier, const std::string& typeName, unsigned int /*number*/ )
        {
            if( typeIdentifier == componentTypeIdentifier_ )
            {
                rpr::EntityType entityType;
                if( ! componentTypeResolver_.Find( typeName, entityType ) )
                    logger_.LogWarning( std::string( "Could not find EntityType for equipment: " ) + typeName );
                listener_.EquipmentChanged( typeIdentifier, entityType, available_, dead_, lightDamages_, heavyDamages_ );
            }
        }
    private:
        const unsigned int componentTypeIdentifier_;
        const unsigned int available_;
        const unsigned int dead_;
        const unsigned int lightDamages_;
        const unsigned int heavyDamages_;
        const rpr::EntityTypeResolver_ABC& componentTypeResolver_;
        EventListener_ABC& listener_;
        dispatcher::Logger_ABC& logger_;
    };
    void NotifyEquipment( EventListener_ABC& listener, const dispatcher::Equipment& equipment, const ComponentTypes_ABC& componentTypes, 
                        const rpr::EntityTypeResolver_ABC& componentTypeResolver, unsigned long agentTypeIdentifier, dispatcher::Logger_ABC& logger )
    {
        ComponentVisitor visitor( equipment.nEquipmentType_, equipment.nNbrAvailable_, equipment.nNbrUnavailable_,
                equipment.nNbrOnSiteFixable_, equipment.nNbrRepairable_, componentTypeResolver, listener, logger );
        componentTypes.Apply( agentTypeIdentifier, visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentProxy constructor
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
AgentProxy::AgentProxy( dispatcher::Agent_ABC& agent, const ComponentTypes_ABC& componentTypes, const rpr::EntityTypeResolver_ABC& componentTypeResolver,
    const LocalAgentResolver_ABC& localAgentResolver, bool doDisaggregation, dispatcher::Logger_ABC& logger )
    : dispatcher::Observer< sword::UnitAttributes >( agent )
    , dispatcher::Observer< sword::UnitEnvironmentType >( agent )
    , agent_                ( agent )
    , componentTypes_       ( componentTypes )
    , componentTypeResolver_( componentTypeResolver )
    , localAgentResolver_( localAgentResolver )
    , doDisaggregation_       ( doDisaggregation )
    , logger_ ( logger )
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
    listeners_.Register( listener );
    listener.SpatialChanged( agent_.GetPosition().X(), agent_.GetPosition().Y(),
                             agent_.GetAltitude(), agent_.GetSpeed(), agent_.GetDirection() );
    agent_.Equipments().Apply( boost::bind( &NotifyEquipment, boost::ref( listener ), _1, boost::cref( componentTypes_ ), boost::cref( componentTypeResolver_ ), agent_.GetType().GetId(), boost::ref( logger_ ) ) );
    std::string parentId( localAgentResolver_.Resolve( agent_.GetSuperior().GetId() ) );
    if( parentId.size() > 0 )
        listener.ParentChanged( parentId );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Unregister
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AgentProxy::Unregister( EventListener_ABC& listener )
{
    listeners_.Unregister( listener );}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Notify
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
void AgentProxy::Notify( const sword::UnitAttributes& attributes )
{
    if( attributes.has_position() || attributes.has_height() || attributes.has_speed() || attributes.has_direction() )
        listeners_.SpatialChanged( agent_.GetPosition().X(), agent_.GetPosition().Y(),
                                      agent_.GetAltitude(), agent_.GetSpeed(), agent_.GetDirection() );
    if( attributes.has_equipment_dotations() && !doDisaggregation_ )
        agent_.Equipments().Apply( boost::bind( &NotifyEquipment, boost::ref( listeners_ ), _1, boost::cref( componentTypes_ ), boost::cref( componentTypeResolver_ ), agent_.GetType().GetId(), boost::ref( logger_ ) ) );
    if( attributes.has_embarked() )
        listeners_.EmbarkmentChanged( agent_.IsMounted() );
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
    listeners_.FormationChanged( isOnRoad );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::PlatformAdded
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void AgentProxy::PlatformAdded( const std::string& name, unsigned int id )
{
    listeners_.PlatformAdded( name, id );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::AddSubordinate
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AgentProxy::AddSubordinate( unsigned int id, Agent_ABC& /*agent*/ )
{
    listeners_.PlatformAdded( "TODO", id );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::RemoveSubordinate
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AgentProxy::RemoveSubordinate( unsigned int /*id*/ )
{
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::HasSubordinate
// Created: AHC 2013-01-15
// -----------------------------------------------------------------------------
bool AgentProxy::HasSubordinate( unsigned int ) const
{
    return false;
}
