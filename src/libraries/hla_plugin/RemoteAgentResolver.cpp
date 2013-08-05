// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteAgentResolver.h"
#include "RemoteAgentSubject_ABC.h"
#include "ContextHandler_ABC.h"
#include "HlaObject_ABC.h"
#include "protocol/Simulation.h"
#include <algorithm>

using namespace plugins::hla;

namespace
{
    static const std::string DEFAULT_ID("");
}
// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver constructor
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
RemoteAgentResolver::RemoteAgentResolver( RemoteAgentSubject_ABC& subject, ContextHandler_ABC< sword::UnitCreation >& unitCreation )
    : subject_     ( subject )
    , unitCreation_( unitCreation )
{
    subject_.Register( *this );
    unitCreation_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver destructor
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
RemoteAgentResolver::~RemoteAgentResolver()
{
    unitCreation_.Unregister( *this );
    subject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::Resolve
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
const std::string& RemoteAgentResolver::Resolve( unsigned int identifier ) const
{
    T_Identifiers::left_const_iterator it = identifiers_.left.find( identifier );
    if( it == identifiers_.left.end() )
        return DEFAULT_ID;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::Resolve
// Created: SLI 2011-09-26
// -----------------------------------------------------------------------------
unsigned int RemoteAgentResolver::Resolve( const std::string& identifier ) const
{
    unsigned int retval = 0;
    T_Identifiers::right_const_iterator it = identifiers_.right.find( identifier );
    if( it == identifiers_.right.end() ) // look in children
    {
        T_Children::const_iterator itCh( children_.find( identifier ) );
        if( itCh != children_.end() )
        {
            it = identifiers_.right.find( itCh->second );
        }
    }
    if( it != identifiers_.right.end() )
        retval = it->second;
    return retval;
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::RemoteCreated
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void RemoteAgentResolver::RemoteCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object )
{
    pendings_.insert( identifier );
    object.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::RemoteDestroyed
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void RemoteAgentResolver::RemoteDestroyed( const std::string& identifier )
{
    identifiers_.right.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::Moved
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void RemoteAgentResolver::Moved( const std::string& /*identifier*/, double /*latitude*/, double /*longitude*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::SideChanged
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void RemoteAgentResolver::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::NameChanged
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void RemoteAgentResolver::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::TypeChanged
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void RemoteAgentResolver::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::EquipmentUpdated
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void RemoteAgentResolver::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*available*/,
                    unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::UniqueIdChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void RemoteAgentResolver::UniqueIdChanged( const std::string& /*identifier*/, const T_UniqueId& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::CallsignChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void RemoteAgentResolver::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::Notify
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
void RemoteAgentResolver::Notify( const sword::UnitCreation& message, const std::string& identifier )
{
    if( pendings_.find( identifier ) == pendings_.end() )
        return;
    identifiers_.insert( T_Identifiers::value_type( message.unit().id(), identifier ) );
    pendings_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::LocalCreated
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void RemoteAgentResolver::LocalCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::LocalDestroyed
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void RemoteAgentResolver::LocalDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::Divested
// Created: AHC 2010-03-02
// -----------------------------------------------------------------------------
void RemoteAgentResolver::Divested( const std::string& /*identifier*/, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void RemoteAgentResolver::Acquired( const std::string& /*identifier*/, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::EmbeddedUnitListChanged
// Created: AHC 2010-05-29
// -----------------------------------------------------------------------------
void RemoteAgentResolver::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< T_UniqueId >& /*units*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void RemoteAgentResolver::GeometryChanged( const std::string& /*identifier*/, const std::vector< rpr::WorldLocation >& /*perimeter*/, ObjectListener_ABC::GeometryType /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::ParentChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteAgentResolver::ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId )
{
    children_[ rtiIdentifier ] = parentRtiId;
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::SubAgregatesChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteAgentResolver::SubAgregatesChanged( const std::string& rtiIdentifier, const std::set< std::string >& children )
{
    std::for_each( children.begin(), children.end(), [&](const std::string& v)
        {
            children_[ v ] = rtiIdentifier;
        });
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::SubEntitiesChanged
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void RemoteAgentResolver::SubEntitiesChanged(const std::string& rtiIdentifier, const std::set< std::string >& children )
{
    std::for_each( children.begin(), children.end(), [&](const std::string& v)
        {
            children_[ v ] = rtiIdentifier;
        });
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentResolver::PropagationChanged
// Created: AHC 2013-07-10
// -----------------------------------------------------------------------------
void RemoteAgentResolver::PropagationChanged( const std::string& /*rtiIdentifier*/, const std::vector< ObjectListener_ABC::PropagationData >& /*data*/,
                int /*col*/, int /*lig*/, double /*xll*/, double /*yll*/, double /*dx*/, double /*dy*/ )
{
    // NOTHING
}
