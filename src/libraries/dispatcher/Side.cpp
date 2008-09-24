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
#include "Model.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "Object.h"
#include "Population.h"
#include "ModelVisitor_ABC.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Side::Side( const Model& model, const ASN1T_MsgTeamCreation& msg )
    : SimpleEntity< kernel::Team_ABC >( msg.oid, msg.nom )
    , model_( model )
    , name_( msg.nom )
    , nType_( msg.type )
{
    // NOTHING
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
// Name: Side::IsFriend
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
bool Side::IsFriend() const
{
    return nType_ == EnumDiplomacy::ami;
}

// -----------------------------------------------------------------------------
// Name: Side::IsEnemy
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
bool Side::IsEnemy() const
{
    return nType_ == EnumDiplomacy::ennemi;
}

// -----------------------------------------------------------------------------
// Name: Side::IsNeutral
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
bool Side::IsNeutral() const
{
    return nType_ == EnumDiplomacy::neutre;
}
