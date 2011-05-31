// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Formation.h"
#include "Model_ABC.h"
#include "Automat_ABC.h"
#include "Team_ABC.h"
#include "LogisticEntity.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::Formation( const Model_ABC& model, const sword::FormationCreation& msg, const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& levels )
    : Formation_ABC  ( msg.formation().id(), QString( msg.name().c_str() ) )
    , model_         ( model )
    , name_          ( msg.name() )
    , team_          ( model.Sides().Get( msg.party().id() ) )
    , level_         ( levels.Get( msg.level() ) )
    , symbol_        ( msg.app6symbol() )
    , logisticEntity_( 0 )
    , parent_        ( msg.has_parent() ? &model.Formations().Get( msg.parent().id() ) : 0 )
{
    if( parent_ )
        parent_->Register( *this );
    else
        team_.Register( *this );

    if( msg.logistic_level() != sword::none )
    {
        logisticEntity_.reset( new LogisticEntity( *this, model.Formations(), model.Automats(), kernel::LogisticLevel::Resolve(  msg.logistic_level() ) ) );
        RegisterSelf( *logisticEntity_ );
        RegisterSelf( logisticEntity_->GetLogisticHierarchy() );
    }
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    if( parent_ )
    {
        MoveChildren( *parent_, formations_ );
        MoveChildren( *parent_, automats_ );
        parent_->Remove( *this );
    }
    else
    {
        MoveChildren( team_, formations_ );
        // at this point, automats should be already deleted
        team_.Remove( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Formation::MoveChildren
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
template< typename Superior, typename Entity >
void Formation::MoveChildren( Superior& superior, tools::Resolver< Entity >& entities )
{
    tools::Iterator< const Entity& > it( entities.CreateIterator() );
    while( it.HasMoreElements() )
    {
        Entity& entity = const_cast< Entity& >( it.NextElement() );
        entity.SetSuperior( superior );
    }
    entities.Clear();
}

// -----------------------------------------------------------------------------
// Name: Formation::SetSuperior
// Created: SBO 2010-06-14
// -----------------------------------------------------------------------------
void Formation::SetSuperior( dispatcher::Formation_ABC& superior )
{
    if( parent_ )
        parent_->Remove( *this );
    else
        team_.Remove( *this );
    parent_ = &superior;
    parent_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation::SetSuperior
// Created: SBO 2010-06-14
// -----------------------------------------------------------------------------
void Formation::SetSuperior( dispatcher::Team_ABC& /*superior*/ )
{
    if( parent_ )
    {
        parent_->Remove( *this );
        parent_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Formation::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Formation::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::FormationCreation message;
    message().mutable_formation()->set_id( GetId() );
    message().mutable_party()->set_id( team_.GetId() );
    message().set_level( sword::EnumNatureLevel( level_.GetId() ) );
    message().set_name( name_ );
    message().set_app6symbol( symbol_ );
    if( logisticEntity_.get() )
        logisticEntity_->Send( message() );
    else
        message().set_logistic_level( sword::none );

    if( parent_ )
        message().mutable_parent()->set_id( parent_->GetId() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Formation::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Formation::SendFullUpdate( ClientPublisher_ABC& publisher) const
{
    {
        client::FormationUpdate asn;
        asn().mutable_formation()->set_id( GetId() );
        for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it !=  extensions_.end(); ++it )
        {
            sword::Extension_Entry* entry = asn().mutable_extension()->add_entries();
            entry->set_name( it->first );
            entry->set_value( it->second );
        }
        asn.Send( publisher );
    }
    if( logisticEntity_.get() )
        logisticEntity_->SendFullUpdate( publisher );
}

// -----------------------------------------------------------------------------
// Name: Formation::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Formation::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::FormationDestruction asn;
    asn().mutable_formation()->set_id( GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Formation::Send
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
void Formation::Send( sword::ParentEntity& msg ) const
{
    msg.mutable_formation()->set_id( GetId() );
}

// -----------------------------------------------------------------------------
// Name: Formation::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Formation::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    formations_.Apply( boost::bind( &dispatcher::Formation_ABC::Accept, _1, boost::ref( visitor ) ) );
    automats_.Apply( boost::bind( &dispatcher::Automat_ABC::Accept, _1, boost::ref( visitor ) ) );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::HierarchyLevel_ABC& Formation::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
dispatcher::Formation_ABC* Formation::GetParent() const
{
    return parent_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
dispatcher::Team_ABC& Formation::GetTeam() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Formation::Register( dispatcher::Formation_ABC& formation )
{
    formations_.Register( formation.GetId(), formation );
}
// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Formation::Remove( dispatcher::Formation_ABC& formation )
{
    formations_.Remove( formation.GetId() );
}
// -----------------------------------------------------------------------------
// Name: Formation::GetFormations
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Formation_ABC >& Formation::GetFormations() const
{
    return formations_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Register( dispatcher::Automat_ABC& automat )
{
    automats_.Register( automat.GetId(), automat );
}
// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Remove( dispatcher::Automat_ABC& automat )
{
    automats_.Remove( automat.GetId() );
}
// -----------------------------------------------------------------------------
// Name: Formation::GetAutomates
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Automat_ABC >& Formation::GetAutomates() const
{
    return automats_;
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLogisticEntity
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
LogisticEntity* Formation::GetLogisticEntity() const
{
    return logisticEntity_.get();
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLogisticLevel
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
const kernel::LogisticLevel& Formation::GetLogisticLevel() const
{
    if( logisticEntity_.get() )
        return logisticEntity_->GetLogisticLevel();
    return kernel::LogisticLevel::none_;
}

// -----------------------------------------------------------------------------
// Name: Formation::DoUpdate
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void Formation::DoUpdate( const sword::FormationUpdate& msg )
{
    if( msg.has_extension() )
        for( int i = 0; i < msg.extension().entries_size(); ++i )
            extensions_[ msg.extension().entries( i ).name() ] = msg.extension().entries( i ).value();
}
