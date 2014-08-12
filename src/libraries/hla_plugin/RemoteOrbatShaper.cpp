// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteOrbatShaper.h"
#include "RemoteAgentSubject_ABC.h"
#include "ContextHandler_ABC.h"
#include "SideResolver_ABC.h"
#include "HlaObject_ABC.h"
#include "UnitTypeResolver_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/KnowledgeGroup_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "protocol/SimulationSenders.h"
#include "rpr/EntityType.h"
#include <tools/Resolver_ABC.h>
#include <algorithm>
#include <set>
#include <boost/make_shared.hpp>

using namespace plugins::hla;

struct plugins::hla::RemoteOrbatShaper::UnitData
{
public:
    enum Kind { UNKNOWN, CREATION_PENDING, UNIT, AUTOMAT, FORMATION };
    UnitData() : kind( UNKNOWN ), hasTeam( false ), level( 6 ) {}

    void SetSimulationId( Kind k, unsigned long id ) { kind = k; simulationId = id; }
    unsigned long GetSimulationId() const { return simulationId; }
    Kind GetKind() const { return kind; }
    void SetTeam( unsigned long id ) { teamId = id; hasTeam = true; }
    unsigned long GetTeam() const { return teamId; }
    bool CanBeCreated() const
    {
        return kind == UNKNOWN &&
            hasTeam &&
            name.size() > 0;
    }
    void CreationSent() { kind = CREATION_PENDING; }

    void AddPendingChild( const std::string& childId ) { pendingChildren.insert( childId ); }
    void RemovePendingChild( const std::string& childId ) { pendingChildren.erase( childId ); }
    void GetPendingChildren( std::set< std::string>& children ) { std::swap( pendingChildren, children); }

    std::string parentRtiId;
    std::string name;
    int level;
    rpr::EntityType entityType;

private:
    Kind kind;
    unsigned long simulationId;
    bool hasTeam;
    unsigned long teamId;
    std::set< std::string > pendingChildren;
};

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper constructor
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
RemoteOrbatShaper::RemoteOrbatShaper( RemoteAgentSubject_ABC& agentSubject, ContextHandler_ABC< sword::FormationCreation >& formationCreation,
                                ContextHandler_ABC< sword::AutomatCreation >& automatCreation,
                                ContextHandler_ABC< sword::UnitCreation >& unitCreation, const SideResolver_ABC& sideResolver,
                                const tools::Resolver_ABC< dispatcher::KnowledgeGroup_ABC, unsigned long >& knowledgeGroups,
                                dispatcher::SimulationPublisher_ABC& publisher, const UnitTypeResolver_ABC& automatTypeResolver )
    : agentSubject_( agentSubject )
    , formationCreation_( formationCreation )
    , automatCreation_( automatCreation )
    , unitCreation_( unitCreation )
    , sideResolver_( sideResolver )
    , publisher_( publisher )
    , automatTypeResolver_( automatTypeResolver )
{
    agentSubject_.Register( *this );
    formationCreation_.Register( *this );
    automatCreation_.Register( *this );
    unitCreation_.Register( *this );

    for( tools::Iterator< const dispatcher::KnowledgeGroup_ABC& > it = knowledgeGroups.CreateIterator(); it.HasMoreElements(); )
    {
        const dispatcher::KnowledgeGroup_ABC& group = it.NextElement();
        party2KnowledgeGroups_[ group.GetTeam().GetId() ] = group.GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper destructor
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
RemoteOrbatShaper::~RemoteOrbatShaper()
{
    agentSubject_.Unregister( *this );
    formationCreation_.Unregister( *this );
    automatCreation_.Unregister( *this );
    unitCreation_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::Created
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::RemoteCreated(  const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object )
{
    units_.insert( T_UnitsData::value_type( identifier, boost::make_shared< UnitData >() ) );
    object.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::Destroyed
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::RemoteDestroyed( const std::string& identifier )
{
    T_UnitsData::iterator it( units_.find( identifier ) );
    if( units_.end() == it )
        return;
    units_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::Moved
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::Moved( const std::string& /*identifier*/, double /*latitude*/, double /*longitude*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::SideChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::SideChanged( const std::string& identifier, rpr::ForceIdentifier side )
{
    T_UnitsData::const_iterator it( units_.find( identifier ) );
    if( units_.end() == it )
        return;
    it->second->SetTeam( sideResolver_.ResolveTeam( side ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::NameChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::NameChanged( const std::string& identifier, const std::string& name )
{
    T_UnitsData::const_iterator it( units_.find( identifier ) );
    if( units_.end() == it )
        return;
    it->second->name = name;
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::TypeChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::TypeChanged( const std::string& identifier, const rpr::EntityType& type )
{
    T_UnitsData::const_iterator it( units_.find( identifier ) );
    if( units_.end() == it )
        return;
    it->second->entityType = type;
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::EquipmentUpdated
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*available*/,
        unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::UniqueIdChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::UniqueIdChanged( const std::string& /*identifier*/, const T_UniqueId& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::CallsignChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::CallsignChanged( const std::string& identifier, const std::string& callsign )
{
    T_UnitsData::const_iterator it( units_.find( identifier ) );
    if( units_.end() == it )
        return;
    it->second->name = callsign;
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::ParentChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId )
{
    T_UnitsData::const_iterator it( units_.find( rtiIdentifier ) );
    if( units_.end() == it )
        return;

    it->second->parentRtiId = parentRtiId;
    CreateParent( rtiIdentifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::Notify
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::Notify( const sword::FormationCreation& message, const std::string& identifier )
{
    if( identifier == "default_remote_formation" )
    {
        party2Formations_.insert( T_PartyMap::value_type( message.party().id(), message.formation().id() ) );
    }
    else
    {
        T_UnitsData::const_iterator it( units_.find( identifier ) );
        if( units_.end() == it )
            return;

        it->second->SetSimulationId( UnitData::FORMATION, message.formation().id() );
        CreateParent( identifier );
        MoveChild( identifier );
        std::set< std::string > pendings;
        it->second->GetPendingChildren( pendings );
        std::for_each( pendings.begin(), pendings.end(), [&](const std::string& v)
            {
                MoveChild( v );
            });
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::Notify
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::Notify( const sword::AutomatCreation& message, const std::string& identifier )
{
    if( identifier == "default_remote_automat" )
        return;

    T_UnitsData::const_iterator it( units_.find( identifier ) );
    if( units_.end() == it )
        return;

    it->second->SetSimulationId( UnitData::AUTOMAT, message.automat().id() );
    CreateParent( identifier );
    MoveChild( identifier );
    std::set< std::string > pendings;
    it->second->GetPendingChildren( pendings );
    std::for_each( pendings.begin(), pendings.end(), [&](const std::string& v)
        {
            MoveChild( v );
        });
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::Notify
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::Notify( const sword::UnitCreation& message, const std::string& identifier )
{
    T_UnitsData::const_iterator it( units_.find( identifier ) );
    if( units_.end() == it )
        return;

    it->second->SetSimulationId( UnitData::UNIT, message.unit().id() );
    it->second->level = 6;
    CreateParent( identifier );
    MoveChild( identifier );
}

namespace
{
    void CreateAutomat( ContextHandler_ABC< sword::AutomatCreation >& automatCreation, unsigned long formation,
        unsigned long knowledgeGroup, unsigned long automatType, const std::string& name, const std::string& rtiId )
    {
        simulation::UnitMagicAction automatCreationMessage;
        automatCreationMessage().mutable_tasker()->mutable_formation()->set_id( formation );            // parent
        automatCreationMessage().set_type( sword::UnitMagicAction::automat_creation );
        automatCreationMessage().mutable_parameters()->add_elem()->add_value()->set_identifier( automatType );        // type
        automatCreationMessage().mutable_parameters()->add_elem()->add_value()->set_identifier(knowledgeGroup );      // knowledge group
        automatCreationMessage().mutable_parameters()->add_elem()->add_value()->set_acharstr( name ); // name
        sword::Extension* ext = automatCreationMessage().mutable_parameters()->add_elem()->add_value()->mutable_extensionlist();
        sword::Extension_Entry* entry = ext->add_entries(); // extension
        entry->set_name("RemoteEntity");
        entry->set_value("true");
        entry = ext->add_entries(); // extension
        entry->set_name( "HLA_ObjectID" );
        entry->set_value( rtiId );
        automatCreation.Send( automatCreationMessage, rtiId );
    }
    void CreateFormation( ContextHandler_ABC< sword::FormationCreation >& formationCreation, unsigned long side,
        int level, const std::string& name, const std::string& rtiId )
    {
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_party()->set_id( side );
        message().set_type( sword::UnitMagicAction::formation_creation );
        message().mutable_parameters()->add_elem()->add_value()->set_areal( level*1.0f );                          // hierarchy level
        message().mutable_parameters()->add_elem()->add_value()->set_acharstr( name ); // name
        message().mutable_parameters()->add_elem()->set_null_value( true );                               // logistic level
        sword::Extension* ext = message().mutable_parameters()->add_elem()->add_value()->mutable_extensionlist();
        sword::Extension_Entry* entry = ext->add_entries(); // extension
        entry->set_name("RemoteEntity");
        entry->set_value("true");
        entry = ext->add_entries(); // extension
        entry->set_name( "HLA_ObjectID" );
        entry->set_value( rtiId );
        formationCreation.Send( message, rtiId );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::CreateParent
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::CreateParent( const std::string& childId )
{
    T_UnitsData::const_iterator childIt( units_.find( childId ) );
    if( childIt->second->GetKind() == UnitData::UNKNOWN || childIt->second->parentRtiId.size() == 0 )
        return;

    const std::string& parentId = childIt->second->parentRtiId;
    T_UnitsData::const_iterator parentIt( units_.find( parentId ) );
    if( units_.end() == parentIt )
        return;

    if( !parentIt->second->CanBeCreated() )
        return;

    parentIt->second->level = childIt->second->level - 1;
    switch( childIt->second->GetKind() )
    {
    case UnitData::UNIT:
        CreateAutomat( automatCreation_, party2Formations_[ childIt->second->GetTeam() ], party2KnowledgeGroups_[ childIt->second->GetTeam() ],
                       automatTypeResolver_.Resolve( parentIt->second->entityType ), parentIt->second->name, parentId );
        parentIt->second->CreationSent();
        break;
    case UnitData::AUTOMAT:
        CreateFormation( formationCreation_, parentIt->second->GetTeam(), parentIt->second->level, parentIt->second->name, parentId );
        parentIt->second->CreationSent();
        break;
    case UnitData::FORMATION:
        CreateFormation( formationCreation_, parentIt->second->GetTeam(), parentIt->second->level, parentIt->second->name, parentId );
        parentIt->second->CreationSent();
        break;
    }
}

namespace
{
    void MoveUnit( dispatcher::SimulationPublisher_ABC& publisher, unsigned long childId, unsigned long parentId )
    {
        simulation::UnitMagicAction message;
        message().set_type( sword::UnitMagicAction::unit_change_superior );
        message().mutable_tasker()->mutable_unit()->set_id( childId );
        message().mutable_parameters()->add_elem()->add_value()->mutable_automat()->set_id( parentId );
        message.Send( publisher );
    }
    void MoveAutomat( dispatcher::SimulationPublisher_ABC& publisher, unsigned long childId, unsigned long parentId )
    {
        simulation::UnitMagicAction message;
        message().set_type( sword::UnitMagicAction::change_formation_superior );
        message().mutable_tasker()->mutable_automat()->set_id( childId );
        message().mutable_parameters()->add_elem()->add_value()->mutable_formation()->set_id( parentId );
        message.Send( publisher );
    }
    void MoveFormation( dispatcher::SimulationPublisher_ABC& publisher, unsigned long childId, unsigned long parentId )
    {
        simulation::UnitMagicAction message;
        message().set_type( sword::UnitMagicAction::change_formation_superior );
        message().mutable_tasker()->mutable_formation()->set_id( childId );
        message().mutable_parameters()->add_elem()->add_value()->mutable_formation()->set_id( parentId );
        message.Send( publisher );
    }
}
// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::MoveChild
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::MoveChild( const std::string& childId )
{
    T_UnitsData::const_iterator childIt( units_.find( childId ) );
    if( units_.end() == childIt ||
        childIt->second->GetKind() == UnitData::UNKNOWN || childIt->second->GetKind() == UnitData::CREATION_PENDING ||
        childIt->second->parentRtiId.size() == 0 )
        return;

    const std::string& parentId = childIt->second->parentRtiId;
    T_UnitsData::const_iterator parentIt( units_.find( parentId ) );
    if( units_.end() == parentIt)
        return;

    if( parentIt->second->GetKind() == UnitData::UNKNOWN  || parentIt->second->GetKind() == UnitData::CREATION_PENDING )
    {
        parentIt->second->AddPendingChild( childId );
        return;
    }

    switch( childIt->second->GetKind() )
    {
    case UnitData::UNIT:
        MoveUnit( publisher_, childIt->second->GetSimulationId(), parentIt->second->GetSimulationId() );
        break;
    case UnitData::AUTOMAT:
        MoveAutomat( publisher_, childIt->second->GetSimulationId(), parentIt->second->GetSimulationId() );
        break;
    case UnitData::FORMATION:
        MoveFormation( publisher_, childIt->second->GetSimulationId(), parentIt->second->GetSimulationId() );
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::SubAgregatesChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::SubAgregatesChanged( const std::string& rtiIdentifier, const std::set< std::string >& children )
{
    for( auto chIt=children.begin(); children.end() != chIt; ++chIt )
    {
        const std::string& childRtiIdentifier = *chIt;
        T_UnitsData::const_iterator it( units_.find( childRtiIdentifier ) );
        if( units_.end() == it )
            return;

        it->second->parentRtiId = rtiIdentifier;
        CreateParent( childRtiIdentifier );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::SubEntitiesChanged
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::SubEntitiesChanged(const std::string& /*rtiIdentifier*/, const std::set< std::string >& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::LocalCreated
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::LocalCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::LocalDestroyed
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::LocalDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::Divested
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::Divested( const std::string& /*identifier*/, const T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::Acquired
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::Acquired( const std::string& /*identifier*/, const T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::EmbeddedUnitListChanged
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< T_UniqueId >& /*embeddedUnits*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::GeometryChanged
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::GeometryChanged( const std::string& /*identifier*/, const std::vector< rpr::WorldLocation >& /*perimeter*/, ObjectListener_ABC::GeometryType /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteOrbatShaper::PropagationChanged
// Created: AHC 2013-07-10
// -----------------------------------------------------------------------------
void RemoteOrbatShaper::PropagationChanged( const std::string& /*rtiIdentifier*/, const std::vector< ObjectListener_ABC::PropagationData >& /*data*/,
                int /*col*/, int /*lig*/, double /*xll*/, double /*yll*/, double /*dx*/, double /*dy*/ )
{
    // NOTHING
}
