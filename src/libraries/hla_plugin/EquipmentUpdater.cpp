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
#include "dispatcher/SimulationPublisher_ABC.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "protocol/SimulationSenders.h"
#include "tools/MessageController_ABC.h"
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater constructor
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
EquipmentUpdater::EquipmentUpdater( RemoteAgentSubject_ABC& subject, ContextHandler_ABC< sword::UnitCreation >& handler,
                                    dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& factory,
                                    const rpr::EntityTypeResolver_ABC& resolver, const ComponentTypes_ABC& componentTypes,
                                    tools::MessageController_ABC< sword::SimToClient_Content >& messageController )
    : subject_       ( subject )
    , handler_       ( handler )
    , publisher_     ( publisher )
    , factory_       ( factory )
    , resolver_      ( resolver )
    , componentTypes_( componentTypes )
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
    ComponentTypeVisitor< T_Agents > visitor( identifier, agentTypes_ );
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
    const T_Components& staticComponents = agentTypes_[ identifier ];
    for( int i = 0; i < message.equipment_dotations().elem_size(); ++i )
    {
        const sword::EquipmentDotations::EquipmentDotation& dotation = message.equipment_dotations().elem( i );
        std::string equipmentName;
        BOOST_FOREACH( const T_Components::value_type& staticComponent, staticComponents )
            if( staticComponent.second.first == dotation.type().id() )
                equipmentName = staticComponent.first;
        const T_Components::const_iterator remoteComponent = remoteComponents.find( equipmentName );
        mustSend = mustSend || ( remoteComponent != remoteComponents.end() && remoteComponent->second.second != dotation.available() );
    }
    if( mustSend )
        SendUpdate( identifier );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::Created
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::Created( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::Destroyed
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void EquipmentUpdater::Destroyed( const std::string& /*identifier*/ )
{
    // NOTHING
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
void EquipmentUpdater::EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number )
{
    const std::string equipmentName = resolver_.Resolve( equipmentType );
    T_Component& component = remoteAgents_[ identifier ][ equipmentName ];
    if( component.second == number )
        return;
    component.second = number;
    SendUpdate( identifier );
}

// -----------------------------------------------------------------------------
// Name: EquipmentUpdater::SendUpdate
// Created: SLI 2011-09-30
// -----------------------------------------------------------------------------
void EquipmentUpdater::SendUpdate( const std::string& identifier )
{
    T_Agents::const_iterator agentType = agentTypes_.find( identifier );
    T_Agents::const_iterator remoteAgent = remoteAgents_.find( identifier );
    if( agentType == agentTypes_.end() || remoteAgent == remoteAgents_.end() )
        return;
    const unsigned int agentIdentifier = identifiers_.left.at( identifier );
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_unit()->set_id( agentIdentifier );
    message().set_type( sword::UnitMagicAction::change_equipment_state );
    sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
    parameter.set_null_value( false );
    BOOST_FOREACH( const T_Components::value_type& component, agentType->second )
    {
        const std::string& componentTypeName = component.first;
        const unsigned int componentTypeIdentifier = component.second.first;
        const unsigned int componentStaticNumber = component.second.second;
        const T_Components::const_iterator remoteComponent = remoteAgent->second.find( componentTypeName );
        if( remoteComponent != remoteAgent->second.end() && remoteComponent->second.second <= componentStaticNumber )
        {
            const unsigned int remoteComponentAvailable = remoteComponent->second.second;
            const int remoteComponentDead = componentStaticNumber - remoteComponentAvailable;
            sword::MissionParameter_Value* componentChanged = parameter.add_value();
            componentChanged->mutable_list()->Add()->set_identifier( componentTypeIdentifier );
            componentChanged->mutable_list()->Add()->set_quantity( remoteComponentAvailable );
            componentChanged->mutable_list()->Add()->set_quantity( remoteComponentDead );
            componentChanged->mutable_list()->Add()->set_quantity( 0 );
            componentChanged->mutable_list()->Add()->set_quantity( 0 );
            componentChanged->mutable_list()->Add()->set_quantity( 0 );
            componentChanged->mutable_list()->Add()->set_quantity( 0 );
            componentChanged->mutable_list()->Add()->mutable_list();
        }
    }
    if( message().parameters().elem( 0 ).value_size() > 0 )
        message.Send( publisher_, factory_.Create() );
}
