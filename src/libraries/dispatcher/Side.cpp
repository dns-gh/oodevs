// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Side.h"
#include "Formation_ABC.h"
#include "Inhabitant_ABC.h"
#include "KnowledgeGroup_ABC.h"
#include "Model.h"
#include "Object_ABC.h"
#include "Population_ABC.h"
#include "Inhabitant_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Side::Side( const Model_ABC& model, const sword::PartyCreation& msg )
    : Team_ABC( msg.party().id(), QString( msg.name().c_str() ) )
    , model_( model )
    , nType_( msg.type() )
{
    switch( nType_ )
    {
        case sword::unknown_diplo: karma_ = kernel::Karma::unknown_; break;
        case sword::friend_diplo : karma_ = kernel::Karma::friend_; break;
        case sword::enemy_diplo  : karma_ = kernel::Karma::enemy_; break;
        case sword::neutral_diplo: karma_ = kernel::Karma::neutral_; break;
    }
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: Side destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Side::~Side()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Side::DoUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Side::DoUpdate( const sword::ChangeDiplomacy& asnMsg )
{
    const kernel::Team_ABC& side = model_.Sides().Get( asnMsg.party2().id() );
    diplomacies_[ &side ] = asnMsg.diplomatie();
}

// -----------------------------------------------------------------------------
// Name: Side::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Side::DoUpdate( const sword::ChangeDiplomacyAck& asnMsg )
{
    const kernel::Team_ABC& side = model_.Sides().Get( asnMsg.party2().id() );
    diplomacies_[ &side ] = asnMsg.diplomatie();
}

// -----------------------------------------------------------------------------
// Name: Side::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Side::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PartyCreation asn;
    asn().mutable_party()->set_id( GetId() );
    asn().set_name( GetName() );
    asn().set_type( nType_ );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Side::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Side::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    for( T_Diplomacies::const_iterator it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        client::ChangeDiplomacy asn;
        asn().mutable_party1()->set_id( GetId() );
        asn().mutable_party2()->set_id( it->first->GetId() );
        asn().set_diplomatie( it->second );
        asn.Send( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: Side::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Side::SendDestruction( ClientPublisher_ABC& ) const
{
    throw std::runtime_error( __FUNCTION__ );
}

namespace
{
    template< typename C >
    void VisitorAdapter( kernel::ModelVisitor_ABC& visitor, kernel::Entity_ABC& entity )
    {
        static_cast< C& >( entity ).Accept( visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: Side::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Side::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    knowledgeGroups_.Apply( boost::bind( &dispatcher::KnowledgeGroup_ABC::Accept, _1, boost::ref( visitor ) ) );
    formations_.Apply( boost::bind( &dispatcher::Formation_ABC::Accept, _1, boost::ref( visitor ) ) );
    objects_.Apply( boost::bind( &dispatcher::Object_ABC::Accept, _1, boost::ref( visitor ) ) );
    populations_.Apply( boost::bind( &dispatcher::Population_ABC::Accept, _1, boost::ref( visitor ) ) );
    inhabitants_.Apply( boost::bind( &dispatcher::Inhabitant_ABC::Accept, _1, boost::ref( visitor ) ) );
}

// -----------------------------------------------------------------------------
// Name: Side::GetKarma
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
const kernel::Karma& Side::GetKarma() const
{
    return karma_;
}

// -----------------------------------------------------------------------------
// Name: Side::Register
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Register( dispatcher::Formation_ABC& formation )
{
    formations_.Register( formation.GetId(), formation );
}

// -----------------------------------------------------------------------------
// Name: Side::Remove
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Remove( dispatcher::Formation_ABC& formation )
{
    formations_.Remove( formation.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Side::Register
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Register( dispatcher::Population_ABC& population )
{
    populations_.Register( population.GetId(), population );
}

// -----------------------------------------------------------------------------
// Name: Side::Remove
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Remove( dispatcher::Population_ABC& population )
{
    populations_.Remove( population.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Side::Register
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
void Side::Register( dispatcher::Inhabitant_ABC& inhabitant )
{
    inhabitants_.Register( inhabitant.GetId(), inhabitant );
}

// -----------------------------------------------------------------------------
// Name: Side::Remove
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
void Side::Remove( dispatcher::Inhabitant_ABC& inhabitant )
{
    inhabitants_.Remove( inhabitant.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Side::Register
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Register( dispatcher::Object_ABC& object )
{
    objects_.Register( object.GetId(), object );
}

// -----------------------------------------------------------------------------
// Name: Side::Remove
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Remove( dispatcher::Object_ABC& object )
{
    objects_.Remove( object.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Side::Register
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Register( dispatcher::KnowledgeGroup_ABC& knGroup )
{
    knowledgeGroups_.Register( knGroup.GetId(), knGroup );
}

// -----------------------------------------------------------------------------
// Name: Side::Remove
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Remove( dispatcher::KnowledgeGroup_ABC& knGroup )
{
    knowledgeGroups_.Remove( knGroup.GetId() );
}
