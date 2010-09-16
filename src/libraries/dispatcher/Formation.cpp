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
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/clientsenders.h"
#include "protocol/SimulationSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::Formation( const Model_ABC& model, const Common::MsgFormationCreation& msg, const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& levels )
    : Formation_ABC( msg.formation().id(), QString(msg.name().c_str()) )
    , model_ ( model )
    , name_  ( msg.name() )
    , team_  ( model.Sides().Get( msg.party().id() ) )
    , level_ ( levels.Get( msg.level() ) )
    , parent_( msg.has_parent() ? &model.Formations().Get( msg.parent().id() ) : 0 )
{
    if( parent_ )
        parent_->Register( *this );
    else
        team_.Register( *this );
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
    message().set_name( name_ );
    message().set_level( Common::EnumNatureLevel( level_.GetId() ) );

    if( parent_ )
        message().mutable_parent()->set_id( parent_->GetId() );

    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Formation::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Formation::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Formation::SendDestruction( ClientPublisher_ABC& ) const
{
    throw std::runtime_error( __FUNCTION__ );
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
