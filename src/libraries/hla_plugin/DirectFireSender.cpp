// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "DirectFireSender.h"
#include "Omt13String.h"
#include "Interactions.h"
#include "InteractionSender_ABC.h"
#include "HlaObject_ABC.h"
#include "RemoteAgentResolver_ABC.h"
#include "RemoteAgentSubject_ABC.h"
#include "LocalAgentResolver_ABC.h"
#include "DotationTypeResolver_ABC.h"
#include "AgentSubject_ABC.h"
#include "ChildListener.h"
#include "Agent_ABC.h"
#include "protocol/Simulation.h"
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DirectFireSender constructor
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
DirectFireSender::DirectFireSender( InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender,
                                    InteractionSender_ABC< interactions::WeaponFire >& weaponFireSender,
                                    const RemoteAgentResolver_ABC& remoteResolver, const LocalAgentResolver_ABC& localResolver,
                                    RemoteAgentSubject_ABC& remoteAgentSubject, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                    const std::string& federateName, const DotationTypeResolver_ABC& munitionTypeResolver, AgentSubject_ABC& agentSubject )
    : interactionSender_ ( interactionSender )
    , weaponFireSender_  ( weaponFireSender )
    , remoteResolver_    ( remoteResolver )
    , localResolver_     ( localResolver )
    , remoteAgentSubject_( remoteAgentSubject )
    , federateName_      ( federateName )
    , munitionTypeResolver_( munitionTypeResolver )
    , localAgentSubject_ ( agentSubject )
{
    CONNECT( controller, *this, start_unit_fire );
    CONNECT( controller, *this, stop_unit_fire );
    remoteAgentSubject_.Register( *this );
    localAgentSubject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender destructor
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
DirectFireSender::~DirectFireSender()
{
    localAgentSubject_.Unregister( *this );
    remoteAgentSubject_.Unregister( *this );
    std::for_each( listeners_.begin(), listeners_.end(), [&](T_LocalListeners::const_reference v)
        {
            v.second.second->Unregister( *v.second.first );
        });
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Notify
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Notify( const sword::StartUnitFire& message, int /*context*/ )
{
    if( message.type() == sword::StartUnitFire::direct )
        fires_[ message.fire().id() ] = message;
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Notify
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Notify( const sword::StopUnitFire& message, int /*context*/ )
{
    const unsigned int fireIdentifier = message.fire().id();
    if( fires_.find( fireIdentifier ) == fires_.end() )
        return;
    const sword::StartUnitFire& startMessage = fires_[ fireIdentifier ];
    std::string targetIdentifier = remoteResolver_.Resolve( startMessage.target().unit().id() );
    if( targetIdentifier.empty() )
    {
        targetIdentifier = localResolver_.Resolve( startMessage.target().unit().id() );
        if( targetIdentifier.empty() )
        {
            fires_.erase( fireIdentifier );
            return;
        }
        ComputeChildrenRtiIds( startMessage.target().unit().id() );
    }
    std::string firingRtiId( localResolver_.Resolve( startMessage.firing_unit().id() ) );
    interactions::MunitionDetonation parameters;
    parameters.articulatedPartData.clear();  // empty array
    parameters.detonationLocation = positions_[ targetIdentifier ];
    parameters.detonationResultCode = 1; // EntityImpact
    parameters.eventIdentifier.eventCount = static_cast< uint16_t >( fireIdentifier );
    parameters.eventIdentifier.issuingObjectIdentifier = Omt13String( firingRtiId );
    parameters.firingObjectIdentifier = Omt13String( firingRtiId );
    parameters.finalVelocityVector = rpr::VelocityVector( 0., 0., 700. ); // $$$$ _RC_ SLI 2011-09-23: Hardcoded
    parameters.fuseType = 0; // Other
    parameters.munitionObjectIdentifier = Omt13String();
    parameters.munitionType = startMessage.has_ammunition() ? 
                                munitionTypeResolver_.Resolve( startMessage.ammunition().id() ) : 
                                rpr::EntityType( "2 8 71 2 10" ); // 12.7mm, Hardcoded
    parameters.quantityFired = 10; // Hardcoded
    parameters.rateOfFire = 40; // Hardcoded
    parameters.relativeDetonationLocation = rpr::VelocityVector(); // Entity location
    parameters.targetObjectIdentifier = Omt13String( targetIdentifier );
    parameters.warheadType = 0; // Other
    interactionSender_.Send( parameters );
    // process platforms
    DoPlatformsFire( fireIdentifier, firingRtiId, startMessage.firing_unit().id(), targetIdentifier, parameters );
    // tidy up
    fires_.erase( fireIdentifier );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::RemoteCreated
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::RemoteCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object)
{
    object.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::RemoteDestroyed
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::RemoteDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Moved
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Moved( const std::string& identifier, double latitude, double longitude )
{
    positions_[ identifier ] = rpr::WorldLocation( latitude, longitude, 0. );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::SideChanged
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::NameChanged
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::TypeChanged
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::EquipmentUpdated
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void DirectFireSender::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*available*/,
                    unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::UniqueIdChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void DirectFireSender::UniqueIdChanged( const std::string& /*identifier*/, const T_UniqueId& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::CallsignChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void DirectFireSender::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::LocalCreated
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void DirectFireSender::LocalCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::LocalDestroyed
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void DirectFireSender::LocalDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Divested
// Created: AHC 2010-03-02
// -----------------------------------------------------------------------------
void DirectFireSender::Divested( const std::string& /*identifier*/, const T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void DirectFireSender::Acquired( const std::string& /*identifier*/, const T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::EmbeddedUnitListChanged
// Created: AHC 2010-05-29
// -----------------------------------------------------------------------------
void DirectFireSender::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< T_UniqueId >& /*units*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::GeometryChanged( const std::string& /*identifier*/, const std::vector< rpr::WorldLocation >& /*points*/, ObjectListener_ABC::GeometryType /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::ParentChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::ParentChanged( const std::string& /*rtiIdentifier*/, const std::string& /*parentRtiId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::SubAgregatesChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::SubAgregatesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::SubEntitiesChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::SubEntitiesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children )
{
    childrenRtiIds_[ rtiIdentifier ] = children;
}

namespace
{
    void UpdateLocation( std::map< std::string, rpr::WorldLocation >& positions, unsigned long id, const LocalAgentResolver_ABC& localResolver, const ChildListener& child )
    {
        const ChildListener::LocationStruct& loc = child.GetLocation();
        std::string name = localResolver.Resolve( id );
        if( !name.empty() )
            positions[ name ]  = rpr::WorldLocation( loc.latitude, loc.longitude, loc.altitude );
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::PlatformCreated
// Created: AHC 2012-11-14
// -----------------------------------------------------------------------------
void DirectFireSender::AggregateCreated( Agent_ABC& agent, unsigned long identifier, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/, const std::string& /*symbol*/, bool /*isLocal*/, const std::vector< char >& /*uniqueId*/ )
{
    boost::shared_ptr< ChildListener > child( new ChildListener( boost::bind( &UpdateLocation, boost::ref( positions_ ), identifier, boost::cref( localResolver_ ), _1 ) ) );
    listeners_[ identifier ] = std::make_pair( child, &agent );
    agent.Register( *child );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::PlatformCreated
// Created: AHC 2012-11-14
// -----------------------------------------------------------------------------
void DirectFireSender::PlatformCreated( Agent_ABC& agent, unsigned int identifier, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/, const std::string& /*symbol*/, const std::vector< char >& /*uniqueId*/ )
{
    boost::shared_ptr< ChildListener > child( new ChildListener( boost::bind( &UpdateLocation, boost::ref( positions_ ), identifier, boost::cref( localResolver_ ), _1 ) ) );
    listeners_[ identifier ] = std::make_pair( child, &agent );
    agent.Register( *child );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::ComputeChildrenRtiId
// Created: AHC 2012-11-15
// -----------------------------------------------------------------------------
void DirectFireSender::ComputeChildrenRtiIds( unsigned int parentSimId )
{
    std::string parentRtiId = localResolver_.Resolve( parentSimId );
    T_LocalListeners::const_iterator itTargetListener( listeners_.find( parentSimId ) );
    if( itTargetListener != listeners_.end() && 
        itTargetListener->second.first.get() != 0 && itTargetListener->second.first->GetPlatforms().size() !=0 )
    {
        std::set< std::string > rtiIDs;
        const std::set< unsigned int >& platforms = itTargetListener->second.first->GetPlatforms();
        std::for_each( platforms.begin(), platforms.end(), [&](unsigned int id)
        {
            std::string rtiId = localResolver_.Resolve( id );
            if( !rtiId.empty() )
                rtiIDs.insert( rtiId );
        });
        childrenRtiIds_[ parentRtiId ] = rtiIDs;
    }
}

namespace
{
    struct ChildrenDistributor
    {
        ChildrenDistributor( const std::set< std::string >& children )
            : children_( children )
            , it_( children_.begin() )
        {
            assert( !children_.empty() );
        }
        ChildrenDistributor( const ChildrenDistributor& rhs )
            : children_( rhs.children_ )
            , it_( children_.begin() )
        {}
        const ChildrenDistributor& operator =( const ChildrenDistributor& rhs ); // not implemented
        Omt13String operator()()
        {
            Omt13String retval( *it_ );
            ++it_;
            if( it_ == children_.end() )
                it_ = children_.begin();
            return retval;
        }
        const std::set< std::string >& children_;
        std::set< std::string >::const_iterator it_;
    };
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::ComputeChildrenRtiId
// Created: AHC 2012-11-15
// -----------------------------------------------------------------------------
void DirectFireSender::DoPlatformsFire( unsigned long fireIdentifier, const std::string& firingRtiId, unsigned long firingSimId, 
        const std::string& targetIdentifier, const interactions::MunitionDetonation& parentDeto )
{
    interactions::MunitionDetonation childDeto = parentDeto;
    
    T_LocalListeners::const_iterator itFiringListener( listeners_.find( firingSimId ) );
    if( itFiringListener == listeners_.end() || itFiringListener->second.first->GetPlatforms().empty() )
        return;
    boost::shared_ptr< ChildListener > firingL( itFiringListener->second.first );

    interactions::WeaponFire fire;
    fire.eventIdentifier.eventCount = childDeto.eventIdentifier.eventCount;
    fire.fireControlSolutionRange = 0.f;
    fire.fireMissionIndex = fireIdentifier;
    fire.fuseType = childDeto.fuseType;
    fire.initialVelocityVector = rpr::VelocityVector( 0., 0., 700. ); // $$$$ _RC_ SLI 2011-09-23: Hardcoded
    fire.munitionObjectIdentifier = childDeto.munitionObjectIdentifier;
    fire.munitionType = childDeto.munitionType;
    fire.quantityFired = childDeto.quantityFired;
    fire.rateOfFire = childDeto.rateOfFire;
    fire.warheadType = childDeto.warheadType;

    // distribute targets
    T_ChildrenRtiIds::const_iterator itTargetChildren( childrenRtiIds_.find( targetIdentifier ) );
    std::vector< Omt13String > targetIdentifiers( firingL->GetPlatforms().size(), Omt13String( targetIdentifier ) );
    if( itTargetChildren != childrenRtiIds_.end() && itTargetChildren->second.size() != 0 )
        std::generate( targetIdentifiers.begin(), targetIdentifiers.end(), ChildrenDistributor( itTargetChildren->second ) );
        
    std::size_t targetIndex = 0;
    std::for_each( firingL->GetPlatforms().begin(), firingL->GetPlatforms().end(), [&](unsigned int chId)
        {
            const Omt13String& childTgtIdentifier = targetIdentifiers[ targetIndex ];
            std::string chRtiId( localResolver_.Resolve( chId ) );
            if( !chRtiId.empty() )
            {
                // WeaponFire
                fire.firingObjectIdentifier = Omt13String( chRtiId );
                fire.eventIdentifier.issuingObjectIdentifier = Omt13String( chRtiId );
                fire.targetObjectIdentifier = childTgtIdentifier;
                fire.firingLocation = positions_.find( chRtiId ) != positions_.end() ? positions_[ chRtiId ] : positions_[ firingRtiId ];
                weaponFireSender_.Send( fire );
                // Munition Detonation
                childDeto.eventIdentifier.issuingObjectIdentifier = Omt13String( chRtiId );
                childDeto.firingObjectIdentifier = Omt13String( chRtiId );
                childDeto.targetObjectIdentifier = childTgtIdentifier;
                childDeto.detonationLocation = positions_.find( childTgtIdentifier.str() )  != positions_.end() ?
                                                positions_[ childTgtIdentifier.str() ] : parentDeto.detonationLocation;
                interactionSender_.Send( childDeto );
            }
            ++targetIndex;
        });
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::PropagationChanged
// Created: AHC 2013-07-10
// -----------------------------------------------------------------------------
void DirectFireSender::PropagationChanged( const std::string& /*rtiIdentifier*/, const std::vector< ObjectListener_ABC::PropagationData >& /*data*/,
                int /*col*/, int /*lig*/, double /*xll*/, double /*yll*/, double /*dx*/, double /*dy*/ )
{
    // NOTHING
}
