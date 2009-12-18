// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Side.h"

#include "ClientPublisher_ABC.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "ModelVisitor_ABC.h"
#include "Object.h"
#include "Population.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Side::Side( const Model& model, const ASN1T_MsgTeamCreation& msg )
    : Sendable< kernel::Team_ABC >( msg.oid, msg.nom )
    , model_( model )
    , name_( msg.nom )
    , nType_( msg.type )
{
    switch( nType_ )
    {
        case EnumDiplomacy::inconnu : karma_ = kernel::Karma::unknown_; break;
        case EnumDiplomacy::ami :     karma_ = kernel::Karma::friend_; break;
        case EnumDiplomacy::ennemi :  karma_ = kernel::Karma::enemy_; break;
        case EnumDiplomacy::neutre :  karma_ = kernel::Karma::neutral_; break;
    }
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
// Name: Side::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Side::Update( const ASN1T_MsgChangeDiplomacy& asnMsg )
{
    const kernel::Team_ABC& side = model_.sides_.Get( asnMsg.oid_camp2 );
    diplomacies_[ &side ] = asnMsg.diplomatie;
}

// -----------------------------------------------------------------------------
// Name: Side::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Side::Update( const ASN1T_MsgChangeDiplomacyAck& asnMsg )
{
    const kernel::Team_ABC& side = model_.sides_.Get( asnMsg.oid_camp2 );   
    diplomacies_[ &side ] = asnMsg.diplomatie;
}

// -----------------------------------------------------------------------------
// Name: Side::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Side::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::TeamCreation asn;
    asn().oid  = GetId();
    asn().nom  = name_.c_str();
    asn().type = nType_;
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
        asn().oid_camp1  = GetId();
        asn().oid_camp2  = it->first->GetId();
        asn().diplomatie = it->second;
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
    void VisitorAdapter( ModelVisitor_ABC& visitor, kernel::Entity_ABC& entity )
    {
        static_cast< C& >( entity ).Accept( visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: Side::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Side::Accept( ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    knowledgeGroups_.Apply( boost::bind( &KnowledgeGroup::Accept, _1, boost::ref( visitor ) ) );
    formations_.Apply( boost::bind( &Formation::Accept, _1, boost::ref( visitor ) ) );
    objects_.Apply( boost::bind( &Object::Accept, _1, boost::ref( visitor ) ) );
    populations_.Apply( boost::bind( &Population::Accept, _1, boost::ref( visitor ) ) );
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
void Side::Register( kernel::Formation_ABC& formation )
{
    //@TODO MGD Remove temp cast with next refactor step
    formations_.Register( formation.GetId(), (dispatcher::Formation&)formation );
}
// -----------------------------------------------------------------------------
// Name: Side::Remove
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Remove( kernel::Formation_ABC& formation )
{
    formations_.Remove( formation.GetId() );
}
// -----------------------------------------------------------------------------
// Name: Side::Register
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Register( kernel::Population_ABC& population )
{
    populations_.Register( population.GetId(), (dispatcher::Population&)population );
}
// -----------------------------------------------------------------------------
// Name: Side::Remove
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Remove( kernel::Population_ABC& population )
{
    populations_.Remove( population.GetId() );
}
// -----------------------------------------------------------------------------
// Name: Side::Register
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Register( kernel::Object_ABC& object )
{
    objects_.Register( object.GetId(), (dispatcher::Object&)object );
}
// -----------------------------------------------------------------------------
// Name: Side::Remove
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Remove( kernel::Object_ABC& object )
{
    objects_.Remove( object.GetId() );
}
// -----------------------------------------------------------------------------
// Name: Side::Register
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Register( kernel::KnowledgeGroup_ABC& knGroup )
{
    knowledgeGroups_.Register( knGroup.GetId(), (dispatcher::KnowledgeGroup&)knGroup );
}
// -----------------------------------------------------------------------------
// Name: Side::Remove
// Created: MGD 2009-12-17
// -----------------------------------------------------------------------------
void Side::Remove( kernel::KnowledgeGroup_ABC& knGroup )
{
    knowledgeGroups_.Remove( knGroup.GetId() );
}
