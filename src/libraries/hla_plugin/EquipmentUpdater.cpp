// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "EquipmentUpdater.h"
#include "RemoteAgentSubject_ABC.h"
#include "ContextHandler_ABC.h"
#include "ContextFactory_ABC.h"
#include "ComponentTypes_ABC.h"
#include "ComponentTypeVisitor_ABC.h"
#include "HlaObject_ABC.h"
#include "LocalAgentResolver_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "protocol/SimulationSenders.h"
#include "tools/MessageController_ABC.h"
#include <algorithm>

using namespace plugins::hla;

EquipmentUpdater::T_Component::T_Component()
    : available_( 0u )
    , dead_( 0u )
    , lightDamages_( 0u )
    , heavyDamages_( 0u )
{
    //NOTHING
}
EquipmentUpdater::T_Component::T_Component( unsigned int avail, unsigned int dead, unsigned int light, unsigned int heavy )
    : available_( avail )
    , dead_( dead )
    , lightDamages_( light )
    , heavyDamages_( heavy )
{
    //NOTHING
}

bool EquipmentUpdater::T_Component::operator == (const T_Component& rhs )
{
    return available_ == rhs.available_ &&
        dead_ == rhs.dead_ &&
        lightDamages_ == rhs.lightDamages_ &&
        heavyDamages_ == rhs.heavyDamages_;
}
// -----------------------------------------------------------------------------
// Name: EquipmentUpdater constructor
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
EquipmentUpdater::EquipmentUpdater( RemoteAgentSubject_ABC& subject, ContextHandler_ABC< sword::UnitCreation >& handler,
                                    dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& factory,
                                    const rpr::EntityTypeResolver_ABC& resolver, const ComponentTypes_ABC& componentTypes,
                                    tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                                    dispatcher::Logger_ABC& logger, const LocalAgentResolver_ABC& localAgentResolver )
    : subject_       ( subject )
    , handler_       ( handler )
    , publisher_     ( publisher )
    , factory_       ( factory )
    , resolver_      ( resolver )
    , componentTypes_( componentTypes )
    , logger_        ( logger )
    , localAgentResolver_( localAgentResolver )
{
    subject_.Register( *this );
    handler_.Register( *this );
    CONNECT( messageController, *this, unit_attributes );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater destructor
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
EquipmentUpdater::~EquipmentUpdater()
{
    handler_.Unregister( *this );
    subject_.Unregister( *this );
}

namespace
{
    template< typename T >
    class ComponentTypeVisitor : public ComponentTypeVisitor_ABC
    {
    public:
        ComponentTypeVisitor( const std::string& identifier, T& agents )
            : identifier_( identifier )
            , agents_    ( agents )
        {
            // NOTHING
        }
        virtual void NotifyEquipment( unsigned int typeIdentifier, const std::string& typeName, unsigned int number )
        {
            agents_[ identifier_ ][ typeName ] = std::make_pair( typeIdentifier, number );
        }
    private:
        const std::string identifier_;
        T& agents_;
    };
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::Notify
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::Notify( const sword::UnitCreation& message, const std::string& identifier )
{
    ComponentTypeVisitor< T_AgentsTypes > visitor( identifier, agentTypes_ );
    componentTypes_.Apply( message.type().id(), visitor );
    identifiers_.left.insert( T_Identifiers::left_value_type( identifier, message.unit().id() ) );
    SendUpdate( identifier );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::Notify
// Created: VPR 2011-10-03
// -----------------------------------------------------------------------------
void EquipmentUpdater::Notify( const sword::UnitAttributes& message, int /*context*/ )
{
    if( !message.has_equipment_dotations() )
        return;
    T_Identifiers::right_const_iterator it = identifiers_.right.find( message.unit().id() );
    if( it == identifiers_.right.end() )
        return;
    bool mustSend = false;
    const std::string& identifier = it->second;
    const T_Components& remoteComponents = remoteAgents_[ identifier ];
    const T_StaticComponents& staticComponents = agentTypes_[ identifier ];
    for( int i = 0; i < message.equipment_dotations().elem_size(); ++i )
    {
        const sword::EquipmentDotations::EquipmentDotation& dotation = message.equipment_dotations().elem( i );
        std::string equipmentName;
        std::for_each( staticComponents.begin(), staticComponents.end(), [&](const T_StaticComponents::value_type& staticComponent)
            {
                if( staticComponent.second.first == dotation.type().id() )
                    equipmentName = staticComponent.first;
            });
        const T_Components::const_iterator remoteComponent = remoteComponents.find( equipmentName );
        mustSend = mustSend || ( remoteComponent != remoteComponents.end() && 
            static_cast< int >( remoteComponent->second.available_ ) != dotation.available() );
    }
    if( mustSend )
        SendUpdate( identifier );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::RemoteCreated
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::RemoteCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object )
{
    hlaObjects_[ identifier ] = &object;
    object.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::RemoteDestroyed
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::RemoteDestroyed( const std::string& identifier )
{
    hlaObjects_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::Moved
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::Moved( const std::string& /*identifier*/, double /*latitude*/, double /*longitude*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::SideChanged
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::NameChanged
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::TypeChanged
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::EquipmentUpdated
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int available,
        unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages )
{
    std::string equipmentName;
    if (! resolver_.Resolve( equipmentType, equipmentName ) )
        logger_.LogWarning( std::string( "Could not find equipment for EntityType: ") + equipmentType.str() );
    T_Component& component = remoteAgents_[ identifier ][ equipmentName ];
    T_Component newValue( available, dead, lightDamages, heavyDamages );
    if( component == newValue )
        return;
    component = newValue;
    SendUpdate( identifier );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::UniqueIdChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void EquipmentUpdater::UniqueIdChanged( const std::string& /*identifier*/, const T_UniqueId& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::CallsignChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void EquipmentUpdater::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::SendUpdate
// Created: SLI 2011-09-30
// -----------------------------------------------------------------------------
void EquipmentUpdater::SendUpdate( const std::string& identifier )
{
    T_AgentsTypes::const_iterator agentType = agentTypes_.find( identifier );
    T_Agents::const_iterator remoteAgent = remoteAgents_.find( identifier );
    if( agentType == agentTypes_.end() || remoteAgent == remoteAgents_.end() )
        return;
    const unsigned int agentIdentifier = identifiers_.left.at( identifier );
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_unit()->set_id( agentIdentifier );
    message().set_type( sword::UnitMagicAction::change_equipment_state );
    sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
    parameter.set_null_value( false );
    std::for_each( agentType->second.begin(), agentType->second.end(), [&](const T_StaticComponents::value_type& component)
    {
        const std::string& componentTypeName = component.first;
        const unsigned int componentTypeIdentifier = component.second.first;
        const unsigned int componentStaticNumber = component.second.second;
        const EquipmentUpdater::T_Components::const_iterator remoteComponent = remoteAgent->second.find( componentTypeName );
        if( remoteComponent != remoteAgent->second.end() && remoteComponent->second.available_ <= componentStaticNumber )
        {
            sword::MissionParameter_Value* componentChanged = parameter.add_value();
            componentChanged->add_list()->set_identifier( componentTypeIdentifier );
            componentChanged->add_list()->set_quantity( remoteComponent->second.available_ );
            componentChanged->add_list()->set_quantity( remoteComponent->second.dead_ );
            componentChanged->add_list()->set_quantity( remoteComponent->second.heavyDamages_ );
            componentChanged->add_list()->set_quantity( remoteComponent->second.lightDamages_ );
            componentChanged->add_list()->set_quantity( 0 );
            componentChanged->add_list()->set_quantity( 0 );
            componentChanged->add_list()->mutable_list();
        }
    });
    if( message().parameters().elem( 0 ).value_size() > 0 )
        message.Send( publisher_, factory_.Create() );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::LocalCreated
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void EquipmentUpdater::LocalCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object )
{
    hlaObjects_[ identifier ] = &object;
    const unsigned long simId = localAgentResolver_.Resolve(identifier);
    if( simId != 0 )
    {
        identifiers_.left.insert( T_Identifiers::left_value_type( identifier, simId ) );
        const unsigned long agentType = localAgentResolver_.AgentType( simId );
        if( agentType != 0 )
        {
            ComponentTypeVisitor< T_AgentsTypes > visitor( identifier, agentTypes_ );
            componentTypes_.Apply( agentType, visitor );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::LocalDestroyed
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void EquipmentUpdater::LocalDestroyed( const std::string& identifier )
{
    hlaObjects_.erase( identifier);
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::Divested
// Created: AHC 2010-03-02
// -----------------------------------------------------------------------------
void EquipmentUpdater::Divested( const std::string& identifier )
{
    T_HLAObjects::const_iterator itObj( hlaObjects_.find( identifier ) );
    if( hlaObjects_.end() != itObj )
        itObj->second->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void EquipmentUpdater::Acquired( const std::string& identifier )
{
    T_HLAObjects::const_iterator itObj( hlaObjects_.find( identifier ) );
    if( hlaObjects_.end() != itObj )
        itObj->second->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::EmbeddedUnitListChanged
// Created: AHC 2010-05-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< T_UniqueId >& /*units*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void EquipmentUpdater::GeometryChanged( const std::string& /*identifier*/, const std::vector< rpr::WorldLocation >& /*perimeter*/, ObjectListener_ABC::GeometryType /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void EquipmentUpdater::ParentChanged( const std::string& /*rtiIdentifier*/, const std::string& /*parentRtiId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void EquipmentUpdater::SubAgregatesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::SubEntitiesChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void EquipmentUpdater::SubEntitiesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::PropagationChanged
// Created: AHC 2013-07-10
// -----------------------------------------------------------------------------
void EquipmentUpdater::PropagationChanged( const std::string& /*rtiIdentifier*/, const std::vector< ObjectListener_ABC::PropagationData >& /*data*/,
                int /*col*/, int /*lig*/, double /*xll*/, double /*yll*/, double /*dx*/, double /*dy*/ )
{
    // NOTHING
}
